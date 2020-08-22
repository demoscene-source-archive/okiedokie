int ptexturepointer,scroffset;
int ushift,umask,vmask,dumask,innerumask,innervmask,mask,bits,tildudx,tildvdx;

float longest,lwidth,rela;
int x_a,x_b;
int dx_a, dx_b;
int y,x,length;
float id,dwidth;

int adsu, adsv, adsz;
int au1, av1, az1;

float su1,sv1,sz1;
float su2,sv2,sz2;
float su3,sv3,sz3;
int adu_a, adv_a, adz_a;
int iu, iv;
unsigned int dcol;


int roundx(float x){
    return(0.9999999+x);
//    return((int)(x));
}

long ceilx(long x){
    x += 0xffff;
    return (x >> 16);
//    return (x);
}

#pragma aux au1 "*";
#pragma aux av1 "*";
#pragma aux x_a "*";
#pragma aux x_b "*";
#pragma aux dx_a "*";
#pragma aux dx_b "*";

#pragma aux adu_a "*";
#pragma aux adv_a "*";
#pragma aux adz_a "*";
#pragma aux adsu "*";
#pragma aux adsv "*";
#pragma aux adsz "*";
#pragma aux ptexturepointer "*";

#pragma aux affinefill "*" parm caller [eax] [ebx] modify [eax ebx ecx edx esi edi 8087];
extern void affinefill(int, int);

TriangleTexture(RotatedVertice * vp1,
                 RotatedVertice * vp2,
                 RotatedVertice * vp3,
                 void * dest){

RotatedVertice *vp;
float dy32, dy31, dy21;
float gdy32,gdy31,gdy21;
float dx31, dx21;
float dx3cr1, dx2cr1;
float dx3cg1, dx2cg1;
float dx3cb1, dx2cb1;
float dx3cz1, dx2cz1;



  // sorter polygon
    if(vp1->py > vp2->py) { vp = vp1; vp1 = vp2; vp2 = vp; }
    if(vp1->py > vp3->py) { vp = vp1; vp1 = vp3; vp3 = vp; }
    if(vp2->py > vp3->py) { vp = vp2; vp2 = vp3; vp3 = vp; }

    dy31=vp3->py - vp1->py;
    if(dy31==0)return;
    ptexturepointer=(int)tiletexture;
    scroffset=(int)dest;
    dy21=vp2->py - vp1->py;
    dx31=vp3->px - vp1->px;
    dx21=vp2->px - vp1->px;
    su1=( vp1->u);
    sv1=( vp1->v);
    su2=( vp2->u);
    sv2=( vp2->v);
    su3=( vp3->u);
    sv3=( vp3->v);

    longest= (dy21)/(dy31) * (dx31) - (dx21);

    dwidth = (vp1->px - vp3->px) * (vp2->py - vp3->py) -
             (vp2->px - vp3->px) * (vp1->py - vp3->py);

    if(dwidth == 0.0) return;
    id = 65536.0/dwidth;
    adsu=(float)(((su1 - su3)*(vp2->py - vp3->py) -
                 (su2 - su3)*(vp1->py - vp3->py))*id);
    adsv=(float)(((sv1 - sv3)*(vp2->py - vp3->py) -
                 (sv2 - sv3)*(vp1->py - vp3->py))*id);
    mask=0x1f;
    bits=5;
    ushift = (3+bits);
    umask  = (mask<<(16+6+bits));
    vmask  = (mask<<(16+6))|0x380000;
    dumask  = vmask|0x8000;
    innerumask = umask|0x77fff;
    innervmask = vmask|0x07fff;
    tildudx = ((adsu<<ushift)&umask)|(adsu&0x70000)|((adsu>>1)&0x7fff)|dumask;
    tildvdx = ((adsv<<3)&vmask)|((adsv>>1)&0x7fff)|0x78000;
    gdy31=65536.0/dy31;
    if(dy21) gdy21=65536.0/dy21;
    if(longest>0){
    if(dy21) {
        dx_a = (float)(dx21* gdy21);
        adu_a= (float)((su2-su1)*gdy21);
        adv_a= (float)((sv2-sv1)*gdy21);
        }
        dx_b = (float)(dx31* gdy31);

    } else {
        dx_a = (float)(dx31* gdy31);
        adu_a= (float)((su3-su1)* gdy31);
        adv_a= (float)((sv3-sv1)* gdy31);
     if(dy21) {
          dx_b = (float)(dx21* gdy21);
        }
    }


    x_a=x_b = (float)(65536*vp1->px);           // start at the top vertex
    au1 = (float)(su1*65536);
    av1 = (float)(sv1*65536);
    x_a+=(float)((dx_a)*(roundx(vp1->py)-vp1->py));
    x_b+=(float)((dx_b)*(roundx(vp1->py)-vp1->py));
    au1+=(float)((adu_a)*(roundx(vp1->py)-vp1->py));
    av1+=(float)((adv_a)*(roundx(vp1->py)-vp1->py));

    if(dy21){
        affinefill(roundx(vp1->py),roundx(vp2->py));
    }

    dy32 = vp3->py - vp2->py;
    if(dy32==0)return;
    gdy32=65536.0/dy32;
    if (longest>0) {
        dx_a = (float)((vp3->px - vp2->px)* gdy32);
        adu_a= (float)((su3-su2)*gdy32);
        adv_a= (float)((sv3-sv2)*gdy32);
        x_a = (float)(vp2->px* 65536) +dx_a*(roundx(vp2->py)  -vp2->py);
        au1 = (float)(su2*65536) +adu_a*(roundx(vp2->py)  -vp2->py);
        av1 = (float)(sv2*65536) +adv_a*(roundx(vp2->py)  -vp2->py);
    }else{

        dx_b = ((vp3->px - vp2->px)* gdy32);
        x_b = (float)(vp2->px* 65536) +dx_b*(roundx(vp2->py)   -vp2->py);

    }
      affinefill(roundx(vp2->py),roundx(vp3->py));
}// end of DrawTriangleGTexture
