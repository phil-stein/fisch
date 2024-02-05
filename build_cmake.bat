@echo off

:: --- build game ---
cd _build\game
cmake -G "MinGW Makefiles" .

:: --- game vs19 ---
copy /y CMakeLists.txt ..\game_vs19\CMakeLists.txt
cd ..\game_vs19
cmake -G "Visual Studio 16 2019" ..\game_vs19 

:: --- build editor ---
cd ..\editor
:: cmake -G "MinGW Makefiles" .
cmake -G "MinGW Makefiles" . 
:: -DGAME:STRING=sandbox

:: --- editor vs19 ---
copy /y CMakeLists.txt ..\editor_vs19\CMakeLists.txt
cd ..\editor_vs19
cmake -G "Visual Studio 16 2019" ..\editor_vs19 
::-DGAME:STRING=sandbox

cd ..\..
