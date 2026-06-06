/*
 * Jytter
 * Version 11
 * June 5, 2026
 * http://jytter.blogspot.com/
 *
 * Copyright (c) 2012-2026 Russell Leidich
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
#ifdef UNIX_EMULATION
#include "unistd.h"
#endif
#include "jytter.h"
#include "seed_word_list.h"

#ifdef _32_
#ifdef _64_
#error "You have defined both _32_ and _64_. Chose one only."
#endif
#elif _64_
#else
#error "Use \"nasm -D_64_\" for 64-bit or \"nasm -D_32_\" for 32-bit code."
#endif

#ifndef UNIX_EMULATION
#ifdef _32_
extern uint32_t jytter_true_random_get(jytter_scratch_space_t *jytter_scratch_space) __attribute__((fastcall));
#else
extern uint32_t jytter_true_random_get(jytter_scratch_space_t *jytter_scratch_space);
#endif
#endif

uint32_t
big_endian_get(uint32_t little_endian){
  uint32_t big_endian;

  big_endian=little_endian<<24;
  big_endian+=((little_endian>>8)&0xFF)<<16;
  big_endian+=((little_endian>>16)&0xFF)<<8;
  big_endian+=little_endian>>24;
  return big_endian;
}

void
sha256_get(uint8_t bits_128_or_256_status, uint32_t list[8]){
  uint32_t hash[8];
  uint32_t hash0;
  uint32_t hash1;
  uint32_t hash2;
  uint32_t hash3;
  uint32_t hash4;
  uint32_t hash5;
  uint32_t hash6;
  uint32_t hash7;
  uint32_t i;
  uint32_t table[64];
  uint32_t temp[64];
  uint32_t temp_idx;
  uint32_t temp0;
  uint32_t temp1;

  hash[0]=0x6A09E667;
  hash[1]=0xBB67AE85;
  hash[2]=0x3C6EF372;
  hash[3]=0xA54FF53A;
  hash[4]=0x510E527F;
  hash[5]=0x9B05688C;
  hash[6]=0x1F83D9AB;
  hash[7]=0x5BE0CD19;
  table[0]=0x428A2F98;
  table[1]=0x71374491;
  table[2]=0xB5C0FBCF;
  table[3]=0xE9B5DBA5;
  table[4]=0x3956C25B;
  table[5]=0x59F111F1;
  table[6]=0x923F82A4;
  table[7]=0xAB1C5ED5;
  table[8]=0xD807AA98;
  table[9]=0x12835B01;
  table[10]=0x243185BE;
  table[11]=0x550C7DC3;
  table[12]=0x72BE5D74;
  table[13]=0x80DEB1FE;
  table[14]=0x9BDC06A7;
  table[15]=0xC19BF174;
  table[16]=0xE49B69C1;
  table[17]=0xEFBE4786;
  table[18]=0x0FC19DC6;
  table[19]=0x240CA1CC;
  table[20]=0x2DE92C6F;
  table[21]=0x4A7484AA;
  table[22]=0x5CB0A9DC;
  table[23]=0x76F988DA;
  table[24]=0x983E5152;
  table[25]=0xA831C66D;
  table[26]=0xB00327C8;
  table[27]=0xBF597FC7;
  table[28]=0xC6E00BF3;
  table[29]=0xD5A79147;
  table[30]=0x06CA6351;
  table[31]=0x14292967;
  table[32]=0x27B70A85;
  table[33]=0x2E1B2138;
  table[34]=0x4D2C6DFC;
  table[35]=0x53380D13;
  table[36]=0x650A7354;
  table[37]=0x766A0ABB;
  table[38]=0x81C2C92E;
  table[39]=0x92722C85;
  table[40]=0xA2BFE8A1;
  table[41]=0xA81A664B;
  table[42]=0xC24B8B70;
  table[43]=0xC76C51A3;
  table[44]=0xD192E819;
  table[45]=0xD6990624;
  table[46]=0xF40E3585;
  table[47]=0x106AA070;
  table[48]=0x19A4C116;
  table[49]=0x1E376C08;
  table[50]=0x2748774C;
  table[51]=0x34B0BCB5;
  table[52]=0x391C0CB3;
  table[53]=0x4ED8AA4A;
  table[54]=0x5B9CCA4F;
  table[55]=0x682E6FF3;
  table[56]=0x748F82EE;
  table[57]=0x78A5636F;
  table[58]=0x84C87814;
  table[59]=0x8CC70208;
  table[60]=0x90BEFFFA;
  table[61]=0xA4506CEB;
  table[62]=0xBEF9A3F7;
  table[63]=0xC67178F2;
  temp_idx=0;
  while(temp_idx<=7){
    temp[temp_idx]=big_endian_get(list[temp_idx]);
    temp[temp_idx+8]=0;
    temp_idx++;
  }
  if(bits_128_or_256_status){
    temp[8]=0x80000000;
    temp[15]=256;
  }else{
    temp[4]=0x80000000;
    temp[5]=0;
    temp[6]=0;
    temp[7]=0;
    temp[15]=128;
  }
  temp_idx=16;
  while(temp_idx<=63){
    temp0=temp[temp_idx-15];
    temp1=temp[temp_idx-16];
    temp1+=temp[temp_idx-7];
    temp1+=((temp0>>7)+(temp0<<25))^((temp0>>18)+(temp0<<14))^(temp0>>3);
    temp0=temp[temp_idx-2];
    temp1+=((temp0>>17)+(temp0<<15))^((temp0>>19)+(temp0<<13))^(temp0>>10);
    temp[temp_idx]=temp1;
    temp_idx++;
  }
  hash0=hash[0];
  hash1=hash[1];
  hash2=hash[2];
  hash3=hash[3];
  hash4=hash[4];
  hash5=hash[5];
  hash6=hash[6];
  hash7=hash[7];
  temp_idx=0;
  while(temp_idx<=63){
    temp1=((hash4>>6)+(hash4<<26))^((hash4>>11)+(hash4<<21))^((hash4>>25)+(hash4<<7));
    temp1+=((hash4&hash5)^((~hash4)&hash6))+hash7;
    temp1+=table[temp_idx]+temp[temp_idx];
    hash7=hash6;
    hash6=hash5;
    hash5=hash4;
    hash4=hash3+temp1;
    temp1+=((hash0>>2)+(hash0<<30))^((hash0>>13)+(hash0<<19))^((hash0>>22)+(hash0<<10));
    temp1+=(hash0&hash1)^(hash0&hash2)^(hash1&hash2);
    hash3=hash2;
    hash2=hash1;
    hash1=hash0;
    hash0=temp1;
    temp_idx++;
  }
  hash[0]+=hash0;
  hash[1]+=hash1;
  hash[2]+=hash2;
  hash[3]+=hash3;
  hash[4]+=hash4;
  hash[5]+=hash5;
  hash[6]+=hash6;
  hash[7]+=hash7;
  list[0]=hash[0];
  list[1]=hash[1];
  list[2]=hash[2];
  list[3]=hash[3];
  list[4]=hash[4];
  list[5]=hash[5];
  list[6]=hash[6];
  list[7]=hash[7];
  return;
}

void
true_random_printf(uint32_t true_random_count){
#ifndef UNIX_EMULATION
  jytter_scratch_space_t jytter_scratch_space;
#endif
  uint32_t true_random;

  while(true_random_count--){
#ifdef UNIX_EMULATION
    do{
    }while(getentropy((void *)(&true_random), sizeof(uint32_t)));
#else
    true_random=jytter_true_random_get(&jytter_scratch_space);
#endif
    printf("%08X",true_random);
  }
  return;
}

void
true_random_password_printf(uint32_t true_random_char_count){
/*
Print out an alphanumeric true random password. Avoid special characters that could be unacceptable in some cases; let the user add them manually.
*/
  uint16_t character;
