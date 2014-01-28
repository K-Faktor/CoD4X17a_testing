#-fvisibility=hidden -mfpmath=sse -march=corei7 -march=atom -march=amdfam10 -march=core2 -fno-omit-frame-pointer -mtune=prescott -march=core2 -mcx16 -msahf -mpopcnt -msse4.2 --param l1-cache-size=32 --param l1-cache-line-size=64 --param l2-cache-size=256
echo Compiling C-code...
gcc -m32 -Wall -O0 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c *.c

#echo Comiling Crypto library...
#gcc -m32 -Wall -O1 -g -fno-omit-frame-pointer -mtune=prescott -Ilib_tomcrypt/headers -Ilib_tomcrypt/math/tommath -c lib_tomcrypt/*.c -o crypto.o | more


echo Compiling NASM...

nasm -f elf qcommon_hooks.asm
nasm -f elf cmd_hooks.asm
nasm -f elf cvar_hooks.asm
nasm -f elf filesystem_hooks.asm
nasm -f elf sys_thread_hooks.asm
nasm -f elf punkbuster_hooks.asm
nasm -f elf sys_main_hooks.asm
nasm -f elf xassets_hooks.asm
nasm -f elf trace_hooks.asm
nasm -f elf misc_hooks.asm
nasm -f elf scr_vm_hooks.asm
nasm -f elf g_sv_hooks.asm
nasm -f elf server_hooks.asm
nasm -f elf msg_hooks.asm

echo Linking...
gcc -m32 -Wl,-ldl,-lpthread,-lm,-Tlinkerscript.ld,--dynamic-list=pluginExports.ld -o cod4x17a_dedrun *.o -L./ -ltomcrypt -ltommath

rm *.o

./version_make_progress.sh
