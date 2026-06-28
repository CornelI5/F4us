#include "backit.h"

int active_honeypots = 0;
int current_tier_level = 1;

const char *bait_names[] = {
    ".hidden_crypto_miner",
    "backup_passwords.txt",
    "root_access.sh",
    ".env_production",
    "systemd-update"
};

void f4us_honeypot_winsert(const char* filepath, int tier) {
    struct stat st;
    
    if (stat(filepath, &st) != 0) {
        return;
    }

    /* Upgrade trap enforcement based on generation tier */
    if (tier > 5) {
        /* Restrict access to root-only to monitor high-privilege escalation */
        chmod(filepath, 0700);
    } else if (tier > 2) {
        /* Read-only bait to detect modification attempts via write failures */
        chmod(filepath, 0444);
    }
}

void f4us_spawn_honeypot_virus(const char* target_path) {
    if (active_honeypots >= MAX_HONEYPOTS) {
        return;
    }

    char full_path[512];
    int name_index = active_honeypots % 5; 
    
    snprintf(full_path, sizeof(full_path), "%s/%s", target_path, bait_names[name_index]);

    FILE *file = fopen(full_path, "w");
    if (file != NULL) {
        fprintf(file, "# F4US_TRAP_ZONE_TIER_%d\n", current_tier_level);
        fclose(file);
        
        /* Initialize with permissive access to lure automated vectors */
        chmod(full_path, 0777);
        
        /* Apply systemic upgrades via winsert engine */
        f4us_honeypot_winsert(full_path, current_tier_level);
        
        active_honeypots++;
    }
}

void f4us_on_trap_triggered(const char* triggered_path) {
    unlink(triggered_path);
    
    /* Increment operational tier for subsequent deployments */
    current_tier_level++;
    
    f4us_spawn_honeypot_virus("/tmp"); 
    f4us_spawn_honeypot_virus("/var/tmp"); 
}
