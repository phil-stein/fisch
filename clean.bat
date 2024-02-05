@echo off

:: cd _build
:: make clean
:: cd ..

:: rmdir /s /q _bin
:: mkdir _bin

cd _build\game
make clean
cd ..\editor
make clean
cd ..\..
