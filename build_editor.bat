@echo off

:: del old exe and run make
del _bin\editor.exe
cd _build\editor
make

:: copy compile_commands.json to root for clangd lsp
copy /y compile_commands.json ..\..\compile_commands.json	

:: run executable
cd ..\..\_bin
editor
cd ..
