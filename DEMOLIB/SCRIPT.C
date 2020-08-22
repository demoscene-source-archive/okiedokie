/* by craft / fudge */

#include        <stdio.h>
#include        <stdlib.h>
#include        <string.h>
#include        "filer.h"

#include        "script.h"

char    s_tempstring[280];
FFILE    *s_scriptfile;
int     s_lastchar;
int     s_linenumber=0;

typedef struct s_definition{
        char    *name;
        int     value;
        struct s_definition *next;
}s_definition;

s_definition *s_definitions=NULL;

int     s_fgetc2(FFILE *f)
{
        int     c;

        c=ffgetc(f);

        if(c=='\n')s_linenumber++;
        return c;
}

s_definition *s_defined(char *s)
{
        s_definition      *def;

        def=s_definitions;

        while(def)
        {
                if(!strcmp(s,def->name))return def;
                def=def->next;
        }

        return def;
}

void    s_define(char *s, int v)
{
        s_definition      *d;

        if(s_defined(s)!=NULL)
        {
                printf("ScriptError identifier already defined:%s\n",s);
                printf("Line: %i\n",s_linenumber);
                exit(1);
        }
        d=malloc(sizeof(s_definition));
        d->next=s_definitions;
        d->value=v;
        d->name=s;
        s_definitions=d;
}

int     s_getvalue(char *s)
{
        s_definition *def;

        def=s_defined(s);
        if(def)return def->value;

        printf("ScriptError unknown identifier:\"%s\"\n",s);
        printf("Line: %i\n",s_linenumber);
        exit(1);
        return 1;
}


int     s_specialchar(char c)
{
        return ((c=='.')||(c=='/')||(c=='#')||(c=='%'));
}

int     s_singlechartoken(char c)
{
        return ((c==',')||(c=='=')||(c=='+')||(c=='.')||(c=='/')||(c=='#')||(c=='%'));
}

int     s_isaz_09(char c)
{
        if((c>='a')&&(c<='z'))return 1;
        if((c>='0')&&(c<='9'))return 1;
        if(c=='_')return 1;
        return 0;
}

char    s_readcharignorespaces()
{
        int     c;

        c=s_fgetc2(s_scriptfile);

        while((c==' ')||(c=='\n'))c=s_fgetc2(s_scriptfile);

        return c;
}

char    s_readcharignorecomments()
{
        int     c;

        c=s_readcharignorespaces(s_scriptfile);

        while(c=='/')
        {
//                printf("%c\n",c);getch();
                c=s_readcharignorespaces(s_scriptfile);
                while(!s_specialchar(c))c=s_fgetc2(s_scriptfile);

        }

//        printf("%c\n",c);

        return c;
}

char    *s_readtoken()
{
        static  char* lasttoken="Begin";
        int     l;
        char    *s;

        if(!strcmp(lasttoken,"."))return lasttoken;

        l=1;
        s_tempstring[0]=s_lastchar;

        if(s_singlechartoken(s_lastchar))
        {
                s_lastchar=s_readcharignorecomments();
        }
        else
        {
                while(s_isaz_09(s_lastchar=s_readcharignorecomments()))
                {
                        s_tempstring[l]=s_lastchar;
                        l++;
                }
        }
        s_tempstring[l]=0;


        s=malloc(l+1);

        sprintf(s,"%s",s_tempstring);

        lasttoken=s;

        return s;
}

void    s_nexttokenmustbe(char *s)
{
        if(strcmp(s,s_readtoken()))
        {
                printf("Script Error %s expected! (Found %s)\n",s);
                printf("Line: %i\n",s_linenumber);
                exit(1);
        }
}

int     s_calcvalue(char **token)
{
        int     v=0;

        do
        {
        *token=s_readtoken();

        if(((*token[0]>='a')&&(*token[0]<='z'))||(*token[0]=='_'))
        {
                v+=s_getvalue(*token);
        }
        else
        if(((*token[0]>='0')&&(*token[0]<='9'))||(*token[0]=='-'))
        {
                int     w;
                sscanf(*token,"%i",&w);
                v+=w;
        }
        else
        {
                printf("Script Error: Number or identifier expected\n");
                printf("Line: %i\n",s_linenumber);
                exit(1);
        }
        *token=s_readtoken();
        }
        while(!strcmp(*token,"+"));

        return v;
}

void    s_check(char *s)
{
        if(strcmp(s,","))
        {
                printf("Script Error: \",\" expected\n");
                printf("Line: %i\n",s_linenumber);
                exit(1);
        }
}

s_script  *s_load(char *filename, int scriptsize)
{
        char    *token;
        int     size=0;

        s_script  *s;

        s_linenumber=0;
        s=malloc(sizeof(s_script)*scriptsize);

        if(!(s_scriptfile=ffopen(filename,"r")))
        {
                printf("ScriptError: unable to open file:%s\n",filename);
                exit(1);
        }


        s_lastchar=s_readcharignorecomments();

        token=s_readtoken();
        while(strcmp(".",token))
        {
                if(!strcmp(token,"#"))
                {
                        char    *id,*token1;
                        int     v;
                        id=s_readtoken();
                        s_nexttokenmustbe("=");
                        v=s_calcvalue(&token);
                        s_define(id,v);
//                        printf("DEF %s,%i\n",id,v);
                }
                else
                if(!strcmp(token,"%"))
                {
                        if(size>scriptsize)
                        {
                                printf("ScriptError: Script is too long\n");
                                exit(1);
                        }
                        s[size].pos=s_calcvalue(&token);
                        s_check(token);
                        s[size].row=s_calcvalue(&token);
                        s_check(token);
                        s[size].swi=s_calcvalue(&token);
                        s_check(token);
                        s[size].val=s_calcvalue(&token);
//                        printf("%i %i %i %i\n",s[size].pos,s[size].row,s[size].swi,s[size].val);
                        size++;
                }
                else
                {
                        printf("Script Error %% or # or . expected\n");
                        printf("Line: %i\n",s_linenumber);
                        exit(1);
                }
        }

        ffclose(s_scriptfile);

        return s;
}
