;texturemappers by craft/fudge
klip_on=1
ideal
p386
model	flat,c
jumps

;global  submap3:near
global	submap3g:near
global	submap3gklip:near
;global  bilinearinterpolation:near
;global  bilinearinterpolation2:near
global	bilinearinterpolation3:near
global	retinttab:near
global	texturetable
global	biltabel
global	setscreenscale:near
global  gouraudtabel

global submap3p_tex
global submap324:near
global submap3gouraud:near

dataseg ;---------------------------------------------------------------

dum1            dd      0

screenscalex	dd	3f800000h
screenscaley	dd	3f800000h
temptemp        dq      0
temp1           dq      0
temp2           dq      0
temp3           dq      0
fpmagicdouble	dq     4338000000000000h
fpmagic160g65536 dq	4338000000a00000h
fpmagic100g65536 dq	4338000000640000h
fpMAGIC 	DD	59C00000H   ; f.p. representation of 2^51 + 2^52
magic           DD      59C00000H   ; f.p. representation of 2^51 + 2^52
fp1d65536	dd	37800000h
fp65536 	dd	47800000h
fp2p24		dd	4b800000h
fp2p60		dd	5dc00000h

inttofloat	dd	       0h
		dd	3F800000h
		dd	40000000h
		dd	40400000h
		dd	40800000h
		dd	40A00000h
		dd	40C00000h
		dd	40E00000h
fp8		dd	41000000h
inttoinvfloat	dd	       0h
		dd	3F800000h
		dd	3F000000h
		dd	3EAAAAABh
		dd	3E800000h
		dd	3E4CCCCDh
		dd	3E2AAAABh
		dd	3E124925h

submap3_dest            dd      0

submap3_venstredata:
submap3_venstredeltax      dd      0
submap3_venstredeltaxint   dd      0
submap3_venstrex           dd      0
submap3_venstretx          dd      0
submap3_venstredeltatx     dd      0,0
submap3_venstrety          dd      0
submap3_venstredeltaty     dd      0,0
submap3_venstrez           dd      0
submap3_venstredeltaz      dd      0,0
submap3_venstreg           dd      0
submap3_venstredeltag      dd      0,0
submap3_hojredata:
submap3_hojredeltax      dd      0
submap3_hojredeltaxint   dd      0
submap3_hojrex           dd      0
;submap3_hojretx          dd      0
;submap3_hojredeltatx     dd      0,0
;submap3_hojrety          dd      0
;submap3_hojredeltaty     dd      0,0
;submap3_hojrez           dd      0
;submap3_hojredeltaz      dd      0,0
;submap3_hojreg           dd      0
;submap3_hojredeltag      dd      0,0
submap3_temp:
submap3_tempx           dd      0
submap3_tempy           dd      0
submap3_tempz           dd      0
submap3_tempsx          dd      0
submap3_tempsy          dd      0
submap3_temptx          dd      0
submap3_tempty          dd      0
submap3_tempg           dd      0
submap3_tempsy2         dd      0

submap3_prestep         dd      0
submap3_prestep2        dd      0
submap3_cury            dd      0
submap3_couy            dd      0

submap3_xdeltatx         dd      0
submap3_xdeltatxint      dd      0
submap3_ydeltatx         dd      0
submap3_xdeltaty         dd      0
submap3_xdeltatyint      dd      0
submap3_ydeltaty         dd      0
submap3_xdeltag          dd      0
submap3_xdeltagint       dd      0
submap3_ydeltag          dd      0
submap3_xdeltaz          dd      0
submap3_xdeltazint       dd      0
submap3_ydeltaz          dd      0

submap3_flag             dd      0

submap3_fakedata:
submap3_fakedeltax      dd      0
submap3_fakedeltaxint   dd      0
submap3_fakex           dd      0
submap3_faketx          dd      0
submap3_fakedeltatx     dd      0,0
submap3_fakety          dd      0
submap3_fakedeltaty     dd      0,0
submap3_fakez           dd      0
submap3_fakedeltaz      dd      0,0
submap3_fakeg           dd      0
submap3_fakedeltag      dd      0,0

submap3_detsign         dd      0

submap3p_add1            dd      0
submap3p_add2            dd      0
submap3p_add3            dd      0
submap3p_mapnr           dd      0
submap3p_tex            dd      0

submap3per_tempa         dd     0
align 8
dum2        dd 0

submap3per_temp1         dq     0
submap3per_temp2         dq     0
submap3per_temp3         dq     0
submap3per_temp4         dq     0
submap3per_oldtx        dq     0
submap3per_oldty        dq     0
submap3per_currenttx     dd     0
submap3per_currentty     dd     0
submap3per_currentz      dd     0
submap3per_8deltatx     dd     0
submap3per_8deltaty     dd     0
submap3per_8deltaz      dd     0

submap_counter      dd     0

submap_innerdestination dd 0
submap_deltauv dd 0

align 32
dum3        dd 0 ;,0 ,0,0,0,0
bil_texture dd 0
bil_counter dd 0
bil_dest    dd 0
bil_source  dd 0
texturetable       dd      256 dup(0)
interpolationtable dd   256*16 dup(0)

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

gouraudtabel       db   512 dup(0)
biltabel           dd   256*4 dup(0)
k_tabel            dd   submap3per24bilg
                   dd   submap3per24bil
                   dd   submap3per24
                   dd   submap32gouraud
                   dd   submap3per24bil
                   dd   submap3per24
                   dd   submap324
                   dd   submap3per24bilphong
                   dd   submap3per24bil

codeseg ;----------------------------------------------------------------
proc	setscreenscale scalex:dword,scaley:dword
	pushad
	mov	eax,[scalex]
	mov	ebx,[scaley]
	mov	[screenscalex],eax
	mov	[screenscaley],ebx
	popad
	ret
endp



proc	bilinearinterpolation3 src:dword, dest:dword
	pushad

	mov	edi,[dest]
	mov	[bil_dest],edi
	mov	esi,[src]

	mov	eax,64000
	xor	ebx,ebx
	xor	ecx,ecx
	xor	edx,edx

	jmp	@@loop2
;	 ebx=000000xx,ecx=000000xx,edx=000000xx
@@loop:  ;sidste pixel skulle interpoleres
	mov	[bil_counter],eax				 ;1
	mov	bl,[esi+4]					; 1
	cmp	bl,0						;2
	je	@@next						; 2
	mov	ebp,[texturetable +ebx*4]			;3
	mov	bh,[esi+1]					; 3
	mov	al,[esi]					;4
  mov cl,[esi-3]						; 4
    shl ecx,2
    mov [esi+4],dh
	shl	eax,4						;5
	mov	bl,[esi+3]					; 5
    ;	shl	bl,2						 ;6
  lea	 edi,[interpolationtable+6*1024+ecx*8]				  ; 6
    ;	shr	ebx,4						 ;7
   and	ebx,63*256+63

	mov	al,[esi+2]					; 7
	add	ebp,ebx 					;8
  mov	  dl,[byte ptr temptemp+4]					 ; 8
	and	eax,255*4*4					;9
   add	esi,8							; 9
  mov	bh,[edi+edx]						;10
	mov	cl,[ebp]					; 10
	fld	[dword ptr interpolationtable+eax]		;11
	fmul	 [qword ptr interpolationtable+4096 +ecx*8]	     ;12
  mov	dl,[byte ptr temptemp+2]					 ;13
	mov	cl,[ebp+1]					; 13
	fld	[interpolationtable+eax +4]			;14
	fmul	 [qword ptr interpolationtable+4096 +ecx*8]	     ;15
  mov	 bl,[edi+edx+1024]					;16
	mov	cl,[ebp+256]					; 16
	fld	[interpolationtable+eax +8]			;17
	fmul	 [qword ptr interpolationtable+4096 +ecx*8]	     ;18
  mov	 dl,[byte ptr temptemp+0]					 ;19
	mov	cl,[ebp+257]					; 19
	fld	[interpolationtable+eax +12]			;20
	fmul	 [qword ptr interpolationtable+4096 +ecx*8]	     ;21
	fxch	st(3)						; 21
	fadd							;22
	fxch	st(2)						; 22
	fadd							;23
  shl	 ebx,5							;24
  mov	ebp,[bil_dest]						 ; 24
  mov	bl,[edi+edx+1024]					;25
  add	ebp,2							; 25
	fadd							;26
  shr	 ebx,3							;27
    mov eax,[bil_counter]					 ; 27
  mov	[ebp-4],bx						     ;28
; nop
; nop
; nop
;; mov bx,[ebp-4]
; nop
; nop
; nop
  mov	[bil_dest],ebp						 ;29
    dec eax							; 29
	fstp	[temptemp]					;30-31
    mov ebx,0
    jne @@loop

	popad
	ret

@@loop2:    ;sidste pixel var sort
	mov	[bil_counter],eax				 ;1
	mov	bl,[esi+4]					; 1
	cmp	bl,0						;2
	je	@@next2 					 ; 2
	mov	ebp,[texturetable +ebx*4]			;3
	mov	bh,[esi+1]					; 3
	mov	al,[esi]					;4
;  mov cl,[esi-3]						 ; 4
;    shl ecx,2
    mov [esi+4],dh
	shl	eax,4						;5
	mov	bl,[esi+3]					; 5
    ;	shl	bl,2						 ;6
;  lea	  edi,[interpolationtable+6*1024+ecx*8] 			   ; 6
    ;	shr	ebx,4						 ;7
   and	ebx,63*256+63
	mov	al,[esi+2]					; 7
	add	ebp,ebx 					;8
;  mov	   dl,[byte ptr temptemp+4]					  ; 8
	and	eax,255*4*4					;9
   add	esi,8							; 9
;  mov	 bh,[edi+edx]						 ;10
	mov	cl,[ebp]					; 10
	fld	[dword ptr interpolationtable+eax]		;11
	fmul	 [qword ptr interpolationtable+4096 +ecx*8]	     ;12
;  mov	 dl,[byte ptr temptemp+2]					  ;13
	mov	cl,[ebp+1]					; 13
	fld	[interpolationtable+eax +4]			;14
	fmul	 [qword ptr interpolationtable+4096 +ecx*8]	     ;15
;  mov	  bl,[edi+edx+1024]					 ;16
	mov	cl,[ebp+256]					; 16
	fld	[interpolationtable+eax +8]			;17
	fmul	 [qword ptr interpolationtable+4096 +ecx*8]	     ;18
;  mov	  dl,[byte ptr temptemp+0]					  ;19
	mov	cl,[ebp+257]					; 19
	fld	[interpolationtable+eax +12]			;20
	fmul	 [qword ptr interpolationtable+4096 +ecx*8]	     ;21
	fxch	st(3)						; 21
	fadd							;22
	fxch	st(2)						; 22
	fadd							;23
;  shl	  ebx,5 						 ;24
  mov	ebp,[bil_dest]						 ; 24
;  mov	 bl,[edi+edx+1024]					 ;25
  add	ebp,2							; 25
	fadd							;26
;  shr	  ebx,3 						 ;27
    mov eax,[bil_counter]					 ; 27
;  mov	 [word ptr ebp-4],0;bx							 ;28	fejl?!?
  mov	[bil_dest],ebp						 ;29
    dec eax							; 29
	fstp	[temptemp]					;30-31
    mov ebx,0
    jne @@loop

	popad
	ret
@@next:    ;sidste pixel skulle interpoleres, men ikke denne
	mov	ebp,[texturetable +ebx*4]			;3
;	 mov	 bh,[esi+1]					 ; 3
;	 mov	 al,[esi]					 ;4
  mov cl,[esi-3]						; 4
    shl ecx,2

;	 shl	 eax,4						 ;5
;	 mov	 bl,[esi+3]					 ; 5
    ;	shl	bl,2						 ;6
  lea	 edi,[interpolationtable+6*1024+ecx*8]				  ; 6
    ;	shr	ebx,4						 ;7
;	 mov	 al,[esi+2]					 ; 7
;	 add	 ebp,ebx					 ;8
  mov	  dl,[byte ptr temptemp+4]					 ; 8
;	 and	 eax,255*4*4					 ;9
   add	esi,8							; 9
  mov	bh,[edi+edx]						;10
;	 mov	 cl,[ebp]					 ; 10
;	 fld	 [dword ptr interpolationtable+eax]		 ;11
;	 fmul	  [qword ptr interpolationtable+4096 +ecx*8]	      ;12
  mov	dl,[byte ptr temptemp+2]					 ;13
;	 mov	 cl,[ebp+1]					 ; 13
;	 fld	 [interpolationtable+eax +4]			 ;14
;	 fmul	  [qword ptr interpolationtable+4096 +ecx*8]	      ;15
  mov	 bl,[edi+edx+1024]					;16
;	 mov	 cl,[ebp+256]					 ; 16
;	 fld	 [interpolationtable+eax +8]			 ;17
;	 fmul	  [qword ptr interpolationtable+4096 +ecx*8]	      ;18
  mov	 dl,[byte ptr temptemp+0]					 ;19
;	 mov	 cl,[ebp+257]					 ; 19
;	 fld	 [interpolationtable+eax +12]			 ;20
;	 fmul	  [qword ptr interpolationtable+4096 +ecx*8]	      ;21
;	 fxch	 st(3)						 ; 21
;	 fadd							 ;22
;	 fxch	 st(2)						 ; 22
;	 fadd							 ;23
  shl	 ebx,5							;24
  mov	ebp,[bil_dest]						 ; 24
  mov	bl,[edi+edx+1024]					;25
  add	ebp,2							; 25
;	 fadd							 ;26
  shr	 ebx,3							;27
    mov eax,[bil_counter]					 ; 27
  mov	[ebp-4],bx						     ;28
  mov	[bil_dest],ebp						 ;29
    dec eax							; 29
;	 fstp	 [temptemp]					 ;30-31
    mov ebx,0
    jne @@loop2

	popad
	ret
@@next2: ;begge pixels sorte
	mov	ebp,[bil_dest]
	add	esi,8
	add	ebp,2
	dec	eax
	mov	[bil_dest],ebp
;	mov	[word ptr ebp-4],0			;fejl?!?
	jne	@@loop2
	popad
	ret

endp


proc	retinttab dest:dword
;Returnerer adressen p† fadertabellerne i [dest]
	pushad
	mov	ebx,[dest]
	lea	eax,[interpolationtable]
	mov	[ebx],eax
	popad
	ret
endp



proc    submap3_calc     data:dword,p1:dword,p3:dword
;data er destinationen med f›lgende form:
;   altid:  langdeltax,langdeltaxint,langx
;   for hver v‘rdi der skal interpoleres: lang,langdelta[2]
;p1 og p3 er to endepunkter
;prestep : stykke der skal steppes ekstre ned til f›rste raster linie


local	xstep:dword
local	k:dword
local	temp:dword
;	 submap_langdeltax=(float)p3->sx/p3->sy*65536;
;	 submap_langdeltaxint=submap_langdeltax;
;	 submap_langx=p1->sx+prestep*submap_langdeltax;
;	 xstepl=((int)submap_langdeltax)>>16;
;For hver variabel der skal interpoleres:
;	 submap_langtx=p1->tx+prestep*(submap_ydeltatx+submap_xdeltatx*submap_langdeltax/65536);
;  submap_langtx-=submap_xdeltatx*(submap_langx&65535)/65536;
;	 submap_langdeltatx[0]=xstepl*submap_xdeltatx+submap_ydeltatx;
;	 submap_langdeltatx[1]=submap_langdeltatx[0]+submap_xdeltatx;


