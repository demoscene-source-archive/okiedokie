/* keyframer for vector engine by craft / fudge */

#include "structs.h"
#include <stdlib.h>
#include "file3ds.h"
#include "filer.h"
#include "engine.h"
#include "spline.h"
#include "qspline.h"
#include "keyframe.h"

void    keyframe_error(char *a, char *b)
{
        printf("keyframe error:");
        printf(a,b);
        exit(1);
}

void    keyframe_privatereadpositiontrack(FFILE* fil,keyframe_multispline *ms, int rel)
{       // loaderen er lige n†et til chunk B020, og har l‘st dette (ikke hele chunket, men kun hovedet)
        // rel afg›r om resten af dataene er relative til de f›rste
        int     t,i,time;
        short   flag;
        float   px,py,pz;

        ms->x=spline_new();
        ms->y=spline_new();
        ms->z=spline_new();

        for(t=0;t<10;t++)ds_readbyte(fil);

        i=ds_readdword(fil);
        for(t=0;t<i;t++)
        {
                time=ds_readdword(fil);
//                printf("keyframetime;%i\n",time);
                flag=ds_readword(fil);
                if((flag&1)==1)ds_readdword(fil);
                if((flag&2)==2)ds_readdword(fil);
                if((flag&4)==4)ds_readdword(fil);
                if((flag&8)==8)ds_readdword(fil);
                if((flag&16)==16)ds_readdword(fil);

                if(rel==0)
                {
                        float   x,y,z;
                        spline_addkey(&(ms->x),time,x=ds_readfloats(fil));
                        spline_addkey(&(ms->z),time,z=ds_readfloats(fil));
                        spline_addkey(&(ms->y),time,y=-ds_readfloats(fil));
//                        printf("%i %f %f %f\n",time,x,y,z);
                }
                else if(rel==1)
                {
                        rel=2;
                        spline_addkey(&(ms->x),time,0);
                        spline_addkey(&(ms->y),time,0);
                        spline_addkey(&(ms->z),time,0);
                        px=ds_readfloats(fil);
                        pz=ds_readfloats(fil);
                        py=-ds_readfloats(fil);
                }
                else if(rel==2)
                {
                        spline_addkey(&(ms->x),time,ds_readfloats(fil)-px);
                        spline_addkey(&(ms->z),time,ds_readfloats(fil)-pz);
                        spline_addkey(&(ms->y),time,-ds_readfloats(fil)-py);
                }
        }
}
/*
{       // loaderen er lige n†et til chunk B020, og har l‘st dette (ikke hele chunket, men kun hovedet)
        // rel afg›r om resten af dataene er relative til de f›rste
        int     t,i,time;
        short   flag;
        float   px,py,pz;

        ms->x=spline_new();
        ms->y=spline_new();
        ms->z=spline_new();

        for(t=0;t<10;t++)ds_readbyte(fil);

        i=ds_readdword(fil);
        for(t=0;t<i;t++)
        {
                time=ds_readdword(fil);
                flag=ds_readword(fil);
                if((flag&1)==1)ds_readdword(fil);
                if((flag&2)==2)ds_readdword(fil);
                if((flag&4)==4)ds_readdword(fil);
                if((flag&8)==8)ds_readdword(fil);
                if((flag&16)==16)ds_readdword(fil);

                if(rel==0)
                {
                        float   x,y,z;
                        spline_addkey(&(ms->x),time,x=ds_readfloats(fil));
                        spline_addkey(&(ms->y),time,y=ds_readfloats(fil));
                        spline_addkey(&(ms->z),time,z=ds_readfloats(fil));
//                        printf("%i %f %f %f\n",time,x,y,z);
                }
                else if(rel==1)
                {
                        rel=2;
                        spline_addkey(&(ms->x),time,0);
                        spline_addkey(&(ms->y),time,0);
                        spline_addkey(&(ms->z),time,0);
                        px=ds_readfloats(fil);
                        py=ds_readfloats(fil);
                        pz=ds_readfloats(fil);
                }
                else if(rel==2)
                {
                        spline_addkey(&(ms->x),time,ds_readfloats(fil)-px);
                        spline_addkey(&(ms->y),time,ds_readfloats(fil)-py);
                        spline_addkey(&(ms->z),time,ds_readfloats(fil)-pz);
                }
        }
}
  */


void    keyframe_animcam(b_camera* cam,float time)
{
        cam->camx=spline_value(cam->camspline.x,time);
        cam->camy=spline_value(cam->camspline.y,time);
        cam->camz=spline_value(cam->camspline.z,time);

        cam->tgtx=spline_value(cam->tgtspline.x,time);
        cam->tgty=spline_value(cam->tgtspline.y,time);
        cam->tgtz=spline_value(cam->tgtspline.z,time);

//        printf("Campos: %f %f %f %f %f %f\n",cam->camx,cam->camy,cam->camz,cam->tgtx,cam->tgty,cam->tgtz);
}

