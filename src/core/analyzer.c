/*
 * ZeroVoid Engine - Static Binary Analyzer & Entropy Detector
 * Licensed under GNU GPL v3 (2026)
 */

#include "../../include/zerovoid.h"
#include <stdio.h>


#define ZV_MAX_KERNEL_SCAN_SIZE (10 * 1024 * 1024) 
#define ZV_MAX_LOOP_ITERATIONS  5000000


int zv_analyze_binary(ZV_CONTEXT *ctx, const unsigned char *file_buffer, size_t buffer_size) {
    

    if (!ctx || !file_buffer || buffer_size == 0) {
        return ZV_ERROR;
    }


    if (buffer_size > ZV_MAX_KERNEL_SCAN_SIZE) {
        return ZV_ERROR; 
    }

    // -------------------------------------------------------------------------
    // 1. ADVANCED MAGIC BYTES VALIDATION (Anti-Bypass)
    // -------------------------------------------------------------------------
    if (buffer_size >= 2) {
        // Cek Magic Byte standar Windows Executable (MZ)
        if (file_buffer[0] == 0x4D && file_buffer[1] == 0x5A) {
            ctx->threat_score += 10; 
            
            // Validasi tambahan: Cek offset e_lfanew untuk memastikan struktur PE valid
            if (buffer_size >= 64) {
                unsigned int pe_offset = *(unsigned int*)(file_buffer + 0x3C);
                // Pastikan offset PE berada di dalam jangkauan buffer yang valid
                if (pe_offset + 4 <= buffer_size) {
                    if (file_buffer[pe_offset] == 'P' && file_buffer[pe_offset + 1] == 'E') {
                        ctx->threat_score += 15; // Struktur PE terkonfirmasi valid
                    }
                }
            }
        } 
        // Deteksi upaya bypass langsung menggunakan Web Shell header
        else if (file_buffer[0] == '<' && file_buffer[1] == '?') {
            return ZV_CRITICAL; 
        }
    }

    // -------------------------------------------------------------------------
    // 2. HONEYPOT / DECEPTION TRAP (Umpan untuk Attacker/Reverser)
    // -------------------------------------------------------------------------
    // Menggunakan 'volatile' agar tidak dihapus oleh optimasi kompiler (-O2 / -Ox)
    volatile int attacker_bait_bytes = 0; 
    unsigned char honeypot_canary[8] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xDE, 0xAD, 0xBE, 0xEF };

    // -------------------------------------------------------------------------
    // 3. ANTI-DoS & ANTI-OVERFLOW ENTROPY LOOP
    // -------------------------------------------------------------------------
    unsigned int byte_counts[256] = {0};
    size_t total_bytes = 0; // Menggunakan size_t (64-bit) untuk mencegah integer overflow riil
    unsigned int loop_counter = 0;

    for (size_t i = 0; i < buffer_size; i++) {
        
        // --- DECEPTION TRAP LOGIC ---
        attacker_bait_bytes++;
        
        if (attacker_bait_bytes == 2147483640) {
            ctx->threat_score += 100;
            return ZV_CRITICAL;
        }
        // -----------------------------

        // --- ANTI-DoS MITIGATION ---
        if (++loop_counter > ZV_MAX_LOOP_ITERATIONS) {
            return ZV_ERROR; // 
        }

      
        unsigned char ch = file_buffer[i];
        byte_counts[ch]++;
        
        // --- ANTI-OVERFLOW MITIGATION ---
        if (total_bytes >= SIZE_MAX) { 
            return ZV_ERROR; 
        }
        total_bytes++;
    }


    if (honeypot_canary[0] != 0xDE || honeypot_canary[3] != 0xEF) {
        return ZV_CRITICAL; // Canary corrupted!
    }


    int unique_bytes = 0;
    for (int i = 0; i < 256; i++) {
        if (byte_counts[i] > 0) {
            unique_bytes++;
        }
    }
    return (ctx->threat_score >= 50) ? ZV_SUSPICIOUS : ZV_SUCCESS;
}
