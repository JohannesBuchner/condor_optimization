/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"
#include "../f2c.h"

/* Subroutine */ int _elfunCRAGGLVY_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal u, v, v1, v2, fval, secu, tanu;
    static integer jcalcf, ielemn;
    static doublereal secusq;
    static integer ieltyp, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : CRAGGLVY */

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
	}

/*  ELEMENT TYPE : EXPN */

L1:
	v = xvalue[ielvar[ilstrt + 1]];
	fval = exp(v);
	if (*ifflag == 1) {
	    fuvals[ielemn] = fval;
	} else {
	    fuvals[igstrt + 1] = fval;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = fval;
	    }
	}
	goto L3;

/*  ELEMENT TYPE : TANG */

L2:
	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	u = v1 - v2;
	tanu = tan(u);
	secu = 1.f / cos(u);
	secusq = secu * secu;
	if (*ifflag == 1) {
	    fuvals[ielemn] = tanu;
	} else {
	    fuvals[igstrt + 1] = secusq;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = secusq * 2.f * tanu;
	    }
	}
L3:
	;
    }
    return 0;
} /* elfun_ */

elfunType elfunCRAGGLVY_=_elfunCRAGGLVY_;

/* Subroutine */ int _groupCRAGGLVY_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset, i__1;
    doublereal d__1, d__2;

    /* Local variables */
    static doublereal gvar;
    static integer jcalcg, igroup, igrtyp, ipstrt;


/*  PROBLEM NAME : CRAGGLVY */

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
	    goto L5;
	}
	ipstrt = istgpa[igroup] - 1;
	switch (igrtyp) {
	    case 1:  goto L1;
	    case 2:  goto L2;
	    case 3:  goto L3;
	    case 4:  goto L4;
	}

/*  GROUP TYPE : L2 */

L1:
	gvar = fvalue[igroup];
	if (! (*derivs)) {
	    gvalue[igroup + gvalue_dim1] = gvar * gvar;
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = gvar + gvar;
	    gvalue[igroup + gvalue_dim1 * 3] = 2.f;
	}
	goto L5;

/*  GROUP TYPE : L4 */

L2:
	gvar = fvalue[igroup];
	if (! (*derivs)) {
/* Computing 4th power */
	    d__1 = gvar, d__1 *= d__1;
	    gvalue[igroup + gvalue_dim1] = d__1 * d__1;
	} else {
/* Computing 3rd power */
	    d__1 = gvar;
	    gvalue[igroup + (gvalue_dim1 << 1)] = d__1 * (d__1 * d__1) * 4.f;
/* Computing 2nd power */
	    d__1 = gvar;
	    gvalue[igroup + gvalue_dim1 * 3] = d__1 * d__1 * 12.f;
	}
	goto L5;

/*  GROUP TYPE : L6 */

L3:
	gvar = fvalue[igroup];
	if (! (*derivs)) {
/* Computing 6th power */
	    d__1 = gvar, d__1 *= d__1;
	    gvalue[igroup + gvalue_dim1] = d__1 * (d__1 * d__1);
	} else {
/* Computing 5th power */
	    d__1 = gvar, d__2 = d__1, d__1 *= d__1;
	    gvalue[igroup + (gvalue_dim1 << 1)] = d__2 * (d__1 * d__1) * 6.f;
/* Computing 4th power */
	    d__1 = gvar, d__1 *= d__1;
	    gvalue[igroup + gvalue_dim1 * 3] = d__1 * d__1 * 30.f;
	}
	goto L5;

/*  GROUP TYPE : L8 */

L4:
	gvar = fvalue[igroup];
	if (! (*derivs)) {
/* Computing 8th power */
	    d__1 = gvar, d__1 *= d__1, d__1 *= d__1;
	    gvalue[igroup + gvalue_dim1] = d__1 * d__1;
	} else {
/* Computing 7th power */
	    d__1 = gvar, d__2 = d__1, d__1 *= d__1, d__2 *= d__1;
	    gvalue[igroup + (gvalue_dim1 << 1)] = d__2 * (d__1 * d__1) * 8.f;
/* Computing 6th power */
	    d__1 = gvar, d__1 *= d__1;
	    gvalue[igroup + gvalue_dim1 * 3] = d__1 * (d__1 * d__1) * 56.f;
	}
L5:
	;
    }
    return 0;
} /* group_ */

groupType groupCRAGGLVY_=_groupCRAGGLVY_;














