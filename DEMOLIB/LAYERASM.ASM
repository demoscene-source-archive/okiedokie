;craft / fudge

ideal
p386
model	flat,c

offset_source        =0
offset_dest          =4
offset_sourcemodulo  =8
offset_destmodulo    =12
offset_sizex         =16
offset_sizey         =20
offset_type          =24
offset_r             =28
offset_next          =32

jumps

global  layer_drawasm:near
global  layer_initasm:near
global  layer_hblurasm:near
global  layer_hblurasmfast:near
global  layer_multable


dataseg


smod    dd      0
dmod    dd      0
sizex   dd      0
mixr    dd      0
fptemp  dd      0

sourceadd       dd      0
destadd         dd      0

b       dd      0
g       dd      0
r       dd      0
a       dd      0
l_multab  dd      0

l_temp    dd      4096 dup (?)


align 32
;        dd      ?
layer_multable db     256*257 dup (?)
fpmagicdouble	dq     4338000000000000h
fpmagicdouble2  dq     4338000000000000h
fptemp2         dq      0
fpshift         dd      0

magic   dd      59c00000h
magicd256 dd    55c00000h
fp256   dd      43800000h


tab1            db      256 dup (0)
tab2            db      256 dup (0)

align 32
;        dd      ?

convert dq      42b1000000000000h
convert_r=byte ptr convert
convert_g=byte ptr convert+2
convert_b=byte ptr convert+4
convert2 dq      42b1000000000000h
convert2_r=byte ptr convert2
convert2_g=byte ptr convert2+2
convert2_b=byte ptr convert2+4
convert3 dq      42b1000000000000h
convert3_r=byte ptr convert3
convert3_g=byte ptr convert3+2
convert3_b=byte ptr convert3+4

tointtab        dd      256*2 dup (?)

thecolor        dd      0

codeseg

proc    layer_initasm
        pushad

        mov     ecx,0
@@loop:
        mov     [fptemp],ecx
        fild    [fptemp]
        fld     [fp256]
        fsub    st(0),st(1)
        fstp    [tointtab+ecx*8]
        fstp    [tointtab+ecx*8+4]
        inc     ecx
        cmp     ecx,256
        jne     @@loop

        popad
        ret
endp

proc    layer_drawasm l:dword
        pushad

        mov     ebx,[l]
@@l:
        cmp     ebx,0
        je      @@ud

        mov     esi,[ebx+offset_source]
        mov     edi,[ebx+offset_dest]
        mov     ecx,[ebx+offset_sourcemodulo]
        mov     edx,[ebx+offset_destmodulo]
        mov     [smod],ecx
        mov     [dmod],edx
        mov     edx,[ebx+offset_type]
        mov     eax,[ebx+offset_sizey]
        mov     ecx,[ebx+offset_sizex]
        push    [dword ptr ebx+offset_next]
        mov     [sizex],ecx
        mov     ecx,[ebx+offset_r]
        mov     [mixr],ecx

        jmp     [dword ptr @@jlist+edx*4]

;************ Clear *************
@@j0:
        cmp     eax,0
        je      @@next
@@loop0a:
        push    eax
        mov     ecx,[sizex]
        mov     eax,[mixr]

        cmp     ecx,17
        jl      @@loop0b
@@loop0c:
        mov     [edi],eax
        mov     [edi+4],eax
        mov     [edi+8],eax
        mov     [edi+12],eax
        mov     [edi+16],eax
        mov     [edi+16+4],eax
        mov     [edi+16+8],eax
        mov     [edi+16+12],eax
        mov     [edi+32],eax
        mov     [edi+32+4],eax
        mov     [edi+32+8],eax
        mov     [edi+32+12],eax
        mov     [edi+32+16],eax
        mov     [edi+32+16+4],eax
        mov     [edi+32+16+8],eax
        mov     [edi+32+16+12],eax
        add     edi,64
        sub     ecx,16
        cmp     ecx,17
        jge     @@loop0c



