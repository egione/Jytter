echo NOTE: Ignore error messages about \"cannot remove X\"
rm jytter.o
rm timestamp.o
rm demo
nasm -D_64_ -O0 -fmacho64 -ojytter.o jytter.asm
nasm -D_64_ -O0 -fmacho64 -otimestamp.o timestamp.asm
gcc -D_64_ jytter.o timestamp.o -odemo -O2 demo.c

