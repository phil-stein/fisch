@echo off

:: --- build game ---
cd _build\game
cmake -G "MinGW Makefiles" .

:: - game vs19 -
copy /y CMakeLists.txt ..\game_vs19\CMakeLists.txt
cd ..\game_vs19
cmake -G "Visual Studio 16 2019" ..\game_vs19 

:: --- build editor ---
cd ..\editor
:: cmake -G "MinGW Makefiles" .

:: make compile_commands.json that tells clangd
:: include dirs, files, etc.
cmake -G "MinGW Makefiles" . -DCMAKE_EXPORT_COMPILE_COMMANDS=1
:: copy compile_commands.json to root for clangd lsp
copy /y compile_commands.json ..\..\compile_commands.json	
:: -DGAME:STRING=sandbox

:: - editor vs19 -
copy /y CMakeLists.txt ..\editor_vs19\CMakeLists.txt
cd ..\editor_vs19
cmake -G "Visual Studio 16 2019" ..\editor_vs19 
::-DGAME:STRING=sandbox

:: --- build tex_viewer ---

cd ..\tex_viewer
:: make compile_commands.json that tells clangd
:: include dirs, files, etc.
cmake -G "MinGW Makefiles" . -DCMAKE_EXPORT_COMPILE_COMMANDS=1
:: copy compile_commands.json to root for clangd lsp
copy /y compile_commands.json ..\..\compile_commands.json	

:: - tex_viewer vs19 -
copy /y CMakeLists.txt ..\tex_viewer_vs19\CMakeLists.txt
cd ..\tex_viewer_vs19
cmake -G "Visual Studio 16 2019" ..\tex_viewer_vs19 


:: go back to root
cd ..\..
