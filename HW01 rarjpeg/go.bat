@echo off
cls
make clean all
@echo on
.\rarjpeg.exe .\notexists.jpg
.\rarjpeg.exe .\empty.jpg
.\rarjpeg.exe .\non-zipjpeg.jpg
@pause
.\rarjpeg.exe .\zipjpeg.jpg
