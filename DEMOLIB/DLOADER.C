/* 3ds loader for vector engine by craft / fudge */

#include "file3ds.h"
#include "filer.h"
#include <assert.h>
#include <stdlib.h>
#include "engine.h"
#include "pcx.h"

th_texturehandler *dl_th;
b_texture dl_stdtexture;

void    dloader_error(char *a, char *b)
{
        printf("dloader error:");
        printf(a,b);
        exit(1);
}

void    dloader_settexturehandler(b_scene *sce, void *texture)
{
        dl_th=&sce->th;

        dl_stdtexture.texturetype=0;
        dl_stdtexture.location=texture;
}

void    dloader_settexturehandlerpcx(b_scene *sce, char *texname, int ttype)
{
        dl_th=&sce->th;

        dl_stdtexture.texturetype=ttype;
        dl_stdtexture.location=pcx_read_picture(texname,1+PCX_ADD2LINES);
        if(ttype==8)
        {
                th_transformtexture(dl_stdtexture.location,65536+512);
        }
}

b_object *loadobject(char *filename, char *objectname)
{
        b_object *object=NULL,*last=NULL;
        FFILE    *file;
        ds_chunk   level0,level1,level2,level3,level4,level5;
        char    objn[33];
        fmatrix tm;


        file=ffopen(filename,"rb");
//        assert(file);
        if(file==NULL)dloader_error("unable to open file %s",filename);


        level0=ds_readchunk(file);
        while(ds_inchunk(level0))
	{
                level1=ds_readchunk(file);
                switch(level1.id){
			case 0x3d3d:
                        while(ds_inchunk(level1))
			{
                                level2=ds_readchunk(file);
                                switch(level2.id){
					case 0x4000:
					{
						//printf("object chunk\n");
                                                ds_readname(file,objn);

                                                if(ds_cmpnames(objn,objectname))
						{
                                                        b_object        *theobject;
                                                        int             t,nver,nver2,nfac;
                                                        b_verlist       verlist,verdestlist;
                                                        b_facelist      facelist;
                                                        //printf("%s\n",objn);

                                                    //    theobjext=A_b_object
                                                        level3=ds_readchunk(file);

                                                        switch(level3.id)
                                                        {
                                                                case 0x4100:   //Trimesh

                                                                        while(ds_inchunk(level3))
                                                                        {
                                                                                level4=ds_readchunk(file);
                                                                                switch(level4.id)
                                                                                {       //antag at chunks kommer i r‘kkef›lge 4110 4140
                                                                                        case 0x4110:  //Vertexlist
                                                                                                nver=ds_readword(file);
                                                                                                verlist=A_b_verlist(nver);
                                                                                                verdestlist=A_b_verlist(nver);
                                                                                                for(t=0;t<nver;t++)
                                                                                                {
                                                                                                        verlist[t].x=ds_readfloats(file);
                                                                                                        verlist[t].z=ds_readfloats(file); //Her byttes om p† y og z
                                                                                                        verlist[t].y=-ds_readfloats(file);
                                                                                                        //printf("%f %f %f\n",verlist[t].x,verlist[t].y,verlist[t].z);
                                                                                                }
                                                                                                break;
                                                                                        case 0x4120: //Facelist
                                                                                                nfac=ds_readword(file);
                                                                                                //printf("%i\n",nfac);
                                                                                                facelist=A_b_facelist(nfac);
                                                                                                for(t=0;t<nfac;t++)
                                                                                                {
                                                                                                        facelist[t].a=verdestlist+ds_readword(file);
                                                                                                        facelist[t].b=verdestlist+ds_readword(file);
                                                                                                        facelist[t].c=verdestlist+ds_readword(file);
                                                                                                        facelist[t].texture=&dl_stdtexture;
                                                                                                        //printf("%i %i %i\n",facelist[t].a,facelist[t].b,facelist[t].c);
                                                                                                        ds_readword(file);
                                                                                                }
                                                                                                while(ds_inchunk(level4))
                                                                                                {
                                                                                                        level5=ds_readchunk(file);
                                                                                                        switch(level5.id)
                                                                                                        {
                                                                                                                case 0x4130: //materiallist
                                                                                                                        {
                                                                                                                                char      matn[20];
                                                                                                                                int       nummatfac,t,facnum;
                                                                                                                                b_texture *texture;

                                                                                                                                ds_readname(file,matn);
                                                                                                                                nummatfac=ds_readword(file);
                                                                                                                                texture=th_gettextureslot(dl_th,matn);
                                                                                                                                if(texture==0)texture=&dl_stdtexture;
                                                                                                                                for(t=0;t<nummatfac;t++)
                                                                                                                                {
                                                                                                                                        facnum=ds_readword(file);
                                                                                                                                        facelist[facnum].texture=texture;
                                                                                                                                }
                                                                                                                        }
                                                                                                                        break;
                                                                                                                case 0x4150:
                                                                                                                        //smoothinggroups
                                                                                                                        break;
                                                                                                        }
                                                                                                        ds_endchunk(file,level5);
                                                                                                }

                                                                                                break;
                                                                                        case 0x4140:  //mappingcoordinates
                                                                                                nver2=ds_readword(file);
                                                                                                //printf("mapping\n");
                                                                                                for(t=0;t<nver2;t++)
                                                                                                {
                                                                                                        verdestlist[t].x=ds_readfloat(file);
                                                                                                        verdestlist[t].y=-ds_readfloat(file);
                                                                                                }

                                                                                                break;
                                                                                        case 0x4160:  //Translation matrix
                                                                                        {
                                                                                                tm.xu=ds_readfloat(file);
                                                                                                tm.yu=ds_readfloat(file);
                                                                                                tm.zu=ds_readfloat(file);
                                                                                                tm.xv=ds_readfloat(file);
                                                                                                tm.yv=ds_readfloat(file);
                                                                                                tm.zv=ds_readfloat(file);
                                                                                                tm.xw=ds_readfloat(file);
                                                                                                tm.yw=ds_readfloat(file);
                                                                                                tm.zw=ds_readfloat(file);

                                                                                                tm.xx=ds_readfloats(file);
                                                                                                tm.zz=ds_readfloats(file);
                                                                                                tm.yy=-ds_readfloats(file);


                                                                                          //      tm.xx=3000;
                                                                                          //      tm.yy=300;
                                                                                          //      tm.zz=0;
                                                                                                break;
                                                                                        }
                                                                                }
                                                                                ds_endchunk(file,level4);
                                                                        }
                                                                        for(t=0;t<nfac;t++)
                                                                        {
                                                                                facelist[t].tx1=facelist[t].a->x;
                                                                                facelist[t].ty1=facelist[t].a->y;
                                                                                facelist[t].tx2=facelist[t].b->x;
                                                                                facelist[t].ty2=facelist[t].b->y;
                                                                                facelist[t].tx3=facelist[t].c->x;
                                                                                facelist[t].ty3=facelist[t].c->y;
                                                                        }
                                                                        if(object)
                                                                        {
                                                                                b_object *ltemp;
                                                                                b_insertobject(ltemp=A_b_object(objn,facelist,nfac,verlist,nver,verdestlist),last,0);
                                                                                b_initsimplenormals(ltemp);
                                                                                ltemp->tm=tm;
                                                                                last=ltemp;
                                                                        }
                                                                        else
                                                                        {
                                                                                object=A_b_object(objn,facelist,nfac,verlist,nver,verdestlist);
                                                                                b_initsimplenormals(object);
                                                                                object->tm=tm;
                                                                                last=object;
                                                                        }
                                                                        break;
                                                        }
                                                        ds_endchunk(file,level3);

                                                }






                                        }
                                        break;
                                }
                                ds_endchunk(file,level2);
                        }
                        break;
                }
                ds_endchunk(file,level1);
        }

        b_generatetinyfaceinfo(object);

        ffclose(file);

        return object;
}
