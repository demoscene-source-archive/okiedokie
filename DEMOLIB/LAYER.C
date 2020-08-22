/* by craft / fudge */

#include "stdlib.h"
#include "layer.h"

#define lsize 4

extern  char    layer_multable[257*256];

layer4  layer_heap[50*15];
layer4* layer_list[200/lsize+1];

layer4* layer_next;
void*   layer_screen;

rgb1    layer_temp[320*lsize];

char    *layer_font;

typedef struct rgb32
{
        int     r,g,b,a;
} rgb32;

rgb32   temp[1024];

rgb1    dest[64000];

int     RGB(int r, int g, int b)
{
        return r*256*256+g*256+b;
}

void    layer_init(void)
{
        int     x,y;

        for(y=0;y<=256;y++)
        for(x=0;x<256;x++)
        {
                layer_multable[(y<<8)+x]=(x*y)>>8;
        }
}

layer   layer_make(void *source, int x, int y, int sx, int sy, int type, int r)
{
        layer   l;

        l.source=source;
        l.x=x;
        l.y=y;
        l.sizex=sx;
        l.sizey=sy;
        l.mix.type=type;
        l.mix.r=r;

        return l;
}

layer   layer_maketext(int x, int y, int sx)
{
        return layer_make(malloc(sx*8*4),x,y,sx,8,2,0);
}

void    layer_makemask(layer l, int v)
{
        int     x,y;
        rgb1    *p;

        p=l.source;

        for(y=0;y<l.sizey;y++)
        for(x=0;x<l.sizex;x++)
        {
                if(p->r||p->g||p->b)
                p->o=v;
                else
                p->o=0;

                p++;
        }

}

layer   layer_loadimage(char* imagename)
{
        rgb1    *image;
        pcx_header h;
        int     x,y;
        layer   l;


        image=pcx_read_picture(imagename,1);
        pcx_read_header(&h,imagename);

        l.x=h.xmin;
        l.y=h.ymin;

        l.sizex=h.xmax-h.xmin+1;
        l.sizey=h.ymax-h.ymin+1;


        for(y=0;y<l.sizey;y++)
        {
                for(x=0;x<l.sizex;x++)
                {
                        image[l.sizex*y+x].o=0;
                }
        }

        l.source=image;
        l.mix.type=1;
        l.mix.r=127;

        return l;
}

layer   layer_loadimageandmask(char* imagename, char* maskname)
{
        rgb1    *image,*mask;
        pcx_header h;
        int     x,y;
        layer   l;


        image=pcx_read_picture(imagename,1);
        mask=pcx_read_picture(maskname,1);
        pcx_read_header(&h,maskname);

        l.x=h.xmin;
        l.y=h.ymin;

        l.sizex=h.xmax-h.xmin+1;
        l.sizey=h.ymax-h.ymin+1;


        for(y=0;y<l.sizey;y++)
        {
                for(x=0;x<l.sizex;x++)
                {
                        image[y*l.sizex+x].o=mask[y*l.sizex+x].r;
                }
        }

        l.source=image;
        l.mix.type=9;
        l.mix.r=127;
        free(mask);

        return l;
}

void    layer_add(layer l)
{
        int     y;
        int     starty,endy;
        layer4  *first;
        layer4  *last;
        int     modulo;
        int     x1,y1,x2,y2,sx,sy;
        char    *screen,*source;
        int     screenmodulo,sourcemodulo;

        x1=l.x;
        y1=l.y;
        x2=l.x+l.sizex;
        y2=l.y+l.sizey;

        source=l.source;

        if(x1<0){source-=x1*4;x1=0;}          //fejl rettet 9/3 1999, statements stod i omvendt orden f›r!
        if(y1<0){source-=y1*l.sizex*4;y1=0;}
        if(x2>320)x2=320;
        if(y2>200)y2=200;

        sx=x2-x1;
        sy=y2-y1;

        if(sx<=0)return;
        if(sy<=0)return;

        screenmodulo=4*(320-sx);
        sourcemodulo=4*(l.sizex-sx);

        starty=(y1/lsize)*lsize;
        endy=(y2/lsize)*lsize;

        screen=(char*)layer_temp+x1*4;//+4*(320*starty);
        source-=l.sizex*4*(y1%lsize);

        first=layer_next;

        starty/=lsize;
        endy/=lsize;

        for(y=starty;y<=endy;y++)
        {
                layer_next->source=source;
                layer_next->dest=screen;
                layer_next->sourcemodulo=sourcemodulo;
                layer_next->destmodulo=screenmodulo;
                layer_next->sizex=sx;
                layer_next->sizey=lsize;
                layer_next->mix=l.mix;
                layer_next->next=layer_list[y];
                layer_list[y]=layer_next;
                layer_next++;
                source+=lsize*l.sizex*4;
          //      screen+=lsize*320*4;
        }

        last=layer_next-1;

        first->sizey=lsize-(y1%lsize);
        first->source=(char*)first->source+(y1%lsize)*l.sizex*4;
        first->dest=(char*)first->dest+(y1%lsize)*320*4;

        last->sizey=y2%lsize;
}

