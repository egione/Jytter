@echo off
echo NOTE: Ignore error messages about "Could Not Find X", and also
echo "Warning: resolving..." linker error.
del jytter.obj
del timestamp.obj
del demo.exe
nasm -D_32_ -O0 -fwin32 -ojytter.obj jytter.asm
nasm -D_32_ -O0 -fwin32 -otimestamp.obj timestamp.asm
gcc -D_32_ jytter.obj timestamp.obj -odemo.exe -O2 demo.c
