@echo off

:: goto root
cd ..

:: del old exe and run make
del /q _bin\editor.exe

:: set run=O 

echo "build_editor args[0]: %~1"

:: check if arg was given
IF "%~1"=="no_run" (
  :: set run=X
  :: echo run: %run%
  set no_run=fuck_batch
  GOTO debug 
) 
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

:: IF run=="X" GOTO eof
IF defined no_run (
  GOTO eof
)

:: run executable
cd _bin
@echo on
editor
@echo off
cd ..

:eof
