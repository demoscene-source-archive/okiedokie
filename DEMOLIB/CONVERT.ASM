;craft / fudge

ideal
p386
model	flat,c

jumps

global  con24to15:near

 global  con15t15:near
global  con15t24:near
global  con15t32:near
 global  con24t15:near
 global  con24t24:near
global  con24t32:near
 global  con32t15:near
global  con32t24:near
 global  con32t32:near

 global  con15:near
 global  con24:near
 global  con32:near



pixelstocopy=(320*200)

dataseg

codeseg

proc    con24to15 src:dword, dest:dword

macro   inner off1
        mov     ebx,[esi+eax*4+off1*4]
        shr     bh,3
        mov     ch,[esi+2+eax*4+off1*4]
        shr     ebx,3
   mov  edx,[esi+eax*4+off1*4+4]
        shr     ch,1
        and     ebx,31*33
   shr     dh,3
        and     ecx,31*32*32
   shl     edx,16-3
    mov [esi+eax*4+off1*4],ebp
        add     ebx,ecx
   mov     ch,[esi+2+eax*4+off1*4+4]
   shl     ecx,16-1
   and     edx,31*33*65536
;        shl  ebx,16
   and     ecx,31*32*32*65536
     add        ebx,edx
    mov [esi+eax*4+off1*4+4],ebp
     add        ebx,ecx

        mov     [edi+eax*2+off1*2],ebx
endm

        pushad

        mov     esi,[src]
        mov     edi,[dest]
        add     esi,64000*4-32
        add     edi,64000*2-16
        mov     eax,-64000+16
        xor     ebp,ebp
@@loop:
        mov     ebx,[esi]
        nop
        nop
        nop
        mov     ebx,[esi+32]
        nop
        inner   0*2
        inner   1*2
        inner   2*2
        inner   3*2
        inner   4*2
        inner   5*2
        inner   6*2
        inner   7*2
;        add     esi,8*4
;        add     edi,8*2
        add     eax,16
        jl      @@loop

        popad
        ret
endp


proc    con15t15 src:dword, dest:dword

macro   inner offset1
        mov     ebx,[esi+offset1*4]
        mov     [edi+offset1*4],ebx
endm

        pushad

        mov     esi,[src]
        mov     edi,[dest]
        mov     ecx,pixelstocopy

@@loop:
        inner   0
        inner   1
        inner   2
        inner   3
        inner   4
        inner   5
        inner   6
        inner   7

        add     esi,32
        add     edi,32
        sub     ecx,16
        jg      @@loop

        popad
        ret
endp

proc    con15t32 src:dword, dest:dword         ;ikke testet/f‘rdig

macro   inner offset1
        mov     ebx,[esi+offset1*4]
        and     ebx,65535
        mov     ecx,ebx
        shl     ecx,9
        mov     edx,ebx
        shl     edx,6
        and     edx,65535
        add     edx,ecx
        shl     ebx,3
        mov     dl,bl
        mov     [edi+offset1*4],edx

endm

        pushad

        mov     esi,[src]
        mov     edi,[dest]
        mov     ecx,pixelstocopy

@@loop:
        inner   0
        inner   1
        inner   2
        inner   3
        inner   4
        inner   5
        inner   6
        inner   7

        add     esi,32
        add     edi,32
        sub     ecx,16
        jg      @@loop

        popad
        ret
endp

proc    con24t15 src:dword, dest:dword





macro   inner offset1
        mov     bl,[esi+offset1*6]
        shl     ebx,5
        mov     cl,[esi+offset1*6+3]
        shl     ecx,5
        mov     bl,[esi+offset1*6+1]
        shl     ebx,5
        mov     cl,[esi+offset1*6+1+3]
        shl     ecx,5
        mov     bl,[esi+offset1*6+2]
        shr     ebx,3
        mov     cl,[esi+offset1*6+2+3]
        shl     ecx,13
        and     ebx,32767
        add     ebx,ecx
        xor     ecx,ecx
        mov     [edi+offset1*4],ebx





;        mov     bh,[esi+offset1*3]        ;bh rrrrrrrr
;
;        shr     bh,3                      ;bh 000rrrrr
;        mov     dl,[esi+offset1*3+2]      ;dl bbbbbbbb
;        mov     bl,[esi+offset1*3+1]      ;bl gggggggg
;        and     edx,31
;        shl     ebx,2
;
;        and     ebx,0111111111100000b
;
;        or      edx,ebx
;
;        mov     [edi+offset1*4],edx
endm

        pushad

        mov     esi,[src]
        mov     edi,[dest]
        mov     ebp,pixelstocopy

@@loop:
        inner   0
        inner   1
        inner   2
        inner   3
        inner   4
        inner   5
        inner   6
        inner   7

        add     esi,3*16
        add     edi,2*16
        sub     ebp,16
        jg      @@loop

        popad
        ret
endp




proc    con32t15 src:dword, dest:dword

macro   inner off1
        mov     ebx,[esi+eax*4+off1*4]
        shr     bh,3
        mov     ch,[esi+2+eax*4+off1*4]
        shr     ebx,3
   mov  edx,[esi+eax*4+off1*4+4]
        shr     ch,1
        and     ebx,31*33
   shr     dh,3
        and     ecx,31*32*32
   shl     edx,16-3
    mov [esi+eax*4+off1*4],ebp
        add     ebx,ecx
   mov     ch,[esi+2+eax*4+off1*4+4]
   shl     ecx,16-1
   and     edx,31*33*65536
;        shl  ebx,16
   and     ecx,31*32*32*65536
     add        ebx,edx
    mov [esi+eax*4+off1*4+4],ebp
     add        ebx,ecx

        mov     [edi+eax*2+off1*2],ebx
