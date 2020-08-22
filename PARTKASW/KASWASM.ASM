ideal
p386
model	flat,c
jumps

kasw_psize=6

global  kasw_toscreen:near
global  kasw_toscreen2:near
global  kasw_uploadtexture:near
global  kasw_interpolatetexture:near
global  kasw_gouraudtabel
global  kasw_biltabel
global  kasw_texture

udataseg

kasw_texture dd      256*257 dup(?)

dataseg

uleft   dd      0
udleft  dd      0
uright  dd      0
udright dd      0
vleft   dd      0
vdleft  dd      0
vright  dd      0
vdright dd      0
gleft   dd      0
gdleft  dd      0
gright  dd      0
gdright dd      0

deltauv dd      0
xdeltagint dd      0
innerdestination dd      0
xgouraud        dd 0


 align   32
 dd ?

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


kasw_gouraudtabel       db   512 dup(?)
kasw_biltabel           dd   256*4 dup(?)

codeseg

;esi : texture
;ebx : uv
;eax : temp
;edx : temp
;ecx : temp
;ebp : temp
;edi : gouraud
;deltauv : deltauv
;xdeltagint : deltag
;innerdestination : dest
macro   innerinit

; mov ebp,[submap_deltauv]
;        add     ebx,ebp

        mov     eax,ebx
        shr     eax,4+8
	mov	edx,ebx
	shr	edx,24
        mov     al,bl
	mov	dh,bh
        and     eax,255*16
        endm
macro	innerd1  offset1
; nop
        mov     ecx,[esi+edx*4]
        mov     [byte ptr pix1r],ch
 mov ebp,[deltauv]
        and     ecx,65536*255+255
       add     ebx,ebp
        mov     [dword ptr pix1],ecx   ;*
        mov     ecx,[esi+edx*4+256*4]
        fld     [pix1]
        fmul    [kasw_biltabel+0+eax]
        mov     [byte ptr pix2r],ch
       mov     ebp,edi
        and     ecx,65536*255+255
       push    ebx
        mov     [dword ptr pix2],ecx   ;*
        mov     ecx,[esi+edx*4+4]
	fld	[pix2]
        fmul    [kasw_biltabel+8+eax]
        mov     [byte ptr pix3r],ch    ;*
        and     ecx,65536*255+255
        mov     [dword ptr pix3],ecx   ;*
        mov     ecx,[esi+edx*4+257*4]
	fadd
	fld	[pix3]
        fmul    [kasw_biltabel+4+eax]
        mov     [byte ptr pix4r],ch
       mov     edx,ebx
       shr     edx,24
        and     ecx,65536*255+255
        mov     [dword ptr pix4],ecx   ;*
 ;      mov     ecx,ebx
       mov     dh,bh
        fadd
	fld	[pix4]
        fmul    [kasw_biltabel+12+eax]
       shr     ebx,4+8
        mov     eax,[xdeltagint]
        fadd
        shr     ebp,8;16
;  mov ebp,128   ;her heR hEr hER Her HeR HEr HER !!!
       mov     bl,[esp]
       and     ebx,255*16
        add     edi,eax
        add     ebp, offset kasw_gouraudtabel
        xor     ecx,ecx
        fstp    [fincol]
     push ebx
        mov     cl,[byte ptr fing]
        xor     ebx,ebx
        xor     eax,eax
        mov     bl,[byte ptr finb]
        mov     cl,[ebp+ecx]
        shl     ecx,16
        mov     al,[byte ptr finr]
        mov     cl,[ebp+ebx]

        mov     ch,[ebp+eax]
        mov     ebp,[innerdestination]
        pop     eax
      pop ebx
        mov     [ebp+offset1*4],ecx



endm

macro   calcdelta offset1, delta, value
        mov     bx,[esi+offset1]
        shl     ebx,16
        sar     ebx,16
        mov     dx,[esi+offset1+41*kasw_psize]
        shl     edx,16
        sar     edx,16
        sub     edx,ebx


        shl     edx,16
        sar     edx,16


        sar     edx,3
        mov     [delta],edx
        mov     [value],ebx
