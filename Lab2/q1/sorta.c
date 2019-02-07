#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct t
{
	char lexemename[50];
	int index;
	int row;
	int col;
	char type[10];
} token;




void checkcomment(FILE* f1,int* rowcount)
{
	char ca=fgetc(f1);
	if(ca == '/') {
			char cb = fgetc(f1);
			if(cb == '/') {
				while(ca!='\n') {
					ca = fgetc(f1);
					
				}
				(*rowcount)++;
			}
			else if(cb == '*') {
				do {
					while(ca!='*') {
						if(ca=='\n')
							(*rowcount)++;
						ca = fgetc(f1);
					}
					cb = fgetc(f1);
				} while(cb!='/');
				(*rowcount)++;
			}
		}else
		{
			fseek(f1,-1,SEEK_CUR);
		}
}
char* checkliteral(FILE* f1,int *colcount)
{
	char ca=fgetc(f1);
	char* literal=malloc(50);
	for(int i=0;i<50;i++)
	{
		literal[i]='\0';
	}
	int index=0;
	if(ca=='\"')
	{
		(*colcount)++;
		literal[index++]=ca;
		char cb=fgetc(f1);

		while(cb!='\"')
		{
			(*colcount)++;
			literal[index++]=cb;
			cb=fgetc(f1);
		}
		(*colcount)++;
		literal[index++]=cb;
		return (char*)literal;
	}
	fseek(f1,-1,SEEK_CUR);
	return NULL;
}
void checkpreprocessor(FILE* f1,int* rowcount)
{
	char ca=fgetc(f1);
	if(ca=='#')
	{
		while(ca!='\n')
		{
			ca=fgetc(f1);
		}
		(*rowcount)++;
	}else
	{
		fseek(f1,-1,SEEK_CUR);
	}
}
char *relationaloperator(FILE* f1,int* colcount,int rowcount,int *index)
{
	char ca=fgetc(f1);
	char* literal=malloc(3);
	for(int i=0;i<3;i++)
	{
		literal[i]='\0';
	}
	if(ca=='='||ca=='!')
	{
		char cb=fgetc(f1);
		if(cb=='=')
		{
			(*colcount)+=2;
			literal[0]='=';
			literal[1]='=';
			return literal;
		}else
		{
			fseek(f1,-2,SEEK_CUR);
			return NULL;
		}
	
	}else if(ca=='<'||ca=='>')
	{
		char cb=fgetc(f1);
		literal[0]=ca;
		(*colcount)++;
		if(cb=='=')
		{
			(*colcount)++;
			literal[1]=cb;
		}
		return literal;
	}
	fseek(f1,-1,SEEK_CUR);
	return NULL;

}	
char* arithmeticoperator(FILE* f1,int* colcount)
{
	char ca=fgetc(f1);
	char* literal=malloc(2);
	for(int i=0;i<2;i++)
	{
		literal[i]='\0';
	}
	if(ca=='='||ca=='+'||ca=='-'||ca=='*'||ca=='/')
	{
		literal[0]=ca;
		(*colcount)++;
		return (char*)literal;

	}
	fseek(f1,-1,SEEK_CUR);
	return NULL;
}
char* logicaloperator(FILE* f1,int* colcount)
{
	//TODO
	char* literal=malloc(2);
	for(int i=0;i<2;i++)
	{
		literal[i]='\0';
	}
	char ca=fgetc(f1);
	if(ca=='!')
	{
		literal[0]=ca;
		return literal;

	}else if(ca=='&'|| ca=='|')
	{
		literal[0]=ca;
		char cb=fgetc(f1);
		if((cb=='&' && ca=='&')||(cb=='|'&&ca=='|'))
		{
			literal[1]=ca;
			return literal;
		}
		fseek(f1,-1,SEEK_CUR);
	}
	fseek(f1,-1,SEEK_CUR);
	return NULL;
}
char* specialsymbol(FILE* f1,int* colcount)
{

	char ca=fgetc(f1);
	char* literal=malloc(1);

	if(ca==','||ca==';')
	{
		literal[0]=ca;
		(*colcount)++;
		return literal;
	}
	fseek(f1,-1,SEEK_CUR);
	return NULL;
}
char* brackets(FILE* f1,int *colcount)
{
	char ca=fgetc(f1);
	char* literal=malloc(1);

	if(ca=='{'||ca=='}'||ca=='('||ca==')')
	{
		literal[0]=ca;
		(*colcount)++;
		return literal;
	}
	fseek(f1,-1,SEEK_CUR);
	return NULL;
}
char* identfier(FILE* f1,int* colcount)
{
	char* literal=malloc(50);
	for(int i=0;i<50;i++)
	{
		literal[i]='\0';
	}
	char ca=fgetc(f1);
	if((ca>=65 && ca <=90)||(ca>=97&&ca<=122))
	{
		literal[0]=ca;
		char cb=fgetc(f1);
		(*colcount)++;
		int cnt=1;
		while((cb>=65 && cb <=90)||(cb>=97&&cb<=122)||(cb>=48&&cb<=57))
		{
			(*colcount)++;
			literal[cnt++]=cb;
			cb=fgetc(f1);
		}
		fseek(f1,-1,SEEK_CUR);

		return literal;
	}
	fseek(f1,-1,SEEK_CUR);
	return NULL;
}
char* keyword(FILE* f1,int* colcount)
{
	char keywords[6][50]={"if","while","for","char","int","printf"};
	char* literal=malloc(50);
	for(int i=0;i<50;i++)
	{
		literal[i]='\0';
	}
	char ca=fgetc(f1);
	if((ca>=65 && ca <=90)||(ca>=97&&ca<=122))
	{
		literal[0]=ca;
		(*colcount)++;
		char cb=fgetc(f1);
		int cnt=1;
		while((cb>=65 && cb <=90)||(cb>=97&&cb<=122)||(cb>=48&&cb<=57))
		{
			(*colcount)++;
			literal[cnt++]=cb;
			cb=fgetc(f1);
		}
		fseek(f1,-1,SEEK_CUR);
		for(int i=0;i<6;i++)
		{
			if(strcmp(literal,keywords[i])==0)
				return literal;
		}
		fseek(f1,-cnt,SEEK_CUR);
		return NULL;
	}
	fseek(f1,-1,SEEK_CUR);
	return NULL;

}
char* numeric(FILE* f1,int* colcount)
{
	char* literal=malloc(50);
	for(int i=0;i<50;i++)
	{
		literal[i]='\0';
	}
	char ca=fgetc(f1);
	if(ca>=48&&ca<=57)
	{
		literal[0]=ca;
		(*colcount)++;
		int cnt=1;
		char cb=fgetc(f1);
		while(cb>=48 && cb<=57)
		{
			(*colcount)++;
			literal[cnt++]=cb;
			cb=fgetc(f1);
		}
		if(cb=='.')
		{
			(*colcount)++;
			literal[cnt++]=cb;
			cb=fgetc(f1);
			while(cb>=48 && cb<=57)
			{
				(*colcount)++;
				literal[cnt++]=cb;
				cb=fgetc(f1);		
			}
		}
		fseek(f1,-1,SEEK_CUR);
		return literal;
	}
	fseek(f1,-1,SEEK_CUR);
	return NULL;

}
void add(char lit[],int* index,int rowcount,int colcount,char* type,token tokens[])
{
	int ind=*index;
	tokens[*index].index=*index;
	strcpy(tokens[*index].lexemename,lit);
	tokens[*index].row=rowcount;
	tokens[*index].col=colcount;
	strcpy(tokens[*index].type,type);
	(*index)++;
}