endm

        pushad

        mov     esi,[src]
        mov     edi,[dest]
        add     esi,64000*4-32
        add     edi,64000*2-16
        mov     eax,-64000+16
        xor     ebp,ebp
@@loop:
        mov     ebx,[esi]
        nop
        nop
        nop
        mov     ebx,[esi+32]
        nop
        inner   0*2
        inner   1*2
        inner   2*2
        inner   3*2
        inner   4*2
        inner   5*2
        inner   6*2
        inner   7*2
;        add     esi,8*4
;        add     edi,8*2
        add     eax,16
        jl      @@loop

        popad
        ret
endp

proc    con24t24 src:dword, dest:dword

macro   inner offset1
        mov     ebx,[esi+offset1*4]
        mov     [edi+offset1*4],ebx
endm

        pushad

        mov     esi,[src]
        mov     edi,[dest]
        mov     ecx,(pixelstocopy*3)/2

@@loop:
        inner   0
        inner   1
        inner   2
        inner   3
        inner   4
        inner   5
        inner   6
        inner   7

        add     esi,32
        add     edi,32
        sub     ecx,16
        jg      @@loop

        popad
        ret
endp

proc    con32t32 src:dword, dest:dword

macro   inner offset1
        mov     ebx,[esi+offset1*4]
        mov     [edi+offset1*4],ebx
endm

        pushad

        mov     esi,[src]
        mov     edi,[dest]
        mov     ecx,pixelstocopy*2

@@loop:
        inner   0
        inner   1
        inner   2
        inner   3
        inner   4
        inner   5
        inner   6
        inner   7

        add     esi,32
        add     edi,32
        sub     ecx,16
        jg      @@loop

        popad
        ret
endp




proc    con32 src:dword, dest:dword, n:dword
macro   inner offset1
        mov     ebx,[esi+offset1*4]
        mov     [edi+offset1*4],ebx

endm

        pushad

        mov     esi,[src]
        mov     edi,[dest]
        mov     ebp,[n]
;        shr     ebp,4

@@loop:
        inner   0
        inner   1
        inner   2
        inner   3
        inner   4
        inner   5
        inner   6
        inner   7
        inner   8
        inner   9
        inner   10
        inner   11
        inner   12
        inner   13
        inner   14
        inner   15

        add     esi,32*2
        add     edi,32*2
        sub     ebp,16
        jg      @@loop

        popad
        ret
endp

proc    con24 src:dword, dest:dword, n:dword
macro   inner offset1
        mov     ecx,[esi+offset1*16+4]
        mov     ebx,[esi+offset1*16]
        shl     ecx,24
        and     ebx,256*256*256-1
        add     ecx,ebx
        mov     ebx,[esi+offset1*16+4]
        mov     [edi+offset1*12],ecx
        shr     ebx,8
        mov     ecx,[esi+offset1*16+8]
        shl     ecx,16
        mov     cx,bx
        mov     ebx,[esi+offset1*16+12]
        shl     ebx,8
        mov     [edi+offset1*12+4],ecx
        mov     bl,[esi+offset1*16+10]
        mov     [edi+offset1*12+8],ebx
endm

        pushad

        mov     esi,[src]
        mov     edi,[dest]
        mov     ebp,[n]
;        shr     ebp,5

@@loop:
        inner   0
        inner   1
        inner   2
        inner   3
        inner   4
        inner   5
        inner   6
        inner   7

        add     esi,16*8
        add     edi,12*8
        sub     ebp,32
        jg      @@loop

        popad
        ret
endp

proc    con15 src:dword, dest:dword, n:dword
macro   inner off1
        mov     ebx,[esi+off1*8]
        shr     bh,3
        mov     ch,[esi+2+off1*8]
        shr     ebx,3
   mov  edx,[esi+off1*8+4]
        shr     ch,1
        and     ebx,31*33
   shr     dh,3
        and     ecx,31*32*32
   shl     edx,16-3
        add     ebx,ecx
   mov     ch,[esi+2+off1*8+4]
   shl     ecx,16-1
   and     edx,31*33*65536
   and     ecx,31*32*32*65536
     add        ebx,edx
     add        ebx,ecx

        mov     [edi+off1*4],ebx
endm

        pushad

        mov     esi,[src]
        mov     edi,[dest]
        mov     ebp,[n]
;        shr     ebp,4

@@loop:
        inner   0
        inner   1
        inner   2
        inner   3
        inner   4
        inner   5
        inner   6
        inner   7

        add     esi,8*8
        add     edi,4*8
        sub     ebp,16
        jg      @@loop

        popad
        ret
endp





end
------------------------------------------

proc    con24t32 src:dword, dest:dword
;rgbrgbrgbrgb
;rgb rgb rgb rgb
macro   inner offset1
        mov     ebx,[esi+offset1*4]
        mov     [edi+offset1*4],ebx
        mov     ecx,[esi+offset1*4]
        shr     ebx,24
        mov     edx,ecx
        shl     edx,8
        mov     eax,[esi+offset1*4]
        add     ebx,edx
        mov     edx,eax
        shl     eax,16
        mov     [edi+offset1*4],ebx
        mov

endm

        pushad

        mov     esi,[src]
        mov     edi,[dest]
        mov     ebp,pixelstocopy*2

@@loop:
        inner   0
        inner   1
        inner   2
        inner   3
        inner   4
        inner   5
        inner   6
        inner   7

        add     esi,32
        add     edi,32
        sub     ebp,16
        jg      @@loop

        popad
        ret
endp

