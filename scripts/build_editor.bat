@echo off

:: goto root
cd ..

:: del old exe and run make
del /q _bin\editor.exe

:: check if arg was given
IF "%~1"=="" GOTO debug 

cd _build\editor\release
echo building RELEASE 
make
GOTO after_make

:debug
cd _build\editor\debug
echo building DEBUG 
make

:after_make

:: copy compile_commands.json to root for clangd lsp
copy /y compile_commands.json ..\..\..\compile_commands.json	
cd ..\..\..

:: run executable
cd _bin
@echo on
editor
@echo off
cd ..
