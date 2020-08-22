void clipright1(RotatedVertice *v1, RotatedVertice *v2, RotatedVertice *v3){
float dz,dx,a, b;
float cy;
float zl, zlt;
RotatedVertice pa,pb,pc;
FacelistPL cf;

					memcpy(&pc, v1, sizeof(pa));
					cf.v1=&pc;
					cf.v2=&pa;
					cf.v3=&pb;

	dz=v2->z-v1->z;
	dx=v2->x-v1->x;
    if(dx)a=dz/dx;else a=99911;
	b=v2->z-(a*v2->x);

	pa.z=pa.x=(b/(1-a));

	zl=v1->z-pa.z;
	zlt=(v2->z-v1->z);
        if(!zlt)zlt+=0.001;
    pa.y=(zl * (v1->y-v2->y))/zlt+v1->y;
    pa.r=(zl * (v1->r-v2->r))/zlt+v1->r;
    pa.g=(zl * (v1->g-v2->g))/zlt+v1->g;
    pa.b=(zl * (v1->b-v2->b))/zlt+v1->b;

    pa.u=(zl * (v1->u-v2->u))/zlt+v1->u;
    pa.v=(zl * (v1->v-v2->v))/zlt+v1->v;

    dz=v3->z-v1->z;
	dx=v3->x-v1->x;
    if(dx)a=dz/dx;else a=99911;
	b=v3->z-(a*v3->x);

	pb.z=pb.x=b/(1-a);

	zl=v1->z-pb.z;
	zlt=(v3->z-v1->z);
        if(!zlt)zlt+=0.001;
	pb.y=(zl * (v1->y-v3->y))/zlt+v1->y;
    pb.r=(zl * (v1->r-v3->r))/zlt+v1->r;
    pb.g=(zl * (v1->g-v3->g))/zlt+v1->g;
    pb.b=(zl * (v1->b-v3->b))/zlt+v1->b;

    pb.u=(zl * (v1->u-v3->u))/zlt+v1->u;
    pb.v=(zl * (v1->v-v3->v))/zlt+v1->v;

    cv(&pa);
    cv(&pb);
    clipleft(&cf);
};

void clipright2(RotatedVertice *v1, RotatedVertice *v2, RotatedVertice *v3){
float zl,zlt;
float dz,dx,a, b;
float cy;
RotatedVertice pa,pb,pc;
FacelistPL cf;

					memcpy(&pa, v2, sizeof(pa));
                    memcpy(&pb, v3, sizeof(pb));
					cf.v1=&pc;
					cf.v2=&pa; //v2
					cf.v3=&pb; //v3

	dz=v3->z-v1->z;
	dx=v3->x-v1->x;
    if(dx)a=dz/dx;else a=99911;
	b=v3->z-(a*v3->x);

	pc.z=pc.x=b/(1-a);

					zl=v1->z-pc.z;
                    zlt=(v3->z-v1->z);
        if(!zlt)zlt+=0.001;
	pc.y=(zl * (v1->y-v3->y))/zlt+v1->y;
    pc.r=(zl * (v1->r-v3->r))/zlt+v1->r;
    pc.g=(zl * (v1->g-v3->g))/zlt+v1->g;
    pc.b=(zl * (v1->b-v3->b))/zlt+v1->b;

    pc.u=(zl * (v1->u-v3->u))/zlt+v1->u;
    pc.v=(zl * (v1->v-v3->v))/zlt+v1->v;

    cv(&pc);
    clipleft(&cf);

	dz=v2->z-v1->z;
	dx=v2->x-v1->x;
    if(dx)a=dz/dx;else a=99911;
	b=v2->z-(a*v2->x);

	pb.z=pb.x=(b/(1-a));

					zl=v1->z-pb.z;
					zlt=(v2->z-v1->z);
        if(!zlt)zlt+=0.001;
	pb.y=(zl * (v1->y-v2->y))/zlt+v1->y;
    pb.r=(zl * (v1->r-v2->r))/zlt+v1->r;
    pb.g=(zl * (v1->g-v2->g))/zlt+v1->g;
    pb.b=(zl * (v1->b-v2->b))/zlt+v1->b;

    pb.u=(zl * (v1->u-v2->u))/zlt+v1->u;
    pb.v=(zl * (v1->v-v2->v))/zlt+v1->v;

    cv(&pb);
    clipleft(&cf);
};