void    layer_addold(layer l)
{
        int     y;
        int     starty,endy;
        layer4  *first;
        layer4  *last;
        int     modulo;
        int     x1,y1,x2,y2,sx,sy;
        char    *screen,*source;
        int     screenmodulo,sourcemodulo;

        x1=l.x;
        y1=l.y;
        x2=l.x+l.sizex;
        y2=l.y+l.sizey;

        source=l.source;

        if(x1<0){x1=0;source-=x1*4;}
        if(y1<0){y1=0;source-=y1*l.sizey*4;}
        if(x2>320)x2=320;
        if(y2>200)y2=200;

        sx=x2-x1;
        sy=y2-y1;

        if(sx<=0)return;
        if(sy<=0)return;

        screenmodulo=4*(320-sx);
        sourcemodulo=4*(l.sizex-sx);

        starty=(y1/lsize)*lsize;
        endy=(y2/lsize)*lsize;

        screen=(char*)layer_screen+4*(320*starty+x1);
        source-=l.sizex*4*(y1%lsize);

        first=layer_next;

        starty/=lsize;
        endy/=lsize;

        for(y=starty;y<=endy;y++)
        {
                layer_next->source=source;
                layer_next->dest=screen;
                layer_next->sourcemodulo=sourcemodulo;
                layer_next->destmodulo=screenmodulo;
                layer_next->sizex=sx;
                layer_next->sizey=lsize;
                layer_next->mix=l.mix;
                layer_next->next=layer_list[y];
                layer_list[y]=layer_next;
                layer_next++;
                source+=lsize*l.sizex*4;
                screen+=lsize*320*4;
        }

        last=layer_next-1;

        first->sizey=lsize-(y1%lsize);
        first->source=(char*)first->source+(y1%lsize)*l.sizex*4;
        first->dest=(char*)first->dest+(y1%lsize)*320*4;

        last->sizey=y2%lsize;
}

void    layer_reset()
{
        int     t;

        for(t=0;t<200/lsize;t++)
        {
                layer_list[t]=NULL;
        }

        layer_next=layer_heap;
}

void    layer_draw()
{
        int     t;

        for(t=0;t<200/lsize;t++)
        {
                layer_drawasm(layer_list[t]);

                layer_list[t]=NULL;
        }

        layer_next=layer_heap;
}

void    layer_draw15(void *dest)
{
        int     t;

        for(t=0;t<200/lsize;t++)
        {
                layer_drawasm(layer_list[t]);

                layer_list[t]=NULL;

                con15(((char*)layer_temp),((char*)dest+t*320*lsize*2),lsize*320);
        }

        layer_next=layer_heap;
}

void    layer_draw24(void *dest)
{
        int     t;

        for(t=0;t<200/lsize;t++)
        {
                layer_drawasm(layer_list[t]);

                layer_list[t]=NULL;

                con24(((char*)layer_temp),((char*)dest+t*320*lsize*3),lsize*320);
        }

        layer_next=layer_heap;
}

void    layer_draw32(void *dest)
{
        int     t;

        for(t=0;t<200/lsize;t++)
        {
                layer_drawasm(layer_list[t]);

                layer_list[t]=NULL;

//                con32(((char*)layer_screen+t*320*lsize*4),((char*)dest+t*320*lsize*4),lsize*320);
//                con32(((char*)layer_screen+t*320*lsize*4+320*lsize*2),((char*)dest+t*320*lsize*4+320*lsize*2),lsize*320/2);
//                con32(((char*)layer_screen+t*320*lsize*4),((char*)dest+t*320*lsize*4),lsize*320/2);
                con32(((char*)layer_temp+320*lsize*2),((char*)dest+t*320*lsize*4+320*lsize*2),lsize*320/2);
                con32(((char*)layer_temp),((char*)dest+t*320*lsize*4),lsize*320/2);
        }

        layer_next=layer_heap;
}

