/**
 * @copyright Copyright (c) 2026. Licensed under the MIT License.
 * This driver operates at Ring 0 to intercept file I/O operations before execution.
 */

#include <fltKernel.h>
#include <ntddk.h>

// Global filter handle required for unregistration
PFLT_FILTER FilterHandle = NULL;

// Global Function Prototypes
DRIVER_INITIALIZE DriverEntry;
NTSTATUS FltUnload(_In_ FLT_REGISTRATION_FLAGS Flags);
FLT_PREOP_CALLBACK_STATUS FltPreCreate(
    _Inout_ PFLT_CALLBACK_DATA Data, 
    _In_ PCFLT_RELATED_OBJECTS FltObjects, 
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext
);

/**
 * @brief Operation registration array.
 * Defines which I/O requests (IRPs) this minifilter will intercept.
 */
const FLT_OPERATION_REGISTRATION Callbacks[] = {
    { IRP_MJ_CREATE, 0, FltPreCreate, NULL }, // Intercept file open/create requests
    { IRP_MJ_OPERATION_END }
};

/**
 * @brief Filter registration structure.
 * Provides the Windows Filter Manager with callback entry points and metadata.
 */
const FLT_REGISTRATION FilterRegistration = {
    sizeof(FLT_REGISTRATION),           // Structure size
    FLT_REGISTRATION_VERSION,           // Registration version
    0,                                  // Flags
    NULL,                               // Context registration
    Callbacks,                          // Operation callbacks array
    FltUnload,                          // Driver unload callback
    NULL,                               // Instance setup callback
    NULL,                               // Instance query teardown
    NULL,                               // Instance teardown start
    NULL,                               // Instance teardown complete
    NULL, NULL, NULL                    // Name generation callbacks (Unused)
};

/**
 * @brief Pre-Operation callback for IRP_MJ_CREATE.
 * Executed whenever a process attempts to open or create a file handler.
 * This is where the F4us signature-scanning engine intercepts potential malware.
 */
FLT_PREOP_CALLBACK_STATUS FltPreCreate(
    _Inout_ PFLT_CALLBACK_DATA Data, 
    _In_ PCFLT_RELATED_OBJECTS FltObjects, 
    _Flt_CompletionContext_Outptr_ PVOID *CompletionContext
) {
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);

    PFLT_FILE_NAME_INFORMATION FileNameInfo = NULL;
    NTSTATUS status;

    // Safely retrieve the file name information from the callback data
    status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &FileNameInfo);
    
    if (NT_SUCCESS(status)) {
        // Parse the filename to make it human-readable
        FltParseFileNameInformation(FileNameInfo);

        // Kernel-space debug log visible via DebugView / WinDbg
        KdPrint(("[F4us.sys] Intercepted I/O request to: %wZ\n", &FileNameInfo->Name));

        /* 
         * TODO: Integrate F4us Signature-Based / Heuristic Scan Engine here.
         * If the file matches a known malware hash, alter the status to STATUS_ACCESS_DENIED:
         * 
         * Data->IoStatus.Status = STATUS_ACCESS_DENIED;
         * Data->IoStatus.Information = 0;
         * return FLT_PREOP_COMPLETE;
         */

        // Clean up the allocated file name structure to prevent memory leaks in Ring 0
        FltReleaseFileNameInformation(FileNameInfo);
    }

    // Pass the request down to the next filter or base file system
    return FLT_PREOP_SUCCESS_NO_CALLBACK; 
}

/**
 * @brief Driver entry point.
 * Initializes the driver, registers it with the Filter Manager, and starts filtering.
 */
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    NTSTATUS status;

    KdPrint(("[F4us.sys] Initializing Ring 0 Core Driver...\n"));

    // Register the minifilter driver with the Windows Filter Manager
    status = FltRegisterFilter(DriverObject, &FilterRegistration, &FilterHandle);
    
    if (NT_SUCCESS(status)) {
        // Start intercepting file I/O operations
        status = FltStartFiltering(FilterHandle);
        
        if (!NT_SUCCESS(status)) {
            KdPrint(("[F4us.sys] Failed to start filtering. Error code: 0x%X\n", status));
            FltUnregisterFilter(FilterHandle);
        } else {
            KdPrint(("[F4us.sys] Filter successfully registered and active.\n"));
        }
    } else {
        KdPrint(("[F4us.sys] Filter registration failed. Error code: 0x%X\n", status));
    }

    return status;
}

/**
 * @brief Driver unload routine.
 * Ensures that the driver detaches cleanly from the file system to avoid kernel deadlocks.
 */
NTSTATUS FltUnload(_In_ FLT_REGISTRATION_FLAGS Flags) {
    UNREFERENCED_PARAMETER(Flags);
    
    KdPrint(("[F4us.sys] Terminating core driver. Unregistering minifilter...\n"));
    
    // Unregister filter to release system resources safely
    FltUnregisterFilter(FilterHandle);
    
    KdPrint(("[F4us.sys] Unloaded successfully. Ring 0 protection disabled.\n"));
    
    return STATUS_SUCCESS;
}
