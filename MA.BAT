q main.c main.err
del demo.exe
del test.lnk
tasm    bilerp -zi -m4
wmake
demo -32