macro	calc	offset1,offset2,xd,yd,xdint
	fild	 [xstep]
	fmul	[xd]
	fld	[submap3_prestep]
	fmul	[yd]
	fxch
	fadd	[yd]
	fld	[k]
	fmul	[xd]
	fxch	st(2)
	fadd	[dword ptr edx+offset1]
	fld	st(1)
	fadd	[magic]
	fxch	st(3)
	fadd
	fxch
	fadd	[xd]
	fxch
	fadd	[magic] 		;stall
	fxch	st(2)
	fstp	[temp2]
	fadd	[magic]
	fxch
	fstp	[temp1] 		;stall
	fstp	[temp3]

	mov	eax,[dword ptr temp1]
	mov	ebx,[dword ptr temp2]
	mov	ecx,[dword ptr temp3]
    add     eax,[xdint]
	mov	[edi+12*offset2+0],eax
	mov	[edi+12*offset2+8],ebx		;byt!
	mov	[edi+12*offset2+4],ecx		;byt!!

endm
;	 submap_langdeltax=(float)p3->sx/p3->sy*65536;
;	 submap_langdeltaxint=submap_langdeltax;
;	 submap_langx=p1->sx+prestep*submap_langdeltax;
;	 xstepl=((int)submap_langdeltax)>>16;



	pushad

	fld	[submap3_prestep]

	mov	esi,[p3]
	mov	edi,[data]
	fild	[dword ptr esi+16]
	mov	edx,[p1]
	fidivr	[dword ptr esi+12]
       fmul	[fp65536]
	fst	[dword ptr edi]
	fist	[dword ptr edi+4]

	fmul
	mov	ebx,[edi+4]
	sar	ebx,16
	fiadd	 [dword ptr edx+12]
	mov	[xstep],ebx
	fistp	[dword ptr edi+8]

	fld	[dword ptr edi]
	fmul	[submap3_prestep]
	mov	ebx,[edi+8]
	and	ebx,65535
	mov	[temp],ebx
	fild	[temp]
	fsub
	fmul	[fp1d65536]
	fstp	[k]

	calc	20,1,submap3_xdeltatx,submap3_ydeltatx,submap3_xdeltatxint
	calc	24,2,submap3_xdeltaty,submap3_ydeltaty,submap3_xdeltatyint
;	 calc	 8,3,submap3_xdeltaz,submap3_ydeltaz,submap3_xdeltazint
	calc	28,4,submap3_xdeltag,submap3_ydeltag,submap3_xdeltagint

	popad
	ret
endp
proc	submap3_calcb	  data:dword,p1:dword,p3:dword
;data er destinationen med f›lgende form:
;   altid:  langdeltax,langdeltaxint,langx
;p1 og p3 er to endepunkter
;prestep : stykke der skal steppes ekstre ned til f›rste raster linie


local	xstep:dword
local	k:dword
local	temp:dword
;	 submap_langdeltax=(float)p3->sx/p3->sy*65536;
;	 submap_langdeltaxint=submap_langdeltax;
;	 submap_langx=p1->sx+prestep*submap_langdeltax;



	pushad

	fld	[submap3_prestep]

	mov	esi,[p3]
	mov	edi,[data]
	fild	[dword ptr esi+16]
	mov	edx,[p1]
	fidivr	[dword ptr esi+12]
       fmul	[fp65536]
	fst	[dword ptr edi]
	fist	[dword ptr edi+4]

	fmul
	fiadd	 [dword ptr edx+12]
	fistp	[dword ptr edi+8]

	popad
	ret
endp

macro	submapper
macro	perspek p
;void	 perspek(mapp2 *p)
;{
;	 p->sx=p->x/p->z*65536*256+160*65536;
;	 p->sy=p->y/p->z*65536*256+100*65536;
;}

	if klip_on

	fld    [dword ptr p+8]
	fdivr	[fp2p24]
	fld	st(0)
	fmul	[dword ptr p+4]       ;Disse to linie
	fxch			      ;
	fmul	[dword ptr p+0]       ;er byttet om !
	fxch
	fmul	[screenscalex]
	fxch
	fmul	[screenscaley]
	fxch
	fadd	[fpmagic160g65536]
	fxch
	fadd	[fpmagic100g65536]
	fxch
	fstp	[temp1]
	fstp	[temp2]
	mov	edx,[dword ptr temp1]
	mov	ecx,[dword ptr temp2]

	mov	[dword ptr p+12],edx
	mov	[dword ptr p+16],ecx

	else

	fld    [dword ptr p+8]
	fdivr	[fp2p24]
	fld	st(0)
	fmul	[dword ptr p+0]
	fxch
	fmul	[dword ptr p+4]
	fxch
	fadd	[fpmagic160g65536]
	fxch
	fadd	[fpmagic100g65536]
	fxch
	fstp	[temp1]
	fstp	[temp2]
	mov	edx,[dword ptr temp1]
	mov	ecx,[dword ptr temp2]

	mov	[dword ptr p+12],edx
	mov	[dword ptr p+16],ecx



	endif

endm


;***************************************************
macro	mrel	p2,p1
;void	 mrel(mapp2 *p1, mapp2 *p2)
;{
;	 p1->sx-=p2->sx;
;	 p1->sy-=p2->sy;
;	 p1->z-=p2->z;
;	 p1->g-=p2->g;
;	 p1->tx-=p2->tx;
;	 p1->ty-=p2->ty;
;}
	mov	eax,[p1+12]
	mov	edx,[p2+12]
	sub	edx,eax
	mov	eax,[p1+16]
	mov	[p2+12],edx
	mov	edx,[p2+16]
	sub	edx,eax
	;nop
	mov	[p2+16],edx

	fld	[dword ptr p2+8]
	fsub	[dword ptr p1+8]
	fld	[dword ptr p2+20]
	fsub	[dword ptr p1+20]
	fld	[dword ptr p2+24]
	fsub	[dword ptr p1+24]
	fld	[dword ptr p2+28]
	fsub	[dword ptr p1+28]
	fxch	st(3)
	fstp	[dword ptr p2+8]
	fstp	[dword ptr p2+24]
	fstp	[dword ptr p2+20]
	fstp	[dword ptr p2+28]
endm
;***************************************************
macro	detcalc3 offset1,dest1,dest1i,dest2
;antag at stakken har form:
;p3->sy/det
;p3->sx/det
;p2->sy/det
;p2->sx/det
;edi er p2 og ebx p3

;	 submap_xdeltatx=det( p2->tx, p2->sy,  p3->tx, p3->sy )*detinv;
;	 submap_ydeltatx=det( p2->sx, p2->tx,  p3->sx, p3->tx )*detinv;

;16 cykler  +6
	fld	[dword ptr edi+offset1] ;2	  ;3sx*2tx
	fmul	st(0),st(2)		;2
	fld	[dword ptr ebx+offset1] ;1	  ;2sy*3tx
	fmul	st(0),st(4)		;1
	fld	[dword ptr edi+offset1] ;1	  ;3sy*2tx
	fmul	st(0),st(3)		;1
	fld	[dword ptr ebx+offset1] ;2	  ;2sx*3tx
	fmul	st(0),st(7)		;2
	fxch	st(2)			;1
	fsub				;1
	fxch	st(2)			;
	fsub				;2
	fxch				;
	fst	[dword ptr dest1]	;1
       fistp   [dword ptr dest1i]
	fstp	[dword ptr dest2]	;2
endm


;***************************************************

	pushad

	mov	esi,[p1]
	mov	edi,[p2]
	mov	ebx,[p3]

	perspek esi					;beregn perspektiv
	perspek edi
	perspek ebx

	mov	ecx,[esi+16]				;sorter punkter
	mov	edx,[edi+16]
	cmp	ecx,edx
	jle	@@noswap1
	mov	eax,esi
	mov	esi,edi
	mov	edi,eax
@@noswap1:
	mov	ecx,[edi+16]
	mov	edx,[ebx+16]
	cmp	ecx,edx
	jle	@@noswap3
	mov	eax,edi
	mov	edi,ebx
	mov	ebx,eax

	mov	ecx,[esi+16]
	cmp	ecx,edx
	jle	@@noswap3
	mov	eax,esi
	mov	esi,edi
	mov	edi,eax
@@noswap3:

	mov	eax,[esi+16]				;beregn destination
	mov	ecx,[dest]
	sar	eax,16
	shl	eax,swidthp
	add	ecx,eax
	shl	eax,2
	add	ecx,eax
	mov	[submap3_dest],ecx

	mov	ecx,[edi+8]				;husk punkt2
	mov	[submap3_tempz],ecx
	mov	ecx,[edi+12]
	mov	[submap3_tempsx],ecx
	mov	ecx,[edi+16]
	mov	[submap3_tempsy],ecx
	mov	ecx,[edi+20]
	mov	[submap3_temptx],ecx
	mov	ecx,[edi+24]
	mov	[submap3_tempty],ecx
	mov	ecx,[edi+28]
	mov	[submap3_tempg],ecx

	mov	ecx,[ebx+16]				;.. og sy v‘rdi til punkt 3
	mov	[submap3_tempsy2],ecx

	mrel	ebx,esi 				;g›r punkter relative
	mrel	edi,esi 				;til f›rste punkt


	fild	[dword ptr edi+12]			;beregn determinant
	fild	[dword ptr edi+16]
	fild	[dword ptr ebx+12]
	fild	[dword ptr ebx+16]
	fld	st(3)
	fld	st(3)
	fxch
	fmul	st(0),st(2)
	fxch
	fmul	st(0),st(3)

	fsub

	fst	[submap3_detsign]

	fdivr	[fp65536]				;inverter
	fmul	st(4),st(0)
	fmul	st(3),st(0)
	fmul	st(2),st(0)
	fmul	st(1),st(0)
	fstp	[temp1]
							;beregn delta v‘rdier
							;ud fra determinanter

if intptx
	detcalc3 20,submap3_xdeltatx,submap3_xdeltatxint,submap3_ydeltatx
endif
if intpty
	detcalc3 24,submap3_xdeltaty,submap3_xdeltatyint,submap3_ydeltaty
endif
if intpz
	detcalc3 8,submap3_xdeltaz,submap3_xdeltazint,submap3_ydeltaz
endif
if intpg
	detcalc3 28,submap3_xdeltag,submap3_xdeltagint,submap3_ydeltag
endif

	fstp	st(3)					;t›m stakken
	fstp	st(2)
	fstp	st(1)
	fstp	[temp1]

	preinit


	mov	edx,[esi+16]				;beregn prestep
	mov	ecx,[submap3_tempsy]
	xor	edx,-1
	xor	ecx,-1
	and	edx,65535
	and	ecx,65535
	mov	[submap3_prestep],edx
	mov	[submap3_prestep2],ecx
	fild	[submap3_prestep]
	fild	[submap3_prestep2]
	fxch
	fmul	[fp1d65536]
	fxch
	fmul	[fp1d65536]
	fxch
	fstp	[submap3_prestep]
	fstp	[submap3_prestep2]


	mov	eax,[submap3_detsign]
	or	eax,eax
	jl	@@kort

;langsiden er til venstre

	mov	ecx,[ebx+16]				;er det ok at dividere?
	or	ecx,ecx
	je	@@quit

	push	ebx
	push	esi
	push	offset submap3_venstredata
	call	submap3_calc				;..og start og delta v‘rdier
	add	esp,12					;til langsiden

	mov	ecx,[edi+16]				;ok at dividere?
	or	ecx,ecx
	je	@@ingenforstedellang

	push	edi
	push	esi
	push	offset submap3_hojredata
	call	submap3_calcb				 ;beregn start	og delta
	add	esp,12					;til den korte side

	mov	dl,1					;set flag <=>
	mov	eax,[esi+16]				;beregn current y
	mov	ecx,[submap3_tempsy]			;og counter-y
	jmp	@@hopind2

@@ingenforstedellang:
@@hophop1:
	mrel	ebx,edi 				;g›r 3. punkt relativt

	mov	edx,[submap3_prestep2]
	mov	[submap3_prestep],edx

	mov	ecx,[ebx+16]				;beregn prestep
	or	ecx,ecx
	je	@@quit					;ok at dividere?

	push	ebx
	push	offset submap3_temp
	push	offset submap3_hojredata
	call	submap3_calcb				 ;kort-start og -delta
	add	esp,12

	xor	edx,edx 				;slet flag ,sidste del
	mov	eax,[submap3_tempsy]			;beregn current y
	mov	ecx,[submap3_tempsy2]			;og counter-y
	jmp	@@hopind2



@@kort:
	mov	ecx,[ebx+16]				;er det ok at dividere?
	or	ecx,ecx
	je	@@quit

	push	ebx
	push	esi
	push	offset submap3_hojredata
	call	submap3_calcb				 ;..og start og delta v‘rdier
	add	esp,12					;til langsiden

	mov	ecx,[edi+16]				;ok at dividere?
	or	ecx,ecx
	je	@@ingenforstedelkort

	push	edi
	push	esi
	push	offset submap3_venstredata
	call	submap3_calc				;beregn start og delta v‘rdier
	add	esp,12					;til den korte side

	mov	dl,2					;set flag
	mov	eax,[esi+16]				;beregn current y
	mov	ecx,[submap3_tempsy]			;og counter-y
	jmp	@@hopind2

@@hophop2:
@@ingenforstedelkort:
	mrel	ebx,edi 				;g›r 3. punkt relativt

	mov	edx,[submap3_prestep2]
	mov	[submap3_prestep],edx

	mov	ecx,[ebx+16]				;beregn prestep
	or	ecx,ecx
	je	@@quit					;ok at dividere?

	push	ebx
	push	offset submap3_temp
	push	offset submap3_venstredata
	call	submap3_calc				;kort-start og -delta
	add	esp,12

	xor	edx,edx 				;slet flag ,sidste del
	mov	eax,[submap3_tempsy]			;beregn current y
	mov	ecx,[submap3_tempsy2]			;og counter-y

;	 jmp	 @@hopind2

@@hopind2:
	sar	eax,16
	mov	[submap3_flag],edx
	sar	ecx,16
	mov	[submap3_cury],eax
	cmp	ecx,200
	jle	@@yok
	mov	ecx,200
@@yok:
	sub	ecx,eax
	mov	[submap3_couy],ecx
	pushad						;gem punktpointer p† stak
			 ;bevar flag
	mov	edi,[submap3_dest]			;initialiser edi

	jmp	@@hopind


@@loop:
	or	eax,eax
	jl	@@udenfor				;indenfor sk‘rm?

							;beregn start og slut x
	mov	eax,[submap3_hojrex]
	mov	esi,offset submap3_venstredata		   ;interpolationsdata
	sar	eax,16
	mov	ebx,[submap3_venstrex]
	sar	ebx,16


	cmp	eax,320 				;h›jre side ok?
	jl	@@hok

	mov	eax,320
@@hok:
	cmp	ebx,0					;venstre side ok?
	jge	@@vok
	xor	ecx,ecx 				;ellers step nogle gange
	sub	ecx,ebx
	xor	ebx,ebx

macro	premul	dest1,fak1,src2
	mov	ebp,src2
	mov	edx,fak1
	imul	edx,ecx
	add	edx,ebp
	mov	dest1,edx
endm
if intptx
	premul	[submap3_faketx],[submap3_xdeltatxint],[esi+12*1],ebp
endif
if intpty
	premul	[submap3_fakety],[submap3_xdeltatyint],[esi+12*2],ebp
endif
if intpz
	premul	[submap3_fakez],[submap3_xdeltazint],[esi+12*3],ebp
endif
if intpg
	premul	[submap3_fakeg],[submap3_xdeltagint],[esi+12*4],ebp
endif

	mov	esi,offset submap3_fakedata
@@vok:

	innerloop

@@udenfor:
							;+ deltav‘rdier
	mov	ebx,[submap3_venstrex]
	mov	edx,[submap3_venstredeltaxint]
	add	ebx,edx
	mov	ecx,[submap3_hojredeltaxint]
	mov	eax,[submap3_hojrex]
	mov	[submap3_venstrex],ebx
	add	eax,ecx
	sub	bx,dx
							;unders›g om der var
							;mente og s‘t flag
	sbb	edx,edx
	mov	[submap3_hojrex],eax

macro	submap_add	d2,s2
	mov	edi,[d2]
	mov	ebx,[s2+4+4*edx]
	add	edi,ebx
	mov	[d2],edi
endm
							;+ passende delta
