/*

CONDOR 1.06 - COnstrained, Non-linear, Direct, parallel Optimization 
              using trust Region method for high-computing load, 
              noisy functions
Copyright (C) 2004 Frank Vanden Berghen

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

If you want to include this tools in any commercial product, 
you can contact the author at fvandenb@iridia.ulb.ac.be

*/
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SIFFunction.h"

//typedef long int integer;
//typedef double doublereal;
//typedef long int logical;

int SIFFunction::rd_Int(integer *n, int w, register int base, bool f__cblank)
{
	int ch, sign;
	integer x = 0;

	if (w <= 0) goto have_x;
	for(;;) 
    {
        do { ch=getc(FF); } while ((ch == 10)||(ch==13));
		if (ch != ' ') break;
		if (!--w) goto have_x;
	}
	sign = 0;
	switch(ch) 
    {
	  case ',':
	  case '\n':
		w = 0;
		goto have_x;
	  case '-':
		sign = 1;
	  case '+':
		break;
	  default:
		if (ch >= '0' && ch <= '9') 
        {
			x = ch - '0';
			break;
		}
	    goto have_x;
	}
	while(--w) 
    {
		ch=getc(FF);
		if (ch >= '0' && ch <= '9') 
        {
			x = x*base + ch - '0';
			continue;
		}
		if (ch != ' ') 
        {
			if (ch == '\n' || ch == ',') w = 0;
			break;
		}
		if (f__cblank) x *= base;
	}
 have_x:
	if (sign) x = -x;
    *n=x;
	if (w) 
    {
		while(--w) ch=getc(FF);
		return myErrno = 115;
    }
	return 0;
}

int SIFFunction::rd_Logical(bool *n, int w)
{
    int ch, dot;
    bool lv;

	if (w <= 0)
		goto bad;
	for(;;) {
		ch=getc(FF);
		if ((ch == 10)||(ch==13)) continue;
		--w;
		if (ch != ' ')
			break;
		if (!w)
			goto bad;
		}
	dot = 0;
 retry:
	switch(ch) {
	  case '.':
		if (dot++ || !w)
			goto bad;
		ch=getc(FF);
		--w;
		goto retry;
	  case 't':
	  case 'T':
		lv = true;
		break;
	  case 'f':
	  case 'F':
		lv = false;
		break;
	  default:
 bad:
		for(; w > 0; --w)
			ch=getc(FF);
		// no break 
	  case ',':
	  case '\n':
		return myErrno = 116;
		}
    *n = lv;	
	while(w-- > 0) {
		ch=getc(FF);
		if (ch == ',' || ch == '\n')
			break;
		}
	return 0;
}

#define FMAX 40
#define EXPMAXDIGS 8
#define EXPMAX 99999999  
// EXPMAX = 10^EXPMAXDIGS - 1 = largest allowed exponent absolute value 

inline bool isdigit(char c) { return (c>='0')&&(c<='9'); };

