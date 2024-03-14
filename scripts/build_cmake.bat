@echo off

:: goto root
cd ..

:: --- build game ---
cd _build\game
:: make compile_commands.json that tells clangd
:: include dirs, files, etc.
:: cmake -G "MinGW Makefiles" . -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake -S . -G "MinGW Makefiles" -B debug   -DCMAKE_BUILD_TYPE=Debug   -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake -S . -G "MinGW Makefiles" -B release -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1
:: copy compile_commands.json to root for clangd lsp
:: copy /y compile_commands.json ..\..\compile_commands.json	

:: - game vs19 -
:: copy /y CMakeLists.txt ..\game_vs19\CMakeLists.txt
cd ..\game_vs19
:: cmake -S ..\game -B . -G "Visual Studio 16 2019"  
cmake -S..\game -B . -G "Visual Studio 17 2022"  

:: --- build editor ---
cd ..\editor

:: make compile_commands.json that tells clangd
:: include dirs, files, etc.
cmake -S . -G "MinGW Makefiles" -B debug   -DCMAKE_BUILD_TYPE=Debug   -DCMAKE_EXPORT_COMPILE_COMMANDS=1
cmake -S . -G "MinGW Makefiles" -B release -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1
:: copy compile_commands.json to root for clangd lsp
:: copy /y release\compile_commands.json ..\..\..\compile_commands.json	
:: copy /y debug\compile_commands.json ..\..\..\compile_commands.json	
:: -DGAME:STRING=sandbox

:: - editor vs19 -
:: copy /y CMakeLists.txt ..\editor_vs19\CMakeLists.txt
cd ..\editor_vs19
:: cmake -S..\editor -B . -G "Visual Studio 16 2019"  
:: cmake -S..\editor -B . -G "Visual Studio 16 2019"  
cmake -S..\editor -B . -G "Visual Studio 17 2022" 
:: -T "x86-Clang-Debug" 
:: -T "clang_cl_x86" 
:: -T "LLVM (clang-cl)"
:: -DCMAKE_CXX_COMPILER="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\bin\clang-cl.exe"
:: -T"clang-cl" 
:: -T"LLVM-vs2017" 
::-DGAME:STRING=sandbox

:: --- build tex_viewer ---

:: cd ..\tex_viewer
:: cmake -S . -G "MinGW Makefiles" -B debug   -DCMAKE_BUILD_TYPE=Debug   -DCMAKE_EXPORT_COMPILE_COMMANDS=1
:: cmake -S . -G "MinGW Makefiles" -B release -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=1
:: :: - tex_viewer vs19 -
:: cd ..\tex_viewer_vs19
:: cmake -S..\tex_viewer -B . -G "Visual Studio 16 2019"  


:: go back to root
cd ..\..
