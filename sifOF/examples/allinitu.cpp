/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/
#include "../SIFFunction.h"
#include "../f2c.h"

/* Subroutine */ int _elfunALLINITU_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal x, y, z__, xx, yy, cosx, sinx;
    static integer jcalcf, ielemn, ieltyp, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : ALLINITU */

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

/*  ELEMENT TYPE : SQR */

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

/*  ELEMENT TYPE : SQR2 */

L2:
	y = xvalue[ielvar[ilstrt + 1]];
	z__ = xvalue[ielvar[ilstrt + 2]];
	x = y + z__;
	if (*ifflag == 1) {
	    fuvals[ielemn] = x * x;
	} else {
	    fuvals[igstrt + 1] = x + x;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = 2.f;
	    }
	}
	goto L5;

/*  ELEMENT TYPE : SINSQR */

L3:
	x = xvalue[ielvar[ilstrt + 1]];
	sinx = sin(x);
	cosx = cos(x);
	if (*ifflag == 1) {
	    fuvals[ielemn] = sinx * sinx;
	} else {
	    fuvals[igstrt + 1] = sinx * 2.f * cosx;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = (cosx * cosx - sinx * sinx) * 2.f;
	    }
	}
	goto L5;

/*  ELEMENT TYPE : PRODSQR */

L4:
	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	xx = x * x;
	yy = y * y;
	if (*ifflag == 1) {
	    fuvals[ielemn] = xx * yy;
	} else {
	    fuvals[igstrt + 1] = x * 2.f * yy;
	    fuvals[igstrt + 2] = xx * 2.f * y;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = yy * 2.f;
		fuvals[ihstrt + 2] = x * 4.f * y;
		fuvals[ihstrt + 3] = xx * 2.f;
	    }
	}
L5:
	;
    }
    return 0;
} /* elfun_ */

elfunType elfunALLINITU_=_elfunALLINITU_;

/* Subroutine */ int _groupALLINITU_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset, i__1;

    /* Local variables */
    static doublereal gvar;
    static integer jcalcg, igroup, igrtyp, ipstrt;


/*  PROBLEM NAME : ALLINITU */

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

/*  GROUP TYPE : TRIVIAL */

L1:
	gvar = fvalue[igroup];
	if (! (*derivs)) {
	    gvalue[igroup + gvalue_dim1] = gvar;
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = 1.f;
	    gvalue[igroup + gvalue_dim1 * 3] = 0.f;
	}
	goto L3;

/*  GROUP TYPE : L2 */

L2:
	gvar = fvalue[igroup];
	if (! (*derivs)) {
	    gvalue[igroup + gvalue_dim1] = gvar * gvar;
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = gvar + gvar;
	    gvalue[igroup + gvalue_dim1 * 3] = 2.f;
	}
L3:
	;
    }
    return 0;
} /* group_ */

groupType groupALLINITU_=_groupALLINITU_;