@@loop0b:
;        mov     eax,[esi]
        mov     [edi],eax
        add     esi,4
        add     edi,4
        dec     ecx
        jne     @@loop0b

        add     edi,[dmod]
        add     esi,[smod]

        pop     eax
        dec     eax
        jne     @@loop0a
        jmp     @@next
;************ Move **************
@@j1:
        cmp     eax,0
        je      @@next
@@loop1a:
        push    eax
        mov     ecx,[sizex]
@@loop1b:
        mov     eax,[esi]
        mov     [edi],eax
        add     esi,4
        add     edi,4
        dec     ecx
        jne     @@loop1b

        add     edi,[dmod]
        add     esi,[smod]

        pop     eax
        dec     eax
        jne     @@loop1a
        jmp     @@next
;************ Movet *************
@@j2:
        cmp     eax,0
        je      @@next
        mov     edx,[mixr]
@@loop2a:
        push    eax
        mov     ecx,[sizex]
@@loop2b:
        mov     eax,[esi]
        cmp     eax,edx
        je      @@nostore2
        mov     [edi],eax
@@nostore2:
        add     esi,4
        add     edi,4
        dec     ecx
        jne     @@loop2b

        add     edi,[dmod]
        add     esi,[smod]

        pop     eax
        dec     eax
        jne     @@loop2a
        jmp     @@next
;************ Add ***************
@@j3:
        cmp     eax,0
        je      @@next
@@loop3a:
        push    eax
        mov     ecx,[sizex]
@@loop3b:
        mov     edx,[edi]
        mov     eax,[esi]
        and     edx,0fefeffh
        and     eax,0fefeffh
        add     eax,edx
        mov     edx,eax
        and     eax,001010100h
        mov     ebx,eax
        shr     eax,7
        sub     ebx,eax
        or      ebx,edx
        mov     [edi],ebx
        add     esi,4
        add     edi,4
        dec     ecx
        jne     @@loop3b

        add     edi,[dmod]
        add     esi,[smod]

        pop     eax
        dec     eax
        jne     @@loop3a
        jmp     @@next
;************ Addl **************
@@j4:
        cmp     eax,0
        je      @@next

        fild    [mixr]
        fld     [magicd256]
        fmul    st(0),st(1)
        fsubr   [magic]
@@loop4a:
        push    eax
        mov     ecx,[sizex]
@@loop4b:
        mov     edx,[esi]
        mov     [convert_r],dl
        mov     [convert_g],dh
        shr     edx,16
        mov     [convert_b],dl
        fld     [convert]
        fmul    st(0),st(2)
        fadd    st(0),st(1)
        fstp    [convert2]
        mov     dl,[convert2_b]
        shl     edx,16
        mov     dh,[convert2_g]
        mov     dl,[convert2_r]

        mov     eax,[edi]
        and     edx,0fefeffh
        and     eax,0fefeffh
        add     eax,edx
        mov     edx,eax
        and     eax,001010100h
        mov     ebx,eax
        shr     eax,7
        sub     ebx,eax
        or      ebx,edx
        mov     [edi],ebx
        add     esi,4
        add     edi,4
        dec     ecx
        jne     @@loop4b

        add     edi,[dmod]
        add     esi,[smod]

        pop     eax
        dec     eax
        jne     @@loop4a
        fstp    [fptemp]
        fstp    [fptemp]
        jmp     @@next
;************ Amix **************
@@j9:
        cmp     eax,0
        je      @@next

        mov     edx,[sizex]
        shl     edx,2
        mov     ecx,[dmod]
        add     ecx,edx
        mov     [destadd],ecx

        mov     ecx,[smod]
        add     ecx,edx
        mov     [sourceadd],ecx


        mov     ecx,[sizex]
        lea     esi,[esi+ecx*4]
        lea     edi,[edi+ecx*4]

@@loop9a:
        push    eax
        mov     ecx,[sizex]
        neg     ecx
        mov     dl,[esi+3+ecx*4]