if intptx and intpty and intpg and ( not intpz )
	mov	edi,[submap3_venstretx]
	mov	eax,[submap3_venstrety]
	mov	ecx,[submap3_venstredeltatx+4+4*edx]
	mov	ebx,[submap3_venstredeltaty+4+4*edx]
	add	edi,ecx
	mov	ecx,[submap3_venstreg]
	add	eax,ebx
	mov	ebx,[submap3_venstredeltag+4+4*edx]
	add	ecx,ebx
	mov	[submap3_venstretx],edi
	mov	[submap3_venstrety],eax
	mov	[submap3_venstreg],ecx
else

if intptx
	submap_add submap3_venstretx,submap3_venstredeltatx
endif
if intpty
	submap_add submap3_venstrety,submap3_venstredeltaty
endif
if intpz
	submap_add submap3_venstrez,submap3_venstredeltaz
endif
if intpg
	submap_add submap3_venstreg,submap3_venstredeltag
endif
endif

							;opdater screen pointer
							;og y-variablerne
	mov	edi,[submap3_dest]
	mov	eax,[submap3_cury]
	add	edi,swidth
	mov	ecx,[submap3_couy]
	inc	eax
	mov	[submap3_dest],edi
	dec	ecx
	mov	[submap3_cury],eax
	mov	[submap3_couy],ecx

@@hopind:
	jg     @@loop
							;restore esi,edi og ebx
	popad
	mov	eax,[submap3_flag]
	cmp	al,1
	je	@@hophop1
	cmp	al,2
	je	@@hophop2
@@quit:

	popad
	ret
;@@quitpop:
;	 popad
;	 popad
;	 ret
dataseg
	align	4
@@jumptable:
	dd	@@jin1
	dd	@@jin2
	dd	@@jin3
	dd	@@jin4
	dd	@@jin5
	dd	@@jin6
	dd	@@jin7
	dd	@@jin8
codeseg
endm


proc	submap3g  p1:dword,p2:dword,p3:dword,mapnr:dword,dest:dword
;Denne rutine tager mod tre punkter, som den selv beregner perspektiv p†.
;Den tegner en trekant mellem punkterne.
;Den f›rste vandrette linie p† sk‘rmen der ‘ndres er x1&ffff0000.
;Denne linie f†r v‘rdier der svarer til linien x1&ffff0000+ffff.
;***************************************************

intptx=1
intpty=1
intpg=1
intpz=0
swidth=320*4*2
swidthp=9

macro	innera	offset1
	mov	edx,ebp 		;u
	mov	[edi+offset1*8+4],ecx	; v
	shl	edx,8			;u
	add	ebp,esi 		; v
	mov	dx,bx			;u
;   mov     ch,[edi]
	adc	ebx,eax 		;u
	mov	eax,[submap3p_add3]	; v
	adc	ch,ah			;u
	mov	[edi+offset1*8],edx	; v
	mov	eax,[submap3p_add2]	;u
endm
macro	innerb	offset1
	mov	edx,ebp 		; v
	shl	edx,8			;u
	add	ebp,esi 		; v
	mov	dx,bx			;u
;   mov     ch,[edi]
	adc	ebx,eax 		;u
	mov	[edi+offset1*8+4],ecx	; v
	mov	eax,[submap3p_add3]	;u
	mov	[edi+offset1*8],edx	; v
	adc	ch,ah			;u
	mov	eax,[submap3p_add2]	; v
endm
;invariant: esi=add2
macro	innera2  offset1
	mov	edx,ebp 		;u
	mov	[edi+offset1*8+4],ecx	; v
	mov	eax,[submap3p_add1]	;u
	mov	dl,bh			; v
	shl	edx,8			;u
	add	ebp,eax 		; v
	mov	dl,bl			;u
	mov	eax,[submap3p_add3]	; v
	adc	ebx,esi 		;u
	mov	[edi+offset1*8],edx	; v
	adc	ch,ah			;u
endm
macro	innerb2  offset1
	mov	edx,ebp 		; v
	mov	eax,[submap3p_add1]	;u
	mov	dl,bh			; v
	shl	edx,8			;u
	add	ebp,eax 		; v
	mov	dl,bl			;u
	mov	[edi+offset1*8+4],ecx	; v
	adc	ebx,esi 		;u
	mov	eax,[submap3p_add3]	; v
	adc	ch,ah			;u
	mov	[edi+offset1*8],edx	; v

endm
macro	innerc	offset1
	mov	edx,ebp 		; v
	shl	edx,8			;u
	add	ebp,esi 		; v
	mov	dx,bx			;u
	mov	[edi+offset1*8+4],ecx	; v
	adc	ebx,eax 		;u
	mov	eax,[submap3p_add3]	; v
	adc	ch,ah			;u
	mov	[edi+offset1*8],edx	; v
	mov	eax,[submap3p_add2]	;u
endm

macro	preinit
	mov	ecx,[submap3_xdeltatyint]
	mov	edx,[submap3_xdeltatxint]
	shl	ecx,24
	and	edx,65536*256-1
	add	ecx,edx
	mov	edx,[submap3_xdeltatyint]
	shr	edx,8
	mov	[submap3p_add1],ecx
	and	edx,65535
	mov	ecx,[submap3_xdeltagint]
	shl	ecx,16
	add	edx,ecx
	mov	ecx,[submap3_xdeltagint]
	mov	[submap3p_add2],edx
	shr	ecx,8
	and	ecx,255*256
	mov	[submap3p_add3],ecx
	mov	cl,[byte ptr mapnr]
	mov	[byte ptr submap3p_mapnr],cl

endm

macro	innerloop
;nop
;nop
   if 0
	sub	eax,ebx
	jle	@@udenfor

	dec	eax
	mov	ecx,eax
	and	eax,7
	shl	ecx,8-3+8
	add	ebx,eax

	mov	cl,[byte ptr submap3p_mapnr]

	mov	ch,[esi+48+2]

	shl	ebx,3
	add	edi,ebx

	mov	edx,[dword ptr @@jumptable+eax*4]

	mov	ebp,[esi+24]	 ;ty
	mov	ebx,[esi+12]	 ;tx
	shl	ebp,24
	and	ebx,65536*256-1
	add	ebp,ebx
	mov	ebx,[esi+24]	 ;ty
	and	ebx,65536*256-1
	shr	ebx,8
	mov	eax,[esi+48]
	shl	eax,16
	add	ebx,eax
	mov	eax,[submap3p_add2]

	mov	esi,[submap3p_add1]
	jmp	edx
   endif
	sub	eax,ebx
	jle	@@udenfor
	dec	eax

	mov	ecx,eax
	and	eax,7
	shl	ecx,8-3+8
	add	ebx,eax
	shl	ebx,3

	add	edi,ebx
	mov	edx,[dword ptr @@jumptable+eax*4]
	mov	ebp,[esi+24]	 ;ty
	mov	ebx,[esi+12]	 ;tx
	shl	ebp,24
	and	ebx,65536*256-1
	add	ebp,ebx
	mov	ebx,[esi+24]	 ;ty
	and	ebx,65536*256-1
	mov	cl,[byte ptr submap3p_mapnr]
	shr	ebx,8
	mov	eax,[esi+48]
	shl	eax,16
	mov	ch,[esi+48+2]
	add	ebx,eax
;	 mov	 esi,[submap3p_add1]
	mov	esi,[submap3p_add2]   ;eax
	jmp	edx
@@xloop:
if 1
@@jin8:
;	 mov	 al,[edi-32-24]
	innerb2  -7
@@jin7: innera2  -6
@@jin6: innerb2  -5
@@jin5: innera2  -4
@@jin4: innerb2  -3
@@jin3: innera2  -2
@@jin2: innerb2  -1
@@jin1: innera2  0
else
@@jin8: innerc	-7
@@jin7: innerc	-6
@@jin6: innerc	-5
@@jin5: innerc	-4
@@jin4: innerc	-3
@@jin3: innerc	-2
@@jin2: innerc	-1
@@jin1: innerc	0
endif
	add	edi,8*4*2
	sub	ecx,65536
	jge	@@xloop
endm
	submapper

endp
;************************************************************************
;************************************************************************
;************************************************************************
;************************************************************************
;************************************************************************
;************************************************************************
;************************************************************************
;************************************************************************

proc	submap3_calcf	  data:dword,p1:dword,p3:dword
;data er destinationen med f›lgende form:
;   altid:  langdeltax,langdeltaxint,langx
;   for hver v‘rdi der skal interpoleres: lang,langdelta[2]
;p1 og p3 er to endepunkter
;prestep : stykke der skal steppes ekstre ned til f›rste raster linie


local	xstep:dword
local	k:dword
local	temp:dword
;	 submap_langdeltax=(float)p3->sx/p3->sy*65536;
;	 submap_langdeltaxint=submap_langdeltax;
;	 submap_langx=p1->sx+prestep*submap_langdeltax;
;	 xstepl=((int)submap_langdeltax)>>16;
;For hver variabel der skal interpoleres:
;	 submap_langtx=p1->tx+prestep*(submap_ydeltatx+submap_xdeltatx*submap_langdeltax/65536);
;  submap_langtx-=submap_xdeltatx*(submap_langx&65535)/65536;
;	 submap_langdeltatx[0]=xstepl*submap_xdeltatx+submap_ydeltatx;
;	 submap_langdeltatx[1]=submap_langdeltatx[0]+submap_xdeltatx;


macro	calc	offset1,offset2,xd,yd,xdint
	fild	 [xstep]
	fmul	[xd]
	fld	[submap3_prestep]
	fmul	[yd]
	fxch
	fadd	[yd]
	fld	[k]
	fmul	[xd]
	fxch	st(2)
	fadd	[dword ptr edx+offset1]
	fld	st(1)
	fadd	[magic]
	fxch	st(3)
	fadd
	fxch
	fadd	[xd]
	fxch
	fadd	[magic] 		;stall
	fxch	st(2)
	fstp	[temp2]
	fadd	[magic]
	fxch
	fstp	[temp1] 		;stall
	fstp	[temp3]

	mov	eax,[dword ptr temp1]
	mov	ebx,[dword ptr temp2]
	mov	ecx,[dword ptr temp3]
    add     eax,[xdint]
	mov	[edi+12*offset2+0],eax
	mov	[edi+12*offset2+8],ebx		;byt!
	mov	[edi+12*offset2+4],ecx		;byt!!

endm
macro	calcf	 offset1,offset2,xd,yd,xdint
	fild	 [xstep]
	fmul	[xd]
	fld	[submap3_prestep]
	fmul	[yd]
	fxch
	fadd	[yd]
	fld	[k]
	fmul	[xd]
	fxch	st(2)
	fadd	[dword ptr edx+offset1]
	fld	st(1)
;	 fadd	 [magic]
	fxch	st(3)
	fadd
	fxch
	fadd	[xd]
	fxch
;	 fadd	 [magic]		 ;stall
	fxch	st(2)
	fstp	[dword ptr edi+12*offset2+8]
;	 fadd	 [magic]
	fxch
	fadd	[xdint]
	fstp	[dword ptr edi+12*offset2+0]		     ;stall
	fstp	[dword ptr edi+12*offset2+4]

;	 mov	 eax,[dword ptr temp1]
;	 mov	 ebx,[dword ptr temp2]
;	 mov	 ecx,[dword ptr temp3]
;    add     eax,[xdint]
;	 mov	 [edi+12*offset2+0],eax
;	 mov	 [edi+12*offset2+8],ebx 	 ;byt!
;	 mov	 [edi+12*offset2+4],ecx 	 ;byt!!

endm
;	 submap_langdeltax=(float)p3->sx/p3->sy*65536;
;	 submap_langdeltaxint=submap_langdeltax;
;	 submap_langx=p1->sx+prestep*submap_langdeltax;
;	 xstepl=((int)submap_langdeltax)>>16;



	pushad

	fld	[submap3_prestep]

	mov	esi,[p3]
	mov	edi,[data]
	fild	[dword ptr esi+16]
	mov	edx,[p1]
	fidivr	[dword ptr esi+12]
       fmul	[fp65536]
	fst	[dword ptr edi]
	fist	[dword ptr edi+4]	       ;! float

	fmul
	mov	ebx,[edi+4]
	sar	ebx,16
	fiadd	 [dword ptr edx+12]
	mov	[xstep],ebx
	fistp	[dword ptr edi+8]		;! float

	fld	[dword ptr edi]
	fmul	[submap3_prestep]
	mov	ebx,[edi+8]
	and	ebx,65535
	mov	[temp],ebx
	fild	[temp]
	fsub
	fmul	[fp1d65536]
	fstp	[k]

	calcf	 20,1,submap3_xdeltatx,submap3_ydeltatx,submap3_xdeltatx
	calcf	 24,2,submap3_xdeltaty,submap3_ydeltaty,submap3_xdeltaty
	calcf	 8,3,submap3_xdeltaz,submap3_ydeltaz,submap3_xdeltaz
        calc    28,4,submap3_xdeltag,submap3_ydeltag,submap3_xdeltagint

	popad
	ret
endp
proc	submap3_calcbf	   data:dword,p1:dword,p3:dword
;data er destinationen med f›lgende form:
;   altid:  langdeltax,langdeltaxint,langx
;p1 og p3 er to endepunkter
;prestep : stykke der skal steppes ekstre ned til f›rste raster linie


local	xstep:dword
local	k:dword
local	temp:dword
;	 submap_langdeltax=(float)p3->sx/p3->sy*65536;
;	 submap_langdeltaxint=submap_langdeltax;
;	 submap_langx=p1->sx+prestep*submap_langdeltax;



	pushad

	fld	[submap3_prestep]

	mov	esi,[p3]
	mov	edi,[data]
	fild	[dword ptr esi+16]
	mov	edx,[p1]
	fidivr	[dword ptr esi+12]
       fmul	[fp65536]
	fst	[dword ptr edi]
	fist	[dword ptr edi+4]		;! float

	fmul
	fiadd	 [dword ptr edx+12]
	fistp	[dword ptr edi+8]		 ;! float

	popad
	ret
endp

macro	submapperper
macro	perspek p
;void	 perspek(mapp2 *p)
;{
;	 p->sx=p->x/p->z*65536*256+160*65536;
;	 p->sy=p->y/p->z*65536*256+100*65536;
;}

	if klip_on

	fld    [dword ptr p+8]
	fdivr	[fp2p24]
	fld	st(0)
	fmul	[dword ptr p+4]       ;Disse to linie
	fxch			      ;
	fmul	[dword ptr p+0]       ;er byttet om !
	fxch
	fmul	[screenscalex]
	fxch
	fmul	[screenscaley]
	fxch
	fadd	[fpmagic160g65536]
	fxch
	fadd	[fpmagic100g65536]
	fxch
	fstp	[temp1]
	fstp	[temp2]
	mov	edx,[dword ptr temp1]
	mov	ecx,[dword ptr temp2]

	mov	[dword ptr p+12],edx
	mov	[dword ptr p+16],ecx

	else

	fld    [dword ptr p+8]
	fdivr	[fp2p24]
	fld	st(0)
	fmul	[dword ptr p+0]
	fxch
	fmul	[dword ptr p+4]
	fxch
	fadd	[fpmagic160g65536]
	fxch
	fadd	[fpmagic100g65536]
	fxch
	fstp	[temp1]
	fstp	[temp2]
	mov	edx,[dword ptr temp1]
	mov	ecx,[dword ptr temp2]

	mov	[dword ptr p+12],edx
	mov	[dword ptr p+16],ecx



	endif


	fld1				;beregn 1/z,u/z,v/z
	fdiv	 [dword ptr p+8]	; og erstat originale
	fst	[dword	ptr p+8]
	fld	[dword ptr p+20]
	fmul	st(0),st(1)
	fxch	st(1)
	fld	[dword ptr p+24]
	fmul
	fxch
	fstp	[dword ptr p+20]
	fstp	[dword ptr p+24]

endm


