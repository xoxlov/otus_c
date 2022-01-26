@echo off

cls
@rm *out*.*
make clean all

@echo on
.\decode.exe .\cp1251.txt cp-1251 .\cp1251out.txt
.\decode.exe .\iso-8859-5.txt iso8859-5 .\iso-8859-5out.txt
.\decode.exe .\koi8.txt koi8-r .\koi8out.txt