void    keyframe_animobjects(b_object* obj,float time)
{
        b_object        *startobj=obj;

        if(obj==NULL)return;
        do
        {
                if(obj->posspline.x)
                {
                        obj->x=spline_value(obj->posspline.x,time);
                        obj->y=spline_value(obj->posspline.y,time);
                        obj->z=spline_value(obj->posspline.z,time);
                }
                if(obj->qspline)
                {
                        obj->quat=qspline_value(obj->qspline,time);
                }
                obj=obj->next;
        }
        while(obj!=startobj);

}


void    keyframe_loadcameraanim(char *filename, char *cameraname, b_camera* cam)
{
        b_object *object=NULL;
        FFILE    *file;
        ds_chunk   level0,level1,level2,level3,level4;
        char    objn[33];
        int     this;



        file=ffopen(filename,"rb");
//        assert(file);
        if(file==NULL)keyframe_error("unable to open file %s",filename);


        level0=ds_readchunk(file);
        while(ds_inchunk(level0))
	{
                level1=ds_readchunk(file);
                switch(level1.id){
                        case 0xb000:
                        while(ds_inchunk(level1))
			{
                                level2=ds_readchunk(file);
                                switch(level2.id){
                                        case 0xb004:
                                        {

                                                this=0;
                                                while(ds_inchunk(level2))
                                                {
                                                        level3=ds_readchunk(file);
                                                        switch(level3.id){
                                                                case 0xb010:
                                                                {       //Info
                                                                        ds_readname(file,objn);
                                                                        //printf("Info:%s\n",objn);
                                                                        this=ds_cmpnames(cameraname,objn);
                                                                        //printf("camerafound1!!! %i\n",this);
                                                                        break;
                                                                }
                                                                case 0xb020:
                                                                {
                                                                        //printf("camerafound2!!! %i\n",this);
                                                                        //printf("Reading camtgt data\n");getch();
                                                                        if(this)
                                                                        keyframe_privatereadpositiontrack(file,&cam->tgtspline,0);

                                                                        break;
                                                                }
                                                        }
                                                        ds_endchunk(file,level3);
                                                }
                                                break;
                                        }
                                        case 0xb003:
                                        {

                                                this=0;
                                                while(ds_inchunk(level2))
                                                {
                                                        level3=ds_readchunk(file);
                                                        switch(level3.id){
                                                                case 0xb010:
                                                                {       //Info
                                                                        ds_readname(file,objn);
                                                                        //printf("Info:%s\n",objn);
                                                                        this=ds_cmpnames(cameraname,objn);
                                                                        break;
                                                                }
                                                                case 0xb020:
                                                                {
                                                                        if(this)
                                                                        keyframe_privatereadpositiontrack(file,&cam->camspline,0);

                                                                        break;
                                                                }
                                                                case 0xb024:
                                                                {
                                                               //         keyframe_privatereadrolltrack(file,&(tempspline),0);

                                                              //printf("Roll chunk\n");getch();
                                                                     break;
                                                                }
                                                        }
                                                        ds_endchunk(file,level3);
                                                }
                                                break;
                                        }
                                        break;
                                }
                                ds_endchunk(file,level2);
                        }
                        break;
                }
                ds_endchunk(file,level1);
        }

        ffclose(file);

//        return object;
}