int SIFFunction::rd_Double(double *p, int w, int d, int f__scale, bool f__cblank)
{
	char s[FMAX+EXPMAXDIGS+4];
	register int ch;
	register char *sp, *spe, *sp1;
	double x;
	int scale1, se;
	long e, exp;

	sp1 = sp = s;
	spe = sp + FMAX;
	exp = -d;
	x = 0.;

	do {
        do { ch=getc(FF); } while ((ch == 10)||(ch==13));
		w--;
		} while (ch == ' ' && w);
	switch(ch) {
		case '-': *sp++ = ch; sp1++; spe++;
		case '+':
			if (!w) goto zero;
			--w;
			ch=getc(FF);
		}
	while(ch == ' ') {
blankdrop:
		if (!w--) goto zero; ch=getc(FF); }
	while(ch == '0')
		{ if (!w--) goto zero; ch=getc(FF); }
	if (ch == ' ' && f__cblank)
		goto blankdrop;
	scale1 = f__scale;
	while(isdigit(ch)) {
digloop1:
		if (sp < spe) *sp++ = ch;
		else ++exp;
digloop1e:
		if (!w--) goto done;
		ch=getc(FF);
		}
	if (ch == ' ') {
		if (f__cblank)
			{ ch = '0'; goto digloop1; }
		goto digloop1e;
		}
	if (ch == '.') {
		exp += d;
		if (!w--) goto done;
		ch=getc(FF);
		if (sp == sp1) { // no digits yet 
			while(ch == '0') {
skip01:
				--exp;
skip0:
				if (!w--) goto done;
				ch=getc(FF);
				}
			if (ch == ' ') {
				if (f__cblank) goto skip01;
				goto skip0;
				}
			}
		while(isdigit(ch)) {
digloop2:
			if (sp < spe)
				{ *sp++ = ch; --exp; }
digloop2e:
			if (!w--) goto done;
			ch=getc(FF);
			}
		if (ch == ' ') {
			if (f__cblank)
				{ ch = '0'; goto digloop2; }
			goto digloop2e;
			}
		}
	switch(ch) {
	  default:
		break;
	  case '-': se = 1; goto signonly;
	  case '+': se = 0; goto signonly;
	  case 'e':
	  case 'E':
	  case 'd':
	  case 'D':
		if (!w--)
			goto bad;
		ch=getc(FF);
		while(ch == ' ') {
			if (!w--)
				goto bad;
			ch=getc(FF);
			}
		se = 0;
	  	switch(ch) {
		  case '-': se = 1;
		  case '+':
signonly:
			if (!w--)
				goto bad;
			ch=getc(FF);
			}
		while(ch == ' ') {
			if (!w--)
				goto bad;
			ch=getc(FF);
			}
		if (!isdigit(ch))
			goto bad;

		e = ch - '0';
		for(;;) {
			if (!w--)
				{ ch = '\n'; break; }
			ch=getc(FF);
			if (!isdigit(ch)) {
				if (ch == ' ') {
					if (f__cblank)
						ch = '0';
					else continue;
					}
				else
					break;
				}
			e = 10*e + ch - '0';
			if (e > EXPMAX && sp > sp1)
				goto bad;
			}
		if (se)
			exp -= e;
		else
			exp += e;
		scale1 = 0;
		}
	switch(ch) {
	  case '\n':
	  case ',':
		break;
	  default:
bad:
		return (myErrno = 115);
		}
done:
	if (sp > sp1) {
		while(*--sp == '0')
			++exp;
		if (exp -= scale1)
			sprintf(sp+1, "e%ld", exp);
		else
			sp[1] = 0;
		x = atof(s);
		}
zero:
	*p= x;
	return(0);
	}

#define VAL(x) (x!='\n'?x:' ')

int SIFFunction::rd_Char(char *p, int len)
{
    int i,ch;
	for (i=0; i<len; i++)
	{	
        do { ch=getc(FF); } while ((ch == 10)||(ch==13));
		*p++=VAL(ch);
	}
	return(0);
}

void SIFFunction::CRLF()
{
    int ch;
    do { ch=getc(FF); } while ((ch!=10)&&(ch!=13));
}

