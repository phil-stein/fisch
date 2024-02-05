@echo off

del _bin\game.exe
cd _build\game
make
cd ..\..\_bin
game
cd ..

