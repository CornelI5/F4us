/*
 * F4us-KShield: Kernel-Space Behavior Interceptor
 * Licensed under GNU GPL v3 (2026)
 */

#ifndef F4US_KSHIELD_H
#define F4US_KSHIELD_H

#define PROC_MODULES "/proc/modules"
#define MAX_MODULE_NAME 64

void f4us_lock_kernel_core();
void f4us_audit_kernel_modules();

#endif