@@loop9b:
        cmp     dl,0
        je      @@black9
        cmp     dl,255
        je      @@white9
        mov     edx,[esi+ecx*4]
        mov     eax,[edi+ecx*4]
        mov     [convert_r],dl
        mov     [convert3_r],al
        mov     [convert_g],dh
        mov     [convert3_g],ah
        shr     edx,16

        shr     eax,16
        mov     [convert_b],dl
        shr     edx,8
        mov     [convert3_b],al
        fld     [convert]
        fmul    [tointtab+edx*8+4]
        fld     [convert3]
        fmul    [tointtab+edx*8]


;        add     esi,4
;        add     edi,4

        fadd
        fstp    [convert2]
        mov     dl,[convert2_b]
        shl     edx,16
        mov     dh,[convert2_g]
        mov     dl,[convert2_r]

        mov     [edi+ecx*4],edx
        mov     dl,[esi+3+4+ecx*4]
        inc     ecx
        jl      @@loop9b

        add     edi,[destadd]
        add     esi,[sourceadd]

        pop     eax
        dec     eax
        jne     @@loop9a
        jmp     @@next
@@white9:
        mov     edx,[esi+ecx*4]
        and     edx,256*256*256-1
        mov     [edi+ecx*4],edx
@@black9:
;        add     esi,4
;        add     edi,4
        mov     dl,[esi+3+4+ecx*4]
        inc     ecx
        jl      @@loop9b

        add     edi,[destadd]
        add     esi,[sourceadd]


        pop     eax
        dec     eax
        jne     @@loop9a
        jmp     @@next

;@@j9:
;        cmp     eax,0
;        je      @@next
;
;@@loop9a:
;        push    eax
;        mov     ecx,[sizex]
;@@loop9b:
;        mov     edx,[esi]
;        mov     [convert_r],dl
;        mov     [convert_g],dh
;        shr     edx,16
;        mov     [convert_b],dl
;        shr     edx,8
;        fld     [convert]
;        fmul    [tointtab+edx*8+4]
;
;        mov     eax,[edi]
;        mov     [convert_r],al
;        mov     [convert_g],ah
;        shr     eax,16
;        mov     [convert_b],al
;        fld     [convert]
;        fmul    [tointtab+edx*8]
;
;        fadd
;        fstp    [convert2]
;        mov     dl,[convert2_b]
;        shl     edx,16
;        mov     dh,[convert2_g]
;        mov     dl,[convert2_r]
;
;        mov     [edi],edx
;        add     esi,4
;        add     edi,4
;        dec     ecx
;        jne     @@loop9b
;
;        add     edi,[dmod]
;        add     esi,[smod]
;
;        pop     eax
;        dec     eax
;        jne     @@loop9a
;        jmp     @@next
;************ Amix2 **************  Konstant alfakanal for hele billedet
@@j10:
        mov     ebx,[mixr]

        cmp     eax,0
        je      @@next

@@loop10a:
        push    eax
        mov     ecx,[sizex]
@@loop10b:
        mov     edx,[esi]
        mov     [convert_r],dl
        mov     [convert_g],dh
        shr     edx,16
        mov     [convert_b],dl
;        shr     edx,8
        fld     [convert]
        fmul    [tointtab+ebx*8+4]

        mov     eax,[edi]
        mov     [convert_r],al
        mov     [convert_g],ah
        shr     eax,16
        mov     [convert_b],al
        fld     [convert]
        fmul    [tointtab+ebx*8]

        fadd
        fstp    [convert2]
        mov     dl,[convert2_b]
        shl     edx,16
        mov     dh,[convert2_g]
        mov     dl,[convert2_r]

        mov     [edi],edx
        add     esi,4
        add     edi,4
        dec     ecx
        jne     @@loop10b

        add     edi,[dmod]
        add     esi,[smod]

        pop     eax
        dec     eax
        jne     @@loop10a
        jmp     @@next

