#ifndef _HAVE_AFL_AS_ARM_H
#define _HAVE_AFL_AS_ARM_H

#include "config.h"
#include "types.h"

static const u8* trampoline_fmt_64 = ".NOT IMPLEMENTED";
static const u8* main_payload_64 = ".NOT IMPLEMENTED";

static const u8* trampoline_fmt_32 =
   "/* --- AFL TRAMPOLINE (32-BIT) --- */\n"
   ".align 4\n"
   "  push    {r0-r5}\n"
   "  movw    r3, #:lower16:0x%1$08x\n"
   "  movt    r3, #:upper16:0x%1$08x\n"
   "  bl      __afl_maybe_log\n"
   "  pop     {r0-r5}\n"
   "/* --- END --- */\n";

static const u8* main_payload_32 =
   "/* --- AFL MAIN PAYLOAD (32-BIT) --- */\n"
   ".text\n"
   ".align 4\n"
   "__afl_maybe_log:\n"
   "  movw    r2, #0x0000\n"
   "  movt    r2, #0x0005\n"
/* 
   cur_location = <COMPILE_TIME_RANDOM>;
   shared_mem[cur_location ^ prev_location]++;
   prev_location = cur_location >> 1;           
*/
   "__afl_store:\n"
#ifndef COVERAGE_ONLY
   "  movw    r5, #:lower16:__afl_prev_loc\n"
   "  movt    r5, #:upper16:__afl_prev_loc\n"
   "  ldr     r4, [r5, #0]\n"
   "  asr     r0, r3, #1\n"
   "  eor     r3, r3, r4\n"
   "  str     r0, [r5, #0]\n"
#endif /* ^!COVERAGE_ONLY */
   "  add     r2, r2, r3\n"
   "  ldrb    r3, [r2, #0]\n"
#ifdef SKIP_COUNTS
   "  orr     r3, r3, #1\n"
#else
   "  add     r3, r3, #1\n"
#endif /* ^SKIP_COUNTS */
   "  strb    r3, [r2, #0]\n"
   "__afl_return:\n"
   "  mov     pc, lr\n"
   ".AFL_VARS:\n"
#ifndef COVERAGE_ONLY
   "  .comm   __afl_prev_loc, 4, 32\n"
#endif /* !COVERAGE_ONLY */
   "/* --- END --- */\n";
#endif /* _HAVE_AFL_AS_ARM_H */
