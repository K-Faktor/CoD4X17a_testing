@echo off

REM Compiling: debugging
REM echo `gcc -g -m32 -Wall -O1 -s -fvisibility=hidden -mtune=core2 -c *.c`

echo Compiling: release
gcc -m32 -Wall -O1 -s -fvisibility=hidden -mtune=core2 -c *.c

echo Linking
gcc -m32 -s -shared -fvisibility=hidden -o censor.dll *.o -L..\ -lcom_plugin
echo Cleaning up
del *.o

pause