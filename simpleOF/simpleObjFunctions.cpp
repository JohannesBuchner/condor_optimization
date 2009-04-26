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
#include <string.h>
#include "simpleObjFunctions.h"
#include "../common/tools.h"

void RandomOF::alloc(int n)
{
    xOptimal.setSize(n);
    xStart.setSize(n);
    A.setSize(n);
    S.setSize(n,n);
    C.setSize(n,n);
    strcpy(name,"RandomOF");
}

RandomOF::RandomOF(int _t,int n)
{
    t=_t;
    if (n==0) n=4;
    alloc(n);
    double *xo=xOptimal, *xs=xStart, *a=A, **s=S, **c=C, sum;
    int i,j;
    initRandom();
    for (i=0; i<n ; i++) 
    {
        xo[i]=(rand1()*2-1)*PI;
        xs[i]=xo[i]+(rand1()*0.2-0.1)*PI;
        for (j=0; j<n; j++)
        {
            s[i][j]=rand1()*200-100;
            c[i][j]=rand1()*200-100;
        }
    }

    for (i=0; i<n; i++)
    {
        sum=0;
        for (j=0; j<n; j++) sum+=s[i][j]*sin(xo[j])+c[i][j]*cos(xo[j]);
        a[i]=sum;
    }

    valueOptimal=0.0;
}

void RandomOF::save(char *filename)
{
    int n=A.sz();

    FILE *f=fopen(filename,"wb");
    fwrite(&n, sizeof(int),1, f);
    fwrite((double*)A, n*sizeof(double),1, f);
    fwrite((double*)xOptimal, n*sizeof(double),1, f);
    fwrite((double*)xStart, n*sizeof(double),1, f);
    fwrite(*S, n*n*sizeof(double),1, f);
    fwrite(*C, n*n*sizeof(double),1, f);
    fclose(f);
}

RandomOF::RandomOF(int _t,char *filename)
{
    t=_t;
    int n;

    FILE *f=fopen(filename,"rb");
    fread(&n, sizeof(int),1, f);

    alloc(n);
    
    fread((double*)A, n*sizeof(double),1, f);
    fread((double*)xOptimal, n*sizeof(double),1, f);
    fread((double*)xStart, n*sizeof(double),1, f);
    fread(*S, n*n*sizeof(double),1, f);
    fread(*C, n*n*sizeof(double),1, f);
    fclose(f);
}

double RandomOF::eval(Vector X, int *nerror)
{
    double *x=X, *a=A, **s=S, **c=C, sum, r=0;
    int i,j,n=X.sz();

    for (i=0; i<n; i++)
    {
        sum=0;
        for (j=0; j<n; j++) sum+=s[i][j]*sin(x[j])+c[i][j]*cos(x[j]);
        r+=sqr(a[i]-sum);
    }
#ifdef WIN32
 //   Sleep(1000); // 30 seconds sleep
#else
 //   sleep(1);
#endif

    updateCounter(r,X);
    return r;

    return r;
}

double RandomOF::ff(Vector X) // fast eval
{
    double *x=X, *a=A, **s=S, **c=C, sum, r=0;
    int i,j,n=X.sz();

    for (i=0; i<n; i++)
    {
        sum=0;
        for (j=0; j<n; j++) sum+=s[i][j]*sin(x[j])+c[i][j]*cos(x[j]);
        r+=sqr(a[i]-sum);
    }
    updateCounter(r,X);
    return r;
}

double Rosenbrock::eval(Vector X, int *nerror)
{
    double *x=X, r=100*sqr(x[1]-sqr(x[0]))+sqr(1-x[0]);
    updateCounter(r,X);
    return r;
}

Rosenbrock::Rosenbrock(int _t)
{
    t=_t;
    strcpy(name,"ROSEN");

    xOptimal.setSize(2);
    xStart.setSize(2);

    xOptimal[0]=1.0;
    xOptimal[1]=1.0;

    valueOptimal=0.0;

    xStart[0]=-1.2;
    xStart[1]=1.0;
}

double NoisyRosenbrock::eval(Vector X, int *nerror)
{
    double *x=X,r;
    r=100*sqr(x[1]-sqr(x[0]))+sqr(1-x[0])+rand1()*1e-4;
    updateCounter(r,X);
    return r;
}

NoisyRosenbrock::NoisyRosenbrock(int _t)
{
    t=_t;
    strcpy(name,"NOROSEN");

    xOptimal.setSize(2);
    xStart.setSize(2);

    xOptimal[0]=1.0;
    xOptimal[1]=1.0;

    valueOptimal=0.0;

    xStart[0]=-1.2;
    xStart[1]=1.0;

    initRandom();
}

double NoisyQuadratic::eval(Vector X, int *nerror)
{
    int i=n;
    double *x=X,r=0.0;
    while (i--) r+=sqr(x[i]-2.0);
    r+=rand1()*1e-5;
    updateCounter(r,X);
    return r;
}