;***************************************************
macro	mrel	p2,p1
;void	 mrel(mapp2 *p1, mapp2 *p2)
;{
;	 p1->sx-=p2->sx;
;	 p1->sy-=p2->sy;
;	 p1->z-=p2->z;
;	 p1->g-=p2->g;
;	 p1->tx-=p2->tx;
;	 p1->ty-=p2->ty;
;}
	mov	eax,[p1+12]
	mov	edx,[p2+12]
	sub	edx,eax
	mov	eax,[p1+16]
	mov	[p2+12],edx
	mov	edx,[p2+16]
	sub	edx,eax
	;nop
	mov	[p2+16],edx

	fld	[dword ptr p2+8]
	fsub	[dword ptr p1+8]
	fld	[dword ptr p2+20]
	fsub	[dword ptr p1+20]
	fld	[dword ptr p2+24]
	fsub	[dword ptr p1+24]
	fld	[dword ptr p2+28]
	fsub	[dword ptr p1+28]
	fxch	st(3)
	fstp	[dword ptr p2+8]
	fstp	[dword ptr p2+24]
	fstp	[dword ptr p2+20]
	fstp	[dword ptr p2+28]
endm
;***************************************************
macro	detcalc3 offset1,dest1,dest1i,dest2
;antag at stakken har form:
;p3->sy/det
;p3->sx/det
;p2->sy/det
;p2->sx/det
;edi er p2 og ebx p3

;	 submap_xdeltatx=det( p2->tx, p2->sy,  p3->tx, p3->sy )*detinv;
;	 submap_ydeltatx=det( p2->sx, p2->tx,  p3->sx, p3->tx )*detinv;

;16 cykler  +6
	fld	[dword ptr edi+offset1] ;2	  ;3sx*2tx
	fmul	st(0),st(2)		;2
	fld	[dword ptr ebx+offset1] ;1	  ;2sy*3tx
	fmul	st(0),st(4)		;1
	fld	[dword ptr edi+offset1] ;1	  ;3sy*2tx
	fmul	st(0),st(3)		;1
	fld	[dword ptr ebx+offset1] ;2	  ;2sx*3tx
	fmul	st(0),st(7)		;2
	fxch	st(2)			;1
	fsub				;1
	fxch	st(2)			;
	fsub				;2
	fxch				;
	fst	[dword ptr dest1]	;1
       fistp   [dword ptr dest1i]
	fstp	[dword ptr dest2]	;2
endm


;***************************************************

	pushad

	mov	esi,[p1]
	mov	edi,[p2]
	mov	ebx,[p3]

	perspek esi					;beregn perspektiv
	perspek edi
	perspek ebx

	mov	ecx,[esi+16]				;sorter punkter
	mov	edx,[edi+16]
	cmp	ecx,edx
	jle	@@noswap1
	mov	eax,esi
	mov	esi,edi
	mov	edi,eax
@@noswap1:
	mov	ecx,[edi+16]
	mov	edx,[ebx+16]
	cmp	ecx,edx
	jle	@@noswap3
	mov	eax,edi
	mov	edi,ebx
	mov	ebx,eax

	mov	ecx,[esi+16]
	cmp	ecx,edx
	jle	@@noswap3
	mov	eax,esi
	mov	esi,edi
	mov	edi,eax
@@noswap3:

	mov	eax,[esi+16]				;beregn destination
	mov	ecx,[dest]
	sar	eax,16
	shl	eax,swidthp
	add	ecx,eax
	shl	eax,2
	add	ecx,eax
	mov	[submap3_dest],ecx

	mov	ecx,[edi+8]				;husk punkt2
	mov	[submap3_tempz],ecx
	mov	ecx,[edi+12]
	mov	[submap3_tempsx],ecx
	mov	ecx,[edi+16]
	mov	[submap3_tempsy],ecx
	mov	ecx,[edi+20]
	mov	[submap3_temptx],ecx
	mov	ecx,[edi+24]
	mov	[submap3_tempty],ecx
	mov	ecx,[edi+28]
	mov	[submap3_tempg],ecx

	mov	ecx,[ebx+16]				;.. og sy v‘rdi til punkt 3
	mov	[submap3_tempsy2],ecx

	mrel	ebx,esi 				;g›r punkter relative
	mrel	edi,esi 				;til f›rste punkt


	fild	[dword ptr edi+12]			;beregn determinant
	fild	[dword ptr edi+16]
	fild	[dword ptr ebx+12]
	fild	[dword ptr ebx+16]
	fld	st(3)
	fld	st(3)
	fxch
	fmul	st(0),st(2)
	fxch
	fmul	st(0),st(3)

	fsub

	fst	[submap3_detsign]

	fdivr	[fp65536]				;inverter
	fmul	st(4),st(0)
	fmul	st(3),st(0)
	fmul	st(2),st(0)
	fmul	st(1),st(0)
	fstp	[temp1]
							;beregn delta v‘rdier
							;ud fra determinanter

if intptx
	detcalc3 20,submap3_xdeltatx,submap3_xdeltatxint,submap3_ydeltatx
endif
if intpty
	detcalc3 24,submap3_xdeltaty,submap3_xdeltatyint,submap3_ydeltaty
endif
if intpz
	detcalc3 8,submap3_xdeltaz,submap3_xdeltazint,submap3_ydeltaz
endif
if intpg
	detcalc3 28,submap3_xdeltag,submap3_xdeltagint,submap3_ydeltag
endif

	fstp	st(3)					;t›m stakken
	fstp	st(2)
	fstp	st(1)
	fstp	[temp1]


	preinit


	mov	edx,[esi+16]				;beregn prestep
	mov	ecx,[submap3_tempsy]
	xor	edx,-1
	xor	ecx,-1
	and	edx,65535
	and	ecx,65535
	mov	[submap3_prestep],edx
	mov	[submap3_prestep2],ecx
	fild	[submap3_prestep]
	fild	[submap3_prestep2]
	fxch
	fmul	[fp1d65536]
	fxch
	fmul	[fp1d65536]
	fxch
	fstp	[submap3_prestep]
	fstp	[submap3_prestep2]


	mov	eax,[submap3_detsign]
	or	eax,eax
	jl	@@kort

;langsiden er til venstre

	mov	ecx,[ebx+16]				;er det ok at dividere?
	or	ecx,ecx
	je	@@quit

	push	ebx
	push	esi
	push	offset submap3_venstredata
	call	submap3_calcf				 ;..og start og delta v‘rdier
	add	esp,12					;til langsiden

	mov	ecx,[edi+16]				;ok at dividere?
	or	ecx,ecx
	je	@@ingenforstedellang

	push	edi
	push	esi
	push	offset submap3_hojredata
	call	submap3_calcbf				  ;beregn start  og delta
	add	esp,12					;til den korte side

	mov	dl,1					;set flag <=>
	mov	eax,[esi+16]				;beregn current y
	mov	ecx,[submap3_tempsy]			;og counter-y
	jmp	@@hopind2

@@ingenforstedellang:
@@hophop1:
	mrel	ebx,edi 				;g›r 3. punkt relativt

	mov	edx,[submap3_prestep2]
	mov	[submap3_prestep],edx

	mov	ecx,[ebx+16]				;beregn prestep
	or	ecx,ecx
	je	@@quit					;ok at dividere?

	push	ebx
	push	offset submap3_temp
	push	offset submap3_hojredata
	call	submap3_calcbf				  ;kort-start og -delta
	add	esp,12

	xor	edx,edx 				;slet flag ,sidste del
	mov	eax,[submap3_tempsy]			;beregn current y
	mov	ecx,[submap3_tempsy2]			;og counter-y
	jmp	@@hopind2



@@kort:
	mov	ecx,[ebx+16]				;er det ok at dividere?
	or	ecx,ecx
	je	@@quit

	push	ebx
	push	esi
	push	offset submap3_hojredata
	call	submap3_calcbf				  ;..og start og delta v‘rdier
	add	esp,12					;til langsiden

	mov	ecx,[edi+16]				;ok at dividere?
	or	ecx,ecx
	je	@@ingenforstedelkort

	push	edi
	push	esi
	push	offset submap3_venstredata
	call	submap3_calcf				 ;beregn start og delta v‘rdier
	add	esp,12					;til den korte side

	mov	dl,2					;set flag
	mov	eax,[esi+16]				;beregn current y
	mov	ecx,[submap3_tempsy]			;og counter-y
	jmp	@@hopind2

@@hophop2:
@@ingenforstedelkort:
	mrel	ebx,edi 				;g›r 3. punkt relativt

	mov	edx,[submap3_prestep2]
	mov	[submap3_prestep],edx

	mov	ecx,[ebx+16]				;beregn prestep
	or	ecx,ecx
	je	@@quit					;ok at dividere?

	push	ebx
	push	offset submap3_temp
	push	offset submap3_venstredata
	call	submap3_calcf				 ;kort-start og -delta
	add	esp,12

	xor	edx,edx 				;slet flag ,sidste del
	mov	eax,[submap3_tempsy]			;beregn current y
	mov	ecx,[submap3_tempsy2]			;og counter-y

;	 jmp	 @@hopind2

@@hopind2:
	sar	eax,16
	mov	[submap3_flag],edx
	sar	ecx,16
	mov	[submap3_cury],eax
	cmp	ecx,200
	jle	@@yok
	mov	ecx,200
@@yok:
	sub	ecx,eax
	mov	[submap3_couy],ecx
	pushad						;gem punktpointer p† stak
			 ;bevar flag
	mov	edi,[submap3_dest]			;initialiser edi

	jmp	@@hopind


@@loop:
	or	eax,eax
	jl	@@udenfor				;indenfor sk‘rm?

							;beregn start og slut x
	mov	eax,[submap3_hojrex]
	mov	esi,offset submap3_venstredata		   ;interpolationsdata
	sar	eax,16
	mov	ebx,[submap3_venstrex]
	sar	ebx,16


	cmp	eax,320 				;h›jre side ok?
	jl	@@hok

	mov	eax,320
@@hok:
	cmp	ebx,0					;venstre side ok?
	jge	@@vok
	xor	ecx,ecx 				;ellers step nogle gange
	sub	ecx,ebx
	xor	ebx,ebx

	mov	[submap3per_tempa],ecx

macro	premul	dest1,fak1,src2
	mov	ebp,[src2]
	mov	edx,fak1
	imul	edx,ecx
	add	edx,ebp
	mov	dest1,edx
endm
macro	premulf  dest1,fak1,src2
	fild	[submap3per_tempa]
	fmul	fak1
	fadd	[dword ptr src2]
	fstp	dest1
endm
if intptx
	premulf  [submap3_faketx],[submap3_xdeltatx],esi+12*1
endif
if intpty
	premulf  [submap3_fakety],[submap3_xdeltaty],esi+12*2
endif
if intpz
	premulf  [submap3_fakez],[submap3_xdeltaz],esi+12*3
endif
if intpg
	premul	[submap3_fakeg],[submap3_xdeltagint],esi+12*4
endif

	mov	esi,offset submap3_fakedata
@@vok:

	innerloop

@@udenfor:
							;+ deltav‘rdier
	mov	ebx,[submap3_venstrex]
	mov	edx,[submap3_venstredeltaxint]
	add	ebx,edx
	mov	ecx,[submap3_hojredeltaxint]
	mov	eax,[submap3_hojrex]
	mov	[submap3_venstrex],ebx
	add	eax,ecx
	sub	bx,dx
							;unders›g om der var
							;mente og s‘t flag
	sbb	edx,edx
	mov	[submap3_hojrex],eax

macro	submap_add	d2,s2
	mov	edi,[d2]
	mov	ebx,[s2+4+4*edx]
	add	edi,ebx
	mov	[d2],edi
endm
macro	submap_addf	 d2,s2
	fld	[d2]
	fadd	[s2+4+4*edx]
	fstp	[d2]
endm
							;+ passende delta

if intptx
	submap_addf submap3_venstretx,submap3_venstredeltatx
endif
if intpty
	submap_addf submap3_venstrety,submap3_venstredeltaty
endif
if intpz
	submap_addf submap3_venstrez,submap3_venstredeltaz
endif
if intpg
	submap_add submap3_venstreg,submap3_venstredeltag
endif

							;opdater screen pointer
							;og y-variablerne
	mov	edi,[submap3_dest]
	mov	eax,[submap3_cury]
	add	edi,swidth
	mov	ecx,[submap3_couy]
	inc	eax
	mov	[submap3_dest],edi
	dec	ecx
	mov	[submap3_cury],eax
	mov	[submap3_couy],ecx

@@hopind:
	jg     @@loop
							;restore esi,edi og ebx
	popad
	mov	eax,[submap3_flag]
	cmp	al,1
	je	@@hophop1
	cmp	al,2
	je	@@hophop2
@@quit:

	popad
	ret
;@@quitpop:
;	 popad
;	 popad
;	 ret
dataseg
	align	4
@@jumptable:
	dd	@@jin1
	dd	@@jin2
	dd	@@jin3
	dd	@@jin4
	dd	@@jin5
	dd	@@jin6
	dd	@@jin7
	dd	@@jin8
codeseg
endm


proc	submap3per  p1:dword,p2:dword,p3:dword,mapnr:dword,dest:dword
;Denne rutine tager mod tre punkter, som den selv beregner perspektiv p†.
;Den tegner en trekant mellem punkterne.
;Den f›rste vandrette linie p† sk‘rmen der ‘ndres er x1&ffff0000.
;Denne linie f†r v‘rdier der svarer til linien x1&ffff0000+ffff.
;***************************************************

intptx=1
intpty=1
intpg=1
intpz=1
swidth=320*4*2
swidthp=9

macro	innera	offset1
	mov	edx,ebp 		;u
	mov	[edi+offset1*8+4],ecx	; v
	shl	edx,8			;u
	add	ebp,esi 		; v
	mov	dx,bx			;u
;   mov     ch,[edi]
	adc	ebx,eax 		;u
	mov	eax,[submap3p_add3]	; v
	adc	ch,ah			;u
	mov	[edi+offset1*8],edx	; v
	mov	eax,[submap3p_add2]	;u
endm
macro	innerb	offset1
	mov	edx,ebp 		; v
	shl	edx,8			;u
	add	ebp,esi 		; v
	mov	dx,bx			;u
;   mov     ch,[edi]
	adc	ebx,eax 		;u
	mov	[edi+offset1*8+4],ecx	; v
	mov	eax,[submap3p_add3]	;u
	mov	[edi+offset1*8],edx	; v
	adc	ch,ah			;u
	mov	eax,[submap3p_add2]	; v
endm
;invariant: esi=add2
macro	innera2  offset1
	mov	edx,ebp 		;u
	mov	[edi+offset1*8+4],ecx	; v
	mov	eax,[submap3p_add1]	;u
	mov	dl,bh			; v
	shl	edx,8			;u
	add	ebp,eax 		; v
	mov	dl,bl			;u
	mov	eax,[submap3p_add3]	; v
	adc	ebx,esi 		;u
	mov	[edi+offset1*8],edx	; v
	adc	ch,ah			;u
endm
macro	innerb2  offset1
	mov	edx,ebp 		; v
	mov	eax,[submap3p_add1]	;u
	mov	dl,bh			; v
	shl	edx,8			;u
	add	ebp,eax 		; v
	mov	dl,bl			;u
	mov	[edi+offset1*8+4],ecx	; v
	adc	ebx,esi 		;u
	mov	eax,[submap3p_add3]	; v
	adc	ch,ah			;u
	mov	[edi+offset1*8],edx	; v

endm
macro	innerc	offset1
	mov	edx,ebp 		; v
	shl	edx,8			;u
	add	ebp,esi 		; v
	mov	dx,bx			;u
	mov	[edi+offset1*8+4],ecx	; v
	adc	ebx,eax 		;u
	mov	eax,[submap3p_add3]	; v
	adc	ch,ah			;u
	mov	[edi+offset1*8],edx	; v
	mov	eax,[submap3p_add2]	;u
endm

macro	innerd1  offset1
	mov	[edi+8*offset1],ebx
	add	ebx,eax
	mov	[edi+4+8*offset1],ecx