;************ Amix+ **************    --alpha kanal i billedet plus en konstant alphakanal for hele bitmappen
@@j11:
        cmp     eax,0
        je      @@next

        mov     ebx,[mixr]
        shl     ebx,8
        and     ebx,256*255

        mov     edx,[sizex]
        shl     edx,2
        mov     ecx,[dmod]
        add     ecx,edx
        mov     [destadd],ecx

        mov     ecx,[smod]
        add     ecx,edx
        mov     [sourceadd],ecx


        mov     ecx,[sizex]
        lea     esi,[esi+ecx*4]
        lea     edi,[edi+ecx*4]

@@loop11a:
        push    eax
        mov     ecx,[sizex]
        neg     ecx
        mov     dl,[esi+3+ecx*4]
@@loop11b:
        cmp     dl,0
        je      @@black11
        mov     edx,[esi+ecx*4]
        mov     eax,[edi+ecx*4]
        mov     [convert_r],dl
        mov     [convert3_r],al
        mov     [convert_g],dh
        mov     [convert3_g],ah
        shr     edx,16

        shr     eax,16
        mov     [convert_b],dl
        shr     edx,8

        mov     dl,[layer_multable+ebx+edx]               ;mul
        mov     [convert3_b],al
        fld     [convert]
        fmul    [tointtab+edx*8+4]
        fld     [convert3]
        fmul    [tointtab+edx*8]

        fadd
        fstp    [convert2]
        mov     dl,[convert2_b]
        shl     edx,16
        mov     dh,[convert2_g]
        mov     dl,[convert2_r]

        mov     [edi+ecx*4],edx
        mov     dl,[esi+3+4+ecx*4]
        inc     ecx
        jl      @@loop11b

        add     edi,[destadd]
        add     esi,[sourceadd]

        pop     eax
        dec     eax
        jne     @@loop11a
        jmp     @@next
@@black11:
        mov     dl,[esi+3+4+ecx*4]
        inc     ecx
        jl      @@loop11b

        add     edi,[destadd]
        add     esi,[sourceadd]


        pop     eax
        dec     eax
        jne     @@loop11a
        jmp     @@next
;************ Fade **************    --crossfade til konstant farve
@@j12:
        cmp     eax,0
        je      @@next

        mov     edx,[mixr]
        shr     edx,24
        cmp     edx,255
        je      @@j1

        push    ebp

        mov     ebx,[mixr]
        shr     ebx,16
        and     ebx,256*255
        xor     ebx,256*255
        mov     edx,[mixr]

        push    eax

        mov     ebp,edx
        shr     ebp,16
        and     ebp,255
        mov     al,[layer_multable+ebp+ebx]
        shl     eax,16

        mov     ebp,edx
        shr     ebp,8
        and     ebp,255
        mov     ah,[layer_multable+ebp+ebx]
        and     edx,255
        mov     al,[layer_multable+edx+ebx]
        mov     [mixr],eax
        xor     ebx,256*255

        pop     eax

        mov     edx,[sizex]
        shl     edx,2
        mov     ecx,[dmod]
        add     ecx,edx
        mov     [destadd],ecx

        mov     ecx,[smod]
        add     ecx,edx
        mov     [sourceadd],ecx


        mov     ecx,[sizex]
        lea     esi,[esi+ecx*4]
        lea     edi,[edi+ecx*4]

@@loop12a:
        push    eax
        mov     ecx,[sizex]
        neg     ecx
        mov     dl,[esi+3+ecx*4]
        mov     ebp,[esi+ecx*4]
@@loop12b:

        mov     edx,ebp
        shr     edx,16
        and     edx,255
        mov     al,[layer_multable+edx+ebx]
        mov     [byte ptr thecolor+2],al

        mov     edx,ebp
        shr     edx,8
        and     edx,255
        mov     al,[layer_multable+edx+ebx]
        mov     [byte ptr thecolor+1],al
        and     ebp,255
        mov     al,[layer_multable+ebp+ebx]
        mov     [byte ptr thecolor],al
        mov     ebp,[esi+ecx*4+4]
        mov     eax,[mixr]
        add     eax,[thecolor]

        mov     [edi+ecx*4],eax
        inc     ecx
        jl      @@loop12b

        add     edi,[destadd]
        add     esi,[sourceadd]

        pop     eax
        dec     eax
        jne     @@loop12a
        pop     ebp
        jmp     @@next