void    layer_putchar(char *source, int c, int x, int y, char *dest, int mod)
{
        int     a,b;

        dest+=(x+y*mod)*4;

        if((c>='a')&&(c<='z'))source+=4*8*(c-'a');
        else
        if((c>='A')&&(c<='Z'))source+=4*8*(c-'A');
        else
        if((c>='0')&&(c<='9'))source+=4*8*(c-'0'+27);
        else
        if(c==':')source+=4*8*(26);
        else
        if(c=='.')source+=4*8*(37);
        else
        source+=4*8*(38);


        for(b=0;b<8;b++)
        for(a=0;a<8;a++)
        {
                ((int*)dest)[b*mod+a]=((int*)source)[b*320+a];
        }
}

void    layer_setfont(void *font)
{
        layer_font=font;
}

void    layer_clear(layer l)
{
        int     t;

        for(t=0;t<l.sizex*l.sizey;t++)((int*)l.source)[t]=0;
}

layer   layer_printf(layer l, char *s)
{
        int     pos=0;

        layer_clear(l);

        while(*s!=0)
        {
                layer_putchar(layer_font,*s,pos,0,l.source,l.sizex);
                pos+=8;
                s++;
        }

        return l;
}

layer   layer_hblur(layer l, float n)
{
        rgb1    *s,*d;
        float   g1;
        int     m;
        float   n2;
        int     x,y;
        int     r,g,b,a;
        float   mul;

        mul=1/n;
        n2=(n+1)/2;
        m=n2;
        g1=n2-m;

        s=l.source;
        d=dest;

        layer_hblurasm(s,d,l.sizey,l.sizex,m,(int)(256*g1),(float)((int)(256*mul))/256.0);

//         layer_hblurasm(s,d,l.sizey,l.sizex,1,0,1 );

/*        for(y=0;y<l.sizey;y++)
        {
                r=0;
                g=0;
                b=0;
                a=0;
                for(x=0;x<m;x++)
                {
                        temp[x].r=r;
                        temp[x].g=g;
                        temp[x].b=b;
                        temp[x].a=a;
                }
                for(x=m;x<l.sizex;x++)
                {
                        r=r-s[x-m].r;
                        temp[x].r=r+g1*s[x-m].r;
                        g=g-s[x-m].g;
                        temp[x].g=g+g1*s[x-m].g;
                        b=b-s[x-m].b;
                        temp[x].b=b+g1*s[x-m].b;
                        a=a-s[x-m].o;
                        temp[x].a=a+g1*s[x-m].o;
                }

                r=0;
                g=0;
                b=0;
                a=0;
                for(x=0;x<m;x++)
                {
                        r+=s[x].r;
                        g+=s[x].g;
                        b+=s[x].b;
                        a+=s[x].o;
                }
                for(x=0;x<l.sizex-m;x++)
                {
                        temp[x].r+=r+g1*s[x+m].r;
                        r+=s[x+m].r;
                        temp[x].g+=g+g1*s[x+m].g;
                        g+=s[x+m].g;
                        temp[x].b+=b+g1*s[x+m].b;
                        b+=s[x+m].b;
                        temp[x].a+=a+g1*s[x+m].o;
                        a+=s[x+m].o;
                }
                for(x=l.sizex-m;x<l.sizex;x++)
                {
                        temp[x].r+=r;
                        temp[x].g+=g;
                        temp[x].b+=b;
                        temp[x].a+=a;
                }

                for(x=0;x<l.sizex;x++)
                {
                        d[x].r=temp[x].r*mul;
                        d[x].g=temp[x].g*mul;
                        d[x].b=temp[x].b*mul;
                        d[x].o=temp[x].a*mul;
                }
                s+=l.sizex;
                d+=l.sizex;
        }
*/
        l.source=dest;

        return l;
}

layer   layer_hblurfast(layer l, float n)
{
        rgb1    *s,*d;
        int     r,g,b,a,t;
        float   mul;
        char    multab1[256];
        char    multab2[256];

        mul=1/n;

        s=l.source;
        d=dest;

        l.source=dest;

        layer_hblurasmfast(s,d,l.sizex*l.sizey,mul*256);
/*
        for(t=0;t<256;t++)
        {
                multab1[t]=t*mul;
                multab2[t]=t-t*mul;
        }

        r=g=b=a=0;

        for(t=0;t<l.sizex*l.sizey;t++)
        {
                r=multab2[r]+multab1[s[t].r];
                g=multab2[g]+multab1[s[t].g];
                b=multab2[b]+multab1[s[t].b];
                a=multab2[a]+multab1[s[t].o];


                d[t].r=r;
                d[t].g=g;
                d[t].b=b;
                d[t].o=a;
        }
*/
        return l;

}

