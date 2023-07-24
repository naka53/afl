#ifndef _HAVE_AFL_AS_RISCV_H
#define _HAVE_AFL_AS_RISCV_H

#include "config.h"
#include "types.h"

static const u8* trampoline_fmt_32 = ".NOT IMPLEMENTED";
static const u8* main_payload_32 = ".NOT IMPLEMENTED";

static const u8* trampoline_fmt_64 =
   ".align 4\n"
   "  addi    sp, sp, -16\n"
   "  sd      ra, 8 (sp)\n"
   "  sd      x18, 0 (sp)\n"

   "  li      x18, 0x%1$08x\n"

   "  call    __afl_maybe_log\n"

   "  ld      ra, 8 (sp)\n"
   "  ld      x18, 0 (sp)\n"
   "  addi    sp, sp, 16\n"
   "/* --- END --- */\n";

static const u8* main_payload_64 =
   ".text\n"
   ".align 4\n"
   ".weak __afl_maybe_log\n"
/* 
   cur_location = <COMPILE_TIME_RANDOM>;
   shared_mem[cur_location ^ prev_location]++;
   prev_location = cur_location >> 1;           
*/
   "__afl_maybe_log:\n"
   "  addi    sp, sp, -32\n"
   "  sd      x19, 24 (sp)\n"
   "  sd      x20, 16 (sp)\n"
   "  sd      x21, 8 (sp)\n"
   "  sd      x22, 0 (sp)\n"
/*
   AFL Bitmap located at fixed address 0x50000
*/
   "  li     x20, 0x50000\n"

#ifndef COVERAGE_ONLY
/*
   AFL prev_loc located at fixed address 0x40000
*/
   "  li     x21, 0x40000\n"
   
   "  ld      x22, 0 (x21)\n"
   "  srli    x19, x18, 1\n"
   "  xor     x18, x18, x22\n"
   "  sd      x19, 0 (x21)\n"
#endif /* COVERAGE_ONLY */
   "  add     x20, x20, x18\n"
   "  ld      x18, 0 (x20)\n"
#ifdef SKIP_COUNTS
   "  ori     x18, x18, 1\n"
#else
   "  addi    x18, x18, 1\n"
#endif /* SKIP_COUNTS */
   "  sd    x18, 0 (x20)\n"

   "  ld      x19, 24 (sp)\n"
   "  ld      x20, 16 (sp)\n"
   "  ld      x21, 8 (sp)\n"
   "  ld      x22, 0 (sp)\n"
   "  addi    sp, sp, 32\n"

   "  ret\n";
#endif /* _HAVE_AFL_AS_RISCV_H */