void clipright(FacelistPL *face){
RotatedVertice *v1,*v2,*v3;
int c1,c2,c3;
				v1=face->v1;
				v2=face->v2;
				v3=face->v3;

                if(!(v1->cb&RIGHT) && !(v2->cb&RIGHT) && !(v3->cb&RIGHT))
                    clipleft(face);
                    else                 {

//printf("cr\n");

				if(v1->cb&RIGHT && v2->cb&RIGHT) clipright1(v3,v1,v2);
				else
				if(v2->cb&RIGHT && v3->cb&RIGHT) clipright1(v1,v2,v3);
				else
				if(v1->cb&RIGHT && v3->cb&RIGHT) clipright1(v2,v1,v3);
				else

				if(v1->cb&RIGHT) clipright2(v1,v2,v3);
				else
				if(v2->cb&RIGHT) clipright2(v2,v3,v1);
				else
				if(v3->cb&RIGHT) clipright2(v3,v1,v2);
                    }
}

void clipleft1(RotatedVertice *v1, RotatedVertice *v2, RotatedVertice *v3){
float dz,dx,a, b;
float cy;
float zl, zlt;
RotatedVertice pa,pb,pc;
FacelistPL cf;

					memcpy(&pc, v1, sizeof(pa));
					cf.v1=&pc;
					cf.v2=&pa;
					cf.v3=&pb;

	dz=v2->z-v1->z;
	dx=v2->x-v1->x;
    if(dx)a=dz/dx;else a=99911;
	b=v2->z-(a*v2->x);

	pa.z=(b/(1+a));
	pa.x=-pa.z;

	zl=v1->z-pa.z;
	zlt=(v2->z-v1->z);
        if(!zlt)zlt+=0.001;
    pa.y=(zl * (v1->y-v2->y))/zlt+v1->y;
    pa.r=(zl * (v1->r-v2->r))/zlt+v1->r;
    pa.g=(zl * (v1->g-v2->g))/zlt+v1->g;
    pa.b=(zl * (v1->b-v2->b))/zlt+v1->b;

    pa.u=(zl * (v1->u-v2->u))/zlt+v1->u;
    pa.v=(zl * (v1->v-v2->v))/zlt+v1->v;

	dz=v3->z-v1->z;
	dx=v3->x-v1->x;
    if(dx)a=dz/dx;else a=99911;
	b=v3->z-(a*v3->x);

	pb.z=b/(1+a);
	pb.x=-pb.z;

	zl=v1->z-pb.z;
	zlt=(v3->z-v1->z);
        if(!zlt)zlt+=0.001;
	pb.y=(zl * (v1->y-v3->y))/zlt+v1->y;
    pb.r=(zl * (v1->r-v3->r))/zlt+v1->r;
    pb.g=(zl * (v1->g-v3->g))/zlt+v1->g;
    pb.b=(zl * (v1->b-v3->b))/zlt+v1->b;

    pb.u=(zl * (v1->u-v3->u))/zlt+v1->u;
    pb.v=(zl * (v1->v-v3->v))/zlt+v1->v;
    cv(&pa);
    cv(&pb);
	clipup(&cf);
};

void clipleft2(RotatedVertice *v1, RotatedVertice *v2, RotatedVertice *v3){
float zl,zlt;
float dz,dx,a, b;
float cy;
RotatedVertice pa,pb,pc;
FacelistPL cf;
					memcpy(&pa, v2, sizeof(pa));
                    memcpy(&pb, v3, sizeof(pb));
					cf.v1=&pc;
					cf.v2=&pa; //v2
					cf.v3=&pb; //v3

	dz=v3->z-v1->z;
	dx=v3->x-v1->x;
    if(dx)a=dz/dx;else a=99911;
	b=v3->z-(a*v3->x);

	pc.z=b/(1+a);
	pc.x=-pc.z;
					zl=v1->z-pc.z;
					zlt=(v3->z-v1->z);
        if(!zlt)zlt+=0.001;
	pc.y=(zl * (v1->y-v3->y))/zlt+v1->y;
    pc.r=(zl * (v1->r-v3->r))/zlt+v1->r;
    pc.g=(zl * (v1->g-v3->g))/zlt+v1->g;
    pc.b=(zl * (v1->b-v3->b))/zlt+v1->b;

    pc.u=(zl * (v1->u-v3->u))/zlt+v1->u;
    pc.v=(zl * (v1->v-v3->v))/zlt+v1->v;

    cv(&pc);
	clipup(&cf);

					cf.v3=&pb;

	dz=v2->z-v1->z;
	dx=v2->x-v1->x;
    if(dx)a=dz/dx;else a=99911;
	b=v2->z-(a*v2->x);

	pb.z=(b/(1+a));
	pb.x=-pb.z;

					zl=v1->z-pb.z;
					zlt=(v2->z-v1->z);
        if(!zlt)zlt+=0.001;
	pb.y=(zl * (v1->y-v2->y))/zlt+v1->y;
    pb.r=(zl * (v1->r-v2->r))/zlt+v1->r;
    pb.g=(zl * (v1->g-v2->g))/zlt+v1->g;
    pb.b=(zl * (v1->b-v2->b))/zlt+v1->b;

    pb.u=(zl * (v1->u-v2->u))/zlt+v1->u;
    pb.v=(zl * (v1->v-v2->v))/zlt+v1->v;

    cv(&pb);
	clipup(&cf);
};

