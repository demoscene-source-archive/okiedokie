




ideal
p386
model	flat,c
jumps

trace_psize=12

global  trace_toscreen:near
global  trace_uploadtexture:near
global  trace_calcgridrgb:near
global  trace_retrgb:near
global  trace_gouraudtabel
global  trace_biltabel
global  trace_texturetable

dataseg

trace_texturetable      dd      trace_texture
                        dd      trace_texture
                        dd 16 dup(?)

trace_texture dd      256*257 dup(?)

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
rleft   dd      0
rdleft  dd      0
rright  dd      0
rdright dd      0
bleft   dd      0
bdleft  dd      0
bright  dd      0
bdright dd      0

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


trace_gouraudtabel       db   512 dup(?)
trace_biltabel           dd   256*4 dup(?)

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

macro   innerd1b  offset1
;Pentium optimeret
; nop
        mov     ecx,[esi+edx*4]
        mov     [byte ptr pix1r],ch
 mov ebp,[deltauv]
        and     ecx,65536*255+255
       add     ebx,ebp
        mov     [dword ptr pix1],ecx   ;*
        mov     ecx,[esi+edx*4+256*4]
        fld     [pix1]
        fmul    [trace_biltabel+0+eax]
        mov     [byte ptr pix2r],ch

  and   edi,65535
       mov     ebp,edi
        and     ecx,65536*255+255
       push    ebx
        mov     [dword ptr pix2],ecx   ;*
        mov     ecx,[esi+edx*4+4]
	fld	[pix2]
        fmul    [trace_biltabel+8+eax]
        mov     [byte ptr pix3r],ch    ;*
        and     ecx,65536*255+255
        mov     [dword ptr pix3],ecx   ;*
        mov     ecx,[esi+edx*4+257*4]
	fadd
	fld	[pix3]
        fmul    [trace_biltabel+4+eax]
        mov     [byte ptr pix4r],ch
       mov     edx,ebx
       shr     edx,24
        and     ecx,65536*255+255
        mov     [dword ptr pix4],ecx   ;*
 ;      mov     ecx,ebx
       mov     dh,bh
        fadd
	fld	[pix4]
        fmul    [trace_biltabel+12+eax]
       shr     ebx,4+8
        mov     eax,[xdeltagint]
        fadd
        shr     ebp,8;16
;  mov ebp,128   ;her heR hEr hER Her HeR HEr HER !!!
       mov     bl,[esp]
       and     ebx,255*16
        add     edi,eax
        add     ebp, offset trace_gouraudtabel
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
macro   innerd1c  offset1
endm
macro   innerd1  offset1
;P6 optimeret

        mov     ecx,[esi+edx*4]
        mov     [byte ptr pix1r],ch
        and     ecx,65536*255+255
        mov     [dword ptr pix1],ecx   ;*

        mov     ecx,[esi+edx*4+256*4]
        mov     [byte ptr pix2r],ch
        and     ecx,65536*255+255
        mov     [dword ptr pix2],ecx   ;*

        mov     ecx,[esi+edx*4+4]
        mov     [byte ptr pix3r],ch    ;*
        and     ecx,65536*255+255
        mov     [dword ptr pix3],ecx   ;*

        mov     ecx,[esi+edx*4+257*4]
        mov     [byte ptr pix4r],ch
        and     ecx,65536*255+255
        mov     [dword ptr pix4],ecx   ;*

        fld     [pix1]
        fmul    [trace_biltabel+0+eax]
        fld     [pix2]
        fmul    [trace_biltabel+8+eax]
        fadd
        fld     [pix3]
        fmul    [trace_biltabel+4+eax]
        fadd
        fld     [pix4]
        fmul    [trace_biltabel+12+eax]
        fadd
        fstp    [fincol]

       mov ebp,[deltauv]
       add     ebx,ebp
       mov     edx,ebx
       shr     edx,24
       mov     dh,bh








 push    ebx

       and   edi,65535
       mov     ebp,edi
