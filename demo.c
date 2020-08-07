/*
 * Jytter
 * Version 5
 * December 15, 2019
 * http://jytter.blogspot.com/
 *
 * Copyright (c) 2012-2019 Russell Leidich
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "jytter.h"

#ifdef _32_
#ifdef _64_
#error "You have defined both _32_ and _64_. Chose one only."
#endif
#elif _64_
#else
#error "Use \"nasm -D_64_\" for 64-bit or \"nasm -D_32_\" for 32-bit code."
#endif

#ifdef _32_
extern uint32_t jytter_true_random_get(jytter_scratch_space_t *jytter_scratch_space) __attribute__((fastcall));
extern void timestamp_get(uint64_t *timestamp) __attribute__((fastcall));
#else
extern uint32_t jytter_true_random_get(jytter_scratch_space_t *jytter_scratch_space);
extern void timestamp_get(uint64_t *timestamp);
#endif

void
true_random_printf(uint32_t true_random_count){
  jytter_scratch_space_t jytter_scratch_space;
  uint32_t true_random;

  while(true_random_count--){
    true_random=jytter_true_random_get(&jytter_scratch_space);
    printf("%08X",true_random);
  }
  return;
}

void
true_random_password_printf(uint32_t true_random_char_count){
/*
Print out an alphanumeric true random password. Avoid special characters that could be unacceptable in some cases; let the user add them manually.
*/
  uint8_t character;
  jytter_scratch_space_t jytter_scratch_space;
  uint32_t true_random;

  while(true_random_char_count--){
    true_random=jytter_true_random_get(&jytter_scratch_space);
    character=(uint8_t)(true_random%(10U+26U+26U));
    if(character<10U){
      character=(uint8_t)(character+48U);
    }else if(character<(10U+26U)){
      character=(uint8_t)(character+65U-10U);
    }else if(character<(10U+26U+26U)){
      character=(uint8_t)(character+97U-10U-26U);
    }
    printf("%s",(char *)(&character));
  }
  return;
}

int
main (int argc, char *argv[]){
  int32_t i;
  uint64_t timestamp_end;
  uint64_t timestamp_start;

  timestamp_get(&timestamp_start);
  printf("True random integers:\n");
  printf("\n64-bit: ");
  true_random_printf(2);
  printf("\n128-bit: ");
  true_random_printf(4);
  printf("\n256-bit: ");
  true_random_printf(8);
  printf("\n\nTrue random alphanumeric passwords:\n");
  printf("\n65-bit: ");
  true_random_password_printf(11);
  printf("\n130-bit: ");
  true_random_password_printf(22);
  printf("\n256-bit: ");
  true_random_password_printf(43);
  timestamp_get(&timestamp_end);
  timestamp_end-=timestamp_start;
  printf("\n\nExecution time in CPU ticks:\n\n%08X%08X\n",(uint32_t)(timestamp_end>>32),(uint32_t)(timestamp_end));
  return 0;
}
