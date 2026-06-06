echo NOTE: Ignore error messages about \"cannot remove X\"
rm jytter.o
rm timestamp.o
rm demo
gcc -D_64_ -DUNIX_EMULATION -odemo -O2 demo.c
