/*
 * F4us Backit: Anti-Malware Counter-Attack & Destroy Engine
 * Licensed under GNU GPL v3 (2026)
 */

#ifndef F4US_BACKIT_H
#define F4US_BACKIT_H

#define MAX_MALWARE_SIGNATURES 3
#define TARGET_DIR "/tmp" // Simulation of a directory attacked by a virus

void f4us_backit_stealth_init();
void f4us_hunt_and_destroy();

#endif
