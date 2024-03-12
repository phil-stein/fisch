@echo off

:: check if arg was given
IF "%~1"=="" GOTO no_arg

:: fisch
echo --- FISCH ---
:: if index.lock file exists git is being used by another process
:: but this keeps bugging out and i have to del it manually
:: this way might break if legitematelly used by another process 
del .git\index.lock
git add .
git commit -m %1
git push origin main

:: global
echo --- GLOBAL ---
cd src\global
del .git\index.lock
git add .
git commit -m %1
git push origin main
cd ..\..

:: math
echo --- MATH --- 
cd src\math
del .git\index.lock
git add .
git commit -m %1
git push origin main
cd ..\..

:: phys
echo --- PHYS ---
cd src\phys
del .git\index.lock
git add .
git commit -m %1
git push origin main
cd ..\..

:: serialization
echo --- SERIALIZATION ---
cd src\serialization
del .git\index.lock
git add .
git commit -m %1
git push origin main
cd ..\..

:: text
echo --- TEXT ---
cd src\text
del .git\index.lock
git add .
git commit -m %1
git push origin main
cd ..\..


GOTO end

:no_arg
echo need to pass arg for commit message, i.e. %0 "DESKTOP: ..."

:end