;        add     edi,[xdeltagint]
        shr     ebp,8;16
        add     ebp, offset trace_gouraudtabel

        mov     eax,[xdeltagint]
        add     edi,eax




        mov     ebx,[innerdestination]

        mov     ecx,[dword ptr fincol]
        and     ecx,255*65536
        shr     ecx,16
        mov     al,[ebp+ecx]
        mov     [ebx+2+offset1*4],al

        mov     ecx,[dword ptr fincol]
        and     ecx,255
        mov     al,[ebp+ecx]
        mov     [ebx+0+offset1*4],al

        mov     ecx,[dword ptr fincol+4]
        and     ecx,255
        mov     al,[ebp+ecx]
        mov     [ebx+1+offset1*4],al

;        mov     cl,[ebp+ecx]
;        shl     ecx,16
;        mov     cl,[ebp+ebx]
;        mov     ch,[ebp+eax]

        mov     ebx,[esp]
        mov     eax,ebx
        and     ebx,15*16;255
       shr     eax,4+8
       and      eax,256*15
       add     eax,ebx
 pop ebx
;       and     eax,255*16

;        mov     [ebp+offset1*4],ecx



endm

macro   calcdelta offset1, delta, value
        mov     bx,[esi+offset1]
        shl     ebx,16
        sar     ebx,16
        mov     dx,[esi+offset1+81*trace_psize]
        shl     edx,16
        sar     edx,16
        sub     edx,ebx


        shl     edx,16
        sar     edx,16


        sar     edx,2 ;*
        mov     [delta],edx
        mov     [value],ebx
endm

macro   calcdeltab offset1, delta, value
        mov     bx,[esi+offset1]
        shl     ebx,16
        shr     ebx,16
        mov     dx,[esi+offset1+81*trace_psize]
        shl     edx,16
        shr     edx,16
        sub     edx,ebx


        shl     edx,16
        sar     edx,16


        sar     edx,2 ;*
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
        sar     ecx,2  ;*
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


        sar     ecx,2  ;*
        shl     ebp,14 ;*
        mov     bp,cx

        shl     edx,16
        mov     dx,bx
endm

macro   inner   offset1
        mov     ebx,edx
        shr     ebx,16
        mov     bl,dh
        mov     ecx,[trace_texture+ebx*4]

;        xor     ecx,ecx
;        mov     cl,dh

        mov     [edi+offset1*4],ecx
        add     edx,ebp
endm

proc    trace_toscreen source:dword, dest:dword
        pushad

        mov     esi,[source]
        mov     edi,[dest]

        mov     eax,50
@@yloop:
        push    eax

        mov     eax,80
@@xloop:

        mov     bl,[esi+8]
        mov     cl,[esi+8+12]
        mov     dl,[esi+8+81*12]
        xor     bl,cl
        xor     dl,cl
        or      bl,dl
        mov     dl,[esi+8+81*12+12]
        xor     dl,cl
        or      bl,dl
        je      @@textureinterpolation
        jmp     @@next

@@rgbinterpolation:

macro innerrgb offset1
        mov     edx,ecx
        shr     edx,8
        add     ecx,ebp
        mov     dh,bh
        add     ebx,esi
        mov     [edi+offset1*4],edx
endm
macro calcdeltargb offset1, delta, value
        xor     ebx,ebx
        xor     ecx,ecx
        mov     bh,[esi+offset1]
        mov     ch,[esi+offset1+81*trace_psize]
        sub     ecx,ebx
        mov     [value],ebx
        sar     ecx,2
        mov     [delta],ecx
endm

        calcdeltargb 2,rdleft,rleft
        calcdeltargb 2+12,rdright,rright
        calcdeltargb 1,gdleft,gleft
        calcdeltargb 1+12,gdright,gright
        calcdeltargb 0,bdleft,bleft
        calcdeltargb 0+12,bdright,bright

        push    esi
        push    edi

        push    eax
        mov     eax,4