endm

macro	innerd2  offset1
	mov	[edi+4+8*offset1],ecx
	mov	[edi+8*offset1],ebx
	add	ebx,eax
endm

macro	preinit
	if 0
	mov	ecx,[submap3_xdeltatyint]
	mov	edx,[submap3_xdeltatxint]
	shl	ecx,24
	and	edx,65536*256-1
	add	ecx,edx
	mov	edx,[submap3_xdeltatyint]
	shr	edx,8
	mov	[submap3p_add1],ecx
	and	edx,65535
	mov	ecx,[submap3_xdeltagint]
	shl	ecx,16
	add	edx,ecx
	mov	ecx,[submap3_xdeltagint]
	mov	[submap3p_add2],edx
	shr	ecx,8
	and	ecx,255*256
	mov	[submap3p_add3],ecx
	endif

	fld	[submap3_xdeltatx]
	fmul	[fp8]
	fld	[submap3_xdeltaty]
	fmul	[fp8]
	fld	[submap3_xdeltaz]
	fmul	[fp8]
	fxch	st(2)
	fstp	[submap3per_8deltatx]
	fstp	[submap3per_8deltaty]
	fstp	[submap3per_8deltaz]


	mov	cl,[byte ptr mapnr]
	mov	[byte ptr submap3p_mapnr],cl

endm

macro	innerloop
	sub	eax,ebx
	jle	@@udenfor
	fld1					  ;beregn z
	fdiv	[submap3_venstrez]
	dec	eax

	mov	ecx,eax
	and	eax,7
	shl	ecx,8-3+8
	add	ebx,eax
	shl	ebx,3

	add	edi,ebx
	mov	edx,[dword ptr @@jumptable+eax*4]


		       ;step a pixels ind...
	fld	[submap3_xdeltaz]
	fmul	[inttofloat+eax*4]
	fld	[submap3_xdeltatx]
	fmul	[inttofloat+eax*4]
	fld	[submap3_xdeltaty]
	fmul	[inttofloat+eax*4]
	fxch	st(2)
	fadd	[submap3_venstrez]
	fxch	st(1)
	fadd	[submap3_venstretx]
	fxch	st(2)
	fadd	[submap3_venstrety]
	fxch	st(1)
	fstp	[submap3per_currentz]
	fxch	st(1)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]

;	 fld	 [submap3_xdeltaz]
;	 fmul	 [inttofloat+eax*4]
;	 fadd	 [submap3_venstrez]
;	 fstp	 [submap3per_currentz]
;	 fld	 [submap3_xdeltatx]
;	 fmul	 [inttofloat+eax*4]
;	 fadd	 [submap3_venstretx]
;	 fstp	 [submap3per_currenttx]
;	 fld	 [submap3_xdeltaty]
;	 fmul	 [inttofloat+eax*4]
;	 fadd	 [submap3_venstrety]
;	 fstp	 [submap3per_currentty]


	fld	[submap3_venstretx]		  ;beregn tx og ty
	fmul	st(0),st(1)
	fxch
	fmul	[submap3_venstrety]
	fxch
	fst   [dword ptr submap3per_temp3]		    ;tx som float
	fadd	[fp2p60]
	fstp   [submap3per_temp1]		 ;tx som int
	fst   [dword ptr submap3per_temp4]		    ;ty som float
	fadd	[fp2p60]
	fstp   [submap3per_temp2]		 ;ty som int

	fld1					  ;beregn n‘ste z
	fdiv	[submap3per_currentz]

	fld	[submap3per_currenttx]		  ;beregn tx og ty
	fmul	st(0),st(1)
	fxch
	fmul	[submap3per_currentty]
	fxch
	fld    [fp2p60]
	fadd   st(0),st(1)
	fstp   [submap3per_oldtx]		  ;og gem som "gamle" (int)
	fxch
	fld    [fp2p60]
	fadd   st(0),st(1)
	fstp   [submap3per_oldty]

	fsub	[dword ptr submap3per_temp4]		    ;beregn delta
	fxch
	fsub	[dword ptr submap3per_temp3]
	fxch
	fmul	[inttoinvfloat+eax*4]
	fxch
	fmul	[inttoinvfloat+eax*4]
	fxch
	fadd	[fp2p60]
	fstp   [submap3per_temp4]		 ;ty
	fadd	[fp2p60]
	fstp   [submap3per_temp3]		 ;tx


	fld	[submap3per_currenttx]		  ;step 8 pixels
	fadd	[submap3per_8deltatx]
	fld	[submap3per_currentty]
	fadd	[submap3per_8deltaty]
	fld	[submap3per_currentz]
	fadd	[submap3per_8deltaz]
	fxch	st(2)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]
	fstp	[submap3per_currentz]

	fld1					  ;start beregning af n‘ste z
	fdiv	[submap3per_currentz]


	mov	eax,[dword submap3per_temp3]
	shl	eax,16
	mov	ax,[word submap3per_temp4]

	mov	ebx,[dword submap3per_temp1]
	shl	ebx,16
	mov	bx,[word submap3per_temp2]

;	 mov	 ecx,256*16+2
	mov	ch,16
	mov	cl,[byte ptr submap3p_mapnr]
	jmp	edx
@@xloop:

	fld	[submap3per_currenttx]
	fmul	st(0),st(1)
	fxch
	fmul	[submap3per_currentty]
	fxch
	fadd	[fp2p60]
	fxch
	fadd	[fp2p60]
	fxch
	fstp   [submap3per_temp1]
	fstp   [submap3per_temp2]

	fld	[submap3per_currenttx]
	fadd	[submap3per_8deltatx]
	fld	[submap3per_currentty]
	fadd	[submap3per_8deltaty]
	fld	[submap3per_currentz]
	fadd	[submap3per_8deltaz]
	fxch	st(2)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]
	fstp	[submap3per_currentz]

	fld1
	fdiv	[submap3per_currentz]

	mov	eax,[dword ptr submap3per_temp1]
	mov	edx,[dword ptr submap3per_temp2]
	sub	eax,[dword ptr submap3per_oldtx]
	sub	edx,[dword ptr submap3per_oldty]

	mov	ebx,[dword ptr submap3per_oldtx]
	shl	ebx,16
	mov	bx,[word ptr submap3per_oldty]

	sar	eax,3
	sar	edx,3

	shl	eax,16
	mov	ax,dx

	mov	edx,[dword ptr submap3per_temp1]
	mov	[dword ptr submap3per_oldtx],edx
	mov	edx,[dword ptr submap3per_temp2]
	mov	[dword ptr submap3per_oldty],edx

	add	ebx,eax
@@jin8: innerd1  -7
@@jin7: innerd2  -6
@@jin6: innerd1  -5
@@jin5: innerd2  -4
@@jin4: innerd1  -3
@@jin3: innerd2  -2
@@jin2: innerd1  -1
@@jin1: innerd2  0
	add	edi,8*4*2
	sub	ecx,65536
	jge	@@xloop
	fstp	[submap3per_temp1]
endm
macro	innerloopold
	sub	eax,ebx
	jle	@@udenfor
	dec	eax

	mov	ecx,eax
	and	eax,7
	shl	ecx,8-3+8
	add	ebx,eax
	shl	ebx,3

	add	edi,ebx
	mov	edx,[dword ptr @@jumptable+eax*4]
	mov	ebp,[esi+24]	 ;ty
	mov	ebx,[esi+12]	 ;tx
	shl	ebp,24
	and	ebx,65536*256-1
	add	ebp,ebx
	mov	ebx,[esi+24]	 ;ty
	and	ebx,65536*256-1
	mov	cl,[byte ptr submap3p_mapnr]
	shr	ebx,8
	mov	eax,[esi+48]
	shl	eax,16
	mov	ch,[esi+48+2]
	add	ebx,eax
	mov	esi,[submap3p_add2]
	jmp	edx
@@xloop:
@@jin8: innerb2  -7
@@jin7: innera2  -6
@@jin6: innerb2  -5
@@jin5: innera2  -4
@@jin4: innerb2  -3
@@jin3: innera2  -2
@@jin2: innerb2  -1
@@jin1: innera2  0
	add	edi,8*4*2
	sub	ecx,65536
	jge	@@xloop
endm
macro	innerloopold2
	sub	eax,ebx
	jle	@@udenfor

	shl	ebx,3
	add	edi,ebx
	mov	ecx,256*16+3;;;;;;;

	fld	[submap3_venstrez]
	fld	[submap3_venstretx]
	fld	[submap3_venstrety]

@@xloop:
	fld1
	fdiv	st(0),st(3)
	fld	st(2)
	fmul	st(0),st(1) ;tx
	fxch
	fmul	st(0),st(2) ;ty
	fxch
	fistp	[submap3per_temp1]     ;tx
	fistp	[submap3per_temp2]     ;ty


	fadd	[submap3_xdeltaty]
	fxch	st(1)
	fadd	[submap3_xdeltatx]
	fxch	st(2)
	fadd	[submap3_xdeltaz]
	fxch	st(2)
	fxch	st(1)

	mov	[edi+4],ecx
	mov	edx,[submap3per_temp1]
	mov	ebx,[submap3per_temp2]
	shl	edx,8
	shr	ebx,8
	mov	dx,bx
	mov	[edi],edx


	add	edi,4*2
	dec	eax

	jne	@@xloop

	fstp	st(2)
	fstp	st(1)
	fstp	[submap3per_temp1]

@@jin8:
@@jin7:
@@jin6:
@@jin5:
@@jin4:
@@jin3:
@@jin2:
@@jin1:
endm
	submapperper

dataseg
;submap3per_temp1	 dd	 ?
;submap3per_temp2	 dd	 ?
codeseg

endp

proc	submap3per24  p1:dword,p2:dword,p3:dword,mapnr:dword,dest:dword
;Denne rutine tager mod tre punkter, som den selv beregner perspektiv p†.
;Den tegner en trekant mellem punkterne.
;Den f›rste vandrette linie p† sk‘rmen der ‘ndres er x1&ffff0000.
;Denne linie f†r v‘rdier der svarer til linien x1&ffff0000+ffff.
;***************************************************

intptx=1
intpty=1
intpg=1
intpz=1
swidth=320*4
swidthp=8

macro	innerd1  offset1
	mov	edx,ebx
	shr	edx,24
	add	ebx,eax
	mov	dh,bh
	mov	edx,[esi+edx*4]
	mov	[edi+offset1*4],edx



;	 mov	 [edi+8*offset1],ebx
;	 add	 ebx,eax
;	 mov	 [edi+4+8*offset1],ecx
endm

macro	innerd2  offset1
	mov	edx,ebx
	shr	edx,24
	add	ebx,eax
	mov	dh,bh
	mov	edx,[esi+edx*4]
	mov	[edi+offset1*4],edx
;	 mov	 [edi+4+8*offset1],ecx
;	 mov	 [edi+8*offset1],ebx
;	 add	 ebx,eax
endm

macro	preinit
;ecx og edx er fri

;;        mov     ecx,[mapnr]
;;        and     ecx,255

	fld	[submap3_xdeltatx]
	fmul	[fp8]
	fld	[submap3_xdeltaty]
	fmul	[fp8]
	fld	[submap3_xdeltaz]
	fmul	[fp8]
	fxch	st(2)
	fstp	[submap3per_8deltatx]
	fstp	[submap3per_8deltaty]
	fstp	[submap3per_8deltaz]

;;        mov     ecx,[texturetable+ecx*4]
;;        mov     [submap3p_tex],ecx


;	 mov	 cl,[byte ptr mapnr]
;	 mov	 [byte ptr submap3p_mapnr],cl

endm

macro	innerloop
	sub	eax,ebx
	jle	@@udenfor
	fld1					  ;beregn z
	fdiv	[submap3_venstrez]
	dec	eax

	mov	ecx,eax
	and	eax,7
	shl	ecx,8-3+8
	add	ebx,eax
	shl	ebx,3	       -1

	add	edi,ebx
	mov	edx,[dword ptr @@jumptable+eax*4]


		       ;step a pixels ind...
	fld	[submap3_xdeltaz]
	fmul	[inttofloat+eax*4]
	fld	[submap3_xdeltatx]
	fmul	[inttofloat+eax*4]
	fld	[submap3_xdeltaty]
	fmul	[inttofloat+eax*4]
	fxch	st(2)
	fadd	[submap3_venstrez]
	fxch	st(1)
	fadd	[submap3_venstretx]
	fxch	st(2)
	fadd	[submap3_venstrety]
	fxch	st(1)
	fstp	[submap3per_currentz]
	fxch	st(1)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]


	fld	[submap3_venstretx]		  ;beregn tx og ty
	fmul	st(0),st(1)
	fxch
	fmul	[submap3_venstrety]
	fxch
	fst   [dword ptr submap3per_temp3]		    ;tx som float
	fadd	[fp2p60]
	fstp   [submap3per_temp1]		 ;tx som int
	fst   [dword ptr submap3per_temp4]		    ;ty som float
	fadd	[fp2p60]
	fstp   [submap3per_temp2]		 ;ty som int

	fld1					  ;beregn n‘ste z
	fdiv	[submap3per_currentz]

	fld	[submap3per_currenttx]		  ;beregn tx og ty
	fmul	st(0),st(1)
	fxch
	fmul	[submap3per_currentty]
	fxch
	fld    [fp2p60]
	fadd   st(0),st(1)
	fstp   [submap3per_oldtx]		  ;og gem som "gamle" (int)
	fxch
	fld    [fp2p60]
	fadd   st(0),st(1)
	fstp   [submap3per_oldty]

	fsub	[dword ptr submap3per_temp4]		    ;beregn delta
	fxch
	fsub	[dword ptr submap3per_temp3]
	fxch
	fmul	[inttoinvfloat+eax*4]
	fxch
	fmul	[inttoinvfloat+eax*4]
	fxch
	fadd	[fp2p60]
	fstp   [submap3per_temp4]		 ;ty
	fadd	[fp2p60]
	fstp   [submap3per_temp3]		 ;tx


	fld	[submap3per_currenttx]		  ;step 8 pixels
	fadd	[submap3per_8deltatx]
	fld	[submap3per_currentty]
	fadd	[submap3per_8deltaty]
	fld	[submap3per_currentz]
	fadd	[submap3per_8deltaz]
	fxch	st(2)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]
	fstp	[submap3per_currentz]

	fld1					  ;start beregning af n‘ste z
	fdiv	[submap3per_currentz]


	mov	eax,[dword submap3per_temp3]
	shl	eax,16
	mov	ax,[word submap3per_temp4]

	mov	ebx,[dword submap3per_temp1]
	shl	ebx,16
	mov	bx,[word submap3per_temp2]
  mov	esi,[submap3p_tex]
	mov	ch,16
	mov	cl,[byte ptr submap3p_mapnr]
	jmp	edx
@@xloop:

	fld	[submap3per_currenttx]
	fmul	st(0),st(1)
	fxch
	fmul	[submap3per_currentty]
	fxch
	fadd	[fp2p60]
	fxch
	fadd	[fp2p60]
	fxch
	fstp   [submap3per_temp1]
	fstp   [submap3per_temp2]

	fld	[submap3per_currenttx]
	fadd	[submap3per_8deltatx]
	fld	[submap3per_currentty]
	fadd	[submap3per_8deltaty]
	fld	[submap3per_currentz]
	fadd	[submap3per_8deltaz]
	fxch	st(2)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]
	fstp	[submap3per_currentz]

	fld1
	fdiv	[submap3per_currentz]

	mov	eax,[dword ptr submap3per_temp1]
	mov	edx,[dword ptr submap3per_temp2]
	sub	eax,[dword ptr submap3per_oldtx]
	sub	edx,[dword ptr submap3per_oldty]

	mov	ebx,[dword ptr submap3per_oldtx]
	shl	ebx,16
	mov	bx,[word ptr submap3per_oldty]

	sar	eax,3
	sar	edx,3

	shl	eax,16
	mov	ax,dx

	mov	edx,[dword ptr submap3per_temp1]
	mov	[dword ptr submap3per_oldtx],edx
	mov	edx,[dword ptr submap3per_temp2]
	mov	[dword ptr submap3per_oldty],edx

	add	ebx,eax