void    keyframe_loadanim(char *filename, b_object* root)
{
        b_object *object=NULL;
        FFILE    *file;
        ds_chunk   level0,level1,level2,level3,level4;
        char    objn[33];
        int     this;
        int     nr=0;
        keyframe_multispline posspline;
        qspline_key *qspline;
        float   px,py,pz;

        posspline.x=NULL;
        posspline.y=NULL;
        posspline.z=NULL;

        file=ffopen(filename,"rb");
//        assert(file);
        if(file==NULL)keyframe_error("unable to open file %s",filename);


        level0=ds_readchunk(file);
        while(ds_inchunk(level0))
	{
                level1=ds_readchunk(file);
                switch(level1.id){
                        case 0xb000:
                        while(ds_inchunk(level1))
			{
                                level2=ds_readchunk(file);
                                switch(level2.id){
                                        case 0xb002:
                                        {
                                                int     found=0;
                                                //notfound=1;
                                                //printf("Keyframer found object\n");
                                                //(*prevnext)=objinfo=(void*)vmem_allocblock(&keyframe_objinfoblock);
                                                //keyframe_initoo(&(objinfo->oodata));
                                                //objinfo->rotx=spline_new();
                                                //objinfo->roty=spline_new();
                                                //objinfo->rotz=spline_new();
                                                //prevnext=&((*prevnext)->next);
                                                //(*prevnext)=NULL;
                                                //px=py=pz=0;

                                                while(ds_inchunk(level2))
                                                {
                                                        level3=ds_readchunk(file);
                                                        switch(level3.id){
                                                                case 0xb010:
                                                                {
                                                                        ds_readname(file,objn);
                                                                        object=b_findobject(root,objn);
                                                                        if(object==NULL)b_error("Fatal keyframer-error: ""object not present in scene!"" \n");
                                                                        found=1;
                                                                        //printf("KEYFRAMINg obj:%s\n",name);
                                                                        //getch();

                                                                        object->keyframenr=nr;
                                                                        {
                                                                                int     f,l;
                                                                                b_object *obj2;
                                                                                ds_readdword(file);
                                                                                f=ds_readsword(file);
                                                                                obj2=b_findobjectnr(root,f);
                                                                                b_removeobjecttree(object);
                                                                                b_insertobject(object,obj2,0);
                                                                                l=b_getrelativelevel(obj2,object);
                                                                                b_changeobjectlevelrelative(object,1-l);
                                                                        }
                                                                      //  notfound=0;
                                                                        nr++;
                                                                        break;
                                                                }
                                                                case 0xb013:
                                                                {   // Pivot point chunk
                                                                        px=ds_readfloats(file);
                                                                        pz=ds_readfloats(file);
                                                                        py=-ds_readfloats(file);
                                                                  //      printf("%f,%f,%f\n",px,py,pz);getch();


                                                                        break;
                                                                }
                                                                case 0xb020:
                                                                {
                                                                       // if(!found)b_error("Keyframer: Chunk b020 without chunk b010");

                                                                        keyframe_privatereadpositiontrack(file,&posspline,1);

                                                                        break;
                                                                }
                                                                case 0xb021:
                                                                {       // Rotation chunk
                                                                        int     t,n,flag,time;
                                                                        float   a,b,c,d;
                                                                        float lrotx=0,lroty=0,lrotz=0;
                                                                        quat    q;

                                                                        q=quat_makerotate(0,0,1,0);
                                                                        qspline=qspline_new();

                                                                    //    if(notfound)fdg_error("Fatal keyframer error. chunk b021 kom inden objektnavn");

                                                                        ds_readdword(file);
                                                                        ds_readdword(file);
                                                                        ds_readword(file);

                                                                        n=ds_readdword(file);
                                                                        for(t=0;t<n;t++)
                                                                        {
                                                                                float   x,y,z;
                                                                                float rotx,roty,rotz;
                                                                                time=ds_readdword(file);
                                                                                flag=ds_readword(file);
                                                                                if((flag&1)==1)ds_readdword(file);
                                                                                if((flag&2)==2)ds_readdword(file);
                                                                                if((flag&4)==4)ds_readdword(file);
                                                                                if((flag&8)==8)ds_readdword(file);
                                                                                if((flag&16)==16)ds_readdword(file);

                                                                                a=ds_readfloat(file);
                                                                                b=ds_readfloat(file);
                                                                                d=ds_readfloat(file);
                                                                                c=-ds_readfloat(file);
                                                                                if(t>0)
                                                                                {
                                                                                    //    rotatematrixf(&mat,b,c,d,-a);
                                                                                    quat_rotate(&q,b,c,d,-a);
                                                                                }
                                                                                qspline_addkey(&qspline,time,q);
                                                                        }
                                                                        break;
                                                                }
                                                        /*        case 0xb029:
                                                                {
                                                                        int     t;
                                                                        int     b;
                                                                        //printf("Hide track\n");
                                                                        for(t=0;t<10;t++)
                                                                        {
                                                                                bob_readbyte(fil);
                                                                        }
                                                                        t=bob_readdword(fil);
                                                                        b=0;
                                                                        while(t>0)
                                                                        {
                                                                                keyframe_addoo(&(objinfo->oodata),bob_readdword(fil),b);
                                                                                bob_readword(fil);
                                                                                b=1-b;
                                                                                t--;
                                                                        }

                                                                        break;
                                                                }*/
                                                        }
                                                        ds_endchunk(file,level3);
                                                }
                                                if(found)
                                                {
                                                        spline_ref(&posspline.x,&object->posspline.x);
                                                        spline_ref(&posspline.y,&object->posspline.y);
                                                        spline_ref(&posspline.z,&object->posspline.z);
                                                        qspline_ref(&qspline,&object->qspline);
                                                        object->pivotx=px+object->tm.xx;
                                                        object->pivoty=py+object->tm.yy;
                                                        object->pivotz=pz+object->tm.zz;
                                                }
                                           /*     if(objinfo->obj!=NULL)
                                                {
                                                        objinfo->obj->pivotx=objinfo->obj->tm.x+px;

                                                  //      printf("pivotx: %f\n",objinfo->obj->pivotx);
                                                        objinfo->obj->pivoty=objinfo->obj->tm.y+py;
                                                        objinfo->obj->pivotz=objinfo->obj->tm.z+pz;
                                                }     */
                                                break;
                                        }
                                        break;
                                }
                                ds_endchunk(file,level2);
                        }
                        break;
                }
                ds_endchunk(file,level1);
        }
        ffclose(file);

//        return object;
}

