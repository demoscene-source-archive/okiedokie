float points[4024*3];
float points2[4024*3];
int faces[4024*3];
int uvcoords[4024*3];
int normals[4024*3];
float fnormals[4024*3];
float fnormalsrot[4024*3];
int faketabs[4000*3];
int faketabscount[4000*3];
int polyz[4000];
int pind[4000];
int pos,size,zpos,framenum;
int totalcount,counter;

typedef struct {
    float x,y,z;
} vector;

typedef struct {
    vector position,target;
    float roll;
} camera3dstype;

typedef struct{
    float x,y,z;
    float px,py;
    int cb;
    float u,v;
    float r,g,b;
}RotatedVertice;

typedef struct{
    RotatedVertice *v1, *v2, *v3;
    int u,v;
    unsigned texturetype;
    int polytype;
    int color;
    char clip;
}FacelistPL;

camera3dstype camera1_3ds;


int drawpolytype,polycolor;

FacelistPL *face2;
RotatedVertice rotatelist[5347*4];

int numfaces,numpoints,i;

float rotmat[4][4];
float torusrotmat[4][4];
float tempmat[4][4];
float cammat[4][4];
float phix,phiy,phiz;

void createrotatematrix(float rotmat[4][4],float phix,float phiy,float phiz)
{
	float rotx[4][4];
	float roty[4][4];
	float rotz[4][4];
	int i,j;

	for (i = 0; i < 4; i++) 					// clear rotation matrices
		for (j = 0; j < 4; j++) {rotx[i][j]=0;roty[i][j]=0;rotz[i][j]=0;rotmat[i][j]=0;}
	for (i = 0; i < 4; i++) rotmat[i][i]=1; 			 //create indentify matrix
	rotx[0][0]=1;
	rotx[3][3]=1;
    rotx[1][1]=fcos(phix);                 //create x-rotation matrix
    rotx[2][1]=fsin(phix);
    rotx[1][2]=-fsin(phix);
    rotx[2][2]=fcos(phix);
	roty[1][1]=1;
	roty[3][3]=1;
    roty[0][0]=fcos(phiy);                //create y-rotation matrix
    roty[0][2]=fsin(phiy);
    roty[2][0]=-fsin(phiy);
    roty[2][2]=fcos(phiy);
	rotz[2][2]=1;
	rotz[3][3]=1;
    rotz[0][0]=fcos(phiz);
    rotz[1][0]=fsin(phiz);                //create z-rotation matrix
    rotz[0][1]=-fsin(phiz);
    rotz[1][1]=fcos(phiz);
	matmult(rotmat,rotx);		  //create final rotation matrix
	matmult(rotmat,roty);		  //create final rotation matrix
	matmult(rotmat,rotz);		  //create final rotation matrix
}

//===========================================================================
//===========================================================================

#define BACK 0x01
#define UP	 0x02
#define DOWN 0x04
#define RIGHT 0x08
#define LEFT 0x10
#define FRONT 0x20

void cv(RotatedVertice * v){
        if(v->z>ZMAXTEST) v->cb=FRONT; else v->cb=0;
        if(v->z<=0){
            v->cb|=BACK;
            if(v->x>=0){
                v->cb|=RIGHT;
                if(v->x<-v->z) v->cb|=LEFT;
            } else{
                v->cb|=LEFT;
                if(v->x>v->z) v->cb|=RIGHT;
            }

            if(v->y>=0){
                v->cb|=UP;
                if(v->y<-v->z) v->cb|=DOWN;
            } else{
                v->cb|=DOWN;
                if(v->y>v->z) v->cb|=UP;
            }
        }else{
            if(v->x>0){
                if(v->x>=v->z)v->cb|=RIGHT;
            } else{
                if(-v->x>=v->z)v->cb|=LEFT;
            }

            if(v->y>0){
                if(v->y>=v->z)v->cb|=UP;
            } else{
                if(-v->y>=v->z)v->cb|=DOWN;
            }
        }
}

#include "clip.c"
//#include "text0019.gen"
//include "space23.inc"
//#include "complex2.inc"
//#include "ball3.inc"
//#include "teapot.inc"

void matmult(float a[4][4], float b[4][4]) // matrix multiplication rutine
{
	float temp[4][4];		// temporary matrix for storing result
	int i, j;

	for (j = 0; j < 4; j++) 		// columns
		for (i = 0; i < 4; i++) 	// rows
			temp[i][j] = a[i][0] * b[0][j] + a[i][1] * b[1][j] +
						 a[i][2] * b[2][j] + a[i][3] * b[3][j];


	for (i = 0; i < 4; i++) 		// copy result matrix into matrix a
		for (j = 0; j < 4; j++)
			a[i][j] = temp[i][j];
}