@@jin8: innerd1  -7
@@jin7: innerd2  -6
@@jin6: innerd1  -5
@@jin5: innerd2  -4
@@jin4: innerd1  -3
@@jin3: innerd2  -2
@@jin2: innerd1  -1
@@jin1: innerd2  0
	add	edi,8*4
	sub	ecx,65536
	jge	@@xloop
	fstp	[submap3per_temp1]
endm
	submapperper

dataseg
;submap3per_temp1	 dd	 ?
;submap3per_temp2	 dd	 ?
codeseg

endp

proc	submap3per24bil  p1:dword,p2:dword,p3:dword,mapnr:dword,dest:dword
;Denne rutine tager mod tre punkter, som den selv beregner perspektiv p†.
;Den tegner en trekant mellem punkterne.
;Den f›rste vandrette linie p† sk‘rmen der ‘ndres er x1&ffff0000.
;Denne linie f†r v‘rdier der svarer til linien x1&ffff0000+ffff.
;***************************************************

intptx=1
intpty=1
intpg=0
intpz=1
swidth=320*4
swidthp=8

macro	innerd1  offset1

	add	ebx,ebp

	mov	edx,ebx
	shr	edx,24
	mov	dh,bh
	mov	ecx,ebx
	shr	ecx,4+8
	mov	cl,bl
;	 shr	 ecx,8
	and	ecx,255*16


	mov	eax,[esi+edx*4]
	mov	[byte ptr pix1r],ah
	and	eax,65536*255+255
	mov	[dword ptr pix1],eax
	mov	eax,[esi+edx*4+4]
	fld	[pix1]
	fmul	[biltabel+0+ecx]

	mov	[byte ptr pix2r],ah
	and	eax,65536*255+255
	mov	[dword ptr pix2],eax
	mov	eax,[esi+edx*4+256*4]
	fld	[pix2]
	fmul	[biltabel+4+ecx]

	mov	[byte ptr pix3r],ah
	and	eax,65536*255+255
	mov	[dword ptr pix3],eax
	mov	eax,[esi+edx*4+257*4]
	fadd
	fld	[pix3]
	fmul	[biltabel+8+ecx]
	mov	[byte ptr pix4r],ah
	and	eax,65536*255+255
	mov	[dword ptr pix4],eax

	fadd
	fld	[pix4]
	fmul	[biltabel+12+ecx]


	fadd
	fstp	[fincol]
	mov	eax,[dword ptr fincol]
	mov	ah,[byte ptr finr]
	mov	[edi+offset1*4],eax

endm


macro	preinit
;ecx og edx er fri

;;        mov     ecx,[mapnr]
;;        and     ecx,255

          fld     [submap3_xdeltatx]
          fmul    [fp8]
          fld     [submap3_xdeltaty]
          fmul    [fp8]
          fld     [submap3_xdeltaz]
          fmul    [fp8]
          fxch    st(2)
          fstp    [submap3per_8deltatx]
          fstp    [submap3per_8deltaty]
          fstp    [submap3per_8deltaz]

;;        mov     ecx,[texturetable+ecx*4]
;;        mov     [submap3p_tex],ecx


;	 mov	 cl,[byte ptr mapnr]
;	 mov	 [byte ptr submap3p_mapnr],cl

endm

macro	innerloop
	sub	eax,ebx
	jle	@@udenfor
	fld1					  ;beregn z
	fdiv	[submap3_venstrez]
	dec	eax

	mov	ecx,eax
	and	eax,7
     ;	 shl	 ecx,8-3+8
   shr ecx,3
   mov [submap_counter],ecx
	add	ebx,eax
	shl	ebx,3	       -1

	add	edi,ebx
	mov	edx,[dword ptr @@jumptable+eax*4]


		       ;step a pixels ind...
	fld	[submap3_xdeltaz]
	fmul	[inttofloat+eax*4]
	fld	[submap3_xdeltatx]
	fmul	[inttofloat+eax*4]
	fld	[submap3_xdeltaty]
	fmul	[inttofloat+eax*4]
	fxch	st(2)
	fadd	[submap3_venstrez]
	fxch	st(1)
	fadd	[submap3_venstretx]
	fxch	st(2)
	fadd	[submap3_venstrety]
	fxch	st(1)
	fstp	[submap3per_currentz]
	fxch	st(1)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]


	fld	[submap3_venstretx]		  ;beregn tx og ty
	fmul	st(0),st(1)
	fxch
	fmul	[submap3_venstrety]
	fxch
	fst   [dword ptr submap3per_temp3]		    ;tx som float
	fadd	[fp2p60]
	fstp   [submap3per_temp1]		 ;tx som int
	fst   [dword ptr submap3per_temp4]		    ;ty som float
	fadd	[fp2p60]
	fstp   [submap3per_temp2]		 ;ty som int

	fld1					  ;beregn n‘ste z
	fdiv	[submap3per_currentz]

	fld	[submap3per_currenttx]		  ;beregn tx og ty
	fmul	st(0),st(1)
	fxch
	fmul	[submap3per_currentty]
	fxch
	fld    [fp2p60]
	fadd   st(0),st(1)
	fstp   [submap3per_oldtx]		  ;og gem som "gamle" (int)
	fxch
	fld    [fp2p60]
	fadd   st(0),st(1)
	fstp   [submap3per_oldty]

	fsub	[dword ptr submap3per_temp4]		    ;beregn delta
	fxch
	fsub	[dword ptr submap3per_temp3]
	fxch
	fmul	[inttoinvfloat+eax*4]
	fxch
	fmul	[inttoinvfloat+eax*4]
	fxch
	fadd	[fp2p60]
	fstp   [submap3per_temp4]		 ;ty
	fadd	[fp2p60]
	fstp   [submap3per_temp3]		 ;tx


	fld	[submap3per_currenttx]		  ;step 8 pixels
	fadd	[submap3per_8deltatx]
	fld	[submap3per_currentty]
	fadd	[submap3per_8deltaty]
	fld	[submap3per_currentz]
	fadd	[submap3per_8deltaz]
	fxch	st(2)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]
	fstp	[submap3per_currentz]

	fld1					  ;start beregning af n‘ste z
	fdiv	[submap3per_currentz]


	mov	eax,[dword submap3per_temp3]
	shl	eax,16
	mov	ax,[word submap3per_temp4]
  mov ebp,eax
	mov	ebx,[dword submap3per_temp1]
	shl	ebx,16
	mov	bx,[word submap3per_temp2]
  mov	esi,[submap3p_tex]
	mov	ch,16
	mov	cl,[byte ptr submap3p_mapnr]
	jmp	edx
@@xloop:

	fld	[submap3per_currenttx]
	fmul	st(0),st(1)
	fxch
	fmul	[submap3per_currentty]
	fxch
	fadd	[fp2p60]
	fxch
	fadd	[fp2p60]
	fxch
	fstp   [submap3per_temp1]
	fstp   [submap3per_temp2]

	fld	[submap3per_currenttx]
	fadd	[submap3per_8deltatx]
	fld	[submap3per_currentty]
	fadd	[submap3per_8deltaty]
	fld	[submap3per_currentz]
	fadd	[submap3per_8deltaz]
	fxch	st(2)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]
	fstp	[submap3per_currentz]

	fld1
	fdiv	[submap3per_currentz]

	mov	eax,[dword ptr submap3per_temp1]
	mov	edx,[dword ptr submap3per_temp2]
	sub	eax,[dword ptr submap3per_oldtx]
	sub	edx,[dword ptr submap3per_oldty]

	mov	ebx,[dword ptr submap3per_oldtx]
	shl	ebx,16
	mov	bx,[word ptr submap3per_oldty]

	sar	eax,3
	sar	edx,3

	shl	eax,16
	mov	ax,dx
   mov ebp,eax
   mov [submap_counter],ecx
	mov	edx,[dword ptr submap3per_temp1]
	mov	[dword ptr submap3per_oldtx],edx
	mov	edx,[dword ptr submap3per_temp2]
	mov	[dword ptr submap3per_oldty],edx

	add	ebx,eax
@@jin8: innerd1  -7
@@jin7: innerd1  -6
@@jin6: innerd1  -5
@@jin5: innerd1  -4
@@jin4: innerd1  -3
@@jin3: innerd1  -2
@@jin2: innerd1  -1
@@jin1: innerd1  0
	add	edi,8*4
	mov	ecx,[submap_counter]
	dec	ecx
;	 sub	 ecx,65536
	jge	@@xloop
	fstp	[submap3per_temp1]
endm
	submapperper

dataseg
;submap3per_temp1	 dd	 ?
;submap3per_temp2	 dd	 ?
codeseg

endp



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
proc    submap3per24bilg  p1:dword,p2:dword,p3:dword,mapnr:dword,dest:dword
; bilinear perspektiv gouraud texture filler

intptx=1
intpty=1
intpg=1
intpz=1
swidth=320*4
swidthp=8

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
;MARK
macro	innerd1  offset1
; nop
        mov     ecx,[esi+edx*4]
        mov     [byte ptr pix1r],ch
 mov ebp,[submap_deltauv]
        and     ecx,65536*255+255
       add     ebx,ebp
        mov     [dword ptr pix1],ecx   ;*
        mov     ecx,[esi+edx*4+256*4]
        fld     [pix1]
        fmul    [biltabel+0+eax]
        mov     [byte ptr pix2r],ch
       mov     ebp,edi
        and     ecx,65536*255+255
       push    ebx
        mov     [dword ptr pix2],ecx   ;*
        mov     ecx,[esi+edx*4+4]
	fld	[pix2]
        fmul    [biltabel+8+eax]
        mov     [byte ptr pix3r],ch    ;*
        and     ecx,65536*255+255
        mov     [dword ptr pix3],ecx   ;*
        mov     ecx,[esi+edx*4+257*4]
	fadd
	fld	[pix3]
        fmul    [biltabel+4+eax]
        mov     [byte ptr pix4r],ch
       mov     edx,ebx
       shr     edx,24
        and     ecx,65536*255+255
        mov     [dword ptr pix4],ecx   ;*
 ;      mov     ecx,ebx
       mov     dh,bh
        fadd
	fld	[pix4]
        fmul    [biltabel+12+eax]
       shr     ebx,4+8
        mov     eax,[submap3_xdeltagint]
        fadd
        shr     ebp,16
       mov     bl,[esp]
       and     ebx,255*16
        add     edi,eax
        add     ebp, offset gouraudtabel
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
        mov     ebp,[submap_innerdestination]
        pop     eax
      pop ebx
        mov     [ebp+offset1*4],ecx



endm


macro	preinit
;ecx og edx er fri

;;        mov     ecx,[mapnr]
;;        and     ecx,255

          fld     [submap3_xdeltatx]
          fmul    [fp8]
          fld     [submap3_xdeltaty]
          fmul    [fp8]
          fld     [submap3_xdeltaz]
          fmul    [fp8]
          fxch    st(2)
          fstp    [submap3per_8deltatx]
          fstp    [submap3per_8deltaty]
          fstp    [submap3per_8deltaz]



;;        mov     ecx,[texturetable+ecx*4]
;;        mov     [submap3p_tex],ecx


;	 mov	 cl,[byte ptr mapnr]
;	 mov	 [byte ptr submap3p_mapnr],cl

endm

macro	innerloop
	sub	eax,ebx
	jle	@@udenfor
	fld1					  ;beregn z
	fdiv	[submap3_venstrez]
	dec	eax

	mov	ecx,eax
	and	eax,7
     ;	 shl	 ecx,8-3+8
   shr ecx,3
   mov [submap_counter],ecx
	add	ebx,eax
	shl	ebx,3	       -1

	add	edi,ebx
	mov	edx,[dword ptr @@jumptable+eax*4]

 mov    [submap_innerdestination],edi
 mov    edi,[submap3_venstreg]

		       ;step a pixels ind...
	fld	[submap3_xdeltaz]
	fmul	[inttofloat+eax*4]
	fld	[submap3_xdeltatx]
	fmul	[inttofloat+eax*4]
	fld	[submap3_xdeltaty]
	fmul	[inttofloat+eax*4]
	fxch	st(2)
	fadd	[submap3_venstrez]
	fxch	st(1)
	fadd	[submap3_venstretx]
	fxch	st(2)
	fadd	[submap3_venstrety]
	fxch	st(1)
	fstp	[submap3per_currentz]
	fxch	st(1)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]


	fld	[submap3_venstretx]		  ;beregn tx og ty
	fmul	st(0),st(1)
	fxch
	fmul	[submap3_venstrety]
	fxch
	fst   [dword ptr submap3per_temp3]		    ;tx som float
	fadd	[fp2p60]
	fstp   [submap3per_temp1]		 ;tx som int
	fst   [dword ptr submap3per_temp4]		    ;ty som float
	fadd	[fp2p60]
	fstp   [submap3per_temp2]		 ;ty som int

	fld1					  ;beregn n‘ste z
	fdiv	[submap3per_currentz]

	fld	[submap3per_currenttx]		  ;beregn tx og ty
	fmul	st(0),st(1)
	fxch
	fmul	[submap3per_currentty]
	fxch
	fld    [fp2p60]
	fadd   st(0),st(1)
	fstp   [submap3per_oldtx]		  ;og gem som "gamle" (int)
	fxch
	fld    [fp2p60]
	fadd   st(0),st(1)
	fstp   [submap3per_oldty]

	fsub	[dword ptr submap3per_temp4]		    ;beregn delta
	fxch
	fsub	[dword ptr submap3per_temp3]
	fxch
	fmul	[inttoinvfloat+eax*4]
	fxch
	fmul	[inttoinvfloat+eax*4]
	fxch
	fadd	[fp2p60]
	fstp   [submap3per_temp4]		 ;ty
	fadd	[fp2p60]
	fstp   [submap3per_temp3]		 ;tx


	fld	[submap3per_currenttx]		  ;step 8 pixels
	fadd	[submap3per_8deltatx]
	fld	[submap3per_currentty]
	fadd	[submap3per_8deltaty]
	fld	[submap3per_currentz]
	fadd	[submap3per_8deltaz]
	fxch	st(2)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]
	fstp	[submap3per_currentz]

	fld1					  ;start beregning af n‘ste z
	fdiv	[submap3per_currentz]


	mov	eax,[dword submap3per_temp3]
	shl	eax,16
	mov	ax,[word submap3per_temp4]
  mov [submap_deltauv],eax
	mov	ebx,[dword submap3per_temp1]
	shl	ebx,16
	mov	bx,[word submap3per_temp2]
  mov	esi,[submap3p_tex]
;        mov     ch,16
;        mov     cl,[byte ptr submap3p_mapnr]
        mov     ecx,edx
        innerinit
        jmp     ecx
@@xloop:

	fld	[submap3per_currenttx]
	fmul	st(0),st(1)
	fxch
	fmul	[submap3per_currentty]
	fxch
	fadd	[fp2p60]
	fxch
	fadd	[fp2p60]
	fxch
	fstp   [submap3per_temp1]
	fstp   [submap3per_temp2]

	fld	[submap3per_currenttx]
	fadd	[submap3per_8deltatx]
	fld	[submap3per_currentty]
	fadd	[submap3per_8deltaty]
	fld	[submap3per_currentz]
	fadd	[submap3per_8deltaz]
	fxch	st(2)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]
	fstp	[submap3per_currentz]

	fld1
	fdiv	[submap3per_currentz]

	mov	eax,[dword ptr submap3per_temp1]
	mov	edx,[dword ptr submap3per_temp2]
	sub	eax,[dword ptr submap3per_oldtx]
	sub	edx,[dword ptr submap3per_oldty]

	mov	ebx,[dword ptr submap3per_oldtx]
	shl	ebx,16
	mov	bx,[word ptr submap3per_oldty]

	sar	eax,3
	sar	edx,3

	shl	eax,16
	mov	ax,dx
  mov [submap_deltauv],eax
   mov [submap_counter],ecx
	mov	edx,[dword ptr submap3per_temp1]
	mov	[dword ptr submap3per_oldtx],edx
	mov	edx,[dword ptr submap3per_temp2]
	mov	[dword ptr submap3per_oldty],edx

	add	ebx,eax
        innerinit
