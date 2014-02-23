@echo off

REM Compiling: debugging
REM echo `gcc -g -m32 -Wall -O1 -s -fvisibility=hidden -mtune=core2 -c *.c`

REM Compiling: release
gcc -m32 -Wall -O1 -s -fvisibility=hidden -mtune=core2 -c *.c

REM Linking
gcc -m32 -s -shared -fvisibility=hidden -o "censor.dll" *.o

REM Cleaning up
del *.o

pause