SIFFunction::SIFFunction(int _t, char *input , elfunType t1, groupType t2)
{
    t=_t;
    _elfun_=t1;
    _group_=t2;

    // Local variables 
    int i;
    
    static integer neltyp, ngrtyp, ialgor, fake;
    bool bfake;
    
    double obfbnd[2], dfake;
    char chtemp[10];
    
//  Set up the input data for the remaining unconstrained 
//  optimization tools.     

/*
    Input the problem dimensions. 

    n         dimension of the search space
    ng        number of groups
    nelnum    number of elementary functions
    ngel      number of nonlinear elements.
    nelvars   size of table index "ielvar"
    nnza      total number of non-zero linear elements for each group
    ngpvlu    total number of parameters for all groups
    nepvlu    total number of parameters for all elementary functions 
*/
    FF=fopen(input,"rb");

    if (FF==NULL)
    {
        printf("%s file not found.\n", input);
        printf("\npress return"); getchar();
        exit(254);
    }

    rd_Int( &n      , 8);          
    rd_Int( &ng     , 8);
    rd_Int( &nelnum , 8);
    rd_Int( &ngel   , 8);
    rd_Int( &nelvars, 8);
    rd_Int( &nnza   , 8);
    rd_Int( &ngpvlu , 8);
    rd_Int( &nepvlu , 8);
    rd_Int( &neltyp , 8);
    rd_Int( &ngrtyp , 8);
    CRLF();

    if (n <= 0) {
        exit(249);  // error
    }
    if (ng <= 0) {
        exit(249);  // error
    }

    //  Set useful int values. 
    ng1=ng+1;
    nel1=nelnum+1;
    
    //  Partition the double workspace. 

    fuvals=(doublereal *)calloc(max(nelnum,FU_SPACE),sizeof(doublereal));
    epvalu=(doublereal *)calloc(nepvlu+ngpvlu+4*ng+nnza+ngel+2*n,sizeof(doublereal));
    b      =epvalu +nepvlu;
    a      =b      +ng;
    escale =a      +nnza;
    gvals  =escale +ngel;
    ft     =gvals  +ng;
    gscale =ft     +ng;
    gpvalu =gscale +ng;
    bl     =gpvalu +ngpvlu;
    bu     =bl     +n;
    xStart.setSize(n);

    //  Partition the int workspace. 

    itypee=(integer*)calloc(nelnum*5+3+nelvars+ng*4+3+nnza+ngel+max(ng,nelnum),sizeof(integer));
    istaev =itypee +nelnum;
    ielvar =istaev +nel1;
    intvar =ielvar +nelvars;
    istadh =intvar +nelnum;
    istep  =istadh +nel1;
    istada =istep  +nel1;
    icna   =istada +ng1;
    istadg =icna   +nnza;
    ieling =istadg +ng1;
    istgp  =ieling +ngel;
    itypeg =istgp  +ng1;
    icalcf =itypeg +ng;
    lcalcg =icalcf  +max(ng,nelnum);
    
//  Partition the logical workspace. 

    gxeqx = (bool*)calloc(ng,sizeof(bool));

//  calculate lengths of arrays. 

    
    //  Input the problem type. 
    rd_Int(&ialgor,2);

    // input for the name of the problem 
    rd_Char(name,8); name[8]='\0';
    CRLF();


//  Dummy inputs ???????  
    if (ialgor == 2) 
    {
        rd_Int(&fake,8); // nslack
        rd_Int(&fake,8); // nobjgr
        CRLF();
    }

//  Input the starting addresses of the elements in each group, 
    for (i = 0; i <  ng1; ++i) rd_Int(istadg + i ,8);
    CRLF();

//  Input the starting addresses of the parameters used for each group and 
    for (i = 0; i <  ng1; ++i) rd_Int(istgp  + i ,8);
    CRLF();

//  Input the starting addresses  of the nonzeros of the linear element in each group. 
    for (i = 0; i <  ng1; ++i) rd_Int(istada + i ,8);
    CRLF();

//  Input the starting addresses of the variables of each element. 
    for (i = 0; i < nel1; ++i) rd_Int(istaev + i ,8);
    CRLF();

//  Input the starting addresses of the parameters of each element. 
    for (i = 0; i < nel1; ++i) rd_Int(istep  + i ,8);
    CRLF();

//  Input the group type of each group 
    for (i = 0; i <   ng; ++i) rd_Int(itypeg + i ,8);
    CRLF();

    if (ialgor >= 2) 
    {
        //   ???   
        for (i = 0; i < ng; ++i) rd_Int(&fake,8); //iwk[knfdoc + i 
        CRLF();
    }

//  Input the element type of each element 
    for (i = 0; i <  nelnum; ++i) rd_Int(itypee + i ,8);
    CRLF();

//  Input the number of internal variables for each element. 
    for (i = 0; i <  nelnum; ++i) rd_Int(intvar + i ,8);
    CRLF();

//  Input the identity of each individual element. 
    for (i = 0; i <    ngel; ++i) rd_Int(ieling + i ,8);
    CRLF();

//  Input the variables in each group's elements. 
    nelvars = istaev [ nelnum ]-1;
    for (i = 0; i < nelvars; ++i) rd_Int(ielvar + i ,8);
    CRLF();

//  Input the column addresses of the nonzeros in each linear element. 
    for (i = 0; i <    nnza; ++i) rd_Int(icna   + i ,8);
    CRLF();

//  Input the values of the nonzeros in each linear element, 
    for (i = 0; i <    nnza; ++i) rd_Double(a   + i ,16,8,1);
    CRLF();

//  Input the constant term in each group, 
    for (i = 0; i <      ng; ++i) rd_Double(b   + i ,16,8,1);
    CRLF();

//  Input the lower and upper bounds on the variables 
    if (ialgor <= 2) 
    {
        for (i = 0; i < n; ++i) rd_Double(&bl[i],16,8,1); CRLF();
        for (i = 0; i < n; ++i) rd_Double(&bu[i],16,8,1); CRLF();
    } else 
    {

    //  Use GVALS and FT as temporary storage for the constraint bounds. 
        for (i = 0; i <  n; ++i) rd_Double(bl    + i ,16,8,1); 
        for (i = 0; i < ng; ++i) rd_Double(gvals + i ,16,8,1); CRLF();
        for (i = 0; i <  n; ++i) rd_Double(bu    + i ,16,8,1); 
        for (i = 0; i < ng; ++i) rd_Double(ft    + i ,16,8,1); CRLF();
    }

    double *x=xStart;
    //  Input the starting point for the minimization. 
    for (i = 0; i < n; ++i) rd_Double(&x[i],16,8,1);
    CRLF();

    if (ialgor >= 2) 
    {
        //    Dummy input ????    
        for (i = 0; i < ng; ++i) rd_Double(&dfake ,16,8,1); // u+i
        CRLF();
    }

//  Input the parameters in each group. 
    for (i = 0; i < ngpvlu; ++i) rd_Double(gpvalu + i ,16,8,1);
    CRLF();

//  Input the parameters in each individual element. 
    for (i = 0; i < nepvlu; ++i) rd_Double(epvalu + i ,16,8,1);
    CRLF();

//  Input the scale factors for the nonlinear elements. 
    for (i = 0; i <   ngel; ++i) rd_Double(escale + i ,16,8,1);
    CRLF();

//  Input the scale factors for the groups. 
    for (i = 0; i <     ng; ++i) rd_Double(gscale + i ,16,8,1);
    CRLF();

//  Dummy Input the scale factors for the variables. 
    for (i = 0; i <     n; ++i) rd_Double(&dfake ,16,8,1); // vscale 
    CRLF();

//  Dummy Input for the lower and upper bounds on the objective function. 
    rd_Double(&obfbnd[0],16,8,1);
    rd_Double(&obfbnd[1],16,8,1);
    CRLF();

//  Dummy Input for a logical array which says whether an element has internal variables. 
    for (i = 0; i < nelnum; ++i) rd_Logical(&bfake,1);
    CRLF();

    alltriv=true;
//  Input a logical array which says whether a group is trivial. 
    for (i=0; i<ng; ++i) 
    {
        rd_Logical(gxeqx + i ,1);
        alltriv&=gxeqx[i];
    }
    CRLF();

//  Dummy Input the names given to the groups. 
    for (i = 0; i <     ng; ++i) rd_Char(chtemp,10);
    CRLF();

//  Dummy Input the names given to the variables.     
    for (i = 0; i <     n; ++i) rd_Char(chtemp,10);
    CRLF();

//  Dummy input for the names given to the element types. 
    for (i = 0; i < neltyp; ++i) rd_Char(chtemp,10);
    CRLF();

//  Dummy input for the names given to the group types. 
    for (i = 0; i < ngrtyp; ++i) rd_Char(chtemp,10);
    CRLF();

//  Dummy Input for the type of each variable. 
    for (i = 0; i <     n; ++i) rd_Int(&fake,8);

    fclose(FF);

    lnelvars = max(1,nelvars);
    lnelnum  = max(1,nelnum);
    lgpvlu   = max(1,ngpvlu);
    lepvlu   = max(1,nepvlu);

    isConstrained=0;
} // constructor 