@@next:
        pop     ebx
        jmp     @@l

@@ud:

        popad
        ret
dataseg
@@jlist:        dd      @@j0,@@j1,@@j2,@@j3,@@j4,5,6,7,8,@@j9,@@j10,@@j11,@@j12
codeseg
endp


proc    layer_hblurasm source:dword, dest:dword, y:dword, x:dword, m:dword, g1:dword, mul:dword
        pushad

        fld    [mul]
        fmul    [fpmagicdouble2]
        fsubr   [fpmagicdouble2]
        fstp    [fpshift]

        xor     eax,eax
        mov     ah,[byte ptr g1]
        lea     eax,[layer_multable+ eax]
        mov     [l_multab],eax
; jmp @@slut
        mov     ebx,[x]
        mov     esi,[source]
        mov     edi,[dest]
        mov     eax,[y]
;        shl     ebx,2
;        mov     [x],ebx
@@yloop:
        push    eax
        xor     eax,eax
        xor     ebx,ebx
   jmp @@hop
@@hop:
        mov     [r],eax
        mov     [g],eax
        mov     [b],eax
        mov     [a],eax

        lea     edx,[l_temp]

        mov     ecx,0
        cmp     ecx,[m]
        jge     @@n1
@@l1:
        mov     [edx],ebx
        mov     [edx+4],ebx
        mov     [edx+8],ebx
        mov     [edx+12],ebx
        add     edx,16

        inc     ecx
        cmp     ecx,[m]
        jne     @@l1
@@n1:

        xor     eax,eax
        push    esi
        push    edi
        mov     edi,[l_multab]
        mov     ecx,[m]
        cmp     ecx,[x]
        jge     @@n2
@@l2:
        mov     ebx,[b]
        mov     al,[esi]
        sub     ebx,eax
        mov     [b],ebx
        mov     al,[edi+eax]
        add     ebx,eax
        mov     [edx],ebx

        mov     ebx,[g]
        mov     al,[esi+1]
        sub     ebx,eax
        mov     [g],ebx
        mov     al,[edi+eax]
        add     ebx,eax
        mov     [edx+4],ebx

        mov     ebx,[r]
        mov     al,[esi+2]
        sub     ebx,eax
        mov     [r],ebx
        mov     al,[edi+eax]
        add     ebx,eax
        mov     [edx+8],ebx

        mov     ebx,[a]
        mov     al,[esi+3]
        sub     ebx,eax
        mov     [a],ebx
        mov     al,[edi+eax]
        add     ebx,eax
        mov     [edx+12],ebx

        add     esi,4
        add     edx,16
        inc     ecx
;        mov     [a],edx
        cmp     ecx,[x]
        jne     @@l2
@@n2:
        pop     edi
        pop     esi


        xor     eax,eax

        mov     ecx,0

        mov     [r],ecx
        mov     [g],ecx
        mov     [b],ecx
        mov     [a],ecx

        cmp     ecx,[m]
        jge     @@n3
@@l3:
        mov     ebx,[b]
        mov     al,[esi+4*ecx]
        add     ebx,eax
        mov     edx,[g]
        mov     [b],ebx
        mov     al,[esi+4*ecx+1]
        add     edx,eax
        mov     ebx,[r]
        mov     [g],edx
        mov     al,[esi+4*ecx+2]
        add     ebx,eax
        mov     edx,[a]
        mov     [r],ebx
        mov     al,[esi+4*ecx+3]
        add     edx,eax
        inc     ecx
        mov     [a],edx
        cmp     ecx,[m]
        jne     @@l3
