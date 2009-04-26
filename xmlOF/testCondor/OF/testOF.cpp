#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

//char inputObjF[] ="C:\\MyProjects\\CONDOR\\BURGMANN\\optim.out\0",
//     outputObjF[]="C:\\MyProjects\\CONDOR\\BURGMANN\\xflos.out\0";

#define DIM 4

unsigned long mysrand;
double rand1()
{
    mysrand=1664525*mysrand+1013904223L;
    double r=((double)mysrand)/4294967297.0;
    //if (r>.52)
    //{
    //    printf("whoups\n");
    //}
    return r;

}

void initRandom(int i=0)
{
    if (i) { mysrand=i; return; }
     struct timeb t;
     ftime(&t);
     mysrand=t.millitm;
//     printf("seed for random number generation: %i\n",mysrand);
}

char isANumber(char c)
{
 return (((c>='0')&&(c<='9'))||
         (c=='.')||
         (c=='e')||
         (c=='E')||
         (c=='+')||
         (c=='-'));
}

#define EOL1 13
#define EOL2 10
int getFromLine(char *line, double *d, int n)
{
    int k;
	char *tline=line, *oldtline;
	for (k=0; k<n; k++)
    {
		while ((*tline==' ')||
			    (*tline=='\t'))tline++;
		if ((*tline==EOL1)||(*tline==EOL2))
        {
	        return k;
        }
        oldtline=tline;
        while(isANumber(*tline)) tline++;
        if (!isANumber(*(tline-1)))
        {
            return k;
            //tline[10]=0;
            //printf( "Error in simulation output file. The full line is:\n"
            //        "%s\n"
            //        "There is an error here:\n"
            //        "%s\n",line,tline);
        }
        if (oldtline==tline)
        {
	        return k;
        };
        if (*tline) { *tline='\0'; tline++; }
        d[k]=atof(oldtline);
    }
    return k;
}

inline double sqr( const double& t )
{
	return t*t;
}

int main(int argc, char **argv)
{
	int i, nerror=0;
	char line[30000];
    double d[DIM], dd=0.0;

    if (argc<4)
    {
      printf("Syntax: testOF <xml-config-file> <inputfilename> <outputfilename>\n");
      exit(255);
    }
    
	FILE *stream=fopen(argv[2],"r"); // inputObjF,"r");
	if (stream==NULL)
	{
		printf("OF error: cannot open file '%s'\n",argv[2]);
		exit(255);
	}

	fgets(line,30000,stream);
    if (getFromLine(line,d,DIM)!=DIM)
    {
      fprintf(stderr,"OF Error: file too short\n");
      exit(255);
    }
    fclose(stream);

    initRandom();

//	printf("OF input:"); vX.print(); fflush(0);

    for (i=0; i<DIM; i++) dd+=sqr(d[i]-2);
    dd+=rand1()*1e-5;
    
#ifdef FAILURE
    if (rand1()>.58) nerror=1;
#endif

    stream=fopen(argv[3],"w"); //outputObjF,"w");
	if (stream==NULL)
	{
		printf("OF error: cannot write to file '%s'\n",argv[3]);
		exit(255);
	}
	fprintf(stream,"A\n%i\n%e\t0\t0\t0\t0\n",1-nerror,dd);
    fclose(stream);

//	printf("OF test   Value:%e\n",dd); fflush(0);
}