#ifndef UNIX_EMULATION
  jytter_scratch_space_t jytter_scratch_space;
#endif
  uint32_t true_random;

  while(true_random_char_count--){
#ifdef UNIX_EMULATION
    do{
    }while(getentropy((void *)(&true_random), sizeof(uint32_t)));
#else
    true_random=jytter_true_random_get(&jytter_scratch_space);
#endif
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

void
true_random_seed_phrase_12_printf(void){
/*
Print out a 12-word true random seed phrase
*/
#ifndef UNIX_EMULATION
  jytter_scratch_space_t jytter_scratch_space;
#endif
  uint32_t seed_word_idx_idx;
  uint32_t seed_word_idx0;
  uint32_t seed_word_idx1;
  uint32_t seed_word_idx_list[12];
  uint32_t sha256_list[8];
  uint32_t true_random;
  uint32_t true_random_bit_idx;
  uint32_t true_random_idx;
  uint32_t true_random_list[5];

  true_random_idx=0;
  while(true_random_idx<=3){
#ifdef UNIX_EMULATION
    do{
    }while(getentropy((void *)(&true_random), sizeof(uint32_t)));
#else
    true_random=jytter_true_random_get(&jytter_scratch_space);
#endif
    sha256_list[true_random_idx]=big_endian_get(true_random);
    true_random_list[true_random_idx]=true_random;
    true_random_idx++;
  }
  sha256_get(0, sha256_list);
  seed_word_idx_idx=0;
  true_random_bit_idx=32U-SEED_WORD_COUNT_LOG2;
  true_random_idx=0;
  true_random_list[4]=(sha256_list[0]>>28)<<28;
  while(seed_word_idx_idx<=11){
    if(32U<true_random_bit_idx){
      seed_word_idx1=true_random_list[true_random_idx];
      true_random_idx++;
      seed_word_idx0=true_random_list[true_random_idx];
      true_random_bit_idx+=32U;
      seed_word_idx0>>=true_random_bit_idx;
      seed_word_idx1<<=32U-true_random_bit_idx;
      seed_word_idx0+=seed_word_idx1;
    }else{
      seed_word_idx0=true_random_list[true_random_idx];
      seed_word_idx0>>=true_random_bit_idx;      
    }
    seed_word_idx0&=0x7FF;
    true_random_bit_idx-=SEED_WORD_COUNT_LOG2;
    printf("%s ",seed_word_list[seed_word_idx0]);
    seed_word_idx_idx++;
  }
  return;
}

void
true_random_seed_phrase_24_printf(void){
/*
Print out a 24-word true random seed phrase
*/
#ifndef UNIX_EMULATION
  jytter_scratch_space_t jytter_scratch_space;
#endif
  uint32_t seed_word_idx_idx;
  uint32_t seed_word_idx0;
  uint32_t seed_word_idx1;
  uint32_t seed_word_idx_list[24];
  uint32_t sha256_list[8];
  uint32_t true_random;
  uint32_t true_random_bit_idx;
  uint32_t true_random_idx;
  uint32_t true_random_list[9];

  true_random_idx=0;
  while(true_random_idx<=7){
#ifdef UNIX_EMULATION
    do{
    }while(getentropy((void *)(&true_random), sizeof(uint32_t)));
#else
    true_random=jytter_true_random_get(&jytter_scratch_space);
#endif
    sha256_list[true_random_idx]=big_endian_get(true_random);
    true_random_list[true_random_idx]=true_random;
    true_random_idx++;
  }
  sha256_get(1, sha256_list);
  seed_word_idx_idx=0;
  true_random_bit_idx=32U-SEED_WORD_COUNT_LOG2;
  true_random_idx=0;
  true_random_list[8]=(sha256_list[0]>>24)<<24;
  while(seed_word_idx_idx<=23){
    if(32U<true_random_bit_idx){
      seed_word_idx1=true_random_list[true_random_idx];
      true_random_idx++;
      seed_word_idx0=true_random_list[true_random_idx];
      true_random_bit_idx+=32U;
      seed_word_idx0>>=true_random_bit_idx;
      seed_word_idx1<<=32U-true_random_bit_idx;
      seed_word_idx0+=seed_word_idx1;
    }else{
      seed_word_idx0=true_random_list[true_random_idx];
      seed_word_idx0>>=true_random_bit_idx;      
    }
    seed_word_idx0&=0x7FF;
    true_random_bit_idx-=SEED_WORD_COUNT_LOG2;
    printf("%s ",seed_word_list[seed_word_idx0]);
    seed_word_idx_idx++;
  }
  return;
}

int
main (int argc, char *argv[]){
#ifdef UNIX_EMULATION
  printf("UNIX Emulation of ");
#endif
  printf("Jytter Demo\n\n");
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
  printf("\n\nTrue random 12-word seed phrase:\n\n");
  true_random_seed_phrase_12_printf();
  printf("\n\nTrue random 24-word seed phrase:\n\n");
  true_random_seed_phrase_24_printf();
  printf("\n");
  return 0;
}
