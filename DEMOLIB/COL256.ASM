ideal
p386
model	flat,c
jumps

global	to256col:near
global	rgb:near

dataseg

codeseg

proc	to256col src:dword,dest:dword

	pushad

	mov	edi,[dest]
	dec	edi
	mov	edx,-64000
	mov	esi,[src]
	add	esi,64000*2
@@loop:

	mov	bx,[esi+edx*2]
	inc	edi
	mov	eax,ebx
	mov	ecx,ebx
	shr	eax,10
	and	ebx,31
	shr	ecx,5
	and	eax,31		;overfl›dig!
	add	ebx,eax
	and	ecx,31
	add	ebx,ecx
	inc	edx
	mov	[edi],bl
	jl	@@loop

	popad
	ret

endp

proc	rgb num:dword,r:dword,g:dword,b:dword

	pushad
	mov	dx,3c8h
	mov	eax,[num]
	out	dx,al
	inc	dx
	mov	eax,[r]
	out	dx,al
	mov	eax,[g]
	out	dx,al
	mov	eax,[b]
	out	dx,al
	popad
	ret

endp

end