int main()
{
	FILE* f1=fopen("test.c","r");
	if(f1==NULL)
	{
		printf("file does not exist\n");
		exit(0);
	}
	int rowcount=0;
	int colcount=0;
	int ca;
	int index=0;
	token tokens[100];
	do
	{

		checkcomment(f1,&rowcount);

		checkpreprocessor(f1,&rowcount);


		

		char* mayberelational=relationaloperator(f1,&colcount,rowcount,&index);
		if(mayberelational)
		{
			add(mayberelational,&index,rowcount,colcount,"RLOP",tokens);
		}
		char* maybelogical=logicaloperator(f1,&colcount);
		if(maybelogical)
		{
			add(maybelogical,&index,rowcount,colcount,"LOP",tokens);
		}
		char* maybearithmetic=arithmeticoperator(f1,&colcount);
		if(maybearithmetic)
		{
			 
			add(maybearithmetic,&index,rowcount,colcount,"AOP",tokens);
		}
		char* maybespecial=specialsymbol(f1,&colcount);
		if(maybespecial)
		{

			add(maybespecial,&index,rowcount,colcount,"SS",tokens);
		}
		
		char* maybekeyword=keyword(f1,&colcount);
		if(maybekeyword)
			{
				add(maybekeyword,&index,rowcount,colcount,"keyword",tokens);
			}
		char* maybeidentifier=identfier(f1,&colcount);
		if(maybeidentifier)
		{
			add(maybeidentifier,&index,rowcount,colcount,"identfier",tokens);

		}
		char* maybenumeric=numeric(f1,&colcount);
		if(maybenumeric)
		{
			add(maybenumeric,&index,rowcount,colcount,"number",tokens);			
		}

		char* maybebrackets=brackets(f1,&colcount);

		if(maybebrackets)
		{



			if(maybebrackets[0]=='{')
				add(maybebrackets,&index,rowcount,colcount,"LC",tokens);
			else if(maybebrackets[0]=='}')
				add(maybebrackets,&index,rowcount,colcount,"RC",tokens);
			else if(maybebrackets[0]=='(')
				add(maybebrackets,&index,rowcount,colcount,"LB",tokens);
			else if(maybebrackets[0]==')')
				add(maybebrackets,&index,rowcount,colcount,"RB",tokens);
		}
		char* maybeliteral=checkliteral(f1,&colcount);
		if(maybeliteral!=NULL)
		{
			add(maybeliteral,&index,rowcount,colcount,"Literal",tokens);
		}

		
		char ca=fgetc(f1);
		if(fgetc(f1)==EOF)
		{
			break;

		}
			fseek(f1,-1,SEEK_CUR);
		
		printf("%c\n",ca);
		if(ca==' '|| ca=='\t' )
		{
			colcount++;
		}
		else if(ca=='\n')
		{
			colcount=0;
			rowcount++;
		}else if(ca>=65 && ca<=90 || ca>=97 && ca<=122 || ca>=48 && ca<=57 || ca=='('|| ca==')'||ca=='{'||ca=='}'||ca==','||ca==';'||ca=='\"'||ca=='='||ca=='<'||ca=='>'||ca=='!')
		{
			fseek(f1,-1,SEEK_CUR);
		}
	}while(1);
	for (int i=0;i<index;i++)
	{
		printf("<%s %d %d %s>\n",tokens[i].lexemename,tokens[i].row,tokens[i].col,tokens[i].type );
	}

}