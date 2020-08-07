;
; Jytter
; Version 5
; December 15, 2019
; http://jytter.blogspot.com/
;
; Copyright (c) 2012-2019 Russell Leidich
;
; Permission is hereby granted, free of charge, to any person obtaining
; a copy of this software and associated documentation files (the
; "Software"), to deal in the Software without restriction, including
; without limitation the rights to use, copy, modify, merge, publish,
; distribute, sublicense, and/or sell copies of the Software, and to
; permit persons to whom the Software is furnished to do so, subject to
; the following conditions:
;
; The above copyright notice and this permission notice shall be included
; in all copies or substantial portions of the Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
; EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
; IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
; CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
; TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
; SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
;
;--------------------------------------------------------------------------
;
; Timestamp Counter (TSC) Reader
;
; Created April 19, 2012 by Russell Leidich.
;
; For use with:
;
;   32-bit IA32 GCC and Microsoft compilers.
;
;   32-bit X32 GCC.
;
;   64-bit AMD64 GCC.
;
; Compile with:
;
;   32-bit: nasm -D_32_ -otimestamp.o(bj) timestamp.asm
;
;   64-bit: nasm -D_64_ -otimestamp.o(bj) timestamp.asm
;
; due to dependency on IA32 and AMD64 calling conventions.
;
%IFDEF _32_
%IFDEF _64_
%FATAL #error "You have defined both _32_ and _64_. Chose one only."
%ELSE
USE32
%ENDIF
%ELIF _64_
USE64
%ELSE
%FATAL "Use \"nasm -D_64_\" for 64-bit or \"nasm -D_32_\" for 32-bit code."
%ENDIF

SECTION .text

%MACRO PROC 1
  GLOBAL %1
  %1:
%ENDMACRO

%MACRO ENDPROC 1
%ENDMACRO

%MACRO INT32 1
  %1: RESD 1
%ENDMACRO

STRUC timestamp_scratch_space
  INT32 timestamp_lo
  INT32 timestamp_hi
ENDSTRUC

PROC timestamp_get
PROC _timestamp_get
;
; Return the Timestamp Counter (TSC). The 64-bit timer value generally marches
; forward, but might reverse due to poorly written power state transition code
; or virtual machine bugs. The most often toggled bit is usually bit 0, but
; might not be, due to core frequency throttling by a power-of-2 multiplier.
;
; The C prototype is defined such that ECX/RDI will end up pointing to
; timestamp_scratch_space. See
; http://en.wikipedia.org/wiki/X86_calling_conventions for more information.
;
; Prototype it like this under GCC. (Replace "__attribute__((fastcall))" with
; the equivalent under different compilers. This attribute causes parameter
; passing in ECX in 32-bit mode.) And note that although this procedure name
; starts with an underscore, it is omitted from the C prototype below:
;
; uint64_t timestamp;
;
; Followed by this in a 32-bit environment:
;
;   extern void timestamp_get(uint64_t *timestamp) __attribute__((fastcall));
;
; Or this in a 64-bit environment:
;
;   extern void timestamp_get(uint64_t *timestamp);
;
; Call it like this:
;
;   uint64_t timestamp;
;
;   timestamp_get(&timestamp);
;
;-----------------------------------------------------------
;
; IN
;
;   ECX/RDI = Pointer to timestamp.
;
;   DS = Segment of timestamp_scratch_space (32-bit environment only).
;
; OUT
;
;   EAX/RAX destroyed.
;
;   timestamp returned (in memory).
;
;-----------------------------------------------------------
;
; We can't easily use the stack due to the possible presence of the compiler
; red zone below ESP or RSP. It doesn't matter because we have ECX/RDI as the
; base of timestamp_scratch_space.
;
%IFDEF _32_
  mov [ecx+timestamp_hi],edx
  rdtsc
  mov [ecx+timestamp_lo],eax
  xchg edx,[ecx+timestamp_hi]
%ELSE
  mov [rdi+timestamp_lo],rdx
  rdtsc
  shl rdx,32
  xchg rdx,[rdi+timestamp_lo]
  mov [rdi+timestamp_lo],eax
%ENDIF
  retn
ENDPROC _timestamp_get
ENDPROC timestamp_get

