/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunDIXMAANK_(doublereal *fuvals, doublereal *xvalue, 
	doublereal *epvalu, integer *ncalcf, integer *itypee, integer *istaev,
	 integer *ielvar, integer *intvar, integer *istadh, integer *istepa, 
	integer *icalcf, integer *ltypee, integer *lstaev, integer *lelvar, 
	integer *lntvar, integer *lstadh, integer *lstepa, integer *lcalcf, 
	integer *lfvalu, integer *lxvalu, integer *lepvlu, integer *ifflag, 
	integer *ifstat)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1;

    /* Local variables */
    static doublereal x, y, f1, f2, df2dy;
    static integer jcalcf, ielemn, ieltyp, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : DIXMAANK */

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
	ieltyp = itypee[ielemn];
	switch (ieltyp) {
	    case 1:  goto L1;
	    case 2:  goto L2;
	    case 3:  goto L3;
	    case 4:  goto L4;
	}

/*  ELEMENT TYPE : SQ */

L1:
	x = xvalue[ielvar[ilstrt + 1]];
	if (*ifflag == 1) {
	    fuvals[ielemn] = x * x;
	} else {
	    fuvals[igstrt + 1] = x + x;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = 2.f;
	    }
	}
	goto L5;

/*  ELEMENT TYPE : SQB */

L2:
	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	f1 = x * x;
	f2 = y + y * y;
	df2dy = y * 2.f + 1.f;
	if (*ifflag == 1) {
	    fuvals[ielemn] = f1 * f2 * f2;
	} else {
	    fuvals[igstrt + 1] = x * 2.f * f2 * f2;
	    fuvals[igstrt + 2] = f1 * 2.f * f2 * df2dy;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = f2 * 2.f * f2;
		fuvals[ihstrt + 2] = x * 4.f * df2dy * f2;
		fuvals[ihstrt + 3] = f1 * 4.f * f2 + f1 * 2.f * df2dy * df2dy;
	    }
	}
	goto L5;

/*  ELEMENT TYPE : SQC */

L3:
	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	f1 = x * x;
/* Computing 4th power */
	d__1 = y, d__1 *= d__1;
	f2 = d__1 * d__1;
	if (*ifflag == 1) {
	    fuvals[ielemn] = f1 * f2;
	} else {
	    fuvals[igstrt + 1] = x * 2.f * f2;
/* Computing 3rd power */
	    d__1 = y;
	    fuvals[igstrt + 2] = f1 * 4.f * (d__1 * (d__1 * d__1));
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = f2 * 2.f;
/* Computing 3rd power */
		d__1 = y;
		fuvals[ihstrt + 2] = x * 8.f * (d__1 * (d__1 * d__1));
/* Computing 2nd power */
		d__1 = y;
		fuvals[ihstrt + 3] = f1 * 12.f * (d__1 * d__1);
	    }
	}
	goto L5;

/*  ELEMENT TYPE : 2PR */

L4:
	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	if (*ifflag == 1) {
	    fuvals[ielemn] = x * y;
	} else {
	    fuvals[igstrt + 1] = y;
	    fuvals[igstrt + 2] = x;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 2] = 1.f;
		fuvals[ihstrt + 1] = 0.;
		fuvals[ihstrt + 3] = 0.;
	    }
	}
L5:
	;
    }
    return 0;
} /* elfun_ */

elfunType elfunDIXMAANK_=_elfunDIXMAANK_;

/* Subroutine */ int _groupDIXMAANK_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset;


/*  PROBLEM NAME : DIXMAANK */

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
    return 0;
} /* group_ */

groupType groupDIXMAANK_=_groupDIXMAANK_;