void cameralookat(float camera[4][4], vector pos, vector target,float rollangle)
{
    float rotx[4][4];
    float roty[4][4];
    float rotz[4][4];
    float rotmat[4][4];

    float x,y,dp,length;
    int i,j;
    vector A,B,C,Up,Forward,Right;

    for (i = 0; i < 4; i++)                     // clear rotation matrices
        for (j = 0; j < 4; j++) {rotmat[i][j]=0;rotz[i][j]=0;}
    for (i = 0; i < 4; i++) rotmat[i][i]=1;              //create indentify matrix

    A.x=pos.x-target.x;
    A.y=pos.y-target.y;
    A.z=pos.z-target.z;
    B.x=A.x*A.x;
    B.y=A.y*A.y;
    B.z=A.z*A.z;                        //     Normalize forward vector
    length=fsqrt(B.x+B.y+B.z);
//    if (length < 1.0e-6) { printf("Eye colocated with point of interest");getch();}
    Forward.x=A.x/length;
    Forward.y=A.y/length;
    Forward.z=A.z/length;
    Up.x=0;
    Up.y=1;
    Up.z=0;
    dp = (Up.x*Forward.x) + (Up.y*Forward.y) + (Up.z*Forward.z);
    Up.x = Up.x - dp*Forward.x;
    Up.y = Up.y - dp*Forward.y;
    Up.z = Up.z - dp*Forward.z;

    B.x=Up.x*Up.x;
    B.y=Up.y*Up.y;
    B.z=Up.z*Up.z;                        //     Normalize Up vector
    length=fsqrt(B.x+B.y+B.z);
//    if (length < 1.0e-6) { printf("Eye colocated with point of interest");getch();}
    Up.x=Up.x/length;
    Up.y=Up.y/length;
    Up.z=Up.z/length;
    Right.x=(Forward.y*Up.z)-(Forward.z*Up.y);
    Right.y=(Forward.z*Up.x)-(Forward.x*Up.z);    //crossproduct of forward- and up-vector
    Right.z=(Forward.x*Up.y)-(Forward.y*Up.x);
    Up.x=(Right.y*Forward.z)-(Right.z*Forward.y);
    Up.y=(Right.z*Forward.x)-(Right.x*Forward.z);    //crossproduct
    Up.z=(Right.x*Forward.y)-(Right.y*Forward.x);
    B.x=Up.x*Up.x;
    B.y=Up.y*Up.y;
    B.z=Up.z*Up.z;                        //     Normalize Up vector
    length=fsqrt(B.x+B.y+B.z);
//    if (length < 1.0e-6) { printf("Eye colocated with point of interest");getch();}
    Up.x=Up.x/length;
    Up.y=Up.y/length;
    Up.z=Up.z/length;

    rotmat[0][0]=Right.x;
    rotmat[1][0]=Right.y;
    rotmat[2][0]=Right.z;
    rotmat[0][1]=Up.x;
    rotmat[1][1]=Up.y;
    rotmat[2][1]=Up.z;
    rotmat[0][2]=Forward.x;
    rotmat[1][2]=Forward.y;
    rotmat[2][2]=Forward.z;
    rotz[2][2]=1;
    rotz[3][3]=1;
    rotz[0][0]=fcos(rollangle);
    rotz[1][0]=fsin(rollangle);                //create rollangle matrix
    rotz[0][1]=-fsin(rollangle);
    rotz[1][1]=fcos(rollangle);
//    matmult(rotmat,rotz);                    // multiply rollanglematrix and camerarotationmatrix
    for (j = 0; j < 4; j++)         // columns
		for (i = 0; i < 4; i++) 	// rows
            camera[i][j] = (rotmat[i][0] * rotz[0][j] + rotmat[i][1] * rotz[1][j] +
                         rotmat[i][2] * rotz[2][j] + rotmat[i][3] * rotz[3][j]*1.0);
    camera[3][0]=-camera1_3ds.position.x;  //add cameraposition to matrix
    camera[3][1]=-camera1_3ds.position.y;
    camera[3][2]=-camera1_3ds.position.z;  //create the final rotation matrix
}


