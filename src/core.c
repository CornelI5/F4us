/*
 * F4us Backit - The Counter-Attack Ghost Core Implementation
 * Description: Scans, intercepts, and forcefully obliterates active virus strains.
 */

#include "../../include/backit.h"

/* Registered historical and modern lethal malware signatures */
const char *malware_signatures[MAX_MALWARE_SIGNATURES] = {
    "love-letter-for-you", // Signature for the legendary ILOVEYOU worm
    "WannaCry",            // Signature for the automated Ransomware core
    "f4us_dummy_virus"     // Local placeholder for validation testing
};

void f4us_backit_stealth_init(void) {
    printf("[*] Backit: Submerging into shadow mode (Anti-Rootkit Layer)...\n");
    printf("[*] Status: Weaponized Counter-Measures ARMED and active.\n");
}

/* * CORE ASSAULT ROUTINE:
 * Deeply inspects raw file contents. If a malware signature is found,
 * it immediately triggers the counter-attack shredding protocol.
 */
void f4us_hunt_and_destroy(void) {
    DIR *dir;
    struct dirent *entry;
    char file_path[512];

    dir = opendir(TARGET_DIR);
    if (!dir) {
        perror("[!] Error: Cannot open target subsystem directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip default filesystem relative directory anchors
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(file_path, sizeof(file_path), "%s/%s", TARGET_DIR, entry->d_name);

        // Open file descriptors for Raw Byte Inspection
        FILE *fp = fopen(file_path, "r");
        if (fp) {
            char content[1024] = {0};
            fread(content, 1, sizeof(content) - 1, fp);
            fclose(fp);

            // Cross-reference file buffer data with known malicious signatures
            for (int i = 0; i < MAX_MALWARE_SIGNATURES; i++) {
                if (strstr(content, malware_signatures[i])) {
                    printf("[!!!] BACKIT DETECTED MALWARE: '%s' matches signature [%s]!\n", entry->d_name, malware_signatures[i]);
                    printf("[*] Action: Executing Offensive Counter-Attack (FORCE SHRED)...\n");

                    /* * COUNTER-ATTACK EXECUTION:
                     * 1. Overwrite the malicious binary with zeros to corrupt its execution vector.
                     * 2. Purge the file pointer permanently from the disk structure using 'unlink'.
                     */
                    FILE *wipe = fopen(file_path, "w");
                    if (wipe) {
                        fprintf(wipe, "0000000000000000"); // Corrupting data integrity
                        fclose(wipe);
                    }
                    
                    if (unlink(file_path) == 0) {
                        printf("[+] SUCCESS: Malware file '%s' has been OBLITERATED from the filesystem!\n", entry->d_name);
                    } else {
                        printf("[!] Warning: Absolute memory/disk unlinking failed on target path.\n");
                    }
                }
            }
        }
    }
    closedir(dir);
}

int main(void) {
    printf("========================================================\n");
    printf("     F4US-BACKIT : THE ANTI-MALWARE GHOST ASSAULT       \n");
    printf("     Target: Hunt, Infiltrate, and Wipe Active Viruses  \n");
    printf("========================================================\n");

    /* SIMULATION SEQUENCE: Creating a dummy script with the ILOVEYOU worm signature */
    printf("[*] Setup: Simulating an infected target file inside %s...\n", TARGET_DIR);
    FILE *v = fopen(TARGET_DIR "/LOVE-LETTER-FOR-YOU.TXT.vbs", "w");
    if (v) {
        fprintf(v, "rem  ILOVEYOU virus script simulation chain\n");
        fprintf(v, "set payload = love-letter-for-you-variable\n");
        fclose(v);
    }

    /* Step 1: Initialize the Backit stealth engine layer */
    f4us_backit_stealth_init();

    /* Step 2: Trigger the aggressive purge cycle */
    printf("\n--- Starting Deep System Sweeping & Counter-Attack Routine ---\n");
    f4us_hunt_and_destroy();

    return 0;
}