SIFFunction::~SIFFunction()
{
    free(fuvals);
    free(epvalu);
    free(itypee);
    free(gxeqx);
}

static integer ifstat, igstat;
static integer c__50000 = FU_SPACE ;
static logical c_false = FALSE_ ;
static integer jobToDo = 1 ; // 1 stands for a simple evaluation of the elements

// #define sqr(a) ((a)*(a))

double SIFFunction::eval(Vector v, int *nerror)
{
    double *x=(double*)v, f=0.;
    integer i, i2, ig, j;
    double ftt;

//  Compute the value of a groups partially separable function 
//  initially written in Standard Input Format (SIF). 

//  there are non-trivial group functions. 

    for (i=0; i<max(nelnum,ng); ++i) icalcf[i]=i+1;

//  evaluate the element function values. 

    (*_elfun_)(fuvals, x, epvalu, &nelnum, itypee, 
	    istaev, ielvar, intvar, istadh, istep, icalcf, &lnelnum,
	     &nel1, &lnelvars, &nel1, &nel1, &nel1, &lnelnum, 
         &c__50000, &n, &lepvlu, &jobToDo, &ifstat);

//  compute the group argument values ft. 

    for (ig=0; ig<ng; ++ig) 
    {
    //  constant term in each group 
	    ftt = -b[ig] ; 

    //  include the contribution from the linear element. 
	    i2=istada[ig+1]-1;
	    for (j=istada[ig]-1; j<i2; ++j) ftt+=a[j]*x[icna[j]-1];

    //  include the contributions from the nonlinear elements. 
	    i2=istadg[ig+1]-1;
	    for (j=istadg[ig]-1; j<i2; ++j) ftt+=escale[j]*fuvals[ieling[j]-1];
        
        ft[ig]=ftt;
    }

    //  compute the group function values. 

    if (alltriv) 
    {

    //  all group functions are trivial. 
          for (i=0; i<ng; i++) f+=gscale[i]*ft[i];
//        memcpy(gvals,ft, ng*sizeof(double));      //  CALL DCOPY ( NG, WK( FT + 1 ), 1, WK( GVALS + 1 ), 1 )
//        for (i=0; i<ng; i++) gvals[ng+i]=1.0;     //  CALL DSETVL( NG, WK( GVALS + NG + 1 ), 1, ONE )

    } else 
    {

    //  evaluate the group function values. 

        (*_group_)(gvals, &ng, ft, gpvalu, &ng, itypeg, istgp, 
		    icalcf, lcalcg, &ng1, lcalcg, lcalcg, &lgpvlu, 
            &c_false, &igstat);
	    
	    for (ig=0; ig<ng; ++ig) 
        {
	        if (gxeqx[ig]) f+=gscale[ig]*ft[ig];
	        else f+=gscale[ig]*gvals[ig];
	    }
    }


//    double f2=100*sqr(x[1]-sqr(x[0]))+sqr(1-x[0]);
//    if (abs(f2-f)>1e-18) {
//        printf("zut!\n"); }

    #ifdef WIN32
//    Sleep(1000); // 1 seconds sleep
#else
//    sleep(1);
#endif
    updateCounter(f,v);
    return f;

}

