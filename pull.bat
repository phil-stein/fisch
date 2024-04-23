@echo off


:: fisch
echo --- FISCH ---
git pull origin main

:: global
echo --- GLOBAL ---
cd src\global
git pull origin main
cd ..\..

:: math
echo --- MATH --- 
cd src\math
git pull origin main
cd ..\..

:: phys
echo --- PHYS ---
cd src\phys
del /q .git\index.lock
git pull origin main
cd ..\..

:: serialization
echo --- SERIALIZATION ---
cd src\serialization
git pull origin main
cd ..\..

:: text
echo --- TEXT ---
cd src\text
git pull origin main
cd ..\..