@@jin8: innerd1  -7
@@jin7: innerd1  -6
@@jin6: innerd1  -5
@@jin5: innerd1  -4
@@jin4: innerd1  -3
@@jin3: innerd1  -2
@@jin2: innerd1  -1
@@jin1: innerd1  0
        add     ebp,8*4
	mov	ecx,[submap_counter]
        mov     [submap_innerdestination],ebp
	dec	ecx
;	 sub	 ecx,65536
	jge	@@xloop
	fstp	[submap3per_temp1]
endm
	submapperper

dataseg
;submap3per_temp1	 dd	 ?
;submap3per_temp2	 dd	 ?
codeseg

endp



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

proc	klip
;kaldes med tre punkter: esi,edi,edx
;og to ekstra ebx,ecx

;klipper med plan x=z

;returnerer: hvis eax=0   ...ingen trekanter
;returnerer: hvis eax=1   ...trekant esi,edi,edx
;returnerer: hvis eax=2   ...trekant esi,ecx,edx og trekant esi,edi,edx

	fld	[dword ptr esi]
	fsub	[dword ptr esi+8]
	fld	[dword ptr edi]
	fsub	[dword ptr edi+8]
	fld	[dword ptr edx]
	fsub	[dword ptr edx+8]
	fxch	st(2)
	fstp	[@@klipdataesi]
	fstp	[@@klipdataedi]
	fstp	[@@klipdataedx]
	mov	ebp,[@@klipdataedx]
	xor	eax,eax
	add	ebp,ebp
	mov	ebp,[@@klipdataedi]
	adc	eax,eax
	add	ebp,ebp
	mov	ebp,[@@klipdataesi]
	adc	eax,eax
	add	ebp,ebp
	adc	eax,eax



	mov	ebp,[@@klipjumptable+eax*4]

	jmp	ebp
@@jump7:
	mov	eax,1
	ret
@@jump0:
	mov	eax,0
	ret
@@jump4:
@@jump3:
	fld	[@@klipdataedx]
	fsub	[@@klipdataedi]
	fdivr	[@@klipdataedx]

	mov	ebp,edx
	mov	edx,esi
	mov	esi,ebp

	fld	[@@klipdataedx]
	fsub	[@@klipdataesi]
	fdivr	[@@klipdataedx]

	jmp	@@kont
@@jump2:
@@jump5:
	fld	[@@klipdataedi]
	fsub	[@@klipdataesi]
	fdivr	[@@klipdataedi]

	mov	ebp,edi
	mov	edi,esi
	mov	esi,ebp

	fld	[@@klipdataedi]
	fsub	[@@klipdataedx]
	fdivr	[@@klipdataedi]

	jmp	@@kont
@@jump1:
@@jump6:
	fld	[@@klipdataesi]
	fsub	[@@klipdataedi]
	fdivr	[@@klipdataesi]

	fld	[@@klipdataesi]
	fsub	[@@klipdataedx]
	fdivr	[@@klipdataesi]

@@kont:
	mov	ebp,[@@klipjumptable2+eax*4]

	fld1
	fsub	st(0),st(2)
	fld1
	fsub	st(0),st(2)


macro	klipcalc offset1
	fld	[dword ptr edi+offset1]
	fmul	st(0),st(4)
	fld	[dword ptr edx+offset1]
	fmul	st(0),st(4)
	fld	[dword ptr esi+offset1]
	fmul	st(0),st(4)
	fld	[dword ptr esi+offset1]
	fmul	st(0),st(4)

	fxch	st(3)
	fadd
	fxch	st(2)
	fadd
	fxch

	fstp	[dword ptr ebx+offset1]
	fstp	[dword ptr ecx+offset1]
endm
	klipcalc 0
	klipcalc 4
	klipcalc 8
	klipcalc 12+8
	klipcalc 16+8
	klipcalc 20+8

	fstp	st(1)
	fstp	st(1)
	fstp	st(1)
	fstp	[@@klipdummy]


	jmp	ebp

@@jump1b:
	mov	eax,1
	mov	edi,ebx
	mov	edx,ecx
	ret
@@jump2b:
	mov	eax,2
	mov	esi,ebx
	ret
dataseg
@@klipjumptable  dd	 @@jump0,@@jump1,@@jump2,@@jump3,@@jump4,@@jump5,@@jump6,@@jump7
@@klipjumptable2 dd	 0,@@jump1b,@@jump1b,@@jump2b,@@jump1b,@@jump2b,@@jump2b
@@klipdataesi	 dd	 0
@@klipdataedi	 dd	 0
@@klipdataedx	 dd	 0
@@klipdummy	 dd	 0
codeseg
endp

dataseg
klipmapnr	dd	0
klipdest	dd	0
codeseg

proc	submap3gklip4
	push	esi
	push	edi
	push	edx
	mov	eax,[esi+0]
	mov	ebx,[edi+0]
	mov	ecx,[edx+0]
	xor	eax,80000000h
	xor	ebx,80000000h
	xor	ecx,80000000h
	mov	[esi+0],eax
	mov	[edi+0],ebx
	mov	[edx+0],ecx

	lea	ecx,[@@p1]
	lea	ebx,[@@p2]

	call	klip

	cmp	eax,1
	jl	@@return



	push	[klipdest]
	push	[klipmapnr]
	push	offset @@q1
	push	offset @@q2
	push	offset @@q3

	je	@@entrekant

macro	pcopy	s,d
	mov	ebp,[s+0]
	mov	[d+0],ebp
	mov	ebp,[s+4]
	mov	[d+4],ebp
	mov	ebp,[s+8]
	mov	[d+8],ebp
	mov	ebp,[s+20]
	mov	[d+20],ebp
	mov	ebp,[s+24]
	mov	[d+24],ebp
	mov	ebp,[s+28]
	mov	[d+28],ebp
endm

;	 push	 esi
;	 push	 ecx
;	 push	 edx

	mov	[@@temp1],esi
	mov	[@@temp2],ecx
	mov	[@@temp3],edx

	pcopy	esi,@@q1
	pcopy	edi,@@q2
	pcopy	edx,@@q3

        mov     ebx,[klipmapnr]
;        mov     ebx,-8
;@@l1:
;	 inc	 ebx
;	 mov	 ebp,[esi+ebx*4+28]
;	 mov	 [@@q1+28+ebx*4],ebp
;	 mov	 ebp,[edi+ebx*4+28]
;	 mov	 [@@q2+28+ebx*4],ebp
;	 mov	 ebp,[edx+ebx*4+28]
;	 mov	 [@@q3+28+ebx*4],ebp
;	 jne	 @@l1

;	 call	 submap3g
;        call    submap3per24bil

;        call    submap3per24bilg
        call    [k_tabel+ebx*4]
;     call    submap3per24

;	 pop	 edx
;	 pop	 edi
;	 pop	 esi
	mov	esi,[@@temp1]
	mov	edi,[@@temp2]
	mov	edx,[@@temp3]
@@entrekant:
	pcopy	esi,@@q1
	pcopy	edi,@@q2
	pcopy	edx,@@q3
        mov     ebx,[klipmapnr]
;	 mov	 ebx,-8
;@@l2:
;	 inc	 ebx
;	 mov	 ebp,[esi+ebx*4+28]
;	 mov	 [@@q1+28+ebx*4],ebp
;	 mov	 ebp,[edi+ebx*4+28]
;	 mov	 [@@q2+28+ebx*4],ebp
;	 mov	 ebp,[edx+ebx*4+28]
;	 mov	 [@@q3+28+ebx*4],ebp
;	 jne	 @@l2
;	 call	 submap3g
;	 call	 submap3per
;        call    submap3per24bil
        call    [k_tabel+ebx*4]
;        call    submap3per24bilg
;      call    submap3per24
	add	esp,20
@@return:
	pop	edx
	pop	edi
	pop	esi
	mov	eax,[esi+0]
	mov	ebx,[edi+0]
	mov	ecx,[edx+0]
	xor	eax,80000000h
	xor	ebx,80000000h
	xor	ecx,80000000h
	mov	[esi+0],eax
	mov	[edi+0],ebx
	mov	[edx+0],ecx
	ret

dataseg
@@p1	  dd	  0,0,0,0,0,0,0,0
@@p2	  dd	  0,0,0,0,0,0,0,0
@@q1	  dd	  0,0,0,0,0,0,0,0
@@q2	  dd	  0,0,0,0,0,0,0,0
@@q3	  dd	  0,0,0,0,0,0,0,0
@@temp1   dd	  0
@@temp2   dd	  0
@@temp3   dd	  0
codeseg
endp

proc	submap3gklip3
	push	esi
	push	edi
	push	edx
	nop
	mov	ebx,[esi+0]
	mov	ecx,[esi+4]
	mov	[esi+4],ebx
	mov	[esi+0],ecx
	mov	ebx,[edi+0]
	mov	ecx,[edi+4]
	mov	[edi+4],ebx
	mov	[edi+0],ecx
	mov	ebx,[edx+0]
	mov	ecx,[edx+4]
	mov	[edx+4],ebx
	mov	[edx+0],ecx
	lea	ecx,[@@p1]
	lea	ebx,[@@p2]

	call	klip

	cmp	eax,1
	jl	@@return

	je	@@entrekant

	push	esi
	push	ecx
	push	edx
	call	submap3gklip4

	pop	edx
	pop	edi
	pop	esi
@@entrekant:
	call	submap3gklip4
@@return:
	pop	edx
	pop	edi
	pop	esi
	nop
	mov	ebx,[esi+0]
	mov	ecx,[esi+4]
	mov	[esi+4],ebx
	mov	[esi+0],ecx
	mov	ebx,[edi+0]
	mov	ecx,[edi+4]
	mov	[edi+4],ebx
	mov	[edi+0],ecx
	mov	ebx,[edx+0]
	mov	ecx,[edx+4]
	mov	[edx+4],ebx
	mov	[edx+0],ecx
	ret

dataseg
@@p1	  dd	  0,0,0,0,0,0,0,0
@@p2	  dd	  0,0,0,0,0,0,0,0
codeseg
endp

proc	submap3gklip2
	push	esi
	push	edi
	push	edx
	mov	eax,[esi+0]
	mov	ebx,[edi+0]
	mov	ecx,[edx+0]
	xor	eax,80000000h
	xor	ebx,80000000h
	xor	ecx,80000000h
	mov	[esi+0],eax
	mov	[edi+0],ebx
	mov	[edx+0],ecx


	lea	ecx,[@@p1]
	lea	ebx,[@@p2]

	call	klip

	cmp	eax,1
	jl	@@return

	je	@@entrekant

	push	esi
	push	ecx
	push	edx
	call	submap3gklip3

	pop	edx
	pop	edi
	pop	esi
@@entrekant:
	call	submap3gklip3
@@return:
	pop	edx
	pop	edi
	pop	esi
	mov	eax,[esi+0]
	mov	ebx,[edi+0]
	mov	ecx,[edx+0]
	xor	eax,80000000h
	xor	ebx,80000000h
	xor	ecx,80000000h
	mov	[esi+0],eax
	mov	[edi+0],ebx
	mov	[edx+0],ecx
	ret

dataseg
@@p1	  dd	  0,0,0,0,0,0,0,0
@@p2	  dd	  0,0,0,0,0,0,0,0
codeseg
endp

proc    submap3gklip q1:dword,q2:dword,q3:dword,mapnr:dword,texture:dword,dest:dword
	pushad
	mov	eax,[dest]
	mov	ebx,[mapnr]
	mov	esi,[q1]
	mov	edi,[q2]
	mov	edx,[q3]

        mov     ecx,[texture]
        mov     [submap3p_tex],ecx

	mov	[klipmapnr],ebx
	mov	[klipdest],eax

                mov     ecx,[esi+8]
                mov     eax,[edi+8]
                and     ecx,eax
                mov     eax,[edx+8]
                and     ecx,eax
                jl      @@return


        lea     ecx,[@@p1]
	lea	ebx,[@@p2]

	call	klip

	cmp	eax,1
	jl	@@return

	je	@@entrekant

	push	esi
	push	ecx
	push	edx
	call	submap3gklip2

	pop	edx
	pop	edi
	pop	esi
@@entrekant:
	call	submap3gklip2
@@return:
	popad
	ret
dataseg
@@p1	  dd	  0,0,0,0,0,0,0,0
@@p2	  dd	  0,0,0,0,0,0,0,0
codeseg
endp







;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³RGB gourad (32bit) ikke perspektiv                         ³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ



proc    submap32gouraud  p1:dword,p2:dword,p3:dword,mapnr:dword,dest:dword

intptx=1
intpty=1
intpg=1
intpz=0
swidth=320*4
swidthp=8

;invariant: esi=add2
macro	innera2  offset1
        mov     edx,ebp                 ;u
        mov     dx,bx
        mov     dl,ch
        mov     eax,[submap3p_add1]     ;u
        add     ebp,eax                 ; v
        mov     eax,[submap3p_add3]     ; v
        adc     ebx,esi                 ;u
        mov     [edi+offset1*4],edx     ; v
        adc     ch,ah                   ;u
endm

macro	innerb2  offset1
        mov     edx,ebp                 ;u
        mov     dx,bx
        mov     dl,ch
        mov     eax,[submap3p_add1]     ;u
        add     ebp,eax                 ; v
        mov     eax,[submap3p_add3]     ; v
        adc     ebx,esi                 ;u
        mov     [edi+offset1*4],edx     ; v
        adc     ch,ah                   ;u
endm

macro	preinit
	mov	ecx,[submap3_xdeltatyint]
	mov	edx,[submap3_xdeltatxint]
	shl	ecx,24
	and	edx,65536*256-1
	add	ecx,edx
	mov	edx,[submap3_xdeltatyint]
	shr	edx,8
	mov	[submap3p_add1],ecx
	and	edx,65535
	mov	ecx,[submap3_xdeltagint]
	shl	ecx,16
	add	edx,ecx
	mov	ecx,[submap3_xdeltagint]
	mov	[submap3p_add2],edx
	shr	ecx,8
	and	ecx,255*256
	mov	[submap3p_add3],ecx
	mov	cl,[byte ptr mapnr]
	mov	[byte ptr submap3p_mapnr],cl

endm

macro	innerloop
;nop
;nop
   if 0
	sub	eax,ebx
	jle	@@udenfor

	dec	eax
	mov	ecx,eax
	and	eax,7
	shl	ecx,8-3+8
	add	ebx,eax

	mov	cl,[byte ptr submap3p_mapnr]

	mov	ch,[esi+48+2]

	shl	ebx,3
	add	edi,ebx

	mov	edx,[dword ptr @@jumptable+eax*4]

	mov	ebp,[esi+24]	 ;ty
	mov	ebx,[esi+12]	 ;tx
	shl	ebp,24
	and	ebx,65536*256-1
	add	ebp,ebx
	mov	ebx,[esi+24]	 ;ty
	and	ebx,65536*256-1
	shr	ebx,8
	mov	eax,[esi+48]
	shl	eax,16
	add	ebx,eax
	mov	eax,[submap3p_add2]

	mov	esi,[submap3p_add1]
	jmp	edx
   endif
	sub	eax,ebx
	jle	@@udenfor
	dec	eax

	mov	ecx,eax
	and	eax,7
	shl	ecx,8-3+8
	add	ebx,eax
        shl     ebx,2

	add	edi,ebx
	mov	edx,[dword ptr @@jumptable+eax*4]
	mov	ebp,[esi+24]	 ;ty
	mov	ebx,[esi+12]	 ;tx
	shl	ebp,24
	and	ebx,65536*256-1
	add	ebp,ebx
	mov	ebx,[esi+24]	 ;ty
	and	ebx,65536*256-1
	mov	cl,[byte ptr submap3p_mapnr]
	shr	ebx,8
	mov	eax,[esi+48]
	shl	eax,16
	mov	ch,[esi+48+2]
	add	ebx,eax