/*

DOCUMENTATION
=============
  
Variables containing dimensions

n         dimension of the search space
ng        number of groups
ng1       ng+1
nelnum    number of elementary functions
nel1      nelnum+1
ngel      number of nonlinear elements.
nelvars   size of table index "ielvar"
nepvlu    total number of parameters for all elementary functions 
lepvlu    max (1, nepvlu);
ngpvlu    total number of parameters for all groups
lgpvlu    max (1, ngpvlu)
nnza      total number of non-zero linear elements for each group
lnelnum   max (1, nelnum)
lnelvars  max (1, nelvars)

Vectors variables

D fuvals  value of the elementary functions evaluated at x (sz: nelnum) (allocated memory is greater)
D x       point of evaluation (sz:n)
D epvalu  parameters of the elementary functions (sz: nepvlu)
I itypee  type of each elementary functions (sz: nelnum)
I istaev  starting addresses of the variables of each elementary functions (sz: nelnum+1) 
I ielvar  index of each variables in each elementary functions (sz: nelvars) 
I intvar  number of internal variables for each element (sz: nelnum)
I istadh  ??? for hessian calculation (sz:nelnum+1)
I istep   starting addresses of the parameters of each element. (nelnum+1)

D b       the constant term in each group (sz:ng)
I istada  starting addresses of the nonzeros of the linear element in each group: (sz: ng+1) 
D a       the values of the nonzeros in each linear element, (sz: nnza)
I icna    column addresses of the nonzeros in each linear element. (sz: nnza)
I istadg  starting addresses of the nonlinear elements in each group, (sz:ng+1)
D escale  the scale factors for the nonlinear elements. (sz: ngel)
I ieling  identity of each nonlinear element (sz: ngel)

D gvals   value of the groups after transformation in _group (sz: ng) 
D ft      value of the groups calculated from _elfun (sz: ng) 
L gxeqx   group "i" is trivial ? (sz: ng)
D gscale  scale factors for the groups. (sz:ng)
D gpvalu  parameters of the groups (sz: ngpvlu)
I istgp   starting addresses  of the parameters used for each group (sz: ng+1) (index inside gpvalu)
I itypeg  type of the group (sz: ng)
I icalcf  index of non-trivial groups (sz: max(ng,nelnum))

I lcalcg ??? (sz:max(1,ng))


C  CONTENTS OF THE ARRAYS ISTADG, ESCALE AND IELING:
C  ------------------------------------------------
C
C           <---------------------- NGEL -------------------------->
C
C           --------------------------------------------------------
C  ESCALE:  | EL.SCALES | EL.SCALES | ............... | EL.SCALES  |
C           | GROUP 1   | GROUP 2   |                 | GROUP NG   |
C           --------------------------------------------------------
C  IELING:  | ELEMENTS  | ELEMENTS  | ............... | ELEMENTS   |
C           | GROUP 1   | GROUP 2   |                 | GROUP NG   |
C           --------------------------------------------------------
C            |           |                             |            |
C            | |--- > ---|                             |            |
C            | |   |-------------------- > ------------|            |
C            | |   | |------------------------- > ------------------|
C            ---------
C  ISTADG:   | ..... |    POINTER TO THE POSITION OF THE 1ST ELEMENT
C            ---------    OF EACH GROUP WITHIN THE ARRAY.
C
C            <-NG+1->
C
C  CONTENTS OF THE ARRAYS IELVAR AND ISTAEV:
C  ----------------------------------------
C
C          <--------------------- NELVAR -------------------------->
C
C          ---------------------------------------------------------
C          | VARIABLES | VARIABLES | ............... |  VARIABLES  |
C  IELVAR: | ELEMENT 1 | ELEMENT 2 |                 | ELEMENT NEL |
C          ---------------------------------------------------------
C           |           |                             |             |
C           | |--- > ---|                             |             |
C           | |    |------------------- > ------------|             |
C           | |    | |----------------- > --------------------------|
C           ----------
C  ISTAEV:  | ...... |    POINTER TO THE POSITION OF THE 1ST VARIABLE
C           ----------    IN EACH ELEMENT (INCLUDING ONE TO THE END).
C
C          <- NEL+1 ->
C
C  CONTENTS OF THE ARRAY INTVAR:
C  -----------------------------
C
C  ON INITIAL ENTRY, INTVAR( I ), I = 1, ... , NEL, GIVES THE NUMBER OF
C  INTERNAL VARIABLES FOR ELEMENT I. THEREAFTER, INTVAR PROVIDES
C  POINTERS TO THE START OF EACH ELEMENT GRADIENT WITH RESPECT TO ITS
C  INTERNAL VARIABLES AS FOLLOWS:
C
C         -> <---------------------- NINVAR -----------------------> <-
C
C         -------------------------------------------------------------
C  PART OF  | GRADIENT  | GRADIENT  | ............... |  GRADIENT   | .
C  FUVALS   | ELEMENT 1 | ELEMENT 2 |                 | ELEMENT NEL |
C         -------------------------------------------------------------
C          | |           |                             |           | |
C       LGXI | |--- > ---|                             |         LHXI|
C            | |   |-------------------- > ------------|             |
C            | |   | |------------------------- > -------------------|
C            ---------
C  INTVAR:   | ..... |    POINTER TO THE POSITION OF THE 1ST ENTRY OF
C            ---------    THE GRADIENT FOR EACH ELEMENT.
C
C            <-NEL+1->
CC
C  CONTENTS OF THE A, ARRAYS ICNA AND ISTADA:
C  ----------------------------------------
C
C          <--------------------- NA ----------------------------->
C
C          ---------------------------------------------------------
C          |   VALUES  |   VALUES  | ............... |    VALUES   |
C  A:      |    A(1)   |    A(2)   |                 |     A(NG)   |
C          ---------------------------------------------------------
C          | VARIABLES | VARIABLES | ............... |  VARIABLES  |
C  ICNA:   |    A(1)   |    A(2)   |                 |     A(NG)   |
C          ---------------------------------------------------------
C           |           |                             |             |
C           | |--- > ---|                             |             |
C           | |    |------------------- > ------------|             |
C           | |    | |----------------- > --------------------------|
C           ----------
C  ISTADA:  | ...... |    POINTER TO THE POSITION OF THE 1ST VARIABLE IN
C           ----------    THE LINEAR ELEMENT FOR EACH GROUP (INCLUDING
C                         ONE TO THE END).
C
C           <- NG+1 ->
  
    */
