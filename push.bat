@echo off

:: check if arg was given
IF "%~1"=="" GOTO no_arg

:: fisch
echo --- FISCH ---
git add .
git commit -m %1
git push origin main

:: global
echo --- GLOBAL ---
cd src\global
git add .
git commit -m %1
git push origin main
cd ..\..

:: math
echo --- MATH --- 
cd src\math
git add .
git commit -m %1
git push origin main
cd ..\..

:: phys
echo --- PHYS ---
cd src\phys
git add .
git commit -m %1
git push origin main
cd ..\..

:: serialization
echo --- SERIALIZATION ---
cd src\serialization
git add .
git commit -m %1
git push origin main
cd ..\..


GOTO end

:no_arg
echo need to pass arg for commit message, i.e. %0 "DESKTOP: ..."

:end

