@echo off

set "IsDebug=1"

set Includes= /Iexternal/raylib/include/

if "%IsDebug%"=="1" (
    set CommonCompilerFlags= -Od -MTd -WL -nologo /fp:fast -Gm- -GR- -EHsc- -Oi -WX -W4 -wd4245 -wd4127 -wd4311 -wd4201 -wd4100 -wd4189 -wd4505 -wd4702 -wd4244 -wd4244 -FC -Z7
    set CommonLinkerFlags= ./external/raylib/lib/debug/raylib.lib
) else (
    set CommonCompilerFlags= -O2 -MT -WL -nologo /fp:fast -Gm- -GR- -EHsc- -Oi -WX -W4 -wd4245 -wd4127 -wd4311 -wd4201 -wd4100 -wd4189 -wd4505 -wd4702 -wd4244 -wd4244 -FC -Z7
    set CommonLinkerFlags= ./external/raylib/lib/release/raylib.lib
)

set CommonLinkerFlags= %CommonLinkerFlags% opengl32.lib gdi32.lib User32.lib shell32.lib winmm.lib

echo Building game...
cl %Includes% %CommonCompilerFlags% main.c /Fe:main.exe /link /NODEFAULTLIB:MSVCRT /SUBSYSTEM:CONSOLE -incremental:no -opt:ref %CommonLinkerFlags%

endlocal
