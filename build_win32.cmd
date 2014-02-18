
@echo off

echo Compiling C-code...
REM gcc -m32  -Wall -O0 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c ./unix/sys_unix.c
REM gcc -m32  -Wall -O0 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c ./unix/sys_mach.c
REM gcc -m32  -Wall -O0 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c ./unix/sys_cod4linker_mach.c
gcc -m32  -Wall -O0 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c *.c


echo Compiling NASM...
nasm -f coff qcommon_hooks.asm --prefix _
nasm -f coff cmd_hooks.asm --prefix _
nasm -f coff filesystem_hooks.asm --prefix _
nasm -f coff xassets_hooks.asm --prefix _
nasm -f coff trace_hooks.asm --prefix _
nasm -f coff misc_hooks.asm --prefix _
nasm -f coff scr_vm_hooks.asm --prefix _
nasm -f coff g_sv_hooks.asm --prefix _
nasm -f coff server_hooks.asm --prefix _
nasm -f coff msg_hooks.asm --prefix _

echo Linking...
gcc -m32 -no_pie -stdlib=libc++ -rdynamic -Wl,-ldl,-lpthread,-lm -o cod4x17a_dedrun *.o -L./ -ltomcrypt_mach -ltommath_mach

REM rm *.o
pause
REM ./version_make_progress.sh
