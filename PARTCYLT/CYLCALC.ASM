ideal
p386
model	flat,c

t_a=0
t_b=4
t_c=8
t_d=12
t_e=16
t_f=20
t_g=24
t_h=28
cyldim=20
cylinder_o=0
cylinder_x=cylinder_o+4
cylinder_y=cylinder_x+4
cylinder_r=cylinder_y+4
cylinder_r2=cylinder_r+4
cylinder_gamma=cylinder_r2+4
cylinder_nabox=cylinder_gamma+4
cylinder_naboy=cylinder_nabox+cyldim*4
cylinder_texnum=cylinder_naboy+cyldim*4

jumps

global trace_ray1:near


global trace_tunnels
global trace_lystabel
global  atan_tabel


dataseg

trace_multable  dd  256 dup(?)
t_temp          dd      0
fpm1            dd      0BF800000h
fpm4            dd      0C0800000h


pos2x           dd      0
pos2y           dd      0
pos2z           dd      0
temp            dd      0
fp_4096         dd      45800000h
fp_1024         dd      44800000h
ihz             dd      0
fp_m1           dd      0BF800000h
fp_pid2         dd      03fc90fdbh
fp_3d2          dd      03fc00000h
atanscale       dd      042800000h
fp_65536d2dpi   dd      04622f983h
fp_0            dd      0
fp_pi           dd      040490fdbh
codeseg

macro   atan2
        fld     st(1)
        fmul    st(0),st(0)
        fld     st(1)
        fmul    st(0),st(0)
        fsub
        fstp    [temp]
        mov     eax,[temp]
        cmp     eax,0
        jge     @@jump
        fxch    st(1)
        fmul    [fp_m1]
        fld     [fp_pid2]
        jmp     @@jump2
@@jump:
        fld     [fp_0]
@@jump2:
        fxch    st(2)

;        fxch    st(1)

        fst     [temp]
        mov     eax,[temp]
        cmp     eax,0
        jge     @@ok
        fxch    st(2)
        fadd    [fp_pi]
        fxch    st(2)
@@ok:
;        fxch    st(1)

        fdiv
        fadd    [fp_3d2]
        fmul    [atanscale]
        fist    [temp]
        fisub   [temp]
        mov     eax,[temp]
        and     eax,255
        fld     [atan_tabel+eax*4+4]
        fsub    [atan_tabel+eax*4]
        fmul
        fadd    [atan_tabel+eax*4]
        fadd
endm


proc    trace_ray1 ctunnel:dword, pos3x:dword,pos3y:dword, pos3z:dword, dx_:dword, dy:dword, dz:dword

        pushad

        mov     edi,[ctunnel]
        mov     eax,20
@@loop:



;        pos2x=pos3x-tunnels[ctunnel].x;
;        pos2y=pos3y-tunnels[ctunnel].y;
;        pos2z=pos3z;
        fld     [pos3x]
        fsub    [dword ptr edi+cylinder_x]
        fld     [pos3y]
        fsub    [dword ptr edi+cylinder_y]
        fxch    st(1)
        mov     ebx,[pos3z]
        mov     [pos2z],ebx
        fstp    [pos2x]
        fstp    [pos2y]


;{
;        float   alfa,beta,gamma,d;
;
;        alfa=dx*dx+dy*dy;
;        beta=(dx*pos2x+dy*pos2y);
;        //gamma=pos2x*pos2x+pos2y*pos2y-tunnels[ctunnel].r2;
;        gamma=tunnels[ctunnel].gamma;
;
;        d=beta*beta-alfa*gamma;
;
;        if(d>0)
;        {
;                t=(-beta+sqrt(d))/(alfa);
;        }
;
;        hx=pos3x+t*dx;
;        hy=pos3y+t*dy;
;        hz=pos3z+t*dz;
;}

        fld     [dx_]
        fmul    [dx_]
        fld     [dy]
        fmul    [dy]
        fld     [dx_]
        fmul    [pos2x]
        fld     [dy]
        fmul    [pos2y]
        fxch    st(3)
        faddp   st(2),st(0)
        fld     [dword ptr edi+cylinder_gamma]
        fxch    st(3)
        fadd
        fxch    st(1)
        fmul    st(2),st(0)
        fxch    st(1)
        nop
        fld     st(0)
        fmul    st(0),st(0)
        fsubrp  st(3),st(0)
        fxch    st(2)
        fsqrt
        fsubrp  st(2),st(0)
        fdiv

        fld     [dx_]
        fmul    st(0),st(1)
        fld     [dy]
        fmul    st(0),st(2)
        fxch    st(1)
        fadd    [pos3x]
        fxch    st(2)
        fmul    [dz]
        fxch    st(1)
        fadd    [pos3y]
        fxch    st(1)
        fadd    [pos3z]