endm

macro   calcdeltab offset1, delta, value
        mov     bx,[esi+offset1]
        shl     ebx,16
        shr     ebx,16
        mov     dx,[esi+offset1+41*kasw_psize]
        shl     edx,16
        shr     edx,16
        sub     edx,ebx


        shl     edx,16
        sar     edx,16


        sar     edx,3
        mov     [delta],edx
        mov     [value],ebx
endm

macro   incuv
        mov     ebx,[uleft]
        mov     ecx,[uright]
        mov     edx,[udleft]
        mov     ebp,[udright]

        add     ebx,edx
        add     ecx,ebp

        mov     [uleft],ebx
        mov     [uright],ecx

        mov     ebx,[vleft]
        mov     ecx,[vright]
        mov     edx,[vdleft]
        mov     ebp,[vdright]

        add     ebx,edx
        add     ecx,ebp

        mov     [vleft],ebx
        mov     [vright],ecx

        mov     ebx,[gleft]
        mov     ecx,[gright]
        mov     edx,[gdleft]
        mov     ebp,[gdright]

        add     ebx,edx
        add     ecx,ebp

        mov     [gleft],ebx
        mov     [gright],ecx
endm

macro   calcdelta2
        mov     ebx,[gleft]
        mov     ecx,[gright]
        sub     ecx,ebx
        shl     ecx,16
        sar     ecx,16
        sar     ecx,3
        mov     [xdeltagint],ecx
        mov     [xgouraud],ebx


        mov     ebx,[uleft]
        mov     ecx,[uright]
        mov     edx,[vleft]
        mov     ebp,[vright]
        sub     ecx,ebx
        sub     ebp,edx


        shl     ecx,16
        sar     ecx,16
        shl     ebp,16
        sar     ebp,16


        sar     ecx,3
        shl     ebp,13
        mov     bp,cx

        shl     edx,16
        mov     dx,bx
endm

macro   inner   offset1
        mov     ebx,edx
        shr     ebx,16
        mov     bl,dh
        mov     ecx,[kasw_texture+ebx*4]

;        xor     ecx,ecx
;        mov     cl,dh

        mov     [edi+offset1*4],ecx
        add     edx,ebp
endm

proc    kasw_toscreen source:dword, dest:dword
        pushad

        mov     esi,[source]
        mov     edi,[dest]

        mov     eax,25
@@yloop:
        push    eax

        mov     eax,40
@@xloop:

        calcdelta  0,udleft,uleft
        calcdelta  6,udright,uright
        calcdelta  2,vdleft,vleft
        calcdelta  8,vdright,vright
        calcdeltab  4,gdleft,gleft
        calcdeltab  10,gdright,gright

        push    edi
        push    eax

        mov     eax,8
@@yloop2:

;        mov     [edi],ebx

        calcdelta2

;esi : texture
;ebx : uv
;eax : temp
;edx : temp
;ecx : temp
;ebp : temp
;edi : gouraud
;deltauv : deltauv
;xdeltagint : deltag
;innerdestination : dest


        push    esi
        lea     esi,[kasw_texture]
        mov     ebx,edx
        mov     [deltauv],ebp
        mov     [innerdestination],edi
        push    eax
        mov     edi,[xgouraud]

        innerinit
        innerd1 0
        innerd1 1
        innerd1 2
        innerd1 3
        innerd1 4
        innerd1 5
        innerd1 6
        innerd1 7

;        inner   0
;        inner   1
;        inner   2
;        inner   3
;        inner   4
;        inner   5
;        inner   6
;        inner   7

        incuv

        mov     edi,[innerdestination]
        pop     eax
        pop     esi

        add     edi,(320)*4

        dec     eax
        jne     @@yloop2

        pop     eax
        pop     edi


        add     edi,4*8
        add     esi,kasw_psize

        dec     eax
        jne     @@xloop

        add     esi,kasw_psize
        add     edi,320*4*7

        pop     eax
        dec     eax
        jne     @@yloop



        popad
        ret
