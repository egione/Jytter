echo NOTE: Ignore error messages about \"cannot remove X\"
rm jytter.o
rm timestamp.o
rm demo
nasm -D_32_ -O0 -felf32 -ojytter.o jytter.asm
nasm -D_32_ -O0 -felf32 -otimestamp.o timestamp.asm
gcc -D_32_ jytter.o timestamp.o -odemo -O2 demo.c

