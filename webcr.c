#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/stat.h>
#define BUF 200
#define Url_Length 1000
#define SEED_URL "http://www.chitkara.edu.in"
#define SEED_URI "https://www.chitkara.edu.in"
#define MAX_URL_PER_PAGE 100
#define INTERVAL_TIME 10
#define CHECK_MALLOC(s) if ((s) == NULL) { \
printf("memory allocation failed in %s: line%d", __FILE__, __LINE__); \
perror(":"); \
exit(-1); \
}
#define SET_MEM(start,size) memset(start,0,size)
struct node      //linked list
{
char *d;
int isvisited;
int depth;
struct node*next;
};
struct hash{                //for hash table
  struct node *starting;
  int count;
  int visit_count;
  struct node *ending;
};

char *tag="href";
int testDir(char *dir)        //testing valid directory
{
  struct stat statbuf;
  if ( stat(dir, &statbuf) == -1 ) 
  {
    
    return 0;
  }
 //Both check if there's a directory and if it's writable
 if ( !S_ISDIR(statbuf.st_mode) ) 
  {
    
    return 0;
  }
  if ( (statbuf.st_mode & S_IWUSR) != S_IWUSR ) 
  {
   
    return 0;
  }
  else
	{
	printf("valid directory\n");
	return 1;
	}
}
int checkdepth(int i)    //checking for depth
{  
  	if(i<=0)
	{
	printf("wrong input");
	return 0;
	}
	else
	{
	 return 1;
	}
}

int getpage(char *url)     //fetch the webage from server
{
	
	char urlbuffer[Url_Length+300]={0};
	strcat(urlbuffer,"wget -O ");
        strcat(urlbuffer,"/home/anmol/example1/temp.txt ");
	strcat(urlbuffer,url);
	if(!system(urlbuffer))
	{	printf("\ngetpage is working fine\n");
		return 1;
	}
	else
	{
		printf("something went wrong");
		return 0;
	}
}


char to_lower(char c) {
	return c > 92 ? c : c + 32;
}

char is_whitespace(char c)
{
return c=='\t' ||c=='\n' || c==' ';
}

//extracting urls.
int url_ext(char *str, int n, char **p) {                            
	char *t = *p, *c = tag;
	char quote;

	while(*t != '\0') {
		while(*t && *c && to_lower(*t) == *c) {
			t++;
			c++;
		}

		if(*c == '\0') {
			int i = 0;
			while(*t != '\0' && is_whitespace(*t))
				t++;
			if(*t++ == '=') {
				while(*t != '\0' && is_whitespace(*t))
					t++;
				if(*t == '\'' || *t == '\"') {
					quote = *t++;
					while(*t != '\0' && *t != quote && i < n - 1)
				 		str[i++] = *t++;
					str[i] = '\0';
				}
			}
			if(i != 0) break;
		}
		c = tag;
		t++;
	}
	*p = t;
	return *t;
}