endp

;esi : texture
;ebx : uv
;eax : temp
;edx : temp
;ecx : temp
;ebp : temp
;edi : gouraud
;deltauv : deltauv
;xdeltagint : deltag
;innerdestination : dest
macro   innerinit

; mov ebp,[submap_deltauv]
;        add     ebx,ebp

        mov     eax,ebx
        shr     eax,4+8
	mov	edx,ebx
	shr	edx,24
        mov     al,bl
	mov	dh,bh
        and     eax,255*16
        endm

macro   innerd1  offset1
        mov     edx,ebx
        mov     edi,[innerdestination]
        shr     edx,20
        and     edx,256*16-1
        mov     ecx,[kasw_texture+edx*4]
        add     ebx,[deltauv]

        mov     [edi+offset1*4],ecx
endm

macro   calcdelta offset1, delta, value
        mov     bx,[esi+offset1]
        shl     ebx,16
        sar     ebx,16
        mov     dx,[esi+offset1+41*kasw_psize]
        shl     edx,16
        sar     edx,16
        sub     edx,ebx


        shl     edx,16
        sar     edx,16


        sar     edx,3
        mov     [delta],edx
        mov     [value],ebx
endm

macro   calcdeltab offset1, delta, value
        mov     bx,[esi+offset1]
        shl     ebx,16
        shr     ebx,16
        mov     dx,[esi+offset1+41*kasw_psize]
        shl     edx,16
        shr     edx,16
        sub     edx,ebx


        shl     edx,16
        sar     edx,16


        sar     edx,3
        mov     [delta],edx
        mov     [value],ebx
endm

macro   incuv
        mov     ebx,[uleft]
        mov     ecx,[uright]
        mov     edx,[udleft]
        mov     ebp,[udright]

        add     ebx,edx
        add     ecx,ebp

        mov     [uleft],ebx
        mov     [uright],ecx

        mov     ebx,[vleft]
        mov     ecx,[vright]
        mov     edx,[vdleft]
        mov     ebp,[vdright]

        add     ebx,edx
        add     ecx,ebp

        mov     [vleft],ebx
        mov     [vright],ecx

        mov     ebx,[gleft]
        mov     ecx,[gright]
        mov     edx,[gdleft]
        mov     ebp,[gdright]

        add     ebx,edx
        add     ecx,ebp

        mov     [gleft],ebx
        mov     [gright],ecx
endm

macro   calcdelta2
        mov     ebx,[gleft]
        mov     ecx,[gright]
        sub     ecx,ebx
        shl     ecx,16
        sar     ecx,16
        sar     ecx,3
        mov     [xdeltagint],ecx
        mov     [xgouraud],ebx


        mov     ebx,[uleft]
        mov     ecx,[uright]
        mov     edx,[vleft]
        mov     ebp,[vright]
        sub     ecx,ebx
        sub     ebp,edx


        shl     ecx,16
        sar     ecx,16
        shl     ebp,16
        sar     ebp,16


        sar     ecx,3
        shl     ebp,13
        mov     bp,cx

        shl     edx,16
        mov     dx,bx
endm

macro   inner   offset1
        mov     ebx,edx
        shr     ebx,16
        mov     bl,dh
        mov     ecx,[kasw_texture+ebx*4]

;        xor     ecx,ecx
;        mov     cl,dh

        mov     [edi+offset1*4],ecx
        add     edx,ebp
endm

proc    kasw_toscreen2 source:dword, dest:dword
        pushad

        mov     esi,[source]
        mov     edi,[dest]

        mov     eax,25
@@yloop:
        push    eax

        mov     eax,40