;	 mov	 esi,[submap3p_add1]
	mov	esi,[submap3p_add2]   ;eax
	jmp	edx
@@xloop:
@@jin8: innerb2  -7
@@jin7: innera2  -6
@@jin6: innerb2  -5
@@jin5: innera2  -4
@@jin4: innerb2  -3
@@jin3: innera2  -2
@@jin2: innerb2  -1
@@jin1: innera2  0
        add     edi,8*4
	sub	ecx,65536
	jge	@@xloop
endm
	submapper


endp


;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³Texture (24bit) ikke perspektiv                            ³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ


proc    submap324  p1:dword,p2:dword,p3:dword,mapnr:dword,dest:dword

intptx=1
intpty=1
intpg=1
intpz=0
swidth=320*4
swidthp=8

;invariant: esi=add2
;macro   innera2  offset1
;        mov     edx,ebp                 ;u
;        mov     dx,bx
;        mov     dl,ch
;        mov     eax,[submap3p_add1]     ;u
;        add     ebp,eax                 ; v
;        mov     eax,[submap3p_add3]     ; v
;        adc     ebx,esi                 ;u
;        mov     [edi+offset1*4],edx     ; v
;        adc     ch,ah                   ;u
;endm
;
;macro   innerb2  offset1
;        mov     edx,ebp                 ;u
;        mov     dx,bx
;        mov     dl,ch
;        mov     eax,[submap3p_add1]     ;u
;        add     ebp,eax                 ; v
;        mov     eax,[submap3p_add3]     ; v
;        adc     ebx,esi                 ;u
;        mov     [edi+offset1*4],edx     ; v
;        adc     ch,ah                   ;u
;endm
macro   innera2  offset1
;pentium
        mov     edx,ebx
        shr     edx,16
        add     ebx,ebp
        mov     dh, ch
        adc     ecx,eax
        mov     edx,[esi+edx*4]
        mov     [edi+offset1*4],edx
endm

macro   innerb2  offset1
;p6
        mov     edx,ebx
        shr     edx,16
        mov     ebp,ecx
        and     edx,255
        and     ebp,255*256
        add     edx, ebp
        add     ebx,[submap3p_add1]
        adc     ecx,eax
        mov     edx,[esi+edx*4]
        mov     [edi+offset1*4],edx
endm

macro	preinit
	mov	ecx,[submap3_xdeltatyint]
	mov	edx,[submap3_xdeltatxint]
	shl	ecx,24
	and	edx,65536*256-1
	add	ecx,edx
	mov	edx,[submap3_xdeltatyint]
	shr	edx,8
	mov	[submap3p_add1],ecx
	and	edx,65535
	mov	ecx,[submap3_xdeltagint]
	shl	ecx,16
	add	edx,ecx
	mov	ecx,[submap3_xdeltagint]
	mov	[submap3p_add2],edx
	shr	ecx,8
	and	ecx,255*256
	mov	[submap3p_add3],ecx
	mov	cl,[byte ptr mapnr]
	mov	[byte ptr submap3p_mapnr],cl

endm

macro	innerloop
;nop
;nop
	sub	eax,ebx
	jle	@@udenfor
	dec	eax

	mov	ecx,eax
	and	eax,7
	shl	ecx,8-3+8
	add	ebx,eax
        shl     ebx,2

	add	edi,ebx
	mov	edx,[dword ptr @@jumptable+eax*4]
	mov	ebp,[esi+24]	 ;ty
	mov	ebx,[esi+12]	 ;tx
	shl	ebp,24
	and	ebx,65536*256-1
	add	ebp,ebx
	mov	ebx,[esi+24]	 ;ty
	and	ebx,65536*256-1
	mov	cl,[byte ptr submap3p_mapnr]
	shr	ebx,8
	mov	eax,[esi+48]
	shl	eax,16
	mov	ch,[esi+48+2]
	add	ebx,eax
;;        mov     esi,[submap3p_add1]
;        mov     esi,[submap3p_add2]   ;eax
        mov     esi,[submap3p_tex]
        mov     eax,ecx
        mov     ax,[word ptr submap3p_add2]
        mov     ecx,ebx
        mov     ebx,ebp
        mov     ebp,[submap3p_add1]

        jmp     edx
@@xloop:
@@jin8: innerb2  -7
@@jin7: innerb2  -6
@@jin6: innerb2  -5
@@jin5: innerb2  -4
@@jin4: innerb2  -3
@@jin3: innerb2  -2
@@jin2: innerb2  -1
@@jin1: innerb2  0
        add     edi,8*4
        sub     eax,65536
	jge	@@xloop
endm
	submapper

endp


;        mov     ecx,[esi+edx*4]
;        mov     [byte ptr pix1r],ch
; mov ebp,[submap_deltauv]
;        and     ecx,65536*255+255
;       add     ebx,ebp
;        mov     [dword ptr pix1],ecx   ;*
;        mov     ecx,[esi+edx*4+256*4]
;        fld     [pix1]
;        fmul    [biltabel+0+eax]
;        mov     [byte ptr pix2r],ch
;       mov     ebp,edi
;        and     ecx,65536*255+255
;       push    ebx
;        mov     [dword ptr pix2],ecx   ;*
;        mov     ecx,[esi+edx*4+4]
;        fld     [pix2]
;        fmul    [biltabel+8+eax]
;        mov     [byte ptr pix3r],ch    ;*
;        and     ecx,65536*255+255
;        mov     [dword ptr pix3],ecx   ;*
;        mov     ecx,[esi+edx*4+257*4]
;        fadd
;        fld     [pix3]
;        fmul    [biltabel+4+eax]
;        mov     [byte ptr pix4r],ch
;       mov     edx,ebx
;       shr     edx,24
;        and     ecx,65536*255+255
;        mov     [dword ptr pix4],ecx   ;*
; ;      mov     ecx,ebx
;       mov     dh,bh
;        fadd
;        fld     [pix4]
;        fmul    [biltabel+12+eax]
;       shr     ebx,4+8
;        mov     eax,[submap3_xdeltagint]
;        fadd
;        shr     ebp,16
;       mov     bl,[esp]
;       and     ebx,255*16
;        add     edi,eax
;        add     ebp, offset gouraudtabel
;        xor     ecx,ecx
;        fstp    [fincol]
;     push ebx
;        mov     cl,[byte ptr fing]
;        xor     ebx,ebx
;        xor     eax,eax
;        mov     bl,[byte ptr finb]
;        mov     cl,[ebp+ecx]
;        shl     ecx,16
;        mov     al,[byte ptr finr]
;        mov     cl,[ebp+ebx]
;
;        mov     ch,[ebp+eax]
;        mov     ebp,[submap_innerdestination]
;        pop     eax
;      pop ebx
;        mov     [ebp+offset1*4],ecx
;
;
;
;endm


;ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
;³Phong (24bit) perspektiv, bilinear                         ³
;ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
proc    submap3per24bilphong  p1:dword,p2:dword,p3:dword,mapnr:dword,dest:dword

intptx=1
intpty=1
intpg=0
intpz=1
swidth=320*4
swidthp=8

macro	innerd1  offset1

	add	ebx,ebp

	mov	edx,ebx
	shr	edx,24
	mov	dh,bh
	mov	ecx,ebx
	shr	ecx,4+8
	mov	cl,bl
;	 shr	 ecx,8
	and	ecx,255*16


	mov	eax,[esi+edx*4]
	mov	[byte ptr pix1r],ah
	and	eax,65536*255+255
	mov	[dword ptr pix1],eax
	mov	eax,[esi+edx*4+4]
	fld	[pix1]
	fmul	[biltabel+0+ecx]

	mov	[byte ptr pix2r],ah
	and	eax,65536*255+255
	mov	[dword ptr pix2],eax
	mov	eax,[esi+edx*4+256*4]
	fld	[pix2]
	fmul	[biltabel+4+ecx]

	mov	[byte ptr pix3r],ah
	and	eax,65536*255+255
	mov	[dword ptr pix3],eax
	mov	eax,[esi+edx*4+257*4]
	fadd
	fld	[pix3]
	fmul	[biltabel+8+ecx]
	mov	[byte ptr pix4r],ah
	and	eax,65536*255+255
	mov	[dword ptr pix4],eax

	fadd
	fld	[pix4]
	fmul	[biltabel+12+ecx]


	fadd
	fstp	[fincol]
	mov	eax,[dword ptr fincol]
	mov	ah,[byte ptr finr]


;        shr     eax,1
;        and     eax,127*65535+127*256+127

;        mov     edx,[edi+offset1*4]
;        shr     edx,1
;        and     edx,127*65535+127*256+127

;        add     eax,edx

;        mov     [edi+offset1*4],eax


        and     eax,255
        add     eax,offset gouraudtabel



        xor     ecx,ecx
        mov     cl,[edi+offset1*4]
        mov     cl,[eax+ecx]
        mov     [edi+offset1*4],cl
        xor     ecx,ecx
        mov     cl,[edi+offset1*4+1]
        mov     cl,[eax+ecx]
        mov     [edi+offset1*4+1],cl
        xor     ecx,ecx
        mov     cl,[edi+offset1*4+2]
        mov     cl,[eax+ecx]
        mov     [edi+offset1*4+2],cl


;        mov     [ebp+offset1*4],ecx



endm


macro	preinit
;ecx og edx er fri

;;        mov     ecx,[mapnr]
;;        and     ecx,255

          fld     [submap3_xdeltatx]
          fmul    [fp8]
          fld     [submap3_xdeltaty]
          fmul    [fp8]
          fld     [submap3_xdeltaz]
          fmul    [fp8]
          fxch    st(2)
          fstp    [submap3per_8deltatx]
          fstp    [submap3per_8deltaty]
          fstp    [submap3per_8deltaz]

;;        mov     ecx,[texturetable+ecx*4]
;;        mov     [submap3p_tex],ecx


;	 mov	 cl,[byte ptr mapnr]
;	 mov	 [byte ptr submap3p_mapnr],cl

endm

macro	innerloop
	sub	eax,ebx
	jle	@@udenfor
	fld1					  ;beregn z
	fdiv	[submap3_venstrez]
	dec	eax

	mov	ecx,eax
	and	eax,7
     ;	 shl	 ecx,8-3+8
   shr ecx,3
   mov [submap_counter],ecx
	add	ebx,eax
	shl	ebx,3	       -1

	add	edi,ebx
	mov	edx,[dword ptr @@jumptable+eax*4]


		       ;step a pixels ind...
	fld	[submap3_xdeltaz]
	fmul	[inttofloat+eax*4]
	fld	[submap3_xdeltatx]
	fmul	[inttofloat+eax*4]
	fld	[submap3_xdeltaty]
	fmul	[inttofloat+eax*4]
	fxch	st(2)
	fadd	[submap3_venstrez]
	fxch	st(1)
	fadd	[submap3_venstretx]
	fxch	st(2)
	fadd	[submap3_venstrety]
	fxch	st(1)
	fstp	[submap3per_currentz]
	fxch	st(1)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]


	fld	[submap3_venstretx]		  ;beregn tx og ty
	fmul	st(0),st(1)
	fxch
	fmul	[submap3_venstrety]
	fxch
	fst   [dword ptr submap3per_temp3]		    ;tx som float
	fadd	[fp2p60]
	fstp   [submap3per_temp1]		 ;tx som int
	fst   [dword ptr submap3per_temp4]		    ;ty som float
	fadd	[fp2p60]
	fstp   [submap3per_temp2]		 ;ty som int

	fld1					  ;beregn n‘ste z
	fdiv	[submap3per_currentz]

	fld	[submap3per_currenttx]		  ;beregn tx og ty
	fmul	st(0),st(1)
	fxch
	fmul	[submap3per_currentty]
	fxch
	fld    [fp2p60]
	fadd   st(0),st(1)
	fstp   [submap3per_oldtx]		  ;og gem som "gamle" (int)
	fxch
	fld    [fp2p60]
	fadd   st(0),st(1)
	fstp   [submap3per_oldty]

	fsub	[dword ptr submap3per_temp4]		    ;beregn delta
	fxch
	fsub	[dword ptr submap3per_temp3]
	fxch
	fmul	[inttoinvfloat+eax*4]
	fxch
	fmul	[inttoinvfloat+eax*4]
	fxch
	fadd	[fp2p60]
	fstp   [submap3per_temp4]		 ;ty
	fadd	[fp2p60]
	fstp   [submap3per_temp3]		 ;tx


	fld	[submap3per_currenttx]		  ;step 8 pixels
	fadd	[submap3per_8deltatx]
	fld	[submap3per_currentty]
	fadd	[submap3per_8deltaty]
	fld	[submap3per_currentz]
	fadd	[submap3per_8deltaz]
	fxch	st(2)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]
	fstp	[submap3per_currentz]

	fld1					  ;start beregning af n‘ste z
	fdiv	[submap3per_currentz]


	mov	eax,[dword submap3per_temp3]
	shl	eax,16
	mov	ax,[word submap3per_temp4]
  mov ebp,eax
	mov	ebx,[dword submap3per_temp1]
	shl	ebx,16
	mov	bx,[word submap3per_temp2]
  mov	esi,[submap3p_tex]
	mov	ch,16
	mov	cl,[byte ptr submap3p_mapnr]
	jmp	edx
@@xloop:

	fld	[submap3per_currenttx]
	fmul	st(0),st(1)
	fxch
	fmul	[submap3per_currentty]
	fxch
	fadd	[fp2p60]
	fxch
	fadd	[fp2p60]
	fxch
	fstp   [submap3per_temp1]
	fstp   [submap3per_temp2]

	fld	[submap3per_currenttx]
	fadd	[submap3per_8deltatx]
	fld	[submap3per_currentty]
	fadd	[submap3per_8deltaty]
	fld	[submap3per_currentz]
	fadd	[submap3per_8deltaz]
	fxch	st(2)
	fstp	[submap3per_currenttx]
	fstp	[submap3per_currentty]
	fstp	[submap3per_currentz]

	fld1
	fdiv	[submap3per_currentz]

	mov	eax,[dword ptr submap3per_temp1]
	mov	edx,[dword ptr submap3per_temp2]
	sub	eax,[dword ptr submap3per_oldtx]
	sub	edx,[dword ptr submap3per_oldty]

	mov	ebx,[dword ptr submap3per_oldtx]
	shl	ebx,16
	mov	bx,[word ptr submap3per_oldty]

	sar	eax,3
	sar	edx,3

	shl	eax,16
	mov	ax,dx
   mov ebp,eax
   mov [submap_counter],ecx
	mov	edx,[dword ptr submap3per_temp1]
	mov	[dword ptr submap3per_oldtx],edx
	mov	edx,[dword ptr submap3per_temp2]
	mov	[dword ptr submap3per_oldty],edx

	add	ebx,eax
@@jin8: innerd1  -7
@@jin7: innerd1  -6
@@jin6: innerd1  -5
@@jin5: innerd1  -4
@@jin4: innerd1  -3
@@jin3: innerd1  -2
@@jin2: innerd1  -1
@@jin1: innerd1  0
	add	edi,8*4
	mov	ecx,[submap_counter]
	dec	ecx
;	 sub	 ecx,65536
	jge	@@xloop
	fstp	[submap3per_temp1]
endm
	submapperper

dataseg
;submap3per_temp1	 dd	 ?
;submap3per_temp2	 dd	 ?
codeseg

endp

















end

x
z
1/z
y/z
x/z
--

fld	st(2)
fmul	st(0),st(1)
fxch	st(2)
fadd	[deltaz]
fxch	st(2)
fmul	st(0),st(3)
fxch
faddmagic
fxch
faddmagic

fadd
fadd
fstp
fstp
fld	1
fdiv	st()