NoisyQuadratic::NoisyQuadratic(int _t, int _n)
{
    int i;
    t=_t; strcpy(name,"NOQUAD");

    if (_n) n=_n; else n=4;

    xOptimal.setSize(n);
    xStart.setSize(n);
    for (i=0; i<n; i++)
    {
        xOptimal[i]=2.0;
        xStart[i]=0.0;
    }
    valueOptimal=0.0;
    initRandom();
}

double SimpleQuadratic::eval(Vector X, int *nerror)
{
    int i=n;
    double *x=X,r=0.0;
    while (i--) r+=sqr(x[i]);
    updateCounter(r,X);
    return r;
}

SimpleQuadratic::SimpleQuadratic(int _t, int _n)
{
    int i;
    t=_t; strcpy(name,"QUAD0");

    if (_n) n=_n; else n=4;
    xOptimal.setSize(n);
    xStart.setSize(n);
    for (i=0; i<n; i++)
    {
        xOptimal[i]=0.0;
        xStart[i]=2.0;
    }
    valueOptimal=0.0;
}

FletcherTest2::FletcherTest2(int _t)
{
    t=_t;
    strcpy(name,"FLETCHE2");

    xOptimal.setSize(3);
    xStart.setSize(3);

    xOptimal[0]=0.0;
    xOptimal[1]=0.0;
    xOptimal[2]=2.0;
    valueOptimal=-2.0;

    xStart[0]=0.0;
    xStart[1]=0.22;
    xStart[2]=0.0;

    initBounds();
    nNLConstraints=0;

    bl[0]=0.0;
    bl[1]=0.0;
    bl[2]=0.0;

    bu[2]=2.0;

    endInit();
}

double FletcherTest2::eval(Vector v, int *nerror)
{
    double *x=v, r=0.75*pow(x[0]*x[0]-x[0]*x[1]+x[1]*x[1],0.75)-x[2];
    updateCounter(r,v);
    return r;
//    return sqr(1.0-v[0])+sqr(1-v[1]);
}

FletcherTest::FletcherTest(int _t)
{
    t=_t;
    strcpy(name,"FLETCHER");

    xOptimal.setSize(2);
    xStart.setSize(2);

    xOptimal[0]=0.5*sqrt(2.0);
    xOptimal[1]=0.5*sqrt(2.0);
    valueOptimal=-sqrt(2.0);

    xStart[0]=0.0;
    xStart[1]=0.0;

    nNLConstraints=2;
    endInit();
}

double FletcherTest::eval(Vector v, int *nerror)
{
    double r=-v[0]-v[1];
    updateCounter(r,v);
    return r;
//    return sqr(1.0-v[0])+sqr(1-v[1]);
}

double FletcherTest::evalNLConstraint(int j, Vector vv, int *nerror)
{
    double *v=vv;
    switch (j)
    {
    case 0: return 1-v[0]*v[0]-v[1]*v[1];
    case 1: return -v[0]*v[0]+v[1];
    }
    return 0;
}

void FletcherTest::evalGradNLConstraint(int j, Vector v, Vector R, int *nerror)
{
    double *r=R;
    switch (j)
    {
    case 0: r[0]=-2*v[0];
            r[1]=-2*v[1];
            break;
    case 1: r[0]=-2*v[0];
            r[1]=1;
            break;
    }
}

SuperSimpleConstrainedObjectiveFunction::SuperSimpleConstrainedObjectiveFunction(int _t)
{
    t=_t;
    strcpy(name,"SIMPLE");

    xStart.setSize(2);
    xOptimal.setSize(2);

    xStart[0]=0.0;
    xStart[1]=0.0;

    xOptimal[0]=0.0;
    xOptimal[1]=4.0;

    initBounds();
    nNLConstraints=0;

    bl[0]=-2.0;
    bl[1]=-3.0;
    bu[0]=4.0;
    bu[1]=4.0;

    A.setSize(1,2);
    b.setSize(1);

    A[0][0]=-1.0;
//    A[0][0]=0.0;
    A[0][1]=-1.0;
    b[0]=-4.0;

    endInit();
}

double SuperSimpleConstrainedObjectiveFunction::eval(Vector v, int *nerror)
{
    double r=sqr(v[0])+sqr(v[1]-5);
    updateCounter(r,v);
    return r;
//    return -v[0]-2*v[1];
//    return v[0]+v[1];
}

#define BADSCALING 1e3

double BADScaleRosenbrock::eval(Vector X, int *nerror)
{
    double *x=X,r=100*sqr(x[1]/BADSCALING-sqr(x[0]))+sqr(1-x[0]);
//    if (rand1()>.52) { *nerror=1; r=-1e6; }
    updateCounter(r,X,*nerror);
    return r;
}

BADScaleRosenbrock::BADScaleRosenbrock(int _t)
{
    t=_t;
    strcpy(name,"BSROSEN");

    xOptimal.setSize(2);
    xStart.setSize(2);

    xOptimal[0]=1.0;
    xOptimal[1]=1.0*BADSCALING;
    valueOptimal=0.0;

    xStart[0]=-1.2;
    xStart[1]=1.0*BADSCALING;

    initRandom();
}
