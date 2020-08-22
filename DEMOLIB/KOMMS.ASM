
ideal
p386
model	flat,c
jumps


global	vret:near
global  fpupre:near

dataseg

OldFPUCW   dw  ?
FPUCW      dw  ?

codeseg

proc    fpupre

        push    eax
   ;// <em><strong>Put the FPU in 32 bit mode</em></strong>
   fstcw   [OldFPUCW]           ; // store copy of CW
   mov     ax,[OldFPUCW]        ; // get it in ax
   and     eax,NOT 0300h       ; // 24 bit precision
   or eax,0200h
   mov     [FPUCW],ax           ; // store it
   fldcw   [FPUCW]              ; // load the FPU

        pop     eax
        ret
endp

proc	vret

	pushad
	mov	dx,3dah
@@L1:
	in	al,dx
	test	al,8
	jnz	@@L1
@@L2:
	in	al,dx
	test	al,8
	jz	@@L2
	popad
	ret

endp

end