void clipleft(FacelistPL *face){
RotatedVertice *v1,*v2,*v3;
int c1,c2,c3;
int zl,zlt;
				v1=face->v1;
				v2=face->v2;
				v3=face->v3;

                if((v1->cb&LEFT) && (v2->cb&LEFT) && (v3->cb&LEFT))return;
                if(!(v1->cb&LEFT) && !(v2->cb&LEFT) && !(v3->cb&LEFT))
					clipup(face);
                    else                  {
//printf("cl\n");

				if(v1->cb&LEFT && v2->cb&LEFT) clipleft1(v3,v1,v2);
				else
				if(v2->cb&LEFT && v3->cb&LEFT) clipleft1(v1,v2,v3);
				else
				if(v1->cb&LEFT && v3->cb&LEFT) clipleft1(v2,v1,v3);
				else

				if(v1->cb&LEFT) clipleft2(v1,v2,v3);
				else
				if(v2->cb&LEFT) clipleft2(v2,v3,v1);
				else
				if(v3->cb&LEFT) clipleft2(v3,v1,v2);
                                        }
}


void clipup1(RotatedVertice *v1, RotatedVertice *v2, RotatedVertice *v3){
float dz,dy,a, b;
float cy;
float zl, zlt;
RotatedVertice pa,pb,pc;
FacelistPL cf;

                    memcpy(&pc, v1, sizeof(pc));
					cf.v1=&pc;
					cf.v2=&pa;
					cf.v3=&pb;

	dz=v2->z-v1->z;
	dy=v2->y-v1->y;
    if(dy)a=dz/dy;else a=99911;
	b=v2->z-(a*v2->y);

	pa.z=pa.y=(b/(1-a));

	zl=v1->z-pa.z;
	zlt=(v2->z-v1->z);
        if(!zlt)zlt+=0.001;
	pa.x=(zl * (v1->x-v2->x))/zlt+v1->x;
    pa.r=(zl * (v1->r-v2->r))/zlt+v1->r;
    pa.g=(zl * (v1->g-v2->g))/zlt+v1->g;
    pa.b=(zl * (v1->b-v2->b))/zlt+v1->b;

    pa.u=(zl * (v1->u-v2->u))/zlt+v1->u;
    pa.v=(zl * (v1->v-v2->v))/zlt+v1->v;

	dz=v3->z-v1->z;
    dy=v3->y-v1->y;
    if(dy)a=dz/dy;else a=99911;
	b=v3->z-(a*v3->y);

	pb.z=pb.y=b/(1-a);

	zl=v1->z-pb.z;
	zlt=(v3->z-v1->z);
        if(!zlt)zlt+=0.001;
	pb.x=(zl * (v1->x-v3->x))/zlt+v1->x;
    pb.r=(zl * (v1->r-v3->r))/zlt+v1->r;
    pb.g=(zl * (v1->g-v3->g))/zlt+v1->g;
    pb.b=(zl * (v1->b-v3->b))/zlt+v1->b;

    pb.u=(zl * (v1->u-v3->u))/zlt+v1->u;
    pb.v=(zl * (v1->v-v3->v))/zlt+v1->v;

    cv(&pa);
    cv(&pb);
	clipdown(&cf);
};

