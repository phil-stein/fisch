@echo off

:: check if arg was given
IF "%~1"=="" GOTO no_arg


:: global
cd src\global
git add .
git commit -m %1
cd ..\..

:: math
cd src\math
git add .
git commit -m %1
cd ..\..

:: phys
cd src\phys
git add .
git commit -m %1
cd ..\..

:: serialization
cd src\serialization
git add .
git commit -m %1
cd ..\..


GOTO end

:no_arg
echo need to pass arg for commit message, i.e. %0 "DESKTOP: ..."

:end

