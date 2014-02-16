
echo Compiling C-code...

gcc -m32 -Wall -O0 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c ./unix/sys_unix.c
gcc -m32 -Wall -O0 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c ./unix/sys_mach.c
gcc -m32 -Wall -O0 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c *.c


#echo Comiling Crypto library...
#gcc -m32 -Wall -O1 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c lib_tomcrypt/*.c -o crypto.o


echo Compiling NASM...

nasm -f macho qcommon_hooks.asm
nasm -f macho cmd_hooks.asm
nasm -f macho filesystem_hooks.asm
nasm -f macho punkbuster_hooks.asm
nasm -f macho xassets_hooks.asm
nasm -f macho trace_hooks.asm
nasm -f macho misc_hooks.asm
nasm -f macho scr_vm_hooks.asm
nasm -f macho g_sv_hooks.asm
nasm -f macho server_hooks.asm
nasm -f macho msg_hooks.asm

echo Linking...
gcc -m32 -no_pie -rdynamic -Wl,-ldl,-lpthread,-lm,-lstdc++ -o cod4x17a_dedrun *.o

rm *.o

./version_make_progress.sh