void clipup2(RotatedVertice *v1, RotatedVertice *v2, RotatedVertice *v3){
float zl,zlt;
float dz,dy,a, b;
float cy;
RotatedVertice pa,pb,pc;
FacelistPL cf;

					memcpy(&pa, v2, sizeof(pa));
					memcpy(&pb, v3, sizeof(pa));
					cf.v1=&pc;
					cf.v2=&pa; //v2
					cf.v3=&pb; //v3

	dz=v3->z-v1->z;
	dy=v3->y-v1->y;
    if(dy)a=dz/dy;else a=99911;
	b=v3->z-(a*v3->y);

	pc.z=pc.y=b/(1-a);

					zl=v1->z-pc.z;
					zlt=(v3->z-v1->z);
        if(!zlt)zlt+=0.001;

	pc.x=(zl * (v1->x-v3->x))/zlt+v1->x;
    pc.r=(zl * (v1->r-v3->r))/zlt+v1->r;
    pc.g=(zl * (v1->g-v3->g))/zlt+v1->g;
    pc.b=(zl * (v1->b-v3->b))/zlt+v1->b;

    pc.u=(zl * (v1->u-v3->u))/zlt+v1->u;
    pc.v=(zl * (v1->v-v3->v))/zlt+v1->v;

    cv(&pc);
	clipdown(&cf);

	dz=v2->z-v1->z;
	dy=v2->y-v1->y;
    if(dy)a=dz/dy;else a=99911;
	b=v2->z-(a*v2->y);

	pb.z=pb.y=(b/(1-a));

					zl=v1->z-pb.z;
					zlt=(v2->z-v1->z);
        if(!zlt)zlt+=0.001;

	pb.x=(zl * (v1->x-v2->x))/zlt+v1->x;
    pb.r=(zl * (v1->r-v2->r))/zlt+v1->r;
    pb.g=(zl * (v1->g-v2->g))/zlt+v1->g;
    pb.b=(zl * (v1->b-v2->b))/zlt+v1->b;

    pb.u=(zl * (v1->u-v2->u))/zlt+v1->u;
    pb.v=(zl * (v1->v-v2->v))/zlt+v1->v;

    cv(&pb);
	clipdown(&cf);
};





void clipup(FacelistPL *face){
RotatedVertice *v1,*v2,*v3;
int c1,c2,c3;
int zl,zlt;
				v1=face->v1;
				v2=face->v2;
				v3=face->v3;
                if((v1->cb&UP) && (v2->cb&UP) && (v3->cb&UP)) return;

                if(!(v1->cb&UP) && !(v2->cb&UP) && !(v3->cb&UP))
					clipdown(face);
                    else                {
//printf("cu\n");

				if(v1->cb&UP && v2->cb&UP) clipup1(v3,v1,v2);
				else
				if(v2->cb&UP && v3->cb&UP) clipup1(v1,v2,v3);
				else
				if(v1->cb&UP && v3->cb&UP) clipup1(v2,v1,v3);
				else

				if(v1->cb&UP) clipup2(v1,v2,v3);
				else
				if(v2->cb&UP) clipup2(v2,v3,v1);
				else
				if(v3->cb&UP) clipup2(v3,v1,v2);
//                else
//                printf("nitteu\n");
                }
}


void clipdown1(RotatedVertice *v1, RotatedVertice *v2, RotatedVertice *v3){
float dz,dy,a, b;
float cy;
float zl, zlt;
RotatedVertice pa,pb,pc;
FacelistPL cf;

                    memcpy(&pc, v1, sizeof(pc));
					cf.v1=&pc;
					cf.v2=&pa;
					cf.v3=&pb;

	dz=v2->z-v1->z;
	dy=v2->y-v1->y;
    if(dy)a=dz/dy;else a=99911;
	b=v2->z-(a*v2->y);

	pa.z=(b/(1+a));
	pa.y=-pa.z;

	zl=v1->z-pa.z;
	zlt=(v2->z-v1->z);
        if(!zlt)zlt+=0.001;
	pa.x=(zl * (v1->x-v2->x))/zlt+v1->x;
    pa.r=(zl * (v1->r-v2->r))/zlt+v1->r;
    pa.g=(zl * (v1->g-v2->g))/zlt+v1->g;
    pa.b=(zl * (v1->b-v2->b))/zlt+v1->b;

    pa.u=(zl * (v1->u-v2->u))/zlt+v1->u;
    pa.v=(zl * (v1->v-v2->v))/zlt+v1->v;

	dz=v3->z-v1->z;
	dy=v3->y-v1->y;
    if(dy)a=dz/dy;else a=99911;
	b=v3->z-(a*v3->y);

	pb.z=b/(1+a);
	pb.y=-pb.z;

	zl=v1->z-pb.z;
	zlt=(v3->z-v1->z);
        if(!zlt)zlt+=0.001;
	pb.x=(zl * (v1->x-v3->x))/zlt+v1->x;
    pb.r=(zl * (v1->r-v3->r))/zlt+v1->r;
    pb.g=(zl * (v1->g-v3->g))/zlt+v1->g;
    pb.b=(zl * (v1->b-v3->b))/zlt+v1->b;

    pb.u=(zl * (v1->u-v3->u))/zlt+v1->u;
    pb.v=(zl * (v1->v-v3->v))/zlt+v1->v;

    cv(&pa);
    cv(&pb);
    triangleclip(&cf);
};

