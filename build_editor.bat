@echo off

del _bin\editor.exe
cd _build\editor
make
cd ..\..\_bin
editor
cd ..
