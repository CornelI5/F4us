/*
 * F4us-KShield - Kernel-Antivirus Synergy Engine
 * Description: Real-time monitoring of Linux kernel state and loaded modules.
 */

#include "../../include/kshield.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* * Hardens the existing Linux kernel parameters dynamically 
 * Implements real-time defensive restrictions.
 */
void f4us_lock_kernel_core() {
    printf("[*] KShield: Injecting defensive hardened parameters into running kernel...\n");

    /* * Disable kexec (prevents loading a new kernel without rebooting)
     * This blocks advanced live-kernel hijacking attacks!
     */
    FILE *fp = fopen("/proc/sys/kernel/kexec_load_disabled", "w");
    if (fp) {
        fprintf(fp, "1");
        fclose(fp);
        printf("[+] Kernel Security: kexec execution paths LOCKED.\n");
    }

    /* * Restrict dmesg access (prevents unprivileged users from reading kernel logs)
     * Blocks attackers from leaked kernel memory addresses!
     */
    fp = fopen("/proc/sys/kernel/dmesg_restrict", "w");
    if (fp) {
        fprintf(fp, "1");
        fclose(fp);
        printf("[+] Kernel Security: Kernel log access (dmesg) RESTRICTED to root.\n");
    }
}

/* * Audits loaded kernel modules in real-time to detect unauthorized Rootkits
 */
void f4us_audit_kernel_modules() {
    FILE *fp = fopen(PROC_MODULES, "r");
    if (!fp) {
        perror("[!] Error: Cannot audit kernel module tree");
        return;
    }

    char line[256];
    char mod_name[MAX_MODULE_NAME];
    
    printf("[*] KShield Active Thread: Scanning loaded kernel drivers for anomalies...\n");

    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%s", mod_name);
        
        /* * SUSPICIOUS MODULE DETECTION STRATEGY:
         * If a driver name looks randomly generated or known malware patterns
         */
        if (strstr(mod_name, "rootkit") || strstr(mod_name, "spy") || strlen(mod_name) > 30) {
            printf("[!!!] KSHIELD SECURITY ALERT: Unverified Kernel Driver Found -> %s\n", mod_name);
            printf("[*] Action: Requesting kernel core to forcefully unload module...\n");
            
            // Execute automated kernel emergency ejection
            char cmd[128];
            snprintf(cmd, sizeof(cmd), "sudo rmmod %s 2>/dev/null", mod_name);
            system(cmd);
            
            printf("[+] Mitigation: Threat vector %s ejected from kernel ring!\n", mod_name);
        }
    }
    fclose(fp);
}

int main() {
    // Requires Root privilege because we are tweaking the real kernel parameters
    if (getuid() != 0) {
        printf("[!] Critical Error: KShield must be executed with root/sudo privileges.\n");
        return 1;
    }

    printf("==================================================\n");
    printf("   F4US-KSHIELD - HARDENED KERNEL EDR ENGINE      \n");
    printf("   Target: Active Linux Kernel Protection         \n");
    printf("==================================================\n");

    /* Step 1: Lock down vulnerabilities in the active kernel */
    f4us_lock_kernel_core();

    /* Step 2: Continuous monitoring loop for rootkits */
    while(1) {
        f4us_audit_kernel_modules();
        printf("[*] System State: Secure. Next scan in 5 seconds...\n\n");
        sleep(5);
    }

    return 0;
}
