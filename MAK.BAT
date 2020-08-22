q main.c \demolib\convert.asm
tasm \demolib\convert.asm -m4
copy convert.obj \demolib\convert.obj
wmake
fx -15
