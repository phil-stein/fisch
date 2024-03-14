@echo off

:: check if arg was given
IF "%~1"=="" GOTO no_arg

echo commit message: "DESKTOP: from fisch, %*"

:: fisch
echo --- FISCH ---
:: if index.lock file exists git is being used by another process
:: but this keeps bugging out and i have to del it manually
:: this way might break if legitematelly used by another process 
del /q .git\index.lock
git add .
git commit -m "DESKTOP: from fisch, %*"
git push origin main

:: global
echo --- GLOBAL ---
del /q .git\modules\src\global\index.lock
cd src\global
git add .
git commit -m "DESKTOP: from fisch, %*"
git push origin main
cd ..\..

:: math
echo --- MATH --- 
del /q .git\modules\src\math\index.lock
cd src\math
git add .
git commit -m "DESKTOP: from fisch, %*"
git push origin main
cd ..\..

:: phys
echo --- PHYS ---
del /q .git\modules\src\phys\index.lock
cd src\phys
del /q .git\index.lock
git add .
git commit -m "DESKTOP: from fisch, %*"
git push origin main
cd ..\..

:: serialization
echo --- SERIALIZATION ---
del /q .git\modules\src\serialization\index.lock
cd src\serialization
git add .
git commit -m "DESKTOP: from fisch, %*"
git push origin main
cd ..\..

:: text
echo --- TEXT ---
del /q .git\modules\src\text\index.lock
cd src\text
git add .
git commit -m %1
git push origin main
cd ..\..


GOTO end

:no_arg
echo need to pass arg for commit message, i.e. %0 added x, removed y 

:end