void tostring(int num,char *str)
{
    	int i, rem, len = 0, n;
    	n = num;
    	while (n != 0)
    		{
        	len++;
        	n /= 10;
    		}
	    for (i = 0; i < len; i++)
		    {
			rem = num % 10;
			num = num / 10;
			str[len - (i + 1)] = rem + '0';
		    }
    str[len] = '\0';
}
int NormalizeURL(char* URL) 
{
  int len = strlen(URL);
  if (len <= 1 )
    return 0;
  //! Normalize all URLs.
  if (URL[len - 1] == '/') 
  {
    URL[len - 1] = 0;
    len--;
  }
  int i, j;
  len = strlen(URL);
  //! Safe check.
  if (len < 2)
    return 0;
  //! Locate the URL's suffix.
  for (i = len - 1; i >= 0; i--)
    if (URL[i] =='.')
      break;
  for (j = len - 1; j >= 0; j--)
    if (URL[j] =='/')
      break;
  //! We ignore other file types.
  //! So if a URL link is to a file that are not in the file type of the following
  //! one of four, then we will discard this URL, and it will not be in the URL list.
  if ((j >= 7) && (i > j) && ((i + 2) < len)) 
  {
    if ((strncmp((URL + i), ".htm", 4)==0) || (strncmp((URL + i), ".HTM", 4)==0) ) 
    {
      len = len; // do nothing.
	printf("%s :\n",URL);
    } 
    else 
    {
      return 0; // bad type
    }
  }
  return 1;
}
int checkbaseurl(char *u)
{
	int i=0,flag=1;
	char o[28]={0};
	strcpy(o,SEED_URI);
	int r=strlen(SEED_URI);
		for(i=0;i<r;i++)
		{
			if(u[i]!=o[i])
				{
				flag=0;
				break;
				}
		}
	int y=strlen(u);

	if (strcmp(strrchr(u, '\0') - 4, ".jpg")==0 || strcmp(strrchr(u, '\0') - 4, ".ico")==0 || strcmp(strrchr(u, '\0') - 4, ".pdf")==0 ||
	strcmp(strrchr(u, '\0') - 8 , ".php?rsd")==0)
	{
	printf("bad type:%s",u);
	flag=0;
	}
	
	if(flag==0)
	{
	return 0;
	}
	else 
	{
	if(NormalizeURL(u))
	{
		return 1;
	}
	else
	return 0;
	}
}

int allocate(char *u,char**all,int count)   //assign pointers to links through array of pointers
{

        int flag=1,i;
	int y=strlen(u);
		for(i=0;i<count;i++)  
		{
		   if(strcmp(u,all[i])==0)
		     {
		       flag=0;
		       break;
		     }
		}
        if(flag==1)
	{
		
                all[count]=(char*)malloc((y+1)*sizeof(char));
		strcpy(all[count],u);
		/*printf("%s\n",all[count]);*/
        	return 1;
         }
        else
          {
           return 0;
          }
	
}

int extracturlsfrompage(char *str1,char**all)
{int counter=0;
printf("string");
 char url[Url_Length];
 while(url_ext(url,Url_Length,&str1))     //recursively calling to get url
         {
            if(checkbaseurl(url) && counter<MAX_URL_PER_PAGE) 
               {
                 if(strlen(url)<=100)
                    {  
                       if(allocate(url,all,counter))
                          {
                            counter++;
                            
                          }
                    }
                }
            
         }
return counter;
}
void savelinked(struct node*start,int fileno,int reach)
{struct node*ptr=start;

FILE*fp;
fp=fopen("savelist.txt","w");

if(fp==NULL)
{
printf("file not found");
}
else
{
	fprintf(fp,"num-%d\n",fileno);
	fprintf(fp,"reach-%d\n",reach);
	while(ptr!=NULL)
	{
		fprintf(fp,"%s,%d,%d-",ptr->d,ptr->isvisited,ptr->depth);
		ptr=ptr->next;	
	}
}
fclose(fp);
return;
}
char* open(struct node*name)     //first designing the path.
{ 	
              
	char urlname[Url_Length];
           //length is extracted in url.
	FILE*h;
	int i=0,j=0;
	struct stat st;       //variable which will count length of file.
        stat ("example1/temp.txt",&st);
        int file_size=st.st_size;
        
        char*str=(char*)malloc((file_size+100)*sizeof(char));  //allocate memory to string
         h=fopen("/home/anmol/example1/temp.txt","r"); //open fetched temp.txt
	if(h==NULL)
	{
		return "/0";
	}
	char c;
	if(name!=NULL)                         //copying url to string 
	{
		strcpy(urlname,name->d);
		while(urlname[j]!='\0')
		{
			str[i]=urlname[j];
			j++;
			i++;
		}
	}
	str[i]='\n';
	i++;
	
	while((c=fgetc(h))!=EOF)
	{
	str[i]=c;
	i++;
	}
	printf("\nCopying file to string\n");
	str[i]='\0';
         char*str1=str;
	fclose(h);
return str1;       
}
int save(char*str,int fileno)
{           char numb[10];        //str to num
  	char dir[100]; 
        char strnewfile[50];        //filename in which content is written.
	FILE*hg;    
	strcpy(dir,"webcrawl");
        testDir(dir);     //testing
        printf("creating path:");          //creating path for copying file.
	strcpy(strnewfile,dir);
	strcat(strnewfile,"/");
	  tostring(fileno,numb); 
        strcat(strnewfile,numb);
        strcat(strnewfile,".txt");
	hg=fopen(strnewfile,"w");
	fprintf(hg,"%s",str);
	fclose(hg);
	printf("\nfilesaved in folder:\n");
return fileno+1;
}
int genkey(char *g)
	{
	int sum=0,i=0;
		while(g[i]!='\0')
		{
		sum=sum+g[i];
		i++;
		}
	sum=sum%99;
	return sum;
	}


