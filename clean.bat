@echo off

:: cd _build
:: make clean
:: cd ..

:: rmdir /s /q _bin
:: mkdir _bin

cd _build\game\release
make clean
echo game release
cd ..\debug
make clean
echo game debug 

cd ..\..\editor\release
make clean
echo editor release
cd ..\debug
make clean
echo editor debug 

cd ..\..\..
