: (C) alexander.xoxlov@gmail.com, 2021
: for Windows: MinGW installed required! Also path should be registered
@set COUNT=0

@set /a COUNT=%COUNT%+1
@echo --------------------------------------------------------------------------
@echo step #%COUNT%: just build the executable and run it
@rm -f ./a.exe ./a.out
gcc sample.c
.\a.exe

@set /a COUNT=%COUNT%+1
@echo.
@echo --------------------------------------------------------------------------
@echo step #%COUNT%: debug information investigation
: http://staff.mmcs.sfedu.ru/~ulysses/IT/C++/using_gcc.html
@rm -f ./smp*.exe
@gcc sample.c -o smp1.exe
: gdb<0..3>
@gcc -ggdb0 sample.c -o smp2.exe
@gcc -ggdb1 sample.c -o smp3.exe
@gcc -ggdb2 sample.c -o smp4.exe
@gcc -ggdb3 sample.c -o smp5.exe
: strip debug information
@gcc -s sample.c -o smp6.exe
ls -l | grep 'smp*' | awk '{printf $9 ": " $5 " B\n"}'
: following line doesn't work in windows command line for unknown reason
: ls -l | grep 'smp*' | awk '{printf "%-10s: %5s B\n", $9, $5}'

@set /a COUNT=%COUNT%+1
@echo.
@echo --------------------------------------------------------------------------
@echo step #%COUNT%: optimization investigation
: http://staff.mmcs.sfedu.ru/~ulysses/IT/C++/using_gcc.html
@rm -f ./opt*.exe
@gcc -O0 sample.c -o opt1.exe
@gcc -O1 sample.c -o opt2.exe
@gcc -O2 sample.c -o opt3.exe
@gcc -O3 sample.c -o opt4.exe
@gcc -Os sample.c -o opt5.exe
ls -l | grep 'opt*' | awk '{printf $9 ": " $5 " B\n"}'
: following line doesn't work in windows command line for unknown reason
: ls -l | grep 'smp*' | awk '{printf "%-10s: %5s B\n", $9, $5}'

@set /a COUNT=%COUNT%+1
@echo.
@echo --------------------------------------------------------------------------
@echo step #%COUNT%: build step-by-step to get intermediate files
: https://narodstream.ru/c-urok-1-nasha-pervaya-programma-chast-2/
@rm -f sample.i sample.s sample.o sample.exe sample
: preprocessing
gcc -E sample.c > sample.i
: translation
gcc -S sample.i -o sample.s
: compilation
gcc -c sample.s -o sample.o
: linking (in fact 'ld')
gcc sample.o -o sample.exe