struct hash*makelinked(struct node*head,struct hash *m,struct node**p,char**all,int index,int reach)       //form linked list
{
int i=0,key;
struct node*y=*p;
struct node*n;
int fl=0;
                
		while(i<index)
		{fl=0;
		key=genkey(all[i]);
		struct node*pop=m[key].starting;
		if(m[key].starting!=NULL)
		{
			while(pop!=m[key].ending)
			{
			if(strcmp(pop->d,all[i])==0)
			{
				fl=1;
				break;
			}
			pop=pop->next;
			}
		}
		if(fl==0)
		{
		n=(struct node*)malloc(sizeof(struct node));
		CHECK_MALLOC(n); 
		n->d=(char*)malloc(Url_Length*sizeof(char));
		strcpy(n->d,all[i]);
		n->isvisited=0;
		n->depth=reach;
		key=genkey(all[i]);
		
			if(m[key].starting==NULL)
			{
			y->next=n;
			n->next=NULL;
		
			y=y->next;
			m[key].starting=head;
			m[key].ending=head;
			}
			else
			{
			struct node*t=m[key].ending;
			n->next=t->next;
			t->next=n;
			m[key].ending=n;
			}
                struct node*gh=head;
		while(gh->next!=NULL)
                 {
			gh=gh->next; 
                 }
                  y=gh;
                 *p=gh;
		}
		i++;
	}
free(all);
return m;
}
struct hash* makenull(struct hash m[100])
{
int i=0;
for(i=0;i<100;i++)
{
m[i].starting=NULL;
m[i].ending=NULL;
}
return m;
}
void printlinked(struct node*st)
{printf("\n");
	while(st!=NULL)
	{
		printf("%s\n",st->d);
		st=st->next;
	}
}
void traverse(struct node*start,struct hash m[100],struct node*p,int depth,int file_no,int reach)
{
char e;
struct node*u=start->next;
struct node*prev=start;
struct hash*o=m;
char *y;
	while(reach<depth)
        {char**all=(char**)malloc(MAX_URL_PER_PAGE*sizeof(char*));
                if(u==NULL || u->next==NULL)
		{
			free(all);
			savelinked(start,file_no,reach);
			reach++;
			u=start;
		}
		if(u->isvisited==0 && u!=NULL && u->depth==reach)
                 {
			
			y=open(prev);
			prev=u;
			file_no=save(y,file_no);
			getpage(u->d);
			y=open(u);
			if(strcmp(y,"\0")==0)
			{
				break;
			}
			int count=extracturlsfrompage(y,all);     //returning no of urls.
			if(count==0)
			{	free(all);
				if(u->next!=NULL)
				{u=u->next;}
				else
				{
					u=start;
					savelinked(start,file_no,reach);
					reach++;
				}
				continue;
			}
			
			u->isvisited=1;
			if(u->next!=NULL)
			{u=u->next;}
			else if(u->next==NULL || u==NULL)
			{
			u=start;
			printf("saving ll");
			savelinked(start,file_no,reach);
			reach++;
			
			}
			o=makelinked(start,o,&p,all,count,reach+1);
			struct node*f=start;	
                 }
		else if(u!=NULL)
		{
		u=u->next;
		}
		 
        }

}
struct node*makehead(struct node*start)
{
start=(struct node*)malloc(sizeof(struct node));
	start->d=(char*)malloc(strlen(SEED_URL)*sizeof(char));
	strcpy(start->d,SEED_URL);
	start->isvisited=0;
	start->depth=1;
return start;
}
int checkvalidity(char*argv[],char**t)
{
char*u=(char*)malloc(30*sizeof(char));
char pre[1000]={0};
int i,depth;
int flag=0;
for(i=1;i<=3;i++)
{
	strcat(pre,"wget --spider ");
	strcat(pre,argv[i]);   
	if(testDir(argv[i]))
	{	
	strcpy(u,argv[i]);
	flag++;
	*t=u;
	}
	else if(atoi(argv[i]))
	{
		depth=atoi(argv[i]);
		if(checkdepth(depth))
		{flag++;}
	}
	else 
	{
	if(system(pre))
	{   printf("d");
		printf("Valid URL\n");
		flag++;
	}
	else
	{
	strcpy(pre,"\0");
	}
        }	
	}

if(flag==3)
{
printf("\neverything is valid\n");
}
else
{
printf("Wrong input");
exit(0);
}
return depth;
}
struct hash*makelistfromfile(struct node**start,struct hash m[100],struct node**p,int *filenumber,int*reach)
{	int i=0,key;
	printf("yo");
	FILE*fp;
	char newurl[1000];
	int visit,dep;
	struct node*rr;
	struct node*n;
	fp=fopen("savelist.txt","r");
	if(fp==NULL)
	{
	printf("ERROR||file doesn't exist");
	exit(0);
	}
	else
	{
	    fscanf(fp,"num-%d\n",filenumber);
	
	
            while(fscanf(fp,"%[^,],%d,%d-",newurl,&visit,&dep)!=EOF)
		{
		if(*start==NULL)
		{	
			n=(struct node*)malloc(sizeof(struct node));
			n->d=(char*)malloc(Url_Length*sizeof(char));
			strcpy(n->d,newurl);
			n->isvisited=visit;
			n->depth=dep;
			*start=n;
			*p=*start;
			rr=*start;
			key=genkey(n->d);
		
			if(m[key].starting==NULL)
			{
			m[key].starting=*start;
			m[key].ending=*start;
			}
		}
		else
		{
			n=(struct node*)malloc(sizeof(struct node));
			n->d=(char*)malloc(Url_Length*sizeof(char));
			strcpy(n->d,newurl);
			n->isvisited=visit;
			n->depth=dep;
			rr->next=n;
			rr=rr->next;
			n->next=NULL;
			key=genkey(n->d);
		
			if(m[key].starting==NULL)
			{
			m[key].starting=n;
			m[key].ending=n;
			}
			else
			{
			m[key].ending=n;
			}	
		}
	}
	*p=rr;
fclose(fp);
		}
return m;	
}

int main(int argc,char*argv[])
{       int dept,reach;
	struct node*start=NULL;
	int filenumber;
	char**all=(char**)malloc(MAX_URL_PER_PAGE*sizeof(char*));         //creating array of pointers
	struct hash m[100];            //hash table
	struct hash*k,*o;
	char *t;
	char choice;
	struct node*p=NULL;
	dept=checkvalidity(argv,&t);
	k=makenull(m);
	
	printf("\nContinue saved data-y or start afresh-n:");
	scanf(" %c",&choice);
	if(choice=='y')
	{
		o=makelistfromfile(&start,k,&p,&filenumber,&reach);
		printf("\nfile retrieved successfully\n");
		traverse(start,o,p,dept,filenumber,reach);
	}
	else
	{
		start=makehead(start);
		p=start;
		getpage(SEED_URL);
		char*str=open(NULL);
		int count=extracturlsfrompage(str,all);
		o=makelinked(start,k,&p,all,count,0);
		struct node*f=start;
		printlinked(f);
		printf("%d",count);
		traverse(start,o,p,dept,1,0);
	}
return 0;
}
