@echo off

del _bin\editor.exe
cd _build\editor
make
cd ..\..\_bin
:: idk gets deleted somewhere 
copy ..\_libs\win64\dlls\libflecs.dll .\libflecs.dll
editor
cd ..
