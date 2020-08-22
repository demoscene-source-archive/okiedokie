.486p

PREC2 equ 4
scrwidth equ 320

        locals  @@
        jumps
	model	flat			; Or use LARGESTACK instead

	dataseg
         extrn x_a,x_b :dword
         extrn dx_a,dx_b :dword

         extrn _vdscr :dword
         extrn _scroffset :dword

         extrn adu_a, adv_a :dword
         extrn adsu,adsv :dword
         extrn au1, av1 :dword
         extrn _tildudx,_tildvdx :dword
         extrn ptexturepointer :dword

         mulconst       dd ?
         screenpos         dd ?
         subtexel       dd ?
         ycounter       dd ?
         scrpos         dd ?
         counter        dd ?
         extraadd       dd ?
         extraadd2       dd ?

        colincr2        dd ?
        colincg2        dd ?
        colincb2        dd ?

         newx_add    dd ?
;         align 4

         iuold          dd ?
         ivold          dd ?

         iu             dd ?
         iv             dd ?

         idu            dd ?
         idv            dd ?
         xcount         dd ?
         lengthx        dd ?
         cr             dd ?
         cg             dd ?
         cb             dd ?
         cz             dd ?
         xtrareg        dd ?
         texadd         dd ?
         yloopcounter   dd ?


codeseg

public  affinefill
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같�
;#pragma aux affinefill "*" parm caller [eax] [ebx]
;extern void near affinefill(int, int);
;같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같같�

affinefill proc
        pusha
        push ebp
        sub ebx,eax
        cmp ebx,0
        jna @@end
        mov [ycounter],ebx
        mov edi,[_scroffset]
        mov esi,[ptexturepointer]

;        shl eax,11
       lea      eax,[eax*4+eax] ;fixed 640 width screen
       shl      eax,8

        add edi,eax
        mov [scrpos],edi  ; calc screenpos


        mov ecx,[ycounter]

       @@yloop:
        mov [yloopcounter],ecx
        mov edi,[scrpos]        ; get screenpos

        mov eax,[x_a]
        shr eax,16
        mov ecx,[x_b]
        shr ecx,16
        sub ecx,eax             ;calc length of scanline
        jz @@skipline

        shl eax,2
         add edi,eax

          ; bitmap (256x256) must be stored as 8x8 tiles
          ; tildudx =  wwwww11111111www1fffffffffffffffb  (w=whole, f=frac)
          ; tildvdx =  11111wwwwwwww1111fffffffffffffffb
          ; eax =  u   wwwww00000000www0fffffffffffffffb
          ; ebx =  v   00000wwwwwwww0000fffffffffffffffb
          ; ecx =  scanline width
          ; edi =  destination ptr
          ; esi =  bitmap ptr
          mov eax,[au1]
          shl eax,8
          and eax,0f8000000h
          mov edx,[au1]
          shr edx,1
          and edx,00007fffh
          add eax,edx
          mov edx,[au1]
          and edx,00070000h
          add eax,edx
;         u = ((u << 8) & 0xf8000000) +
;             ((u >> 1) & 0x00007fff) +
;              (u       & 0x00070000);
          mov ebx,[av1]
          shl ebx,3
          and ebx,07f80000h
          mov edx,[av1]
          shr edx,1
          and edx,00007fffh
          add ebx,edx
      
;         v = ((v << 3) & 0x07f80000) +
;             ((v >> 1) & 0x00007fff);
  
          lea   edi, [edi+ecx*4]
          xor   ecx, -1
          lea   ebp, [eax+ebx]                            ; u+v
          inc   ecx
  
       inner:
          add   eax, [_tildudx]                            ; u += tildudx
          add   ebx, [_tildvdx]                            ; v += tildvdx
          shr   ebp, 16                                   ; (u+v) >> 16
          and   eax, 11111000000001110111111111111111b    ; clear bitgaps
          and   ebx, 00000111111110000111111111111111b
          mov   edx, [esi+ebp*4]                             ; get color
          inc   ecx
          lea   ebp, [eax+ebx]                            ; u+v
          mov   [edi+ecx*4], edx                             ; plot pixel
          jnz   inner

;        mov eax,_flatcol
;       @@xloop:
;         mov [edi],ax
;         add edi,2
;         dec ecx
;         jnz @@xloop
       @@skipline:

        mov eax,[x_a]
        add eax,[dx_a]
        mov [x_a],eax

        mov eax,[x_b]
        add eax,[dx_b]
        mov [x_b],eax

        mov eax,[au1]
        add eax,[adu_a]
        mov [au1],eax

        mov eax,[av1]
        add eax,[adv_a]
        mov [av1],eax

        mov edi,[scrpos]
        add edi,320*4
        mov [scrpos],edi

        mov ecx,[yloopcounter]
        dec ecx
        jnz @@yloop

       @@end:

        pop ebp
        popa
        ret
affinefill endp

;    for (y=y1;y<y2;y++)
;    {
;            dest=y*640+((int)(x_a)>>16);
;            length=((int)(x_b)>>16)-((int)(x_a)>>16);
;            u=u1+((dsu*(((int)(x_a>>16)<<16)-x_a))>>16);
;            v=v1+((dsv*(((int)(x_a>>16)<<16)-x_a))>>16);
;            u = (( u<<ushift)&umask)|( u&0x70000)|(( u>>1)&0x7fff);
;            v = (( v<<3)&vmask)|(( v>>1)&0x7fff);
;            for (x=0;x<length;x++)
;            {
;              wscr2[dest+x]= tiletexture[((unsigned int)(u+v)>>16)];
;              u =(u+tildudx)&innerumask;  // addition + masking out the 1s in the gaps
;              v =(v+tildvdx)&innervmask;  // same as above
;
;            }
;            x_a+=dx_a;
;            x_b+=dx_b;
;            u1+=du_a;
;            v1+=dv_a;
;    }*/


end
