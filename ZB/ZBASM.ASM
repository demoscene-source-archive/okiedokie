ideal
p386
model	flat,c
jumps

global  zb_mix:near
global  zb_biltab:near
global  zb_deltau:near
global  zb_deltav:near
global  zb_multab:near
global  zb_right:near
global  zb_left:near
global  zb_rightnoclip:near
global  zb_leftnoclip:near
global  zb_ytabel

dataseg

        dd 0

zb_deltau       dd      0
zb_deltav       dd      0
zb_multab       db      256*2 dup (0)

zb_biltab       dd      256*4 dup (0)

pix1	dq	3ff0000000000000h
pix1r=pix1+4
pix2	dq	3ff0000000000000h
pix2r=pix2+4
pix3	dq	3ff0000000000000h
pix3r=pix3+4
pix4	dq	3ff0000000000000h
pix4r=pix4+4
fincol	dq	3ff0000000000000h
finr=fincol+4
finb=fincol+0
fing=fincol+2

zb_mix          dd      0

temp    dd      0
tmpscreen    dd      0
zb_ytabel       dd 256 dup(0)


codeseg

proc    zb_right start:dword, screen:dword, n:dword, u:dword, v:dword
        pushad

        xor     eax,eax
        xor     ebx,ebx
        xor     ecx,ecx
        xor     edx,edx
        xor     esi,esi
        xor     edi,edi

        mov     eax,[v]
        mov     ebx,[u]

        mov     edi,[start]
        mov     esi,[screen]
        mov     [tmpscreen],esi
        mov     ebp,[n]
        cmp     ebp,0
        jle      @@return

        mov     esi,[tmpscreen]
        mov     edx,eax
        shr     edx,16-6-2
        mov     ecx,ebx
        cmp     ecx,0
        jge     @@j1
        xor     ecx,ecx
@@j1:
;        shr     ecx,16-2
;        and     edx,255*64*4
;        and     ecx,511*4
;        add     ecx,esi
;        lea     esi,[edx+edx*4]
;        add     esi,ecx







        sar     ecx,16
        mov     edx,eax
        sar     edx,16;-6-2

        and     edx,255;*64*4
        lea     ecx,[esi+ecx*4]

        mov     esi,[zb_ytabel+edx*4]
        add     esi,ecx