@@rgbyloop:
        mov     ecx,[rleft]
        mov     ebp,[rright]
        sub     ebp,ecx
        sar     ebp,2
        shl     ebp,16
        shl     ecx,16


        mov     ebx,[bleft]
        mov     edx,[bright]
        sub     edx,ebx
        sar     edx,2
        mov     cx,bx
        mov     bp,dx

        mov     ebx,[gleft]
        mov     esi,[gright]
        sub     esi,ebx
        sar     esi,2

;    xor     ecx,ecx
;    xor     ebp,ebp

        innerrgb        0
        innerrgb        1
        innerrgb        2
        innerrgb        3


        mov     ebx,[rright]
        mov     ecx,[rdright]
        add     ebx,ecx
        mov     ecx,[rdleft]
        mov     edx,[rleft]
        add     edx,ecx
        mov     [rright],ebx
        mov     [rleft],edx

        mov     ebx,[gright]
        mov     ecx,[gdright]
        add     ebx,ecx
        mov     ecx,[gdleft]
        mov     edx,[gleft]
        add     edx,ecx
        mov     [gright],ebx
        mov     [gleft],edx

        mov     ebx,[bright]
        mov     ecx,[bdright]
        add     ebx,ecx
        mov     ecx,[bdleft]
        mov     edx,[bleft]
        add     edx,ecx
        mov     [bright],ebx
        mov     [bleft],edx

        add     edi,320*4

        dec     eax
        jne     @@rgbyloop
        pop     eax
        pop     edi
        pop     esi

        jmp     @@next

@@textureinterpolation:

        calcdelta  4,udleft,uleft
        calcdelta  4+12,udright,uright
        calcdelta  6,vdleft,vleft
        calcdelta  6+12,vdright,vright
        calcdeltab  10,gdleft,gleft
        calcdeltab  10+12,gdright,gright

        push    edi
        push    eax

        mov     eax,4
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
        lea     esi,[trace_texture]         ;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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

        incuv

        mov     edi,[innerdestination]
        pop     eax
        pop     esi

        add     edi,(320)*4

        dec     eax
        jne     @@yloop2

        pop     eax
        pop     edi

@@next:
        add     edi,4*4
        add     esi,trace_psize

        dec     eax
        jne     @@xloop

        add     esi,trace_psize
        add     edi,320*4*3

        pop     eax
        dec     eax
        jne     @@yloop



        popad
        ret
endp

proc    trace_uploadtexture source:dword
        pushad

        mov     ecx,65536
        mov     esi,[source]
        lea     edi,[trace_texture]

        xor     eax,eax
@@loop:
        mov     al,[esi+2]
        shl     eax,16
        mov     al,[esi+0]
        mov     ah,[esi+1]
        mov     [edi],eax
        add     esi,3
        add     edi,4
        dec     ecx
        jne     @@loop


        mov     ecx,256
        lea     esi,[trace_texture]
@@loop2:
        mov     eax,[esi]
        mov     [esi+65536*4],eax
        add     esi,4
        dec     ecx
        jne     @@loop2


        popad
        ret
endp

proc    trace_calcgridrgb screen:dword
        pushad

        mov     esi,[screen]
        mov     edi,81*51
@@loop:

        xor     ebx,ebx
        mov     bl,[esi+8]

        mov     edx,[esi+4]
        shr     edx,24
                                                        and     ebx,15
        mov     dh,[esi+5]


        mov     eax,[esi+4]
        shr     eax,4+8
        mov     al,[esi+4]
        and     eax,255*16
; xor eax,eax
        mov     ebp,[esi+8]
        shr     ebp,24

        push    esi

        mov     esi,[trace_texturetable+ebx*4]
;        mov     esi,[trace_texturetable]