void sort(int l,int r)
{
 int i,j,x,y,f1,a=0,b;
 i=l; j=r; x=polyz[(l+r)>>1];
 while(i<j)
 {
  while(polyz[i]<x) i++;
  while(x<polyz[j]) j--;
  if(i<=j) {
			y=polyz[i]; polyz[i]=polyz[j]; polyz[j]=y;
			y=pind[i]; pind[i]=pind[j]; pind[j]=y;
			i++; j--;
		   }
 }
 if(l<j) sort(l,j);
 if(i<r) sort(i,r);
}

void quicksort(int lo,int hi)
{
 sort(lo,hi);
}

void renderpoly(FacelistPL * face)
{
        TriangleTexture(face->v1, face->v2, face->v3, vdscr);

}

void triangleclip(FacelistPL * face){
float kudi;

    kudi=1/face->v1->z;
    face->v1->px=129+(face->v1->x*sizex*kudi);
    face->v1->py=100+(face->v1->y*sizey*kudi);
    kudi=1/face->v2->z;
    face->v2->px=129+(face->v2->x*sizex*kudi);
    face->v2->py=100+(face->v2->y*sizey*kudi);
    kudi=1/face->v3->z;
    face->v3->px=129+(face->v3->x*sizex*kudi);
    face->v3->py=100+(face->v3->y*sizey*kudi);
    renderpoly(face);
}

void rotateelement()
{   float ppx,ppy,ppz,x,y,z;
    int i;

    i=0;
	while (i<numpoints)
	{
      ppx=points2[i+i+i];
      ppy=points2[i+i+i+1];
      ppz=points2[i+i+i+2];
        rotatelist[i].x = (ppx*rotmat[0][0] +ppy*rotmat[1][0]+ppz*rotmat[2][0]);
        rotatelist[i].y = (ppx*rotmat[0][1] +ppy*rotmat[1][1]+ppz*rotmat[2][1]);
        rotatelist[i].z = ((ppx*rotmat[0][2]+ppy*rotmat[1][2]+ppz*rotmat[2][2]));
        points[i+i+i]  =rotatelist[i].x;
        points[i+i+i+1]=rotatelist[i].y;
        points[i+i+i+2]=rotatelist[i].z;
        rotatelist[i].z+=100;

        if(rotatelist[i].z>ZMAXTEST) rotatelist[i].cb=FRONT; else rotatelist[i].cb=0;
        if(rotatelist[i].z<=0){
            rotatelist[i].cb|=BACK;
            if(rotatelist[i].x>=0){
                rotatelist[i].cb|=RIGHT;
                if(rotatelist[i].x<-rotatelist[i].z) rotatelist[i].cb|=LEFT;
            } else{
                rotatelist[i].cb|=LEFT;
                if(rotatelist[i].x>rotatelist[i].z) rotatelist[i].cb|=RIGHT;
            }

            if(rotatelist[i].y>=0){
                rotatelist[i].cb|=UP;
                if(rotatelist[i].y<-rotatelist[i].z) rotatelist[i].cb|=DOWN;
            } else{
                rotatelist[i].cb|=DOWN;
                if(rotatelist[i].y>rotatelist[i].z) rotatelist[i].cb|=UP;
            }
        }else{
            if(rotatelist[i].x>0){
                if(rotatelist[i].x>=rotatelist[i].z)rotatelist[i].cb|=RIGHT;
            } else{
                if(-rotatelist[i].x>=rotatelist[i].z)rotatelist[i].cb|=LEFT;
            }

            if(rotatelist[i].y>0){
                if(rotatelist[i].y>=rotatelist[i].z)rotatelist[i].cb|=UP;
            } else{
                if(-rotatelist[i].y>=rotatelist[i].z)rotatelist[i].cb|=DOWN;
            }
        }
        if(!rotatelist[i].cb){
//        if (z==0) z+=0.1;

          rotatelist[i].px=159+(rotatelist[i].x*sizex)/(rotatelist[i].z);
          rotatelist[i].py=100+(rotatelist[i].y*sizey)/(rotatelist[i].z);
        }
		i++;
	}
}