@@xloop:

        calcdelta  0,udleft,uleft
        calcdelta  6,udright,uright
        calcdelta  2,vdleft,vleft
        calcdelta  8,vdright,vright
        calcdeltab  4,gdleft,gleft
        calcdeltab  10,gdright,gright

        push    edi
        push    eax

        mov     eax,8
@@yloop2:

;        mov     [edi],ebx

        calcdelta2

;esi : texture
;ebx : uv
;eax : temp
;edx : temp
;ecx : temp
;ebp : temp
;edi : gouraud
;deltauv : deltauv
;xdeltagint : deltag
;innerdestination : dest


        push    esi
        lea     esi,[kasw_texture]
        mov     ebx,edx
        mov     [deltauv],ebp
        mov     [innerdestination],edi
        push    eax
        mov     edi,[xgouraud]

        innerinit
        innerd1 0
        innerd1 1
        innerd1 2
        innerd1 3
        innerd1 4
        innerd1 5
        innerd1 6
        innerd1 7

;        inner   0
;        inner   1
;        inner   2
;        inner   3
;        inner   4
;        inner   5
;        inner   6
;        inner   7

        incuv

        mov     edi,[innerdestination]
        pop     eax
        pop     esi

        add     edi,(320)*4

        dec     eax
        jne     @@yloop2

        pop     eax
        pop     edi


        add     edi,4*8
        add     esi,kasw_psize

        dec     eax
        jne     @@xloop

        add     esi,kasw_psize
        add     edi,320*4*7

        pop     eax
        dec     eax
        jne     @@yloop



        popad
        ret
endp

proc    kasw_uploadtexture source:dword
        pushad

        mov     ecx,65536
        mov     esi,[source]
        lea     edi,[kasw_texture]

        xor     eax,eax
@@loop:
        mov     al,[esi+0]
        shl     eax,16
        mov     al,[esi+2]
        mov     ah,[esi+1]
        mov     [edi],eax
        add     esi,3
        add     edi,4
        dec     ecx
        jne     @@loop


        mov     ecx,256
        lea     esi,[kasw_texture]
@@loop2:
        mov     eax,[esi]
        mov     [esi+65536*4],eax
        add     esi,4
        dec     ecx
        jne     @@loop2


        popad
        ret
endp

proc    kasw_interpolatetexture source:dword, dest:dword
        pushad

        mov     esi,[source]
        mov     edi,[dest]

        mov     ecx,257
@@loop:
        push    ecx
; eax: , ebx:delta r ec

        mov     edx,[esi+4]
        mov     ecx,[esi]
        and     edx,255*256*256
        and     ecx,255*256*256
        sub     edx,ecx
        sar     edx,4
        shl     edx,8
        shl     ecx,8

        mov     ebx,[esi+4]
        mov     eax,[esi]
        and     ebx,255
        shl     ebx,8
        and     eax,255
        shl     eax,8
        sub     ebx,eax
        sar     ebx,4

        and     ecx,65536*65535
        and     eax,65535
        and     edx,65536*65535
        and     ebx,65535
        add     eax,ecx
        add     ebx,edx

        mov     ecx,[esi]
        mov     edx,[esi+4]
        and     ecx,256*255
        and     edx,256*255
        sub     edx,ecx
        mov     ebp,ebx
        sar     edx,4
macro   intinner offset1
        mov     ebx,eax
        shr     ebx,8
        add     eax,ebp
;       mov      ebx,0
        mov     bh,ch
        add     ecx,edx
        mov     [edi+offset1*4],ebx
endm

        intinner 0
        intinner 1
        intinner 2
        intinner 3
        intinner 4
        intinner 5
        intinner 6
        intinner 7
        intinner 8
        intinner 9
        intinner 10
        intinner 11
        intinner 12
        intinner 13
        intinner 14
        intinner 15


        add     edi,4*4*4
        add     esi,4
        pop     ecx
        dec     ecx
        jne     @@loop

        popad
        ret
endp

end
