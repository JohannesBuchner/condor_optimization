/* ELFUN.f -- translated by f2c (version 20000121).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunRosen_(
doublereal *fuvals, doublereal *xvalue, doublereal *epvalu,
integer *ncalcf, integer *itypee, integer *istaev, integer *ielvar, integer *intvar, integer *istadh, integer *istepa, integer *
	icalcf, integer *ltypee, integer *lstaev, integer *lelvar, integer *lntvar, integer *lstadh, integer *lstepa, integer *lcalcf,
	 integer *lfvalu, integer *lxvalu, integer *lepvlu, integer *ifflag, integer *ifstat)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static doublereal v1;
    static integer jcalcf, ielemn, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : ROSENBR */

    /* Parameter adjustments */
    --itypee;
    --istaev;
    --ielvar;
    --intvar;
    --istadh;
    --istepa;
    --icalcf;
    --fuvals;
    --xvalue;
    --epvalu;

    /* Function Body */
    *ifstat = 0;
    i__1 = *ncalcf;
    for (jcalcf = 1; jcalcf <= i__1; ++jcalcf) {
	ielemn = icalcf[jcalcf];
	ilstrt = istaev[ielemn] - 1;
	igstrt = intvar[ielemn] - 1;
	ipstrt = istepa[ielemn] - 1;
	if (*ifflag == 3) {
	    ihstrt = istadh[ielemn] - 1;
	}

/*  ELEMENT TYPE : SQ */

	v1 = xvalue[ielvar[ilstrt + 1]];
	if (*ifflag == 1) {
	    fuvals[ielemn] = v1 * v1;
	} else {
	    fuvals[igstrt + 1] = v1 + v1;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = (float)2.;
	    }
	}
/* L2: */
    }
    return 0;
} /* elfun_ */

elfunType elfunRosen_=_elfunRosen_;

/* GROUP.f -- translated by f2c (version 20000121).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

int _groupRosen_(doublereal *gvalue, integer *lgvalu, doublereal *fvalue, doublereal *gpvalu,
integer *ncalcg, integer *itypeg, integer *istgpa, integer *icalcg, 
integer *ltypeg, integer *lstgpa, integer *lcalcg, integer *lfvalu, 
integer *lgpvlu, logical *derivs, integer *igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset, i__1;

    /* Local variables */
    static doublereal gvar;
    static integer jcalcg, igroup, igrtyp, ipstrt;


/*  PROBLEM NAME : ROSENBR */

    /* Parameter adjustments */
    gvalue_dim1 = *lgvalu;
    gvalue_offset = 1 + gvalue_dim1 * 1;
    gvalue -= gvalue_offset;
    --itypeg;
    --istgpa;
    --icalcg;
    --fvalue;
    --gpvalu;

    /* Function Body */
    *igstat = 0;
    i__1 = *ncalcg;
    for (jcalcg = 1; jcalcg <= i__1; ++jcalcg) {
	igroup = icalcg[jcalcg];
	igrtyp = itypeg[igroup];
	if (igrtyp == 0) {
	    goto L2;
	}
	ipstrt = istgpa[igroup] - 1;

/*  GROUP TYPE : L2 */

	gvar = fvalue[igroup];
	if (! (*derivs)) {
	    gvalue[igroup + gvalue_dim1] = gvar * gvar;
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = gvar + gvar;
	    gvalue[igroup + gvalue_dim1 * 3] = (float)2.;
	}
L2:
	;
    }
    return 0;
} /* group_ */

groupType groupRosen_=_groupRosen_;