void clipdown2(RotatedVertice *v1, RotatedVertice *v2, RotatedVertice *v3){
float zl,zlt;
float dz,dy,a, b;
float cy;
RotatedVertice pa,pb,pc;
FacelistPL cf;

					memcpy(&pa, v2, sizeof(pa));
					memcpy(&pb, v3, sizeof(pa));
					cf.v1=&pc;
					cf.v2=&pa; //v2
					cf.v3=&pb; //v3

	dz=v3->z-v1->z;
	dy=v3->y-v1->y;
    if(dy)a=dz/dy;else a=99911;
	b=v3->z-(a*v3->y);

	pc.z=b/(1+a);
	pc.y=-pc.z;
					zl=v1->z-pc.z;
					zlt=(v3->z-v1->z);
        if(!zlt)zlt+=0.001;
	pc.x=(zl * (v1->x-v3->x))/zlt+v1->x;
    pc.r=(zl * (v1->r-v3->r))/zlt+v1->r;
    pc.g=(zl * (v1->g-v3->g))/zlt+v1->g;
    pc.b=(zl * (v1->b-v3->b))/zlt+v1->b;

    pc.u=(zl * (v1->u-v3->u))/zlt+v1->u;
    pc.v=(zl * (v1->v-v3->v))/zlt+v1->v;

    cv(&pc);
    triangleclip(&cf);
					cf.v3=&pb;

	dz=v2->z-v1->z;
	dy=v2->y-v1->y;
    if(dy)a=dz/dy;else a=99911;
	b=v2->z-(a*v2->y);

	pb.z=(b/(1+a));
	pb.y=-pb.z;

					zl=v1->z-pb.z;
					zlt=(v2->z-v1->z);
        if(!zlt)zlt+=0.001;

	pb.x=(zl * (v1->x-v2->x))/zlt+v1->x;
    pb.r=(zl * (v1->r-v2->r))/zlt+v1->r;
    pb.g=(zl * (v1->g-v2->g))/zlt+v1->g;
    pb.b=(zl * (v1->b-v2->b))/zlt+v1->b;

    pb.u=(zl * (v1->u-v2->u))/zlt+v1->u;
    pb.v=(zl * (v1->v-v2->v))/zlt+v1->v;

    cv(&pb);
    triangleclip(&cf);

};

void clipdown(FacelistPL *face){
RotatedVertice *v1,*v2,*v3;
int c1,c2,c3;
int zl,zlt;
				v1=face->v1;
				v2=face->v2;
				v3=face->v3;

                if((v1->cb&DOWN) && (v2->cb&DOWN) && (v3->cb&DOWN)) return;
                if(!(v1->cb&DOWN) && !(v2->cb&DOWN) && !(v3->cb&DOWN))
                    triangleclip(face);
                    else{
//printf("cd\n");

				if(v1->cb&DOWN && v2->cb&DOWN) clipdown1(v3,v1,v2);
				else
				if(v2->cb&DOWN && v3->cb&DOWN) clipdown1(v1,v2,v3);
				else
				if(v1->cb&DOWN && v3->cb&DOWN) clipdown1(v2,v1,v3);
				else

				if(v1->cb&DOWN) clipdown2(v1,v2,v3);
				else
				if(v2->cb&DOWN) clipdown2(v2,v3,v1);
				else
				if(v3->cb&DOWN) clipdown2(v3,v1,v2);
//                else
//                printf("nitted\n");
                }

};
