@echo off

:init
if "%1" == "" (goto no_arg_given) else (set filename=%1)
if "%2" == "" (set start=Start) else (set start=%2)

:compiling
nasm -f win32 %filename%.asm -o %filename%.obj
if %errorlevel% == 0 (goto linking) else (goto done)

:linking
golink /entry:%start% /console kernel32.dll user32.dll %filename%.obj
del %filename%.obj
goto done

:no_arg_given
echo No argument given
goto done

:done