@@n3:



        lea     edx,[l_temp]
        xor     eax,eax
        push    esi
        push    edi
        mov     edi,[l_multab]
        mov     ecx,[m]

        add     esi,ecx
        add     esi,ecx
        add     esi,ecx
        add     esi,ecx

        cmp     ecx,[x]
        jge     @@n4
@@l4:
        mov     ebx,[b]
        mov     eax,[edx]
        add     ebx,eax
        xor     eax,eax
        mov     al,[esi]
        mov     al,[edi+eax]
        add     eax,ebx
        mov     [edx],eax
       mov      ebx,[b]
        xor     eax,eax
        mov     al,[esi]
        add     ebx,eax
        mov     [b],ebx

        mov     ebx,[g]
        mov     eax,[edx+4]
        add     ebx,eax
        xor     eax,eax
        mov     al,[esi+1]
        mov     al,[edi+eax]
        add     eax,ebx
        mov     [edx+4],eax
       mov      ebx,[g]
        xor     eax,eax
        mov     al,[esi+1]
        add     ebx,eax
        mov     [g],ebx

        mov     ebx,[r]
        mov     eax,[edx+8]
        add     ebx,eax
        xor     eax,eax
        mov     al,[esi+2]
        mov     al,[edi+eax]
        add     eax,ebx
        mov     [edx+8],eax
       mov      ebx,[r]
        xor     eax,eax
        mov     al,[esi+2]
        add     ebx,eax
        mov     [r],ebx

        mov     ebx,[a]
        mov     eax,[edx+12]
        add     ebx,eax
        xor     eax,eax
        mov     al,[esi+3]
        mov     al,[edi+eax]
        add     eax,ebx
        mov     [edx+12],eax
       mov      ebx,[a]
        xor     eax,eax
        mov     al,[esi+3]
        add     ebx,eax
        mov     [a],ebx

        add     esi,4
        add     edx,16
        inc     ecx
;        mov     [a],edx
        cmp     ecx,[x]
        jne     @@l4
@@n4:
        pop     edi
        pop     esi

        mov     ecx,0
        cmp     ecx,[m]
        jge     @@n5
@@l5:
        mov     eax,[edx]
        mov     ebx,[b]
        add     ebx,eax
        mov     eax,[edx+4]
        mov     [edx],ebx
        mov     ebx,[g]
        add     ebx,eax
        mov     eax,[edx+8]
        mov     [edx+4],ebx
        mov     ebx,[r]
        add     ebx,eax
        mov     eax,[edx+12]
        mov     [edx+8],ebx
        mov     ebx,[a]
        add     ebx,eax
        mov     [edx+12],ebx

        add     edx,16
        inc     ecx
;        mov     [a],edx
        cmp     ecx,[m]
        jne     @@l5
@@n5:

        lea     edx,[l_temp]
        mov     ecx,0
        cmp     ecx,[x]
        jge     @@n6
@@l6:
        mov     eax,[edx]
        mov     ebx,[edx+4]
;    xor eax,eax
        add     ebx,43380000h
        mov     [dword ptr fpmagicdouble],eax
        mov     [dword ptr fpmagicdouble +4],ebx

        fld     [fpmagicdouble]
        fmul    [mul]
        fadd     [fpshift]
        fstp    [fptemp2]
        mov     bl,[byte ptr fptemp2]
        mov     [edi+0],bl
        mov     bl,[byte ptr fptemp2+4]
        mov     [edi+1],bl

        mov     eax,[edx+8]
;    xor eax,eax
        mov     ebx,[edx+12]
        add     ebx,43380000h
        mov     [dword ptr fpmagicdouble],eax
        mov     [dword ptr fpmagicdouble +4],ebx

        fld     [fpmagicdouble]
        fmul    [mul]
        fadd     [fpshift]
        fstp    [fptemp2]
        mov     bl,[byte ptr fptemp2]
        mov     [edi+2],bl
        mov     bl,[byte ptr fptemp2+4]
        mov     [edi+3],bl


        fild    [dword ptr edx]
        fmul    [mul]
        fild    [dword ptr edx+4]
        fmul    [mul]
        fild    [dword ptr edx+8]
        fmul    [mul]
        fild    [dword ptr edx+12]
        fmul    [mul]