void rotateobject()
{   float ppx,ppy,ppz,x,y,z;
    int i;

    i=0;
	while (i<numpoints)
	{
      ppx=points2[i+i+i];
      ppy=points2[i+i+i+1];
      ppz=points2[i+i+i+2];
        rotatelist[i].x = (ppx*rotmat[0][0] +ppy*rotmat[1][0]+ppz*rotmat[2][0]);
        rotatelist[i].y = (ppx*rotmat[0][1] +ppy*rotmat[1][1]+ppz*rotmat[2][1]);
        rotatelist[i].z = ((ppx*rotmat[0][2]+ppy*rotmat[1][2]+ppz*rotmat[2][2]));
      ppx=fnormals[i+i+i];
      ppy=fnormals[i+i+i+1];
      ppz=fnormals[i+i+i+2];
        fnormalsrot[i*3+0] = (ppx*rotmat[0][0] +ppy*rotmat[1][0]+ppz*rotmat[2][0]);
        fnormalsrot[i*3+1] = (ppx*rotmat[0][1] +ppy*rotmat[1][1]+ppz*rotmat[2][1]);
        fnormalsrot[i*3+2] = ((ppx*rotmat[0][2]+ppy*rotmat[1][2]+ppz*rotmat[2][2]));
//        rotatelist[i].x=ppx;
//        rotatelist[i].y=ppy;
//        rotatelist[i].z=ppz;
        points[i+i+i]  =rotatelist[i].x;
        points[i+i+i+1]=rotatelist[i].y;
        points[i+i+i+2]=rotatelist[i].z;
        rotatelist[i].z+=camera1_3ds.position.z;

        if(rotatelist[i].z>ZMAXTEST) rotatelist[i].cb=FRONT; else rotatelist[i].cb=0;
        if(rotatelist[i].z<=0){
            rotatelist[i].cb|=BACK;
            if(rotatelist[i].x>=0){
                rotatelist[i].cb|=RIGHT;
                if(rotatelist[i].x<-rotatelist[i].z) rotatelist[i].cb|=LEFT;
            } else{
                rotatelist[i].cb|=LEFT;
                if(rotatelist[i].x>rotatelist[i].z) rotatelist[i].cb|=RIGHT;
            }

            if(rotatelist[i].y>=0){
                rotatelist[i].cb|=UP;
                if(rotatelist[i].y<-rotatelist[i].z) rotatelist[i].cb|=DOWN;
            } else{
                rotatelist[i].cb|=DOWN;
                if(rotatelist[i].y>rotatelist[i].z) rotatelist[i].cb|=UP;
            }
        }else{
            if(rotatelist[i].x>0){
                if(rotatelist[i].x>=rotatelist[i].z)rotatelist[i].cb|=RIGHT;
            } else{
                if(-rotatelist[i].x>=rotatelist[i].z)rotatelist[i].cb|=LEFT;
            }

            if(rotatelist[i].y>0){
                if(rotatelist[i].y>=rotatelist[i].z)rotatelist[i].cb|=UP;
            } else{
                if(-rotatelist[i].y>=rotatelist[i].z)rotatelist[i].cb|=DOWN;
            }
        }
        if(!rotatelist[i].cb){
//        if (z==0) z+=0.1;

          rotatelist[i].px=129+(rotatelist[i].x*sizex)/(rotatelist[i].z);
          rotatelist[i].py=100+(rotatelist[i].y*sizey)/(rotatelist[i].z);
        }
		i++;
	}
}

