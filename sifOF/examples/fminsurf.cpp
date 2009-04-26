/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunFMINSURF_(doublereal *fuvals, doublereal *xvalue, 
	doublereal *epvalu, integer *ncalcf, integer *itypee, integer *istaev,
	 integer *ielvar, integer *intvar, integer *istadh, integer *istepa, 
	integer *icalcf, integer *ltypee, integer *lstaev, integer *lelvar, 
	integer *lntvar, integer *lstadh, integer *lstepa, integer *lcalcf, 
	integer *lfvalu, integer *lxvalu, integer *lepvlu, integer *ifflag, 
	integer *ifstat)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static doublereal u, v1, v2;
    static integer jcalcf, ielemn, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : FMINSURF */

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

/*  ELEMENT TYPE : ISQ */

	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	u = v1 - v2;
	if (*ifflag == 1) {
	    fuvals[ielemn] = u * u;
	} else {
	    fuvals[igstrt + 1] = u + u;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = 2.f;
	    }
	}
/* L2: */
    }
    return 0;
} /* elfun_ */

elfunType elfunFMINSURF_=_elfunFMINSURF_;

/* Subroutine */ int _groupFMINSURF_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset, i__1;

    /* Local variables */
    static doublereal gvar, alpha, sqral;
    static integer jcalcg, igroup, igrtyp, ipstrt;


/*  PROBLEM NAME : FMINSURF */

    /* Parameter adjustments */
    gvalue_dim1 = *lgvalu;
    gvalue_offset = 1 + gvalue_dim1;
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
	    goto L3;
	}
	ipstrt = istgpa[igroup] - 1;
	switch (igrtyp) {
	    case 1:  goto L1;
	    case 2:  goto L2;
	}

/*  GROUP TYPE : SQROOT */

L1:
	alpha = fvalue[igroup];
	sqral = sqrt(alpha);
	if (! (*derivs)) {
	    gvalue[igroup + gvalue_dim1] = sqral;
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = .5 / sqral;
	    gvalue[igroup + gvalue_dim1 * 3] = -.25 / (sqral * alpha);
	}
	goto L3;

/*  GROUP TYPE : L2 */

L2:
	gvar = fvalue[igroup];
	if (! (*derivs)) {
	    gvalue[igroup + gvalue_dim1] = gvar * gvar;
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = gvar + gvar;
	    gvalue[igroup + gvalue_dim1 * 3] = 2.;
	}
L3:
	;
    }
    return 0;
} /* group_ */

groupType groupFMINSURF_=_groupFMINSURF_;