;
        fistp   [a]
        fistp   [r]
        fistp   [g]
        fistp   [b]
;
        mov     ebx,[b]
        mov     [edi],bl
        mov     ebx,[g]
;;;;    mov bl,128
        mov     [edi+1],bl
        mov     ebx,[r]
        mov     [edi+2],bl
        mov     ebx,[a]
        mov     [edi+3],bl
;

;        mov     bl,[edx]
; mov     bl,128
;        mov     [edi+0],bl
;        mov     bl,[edx+4]
; mov     bl,128
;        mov     [edi+1],bl
;        mov     bl,[edx+8]
;        mov     [edi+2],bl
;        mov     bl,[edx+12]
;        mov     [edi+3],bl

        add     edx,16
        add     edi,4
        inc     ecx
;        mov     [a],edx
        cmp     ecx,[x]
        jne     @@l6
@@n6:



        pop     eax
        mov     ebx,[x]
        lea     esi,[esi+ebx*4]
;        add     edi,ebx
        dec     eax
        jne     @@yloop
@@slut:
        popad
        ret
endp


proc    layer_hblurasmfast source:dword, dest:dword, x:dword, mul:dword
        pushad


        mov     eax,256
        sub     eax,[mul]

        mov     ebx,[mul]

        xor     ecx,ecx
        xor     edx,edx

        mov     esi,0
@@l:
        mov     [tab2+esi],ch
        mov     [tab1+esi],dh
        add     ecx,eax
        add     edx,ebx

        inc     esi
        cmp     esi,256
        jne     @@l

        mov     esi,[source]
        mov     edi,[dest]
        mov     ebp,[x]
;        neg     ecx
;        shl     ecx,16

        xor     eax,eax
        xor     ebx,ebx
        xor     edx,edx
@@loop:
        mov     bl,cl
        mov     al,[esi]
        mov     cl,[edi]
        add     esi,4
        mov     cl,[tab2+ebx]
        mov     al,[tab1+eax]
        add     cl,al
        mov     bl,ch
        mov     al,[esi+1-4]
        add     edi,4
        mov     ch,[tab2+ebx]
        mov     bl,dl
        mov     al,[tab1+eax]

        add     ch,al
        mov     al,[esi+2-4]
        mov     dl,[tab2+ebx]
        mov     bl,dh
        nop
        mov     [edi-4],cx
        mov     al,[tab1+eax]
        add     dl,al
        mov     al,[esi+3-4]
        mov     dh,[tab2+ebx]

        mov     al,[tab1+eax]

        add     dh,al
        dec     ebp
        mov     [edi+2-4],dx
        jne      @@loop

        popad
        ret
endp

end
;@@loop:
;        mov     bl,cl
;        mov     al,[esi]
;
;        mov     cl,[tab2+ebx]
;        mov     al,[tab1+eax]
;        add     cl,al
;
;
;
;        mov     bl,ch
;        mov     al,[esi+1]
;
;        mov     ch,[tab2+ebx]
;        mov     al,[tab1+eax]
;        add     ch,al
;
;
;
;        mov     bl,dl
;        mov     al,[esi+2]
;
;        mov     dl,[tab2+ebx]
;        mov     al,[tab1+eax]
;        add     dl,al
;
;
;
;        mov     bl,dh
;        mov     al,[esi+3]
;
;        mov     dh,[tab2+ebx]
;        mov     al,[tab1+eax]
;
;        add     dh,al
;
;       mov      cx,255*257
;       mov      dx,255*257
;
;        mov     [edi],cx
;        mov     [edi+2],dx
;
;        add     edi,4
;        add     esi,4
;;        add     ecx,65536
;        dec     ebp
;        jne      @@loop
;
;        popad
;        ret
;endp

