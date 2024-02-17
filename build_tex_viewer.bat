@echo off

:: del old exe and run make
del _bin\tex_viewer.exe
cd _build\tex_viewer
make

:: copy compile_commands.json to root for clangd lsp
copy /y compile_commands.json ..\..\compile_commands.json	

:: run executable
cd ..\..\_bin\tex_viewer
tex_viewer
cd ..\..