;----------------------------------------------
        mov     ecx,[esi+edx*4]
        mov     [byte ptr pix1r],ch
        and     ecx,65536*255+255
        mov     [dword ptr pix1],ecx   ;*
        mov     ecx,[esi+edx*4+256*4]
        fld     [pix1]
        fmul    [trace_biltabel+0+eax]
        mov     [byte ptr pix2r],ch
        and     ecx,65536*255+255
        mov     [dword ptr pix2],ecx   ;*
        mov     ecx,[esi+edx*4+4]
	fld	[pix2]
        fmul    [trace_biltabel+8+eax]
        mov     [byte ptr pix3r],ch    ;*
        and     ecx,65536*255+255
        mov     [dword ptr pix3],ecx   ;*
        mov     ecx,[esi+edx*4+257*4]
	fadd
	fld	[pix3]
        fmul    [trace_biltabel+4+eax]
        mov     [byte ptr pix4r],ch
        and     ecx,65536*255+255
        mov     [dword ptr pix4],ecx   ;*
        fadd
	fld	[pix4]
        fmul    [trace_biltabel+12+eax]
        fadd
        add     ebp, offset trace_gouraudtabel
        xor     ecx,ecx
        fstp    [fincol]
        mov     cl,[byte ptr fing]
        xor     ebx,ebx
        xor     eax,eax
        mov     bl,[byte ptr finb]
        mov     cl,[ebp+ecx]
        shl     ecx,16
        mov     al,[byte ptr finr]
        mov     cl,[ebp+ebx]

        mov     ch,[ebp+eax]
;----------------------------------------------
   ;     and     ecx,255*256*256+255


        pop     esi
        mov     [esi],ecx


        add     esi,12
        dec     edi
        jne     @@loop

        popad
        ret
endp

proc    trace_retrgb
; trasher alle registre
; tager imod grid information som pointer (esi)
; og returnerer farven i ecx

        push    ebp

        xor     ebx,ebx
        mov     bl,[esi+8]

        mov     edx,[esi+4]
        shr     edx,24
                                                   and     ebx,15
        mov     dh,[esi+5]


        mov     eax,[esi+4]
        shr     eax,4+8
        mov     al,[esi+4]
        and     eax,255*16
        mov     ebp,[esi+8]
        shr     ebp,24

        mov     esi,[trace_texturetable+ebx*4]


;----------------------------------------------
        mov     ecx,[esi+edx*4]
        mov     [byte ptr pix1r],ch
        and     ecx,65536*255+255
        mov     [dword ptr pix1],ecx   ;*
        mov     ecx,[esi+edx*4+256*4]
        fld     [pix1]
        fmul    [trace_biltabel+0+eax]
        mov     [byte ptr pix2r],ch
        and     ecx,65536*255+255
        mov     [dword ptr pix2],ecx   ;*
        mov     ecx,[esi+edx*4+4]
	fld	[pix2]
        fmul    [trace_biltabel+8+eax]
        mov     [byte ptr pix3r],ch    ;*
        and     ecx,65536*255+255
        mov     [dword ptr pix3],ecx   ;*
        mov     ecx,[esi+edx*4+257*4]
	fadd
	fld	[pix3]
        fmul    [trace_biltabel+4+eax]
        mov     [byte ptr pix4r],ch
        and     ecx,65536*255+255
        mov     [dword ptr pix4],ecx   ;*
        fadd
	fld	[pix4]
        fmul    [trace_biltabel+12+eax]
        fadd
        add     ebp, offset trace_gouraudtabel
        xor     ecx,ecx
        fstp    [fincol]
        mov     cl,[byte ptr fing]
        xor     ebx,ebx
        xor     eax,eax
        mov     bl,[byte ptr finb]
        mov     cl,[ebp+ecx]
        shl     ecx,16
        mov     al,[byte ptr finr]
        mov     cl,[ebp+ebx]

        mov     ch,[ebp+eax]
;----------------------------------------------

        mov     eax,ecx


        pop     ebp

        ret
endp

end
