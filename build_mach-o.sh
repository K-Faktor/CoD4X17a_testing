
echo Compiling C-code...
gcc -m32  -Wall -O0 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c ./unix/sys_unix.c
gcc -m32  -Wall -O0 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c ./unix/sys_mach.c
gcc -m32  -Wall -O0 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c ./unix/sys_cod4linker_mach.c
gcc -m32  -Wall -O0 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c *.c


echo Compiling NASM...
nasm -f macho qcommon_hooks.asm --prefix _
nasm -f macho cmd_hooks.asm --prefix _
nasm -f macho filesystem_hooks.asm --prefix _
nasm -f macho punkbuster_hooks.asm --prefix _
nasm -f macho xassets_hooks.asm --prefix _
nasm -f macho trace_hooks.asm --prefix _
nasm -f macho misc_hooks.asm --prefix _
nasm -f macho scr_vm_hooks.asm --prefix _
nasm -f macho g_sv_hooks.asm --prefix _
nasm -f macho server_hooks.asm --prefix _
nasm -f macho msg_hooks.asm --prefix _

echo Linking...
g++ -m32 -no_pie -stdlib=libc++ -rdynamic -Wl,-ldl,-lpthread,-lm -o cod4x17a_dedrun *.o -L./ -ltomcrypt_mach -ltommath_mach

#rm *.o

./version_make_progress.sh