;//find tunnel x og y
;
;{
;        int     ihz=hz;
;        if((ihz>=0)&&(ihz<cyldim))
;        {
;                tunnelx=tunnels[ctunnel].nabox[ihz];
;                tunnely=tunnels[ctunnel].naboy[ihz];
;        }
;        else
;        break;
;}

        fist    [ihz]
        mov     ebx,[ihz]
        cmp     ebx,0
        jl      @@break
        cmp     ebx,cyldim
        jge     @@break
        mov     ecx,[edi+cylinder_nabox+ebx*4]
        mov     edx,[edi+cylinder_nabox+ebx*4]


;if(tunnelx)
;{
;        d1=hz-tunnels[tunnelx].y;
;        d2=hy-tunnels[tunnelx].x;
;
;        if(d1*d1+d2*d2<tunnels[tunnelx].r2)
;        {
;                float   temp;
;
;                temp=pos3x;
;                pos3x=pos3y;
;                pos3y=pos3z;
;                pos3z=temp;
;
;                temp=dx;
;                dx=dy;
;                dy=dz;
;                dz=temp;
;
;                ctunnel=tunnelx;
;        }
;}

        cmp     ecx,0
        je      @@notunnelx
        fld     [dword ptr ecx+cylinder_y]
        fsub    st(0),st(1)
        fld     [dword ptr ecx+cylinder_x]
        fsub    st(0),st(3)

        fxch    st(1)
        fmul    st(0),st(0)
        fxch    st(1)
        fmul    st(0),st(0)
        fxch    st(1)
        fsub    [dword ptr ecx+cylinder_r2]
        fadd
        fstp    [temp]
        mov     ebx,[temp]
        cmp     ebx,0
        jg      @@notunnelx

        mov     ebx,[pos3x]
        mov     edx,[pos3y]
        mov     [pos3x],edx
        mov     edx,[pos3z]
        mov     [pos3y],edx
        mov     [pos3z],ebx

        mov     ebx,[dx_]
        mov     edx,[dy]
        mov     [dx_],edx
        mov     edx,[dz]
        mov     [dy],edx
        mov     [dz],ebx

        mov     edi,ecx
        jmp     @@next
@@notunnelx:


;if(tunnely)
;{
;        d1=hx-tunnels[tunnely].y;
;        d2=hz-tunnels[tunnely].x;
;
;        if(d1*d1+d2*d2<tunnels[tunnely].r2)
;        {
;                float   temp;
;
;                temp=pos3x;
;                pos3x=pos3z;
;                pos3z=pos3y;
;                pos3y=temp;
;
;                temp=dx;
;                dx=dz;
;                dz=dy;
;                dy=temp;
;
;                ctunnel=tunnely;
;        }
;
;        else break;
;}
;else break;

        cmp     edx,0
        je      @@break
        fld     [dword ptr edx+cylinder_y] ;--------------------mulmulmul
        fsub    st(0),st(3)
        fld     [dword ptr edx+cylinder_x]
        fsub    st(0),st(2)

        fxch    st(1)
        fmul    st(0),st(0)
        fxch    st(1)
        fmul    st(0),st(0)
        fxch    st(1)
        fsub    [dword ptr edx+cylinder_r2]
        fadd
        fstp    [temp]
        mov     ebx,[temp]
        cmp     ebx,0
        jg      @@break

        mov     edi,edx
        mov     ebx,[pos3y]
        mov     edx,[pos3x]
        mov     [pos3y],edx
        mov     edx,[pos3z]
        mov     [pos3x],edx
        mov     [pos3z],ebx

        mov     ebx,[dy]
        mov     edx,[dx_]
        mov     [dy],edx
        mov     edx,[dz]
        mov     [dx_],edx
        mov     [dz],ebx




