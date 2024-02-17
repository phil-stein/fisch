@echo off

del _bin\tex_viewer.exe
cd _build\tex_viewer
make
cd ..\..\_bin\tex_viewer
tex_viewer
cd ..\..
