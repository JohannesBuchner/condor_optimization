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
#ifndef SIFFUNCTION_INCLUDE
#define SIFFUNCTION_INCLUDE

#include <stdio.h>
#include <math.h>
#include "../common/Vector.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include "f2c.h"
#include "../common/ObjectiveFunction.h"

#define FU_SPACE 5000

/*
int elfun_(doublereal *fuvals, doublereal *xvalue, doublereal *epvalu,
integer *ncalcf, integer *itypee, integer *istaev, integer *ielvar, 
integer *intvar, integer *istadh, integer *istepa, integer *icalcf, 
integer *ltypee, integer *lstaev, integer *lelvar, integer *lntvar, 
integer *lstadh, integer *lstepa, integer *lcalcf, integer *lfvalu, 
integer *lxvalu, integer *lepvlu, integer *ifflag, integer *ifstat);

int group_(doublereal *gvalue, integer *lgvalu, doublereal *fvalue, doublereal *gpvalu,
integer *ncalcg, integer *itypeg, integer *istgpa, integer *icalcg, 
integer *ltypeg, integer *lstgpa, integer *lcalcg, integer *lfvalu, 
integer *lgpvlu, logical *derivs, integer *igstat);
*/
/*
typedef int (__cdeclsimpleMain.cpp  simpleObjFunctions.cpp  simpleObjFunctions.hsimpleMain.cpp  simpleObjFunctions.cpp  simpleObjFunctions.h *elfunType)(double *,double *,double *,
    long *,long *,long *,long *,long *,long *,long *,long *,long *,long *,
    long *,long *,long *,long *,long *,long *,long *,long *,long *,long *);

typedef int (__cdecl *groupType)(double *,long *,double *,double *,
    long *,long *,long *,long *,long *,long *,long *,long *,long *,long *,long *);
*/
typedef int ( *elfunType)(double *,double *,double *,
    long *,long *,long *,long *,long *,long *,long *,long *,long *,long *,
    long *,long *,long *,long *,long *,long *,long *,long *,long *,long *);

typedef int ( *groupType)(double *,long *,double *,double *,
    long *,long *,long *,long *,long *,long *,long *,long *,long *,long *,long *);

class SIFFunction: public UnconstrainedObjectiveFunction
{
  public:
    SIFFunction(int _t,char *, elfunType , groupType );
    ~SIFFunction();
    double eval(Vector v, int *nerror=NULL);

  private:
    FILE *FF;

    doublereal *fuvals, *epvalu, *b, *a, *escale, *gvals, *ft, *gscale, *gpvalu, *bl, *bu;
    integer *itypee, *istaev, *ielvar, *intvar, *istadh, *istep, *istada, *icna, *istadg, *ieling, *istgp, *itypeg, *icalcf, *lcalcg;
    integer n,ngel,nnza,ng, nelnum, nelvars, nepvlu, ngpvlu;
    bool alltriv;
    bool *gxeqx;

    int myErrno;

    integer lnelvars,nel1,lnelnum,lgpvlu,lepvlu,ng1;

    elfunType _elfun_;
    groupType _group_;

    int rd_Int(integer *n, int w, register int base=10, bool f__cblank=false);
    int rd_Logical(bool *n, int w);
    int rd_Double(double *p, int w, int d, int f__scale=0, bool f__cblank=false);
    int rd_Char(char *p, int len);
    void CRLF();
};

inline double d_sign(double *a,double *b)
{
    if(*b>=0) return abs(*a);
    return -abs(*a);
}

inline double pow_dd(doublereal *ap, doublereal *bp)
{
    return(pow(*ap, *bp) );
}


#endif