@@next:
        fstp    [temp]
        fstp    [temp]
        fstp    [temp]

        dec     eax
        jne     @@loop
@@break:


;{
;        trace_p screen;
;        float   nx,ny,nz;
;        float   vx,vy,vz;
;
;        vx=hx-pos3x;
;        vy=hy-pos3y;
;        vz=hz-pos3z;
;
;        nx=hx-tunnels[ctunnel].x;
;        ny=hy-tunnels[ctunnel].y;
;        nz=0;
;
;        screen.c=trace_lystabel[(int)(1024*(vx*nx+vy*ny)*(vx*nx+vy*ny)/((nx*nx+ny*ny)*(vx*vx+vy*vy+vz*vz)))];
;        screen.u=atan2(nx,ny)*256*256.0/2/pi;
;        screen.v=hz*256.0*256/32*2;
;        screen.t=1+ctunnel<<4;
;
;        return screen;
;}

        fld     [dword ptr edi+cylinder_x]
        fsubr   st(0),st(3)
        fld     [dword ptr edi+cylinder_y]
        fsubr   st(0),st(3)


        fld     st(2)
        fmul    [fp_4096]

        mov     bl,[byte ptr edi+cylinder_texnum]


;        fistp   [word ptr esp+10] ;v


        mov     eax,0
        mov     [dword ptr ss:esi+0],eax
        mov     [dword ptr ss:esi+4],eax
        mov     [dword ptr ss:esi+8],eax

        fistp   [temp]
        mov     eax,[temp]
        mov     [word ptr esi+6],ax

        fxch    st(2)
        fsub    [pos3z]
        fxch    st(3)
        fsub    [pos3y]
        fxch    st(4)
        fsub    [pos3x]


        fld     st(1)    ;ny
        fmul    st(0),st(5)   ;vy
        fld     st(3)   ;nx
        fmul    st(0),st(2)   ;vx
        fadd
        fmul

        fxch    st(1)   ;vx
        fmul    st(0),st(0)
        fxch    st(5)   ;vy
        fmul    st(0),st(0)
        fxch    st(4)   ;vz
        fmul    st(0),st(0)

        fld     st(2)
        fmul    st(0),st(0)
        fld     st(4)
        fmul    st(0),st(0)
        fadd

        fxch    st(2)
        faddp   st(6),st(0)
        fxch    st(4)
        fmulp   st(5),st(0)

        fxch    st(1)
        fmulp   st(4),st(0)

        fdivp   st(3),st(0)
        fxch    st(2)
        fmul    [fp_1024]
        fistp   [temp]

        mov     eax,[temp]
        and     eax,1023
        mov     eax,[trace_lystabel+eax*4]
        mov     [word ptr ss:esi+10],ax


;        fld     st(2)
;        fld     st(2)
        atan2
        fmul    [fp_65536d2dpi]
        fistp   [temp]
        mov     eax,[temp]
        mov     [word ptr ss:esi+4],ax   ;u
;        fstp    [temp]
;        fstp    [temp]
;        fstp    [temp]
;        fstp    [temp]
;        fstp    [temp]




        mov     [byte ptr ss:esi +8],bl

        popad
        ret
endp


proc    trace_initmt s:dword, d:dword
        pushad

        mov     eax,256
        lea     edi,[trace_multable]
        mov     esi,[s]
@@loop:
        mov     [edi],esi
        add     esi,[d]
        add     edi,4
        dec     eax
        jne     @@loop

        popad
        ret
endp

end