void cameraobject()
{   float ppx,ppy,ppz,x,y,z;
    int i;

    i=0;
	while (i<numpoints)
	{
      ppx=points2[i+i+i]  -rotmat[3][0];
      ppy=points2[i+i+i+1]-rotmat[3][1];
      ppz=points2[i+i+i+2]-rotmat[3][2];
        rotatelist[i].x = (ppx*rotmat[0][0] +ppy*rotmat[1][0]+ppz*rotmat[2][0]);
        rotatelist[i].y = (ppx*rotmat[0][1] +ppy*rotmat[1][1]+ppz*rotmat[2][1]);
        rotatelist[i].z = ((ppx*rotmat[0][2]+ppy*rotmat[1][2]+ppz*rotmat[2][2]));
/*      ppx=fnormals[i+i+i];
      ppy=fnormals[i+i+i+1];
      ppz=fnormals[i+i+i+2];
        fnormalsrot[i*3+0] = (ppx*rotmat[0][0] +ppy*rotmat[1][0]+ppz*rotmat[2][0]);
        fnormalsrot[i*3+1] = (ppx*rotmat[0][1] +ppy*rotmat[1][1]+ppz*rotmat[2][1]);
        fnormalsrot[i*3+2] = ((ppx*rotmat[0][2]+ppy*rotmat[1][2]+ppz*rotmat[2][2]));*/
//        rotatelist[i].x=ppx;
//        rotatelist[i].y=ppy;
//        rotatelist[i].z=ppz;
        points[i+i+i]  =rotatelist[i].x;
        points[i+i+i+1]=rotatelist[i].y;
        points[i+i+i+2]=rotatelist[i].z;
//        rotatelist[i].z+=500;

        if(rotatelist[i].z>ZMAXTEST) rotatelist[i].cb=FRONT; else rotatelist[i].cb=0;
        if(rotatelist[i].z<=0){
            rotatelist[i].cb|=BACK;
            if(rotatelist[i].x>=0){
                rotatelist[i].cb|=RIGHT;
                if(rotatelist[i].x<-rotatelist[i].z) rotatelist[i].cb|=LEFT;
            } else{
                rotatelist[i].cb|=LEFT;
                if(rotatelist[i].x>rotatelist[i].z) rotatelist[i].cb|=RIGHT;
            }

            if(rotatelist[i].y>=0){
                rotatelist[i].cb|=UP;
                if(rotatelist[i].y<-rotatelist[i].z) rotatelist[i].cb|=DOWN;
            } else{
                rotatelist[i].cb|=DOWN;
                if(rotatelist[i].y>rotatelist[i].z) rotatelist[i].cb|=UP;
            }
        }else{
            if(rotatelist[i].x>0){
                if(rotatelist[i].x>=rotatelist[i].z)rotatelist[i].cb|=RIGHT;
            } else{
                if(-rotatelist[i].x>=rotatelist[i].z)rotatelist[i].cb|=LEFT;
            }

            if(rotatelist[i].y>0){
                if(rotatelist[i].y>=rotatelist[i].z)rotatelist[i].cb|=UP;
            } else{
                if(-rotatelist[i].y>=rotatelist[i].z)rotatelist[i].cb|=DOWN;
            }
        }
        if(!rotatelist[i].cb){
//        if (z==0) z+=0.1;
          rotatelist[i].px=159+(rotatelist[i].x*sizex)/(rotatelist[i].z);
          rotatelist[i].py=100+(rotatelist[i].y*sizey)/(rotatelist[i].z);
        }
		i++;
	}
}
void fastcalcnormals()
{
int i;
double kx,ky,kz,ax,ay,az,bx,by,bz;
double length,te;
double tmpx,tmpy,tmpz;

  memset(faketabscount,0,numpoints*4);
  memset(faketabs,0,numpoints*3*4);
  for (i=0;i<numfaces;i++)
  {
    ax=points[faces[i*3+1]*3]-points[faces[i*3]*3];
    ay=points[faces[i*3+1]*3+1]-points[faces[i*3]*3+1];
    az=points[faces[i*3+1]*3+2]-points[faces[i*3]*3+2];
    bx=points[faces[i*3+2]*3]-points[faces[i*3]*3];
    by=points[faces[i*3+2]*3+1]-points[faces[i*3]*3+1];
    bz=points[faces[i*3+2]*3+2]-points[faces[i*3]*3+2];
    kx=(ay*bz)-(az*by);
    ky=(az*bx)-(ax*bz);
    kz=(ax*by)-(ay*bx);
    tmpx=(double)(kx*kx);
    tmpy=(double)(ky*ky);
    tmpz=(double)(kz*kz);
    te=tmpx+tmpy+tmpz;
    length=fsqrt(te);
    normals[i*3]=((kx/length)*1024);
    normals[i*3+1]=((ky/length)*1024);
    normals[i*3+2]=((kz/length)*1024);
    faketabs[faces[i*3+0]*3]+=normals[i*3];
    faketabs[faces[i*3+0]*3+1]+=normals[i*3+1];
    faketabs[faces[i*3+0]*3+2]+=normals[i*3+2];
    faketabscount[faces[i*3+0]]+=1;
    faketabs[faces[i*3+1]*3]+=normals[i*3];
    faketabs[faces[i*3+1]*3+1]+=normals[i*3+1];
    faketabs[faces[i*3+1]*3+2]+=normals[i*3+2];
    faketabscount[faces[i*3+1]]+=1;
    faketabs[faces[i*3+2]*3]+=normals[i*3];
    faketabs[faces[i*3+2]*3+1]+=normals[i*3+1];
    faketabs[faces[i*3+2]*3+2]+=normals[i*3+2];
    faketabscount[faces[i*3+2]]+=1;
	}
}

void fastcalcfakenormals()
{
   int num,i,a;
   float kx,ky,kz,ax,ay,az,bx,by,bz;

  for (i=0;i<numpoints;i++)
	{
    num=faketabscount[i];
    if (num==0) num=1;
    ax=faketabs[i*3];
    ay=faketabs[i*3+1];
    az=faketabs[i*3+2];
    ax=ax/num;
	ay=ay/num;
	az=az/num;
	fnormals[i*3]=ax;
    fnormals[i*3+1]=ay;
    fnormals[i*3+2]=az;
  }
}