@@loop:

        mov     ecx,eax
        shr     ecx,4
        mov     edx,ebx
        shr     edx,8
        and     ecx,15*16*16
        and     edx,15*16
        add     edx,ecx

   ;nop
        mov     ecx,[esi]
        mov     [byte ptr pix1r],ch           ;---
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx
        mov     ecx,[esi+4]

        fld     [pix1]
        fmul    [zb_biltab+edx]

        mov     [byte ptr pix2r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix2],ecx
        mov     ecx,[esi+4*320]

        fld     [pix2]
        fmul    [zb_biltab+edx+4]

        mov     [byte ptr pix3r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix3],ecx
        mov     ecx,[esi+321*4]

        fadd
        fld     [pix3]
        fmul    [zb_biltab+edx+8]

        mov     [byte ptr pix4r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix4],ecx
        mov     ecx,[edi]

        fadd
        fld     [pix4]
        fmul    [zb_biltab+edx+12]

        mov     [byte ptr pix1r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx
        mov     edx,[zb_deltav]
        fadd
        fld     [pix1]
        fmul    [zb_mix]
        add     eax,edx
        mov     edx,[zb_deltau]
        add     ebx,edx
  ;nop
        fadd


        mov     esi,[tmpscreen]
        mov     ecx,ebx
;        shr     ecx,16-2
        sar     ecx,31
        not     ecx
        and     ecx,ebx
        sar     ecx,16
        mov     edx,eax
        sar     edx,16;-6-2
;        and     ecx,511*4

        and     edx,255;*64*4
        lea     ecx,[esi+ecx*4]

  ;nop      ;3 cyk
  ;nop
  ;nop
  ;nop
  ;nop
  ;nop
        fstp    [fincol]
        mov     esi,[zb_ytabel+edx*4]
    ;    add     esi,edx
    ;    lea     esi,[edx+edx*4]
        mov     edx,[dword ptr fincol]
        mov     dh,[byte ptr finr]
        add     esi,ecx
        mov     [edi],edx
        add     edi,4
        dec     ebp
        jne     @@loop


@@return:
        popad
        ret
endp

proc    zb_left start:dword, screen:dword, n:dword, u:dword, v:dword
        pushad

        xor     eax,eax
        xor     ebx,ebx
        xor     ecx,ecx
        xor     edx,edx
        xor     esi,esi
        xor     edi,edi

        mov     eax,[v]
        mov     ebx,[u]

        mov     edi,[start]
        mov     esi,[screen]
        mov     [tmpscreen],esi
        mov     ebp,[n]
        cmp     ebp,0
        jle      @@return

        mov     esi,[tmpscreen]
        mov     edx,eax
        shr     edx,16-6-2
        mov     ecx,ebx
        cmp     ecx,0
        jge     @@j1
        xor     ecx,ecx
@@j1:
        shr     ecx,16-2
        cmp     ecx,318*4
        jle     @@j2
        mov     ecx,318*4
@@j2:
        and     edx,255*64*4
        and     ecx,511*4
        add     ecx,esi
        lea     esi,[edx+edx*4]
        add     esi,ecx
@@loop:

        mov     ecx,eax
        shr     ecx,4
        mov     edx,ebx
        shr     edx,8
        and     ecx,15*16*16
        and     edx,15*16
        add     edx,ecx

   ;nop
        mov     ecx,[esi]
        mov     [byte ptr pix1r],ch           ;---
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx
        mov     ecx,[esi+4]

        fld     [pix1]
        fmul    [zb_biltab+edx]

        mov     [byte ptr pix2r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix2],ecx
        mov     ecx,[esi+4*320]

        fld     [pix2]
        fmul    [zb_biltab+edx+4]

        mov     [byte ptr pix3r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix3],ecx
        mov     ecx,[esi+321*4]

        fadd
        fld     [pix3]
        fmul    [zb_biltab+edx+8]

        mov     [byte ptr pix4r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix4],ecx
        mov     ecx,[edi]

        fadd
        fld     [pix4]
        fmul    [zb_biltab+edx+12]

        mov     [byte ptr pix1r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx
        mov     edx,[zb_deltav]
        fadd
        fld     [pix1]
        fmul    [zb_mix]
        add     eax,edx
        mov     edx,[zb_deltau]
        add     ebx,edx
  ;nop
        fadd


        mov     esi,[tmpscreen]
        mov     ecx,ebx
;        shr     ecx,16-2
        sar     ecx,31
        not     ecx
        and     ecx,ebx
        sar     ecx,16

        cmp     ecx,318
        sbb     edx,edx
        and     ecx,edx
        not     edx
        and     edx,318
        add     ecx,edx



        mov     edx,eax
        sar     edx,16;-6-2
;        and     ecx,511*4

        and     edx,255;*64*4
        lea     ecx,[esi+ecx*4]

  ;nop      ;3 cyk
  ;nop
  ;nop
  ;nop
  ;nop
  ;nop
        fstp    [fincol]
        mov     esi,[zb_ytabel+edx*4]
    ;    add     esi,edx
    ;    lea     esi,[edx+edx*4]
        mov     edx,[dword ptr fincol]
        mov     dh,[byte ptr finr]
        add     esi,ecx
        mov     [edi],edx
        sub     edi,4
        dec     ebp
        jne     @@loop


@@return:
        popad
        ret
endp

proc    zb_left789312 start:dword, screen:dword, n:dword, u:dword, v:dword
        pushad

        xor     eax,eax
        xor     ebx,ebx
        xor     ecx,ecx
        xor     edx,edx
        xor     esi,esi
        xor     edi,edi

        mov     eax,[v]
        mov     ebx,[u]

        mov     edi,[start]
        mov     esi,[screen]
        mov     [tmpscreen],esi
        mov     ebp,[n]
        cmp     ebp,0
        jle      @@return

        mov     esi,[tmpscreen]
        mov     edx,eax
        shr     edx,16-6-2
        mov     ecx,ebx
        shr     ecx,16-2
        and     edx,255*64*4
        and     ecx,511*4
        add     ecx,esi
        lea     esi,[edx+edx*4]
        add     esi,ecx
@@loop:

        mov     ecx,eax
        shr     ecx,4
        mov     edx,ebx
        shr     edx,8
        and     ecx,15*16*16
        and     edx,15*16
        add     edx,ecx

   ;nop
        mov     ecx,[esi]
        mov     [byte ptr pix1r],ch           ;---
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx
        mov     ecx,[esi+4]

        fld     [pix1]
        fmul    [zb_biltab+edx]

        mov     [byte ptr pix2r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix2],ecx
        mov     ecx,[esi+4*320]

        fld     [pix2]
        fmul    [zb_biltab+edx+4]

        mov     [byte ptr pix3r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix3],ecx
        mov     ecx,[esi+321*4]

        fadd
        fld     [pix3]
        fmul    [zb_biltab+edx+8]

        mov     [byte ptr pix4r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix4],ecx
        mov     ecx,[edi]

        fadd
        fld     [pix4]
        fmul    [zb_biltab+edx+12]

        mov     [byte ptr pix1r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx
        mov     edx,[zb_deltav]
        fadd
        fld     [pix1]
        fmul    [zb_mix]
        add     eax,edx
        mov     edx,[zb_deltau]
        add     ebx,edx
  ;nop
        fadd


        mov     esi,[tmpscreen]
        mov     ecx,ebx
        shr     ecx,16-2
        mov     edx,eax
        shr     edx,16;-6-2
        and     ecx,511*4
        and     edx,255;*64*4
        add     ecx,esi

  ;nop      ;3 cyk
  ;nop
  ;nop
  ;nop
  ;nop
  ;nop
        fstp    [fincol]
        mov     esi,[zb_ytabel+edx*4]
    ;    add     esi,edx
    ;    lea     esi,[edx+edx*4]
        mov     edx,[dword ptr fincol]
        mov     dh,[byte ptr finr]
        add     esi,ecx
        mov     [edi],edx
        sub     edi,4
        dec     ebp
        jne     @@loop


@@return:
        popad
        ret
endp

proc    zb_left13124 start:dword, screen:dword, n:dword, u:dword, v:dword
        pushad

        mov     eax,[v]
        mov     ebx,[u]

        mov     edi,[start]
        mov     esi,[screen]
        mov     [tmpscreen],esi
        mov     ebp,[n]
        cmp     ebp,0
        jle      @@return

        mov     esi,[tmpscreen]
        mov     edx,eax
        shr     edx,16-6-2
        mov     ecx,ebx
        shr     ecx,16-2
        and     edx,255*64*4
        and     ecx,511*4
        add     ecx,esi
        lea     esi,[edx+edx*4]
        add     esi,ecx
@@loop:

        mov     ecx,eax
        shr     ecx,4
        mov     edx,ebx
        shr     edx,8
        and     ecx,15*16*16
        and     edx,15*16
        add     edx,ecx

   ;nop
        mov     ecx,[esi]
        mov     [byte ptr pix1r],ch           ;---
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx
        mov     ecx,[esi+4]

        fld     [pix1]
        fmul    [zb_biltab+edx]

        mov     [byte ptr pix2r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix2],ecx
        mov     ecx,[esi+320*4]

        fld     [pix2]
        fmul    [zb_biltab+edx+4]

        mov     [byte ptr pix3r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix3],ecx
        mov     ecx,[esi+321*4]

        fadd
        fld     [pix3]
        fmul    [zb_biltab+edx+8]

        mov     [byte ptr pix4r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix4],ecx
        mov     ecx,[edi]

        fadd
        fld     [pix4]
        fmul    [zb_biltab+edx+12]

        mov     [byte ptr pix1r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx
        mov     edx,[zb_deltav]
        fadd
        fld     [pix1]
        fmul    [zb_mix]
        add     eax,edx
        mov     edx,[zb_deltau]
        add     ebx,edx
  ;nop
        fadd


        mov     esi,[tmpscreen]
        mov     ecx,ebx
        shr     ecx,16-2
        mov     edx,eax
        shr     edx,16-6-2
        and     ecx,511*4
        and     edx,255*64*4
        add     ecx,esi

  ;nop      ;3 cyk
  ;nop
  ;nop
  ;nop
  ;nop
  ;nop
        fstp    [fincol]
        lea     esi,[edx+edx*4]
        mov     edx,[dword ptr fincol]
        mov     dh,[byte ptr finr]
        add     esi,ecx
        mov     [edi],edx
        sub     edi,4
        dec     ebp
        jne     @@loop


@@return:
        popad
        ret
endp

proc    zb_rightnoclip start:dword, screen:dword, n:dword, u:dword, v:dword
        pushad

        xor     eax,eax
        xor     ebx,ebx
        xor     ecx,ecx
        xor     edx,edx
        xor     esi,esi
        xor     edi,edi

        mov     eax,[v]
        mov     ebx,[u]

        mov     edi,[start]
        mov     esi,[screen]
        mov     [tmpscreen],esi
        mov     ebp,[n]
        cmp     ebp,0
        jle      @@return

        mov     esi,[tmpscreen]
        mov     edx,eax
        shr     edx,16-6-2
        mov     ecx,ebx
        shr     ecx,16-2
        and     edx,255*64*4
        and     ecx,511*4
        add     ecx,esi
        lea     esi,[edx+edx*4]
        add     esi,ecx
@@loop:

        mov     ecx,eax
        shr     ecx,4
        mov     edx,ebx
        shr     edx,8
        and     ecx,15*16*16
        and     edx,15*16
        add     edx,ecx

   ;nop
        mov     ecx,[esi]
        mov     [byte ptr pix1r],ch           ;---
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx
        mov     ecx,[esi+4]

        fld     [pix1]
        fmul    [zb_biltab+edx]

        mov     [byte ptr pix2r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix2],ecx
        mov     ecx,[esi+4*320]

        fld     [pix2]
        fmul    [zb_biltab+edx+4]

        mov     [byte ptr pix3r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix3],ecx
        mov     ecx,[esi+321*4]

        fadd
        fld     [pix3]
        fmul    [zb_biltab+edx+8]

        mov     [byte ptr pix4r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix4],ecx
        mov     ecx,[edi]

        fadd
        fld     [pix4]
        fmul    [zb_biltab+edx+12]

        mov     [byte ptr pix1r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx
        mov     edx,[zb_deltav]
        fadd
        fld     [pix1]
        fmul    [zb_mix]
        add     eax,edx
        mov     edx,[zb_deltau]
        add     ebx,edx
  ;nop
        fadd


        mov     esi,[tmpscreen]
        mov     ecx,ebx
        shr     ecx,16-2
        mov     edx,eax
        shr     edx,16-6-2
        and     ecx,511*4
        and     edx,255*64*4
        add     ecx,esi

  ;nop      ;3 cyk
  ;nop
  ;nop
  ;nop
  ;nop
  ;nop
        fstp    [fincol]
        lea     esi,[edx+edx*4]
        mov     edx,[dword ptr fincol]
        mov     dh,[byte ptr finr]
        add     esi,ecx
        mov     [edi],edx
        add     edi,4
        dec     ebp
        jne     @@loop


@@return:
        popad
        ret
endp

proc    zb_leftnoclip start:dword, screen:dword, n:dword, u:dword, v:dword
        pushad

        mov     eax,[v]
        mov     ebx,[u]

        mov     edi,[start]
        mov     esi,[screen]
        mov     [tmpscreen],esi
        mov     ebp,[n]
        cmp     ebp,0
        jle      @@return

        mov     esi,[tmpscreen]
        mov     edx,eax
        shr     edx,16-6-2
        mov     ecx,ebx
        shr     ecx,16-2
        and     edx,255*64*4
        and     ecx,511*4
        add     ecx,esi
        lea     esi,[edx+edx*4]
        add     esi,ecx
@@loop:

        mov     ecx,eax
        shr     ecx,4
        mov     edx,ebx
        shr     edx,8
        and     ecx,15*16*16
        and     edx,15*16
        add     edx,ecx

   ;nop
        mov     ecx,[esi]
        mov     [byte ptr pix1r],ch           ;---
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx
        mov     ecx,[esi+4]

        fld     [pix1]
        fmul    [zb_biltab+edx]

        mov     [byte ptr pix2r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix2],ecx
        mov     ecx,[esi+320*4]

        fld     [pix2]
        fmul    [zb_biltab+edx+4]

        mov     [byte ptr pix3r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix3],ecx
        mov     ecx,[esi+321*4]

        fadd
        fld     [pix3]
        fmul    [zb_biltab+edx+8]

        mov     [byte ptr pix4r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix4],ecx
        mov     ecx,[edi]

        fadd
        fld     [pix4]
        fmul    [zb_biltab+edx+12]

        mov     [byte ptr pix1r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx
        mov     edx,[zb_deltav]
        fadd
        fld     [pix1]
        fmul    [zb_mix]
        add     eax,edx
        mov     edx,[zb_deltau]
        add     ebx,edx
  ;nop
        fadd


        mov     esi,[tmpscreen]
        mov     ecx,ebx
        shr     ecx,16-2
        mov     edx,eax
        shr     edx,16-6-2
        and     ecx,511*4
        and     edx,255*64*4
        add     ecx,esi

  ;nop      ;3 cyk
  ;nop
  ;nop
  ;nop
  ;nop
  ;nop
        fstp    [fincol]
        lea     esi,[edx+edx*4]
        mov     edx,[dword ptr fincol]
        mov     dh,[byte ptr finr]
        add     esi,ecx
        mov     [edi],edx
        sub     edi,4
        dec     ebp
        jne     @@loop


@@return:
        popad
        ret
endp

proc    zb_left3 start:dword, screen:dword, n:dword, u:dword, v:dword
        pushad

        mov     eax,[v]
        mov     ebx,[u]

        mov     edi,[start]
        mov     esi,[screen]
        mov     [tmpscreen],esi
        mov     ebp,[n]
        cmp     ebp,0
        je      @@return
@@loop:
        mov     ecx,ebx
        mov     edx,eax
        shr     edx,16-6-2
        shr     ecx,16-2
        and     ecx,511*4
        and     edx,255*64*4
        add     ecx,[tmpscreen]
        lea     esi,[edx+edx*4]
        add     esi,ecx

        mov     ecx,eax
        shr     ecx,4
        and     ecx,15*16*16
        mov     edx,ebx
        shr     edx,8
        and     edx,15*16
        add     edx,ecx


        mov     ecx,[esi]
        mov     [byte ptr pix1r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx

        fld     [pix1]
        fmul    [zb_biltab+edx]

        mov     ecx,[esi+4]
        mov     [byte ptr pix2r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix2],ecx

        fld     [pix2]
        fmul    [zb_biltab+edx+4]

        mov     ecx,[esi+320*4]
        mov     [byte ptr pix3r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix3],ecx

        fadd
        fld     [pix3]
        fmul    [zb_biltab+edx+8]

        mov     ecx,[esi+321*4]
        mov     [byte ptr pix4r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix4],ecx

        fadd
        fld     [pix4]
        fmul    [zb_biltab+edx+12]




        mov     ecx,[edi]
        mov     [byte ptr pix1r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx

        fadd
        fld     [pix1]
        fmul    [zb_mix]
        ;2 cyk
        fadd
        ;3 cyk
        fstp    [fincol]

        mov     ecx,[dword ptr fincol]
        mov     ch,[byte ptr finr]

        mov     edx,[zb_deltav]
        add     eax,edx
        mov     edx,[zb_deltau]
        add     ebx,edx

        mov     [edi],ecx
        sub     edi,4
        dec     ebp
        jne     @@loop


@@return:
        popad
        ret
endp

proc    zb_left2 start:dword, screen:dword, n:dword, u:dword, v:dword
        pushad

        mov     eax,[v]
        mov     ebx,[u]

        mov     edi,[start]
        mov     esi,[screen]
        mov     [tmpscreen],esi
        mov     ebp,[n]
        cmp     ebp,0
        je      @@return
@@loop:
        mov     ecx,ebx
        mov     edx,eax
        shr     edx,16-6-2
        shr     ecx,16-2
        and     ecx,511*4
        and     edx,255*64*4
        add     ecx,[tmpscreen]
        lea     esi,[edx+edx*4]
        add     esi,ecx

        mov     ecx,eax
        shr     ecx,8
        and     ecx,15*16
        mov     edx,ebx
        shr     edx,4
        and     edx,15*256
        add     edx,ecx


        mov     ecx,[esi]
        mov     [byte ptr pix1r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx

        fld     [pix1]
        fmul    [zb_biltab+edx]

        mov     ecx,[esi+4]
        mov     [byte ptr pix2r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix2],ecx

        fld     [pix2]
        fmul    [zb_biltab+edx+4]

        mov     ecx,[esi+320*4]
        mov     [byte ptr pix3r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix3],ecx

        fadd
        fld     [pix3]
        fmul    [zb_biltab+edx+8]

        mov     ecx,[esi+321*4]
        mov     [byte ptr pix4r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix4],ecx

        fadd
        fld     [pix4]
        fmul    [zb_biltab+edx+12]




        mov     ecx,[edi]
        mov     [byte ptr pix1r],ch
        and     ecx,255+65536*255
        mov     [dword ptr pix1],ecx

        fadd
        fld     [pix1]
        fmul    [zb_mix]
        ;2 cyk
        fadd
        ;3 cyk
        fstp    [fincol]

        mov     ecx,[dword ptr fincol]
        mov     ch,[byte ptr finr]

        mov     edx,[zb_deltav]
        add     eax,edx
        mov     edx,[zb_deltau]
        add     ebx,edx

        mov     [edi],ecx
        sub     edi,4
        dec     ebp
        jne     @@loop


@@return:
        popad
        ret
endp

proc    zb_rightold start:dword, screen:dword, n:dword, u:dword, v:dword
        pushad

        mov     eax,[v]
        mov     ebx,[u]

        mov     edi,[start]
        mov     esi,[screen]
        mov     ebp,[n]
        cmp     ebp,0
        je      @@return
@@loop:
        mov     edx,[zb_deltav]
        add     eax,edx
        mov     edx,[zb_deltau]
        add     ebx,edx
        mov     ecx,ebx
        mov     edx,eax
        shr     edx,16-6
        shr     ecx,16
        and     ecx,511
        and     edx,255*64
        lea     edx,[edx+edx*4]
        add     ecx,edx
        mov     ecx,[esi+ecx*4]

        mov     edx,ecx
        shr     edx,16
        and     edx,255
        mov     dl,[zb_multab+edx]
        shl     edx,16
        and     ecx,65535
        add     ecx,edx
        xor     edx,edx
        mov     dl,cl
        mov     cl,[zb_multab+edx]
        mov     dl,ch
        mov     ch,[zb_multab+edx]

        mov     [temp],ecx
        mov     ecx,[edi]

        mov     edx,ecx
        shr     edx,16
        and     edx,255
        mov     dl,[zb_multab+edx+256]
        shl     edx,16
        and     ecx,65535
        add     ecx,edx
        xor     edx,edx
        mov     dl,cl
        mov     cl,[zb_multab+edx+256]
        mov     dl,ch
        mov     ch,[zb_multab+edx+256]

        add     ecx,[temp]

        mov     [edi],ecx
        add     edi,4
        dec     ebp
        jne     @@loop


@@return:
        popad
        ret
endp

end
