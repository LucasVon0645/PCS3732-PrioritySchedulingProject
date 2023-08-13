#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../types.h"

extern int latest_tid;

tcb_t* create_tcb(uint32_t priority, uint32_t exc_slots, uint32_t entry_point, uint32_t exit_point);
tcb_t* copy_tcb(tcb_t* original_tcb);