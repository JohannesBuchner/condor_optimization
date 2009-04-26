/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"
#include "../f2c.h"

/* Subroutine */ int _elfunHAIRY_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal v, v1, v2, dv1, dv2, den, arg, vsq, tdl2, c2sq, tdv1, 
	    tdv2, s1sq, dens, stdv1, stdv2, sqarg;
    static integer jcalcf, ielemn, ieltyp;
    static doublereal smooth;
    static integer igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : HAIRY */

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
	}

/*  ELEMENT TYPE : FUR */

L1:
	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	dens = epvalu[ipstrt + 1];
	dv1 = dens * v1;
	dv2 = dens * v2;
	tdv1 = dv1 + dv1;
	tdv2 = dv2 + dv2;
	tdl2 = dens * 2.f * dens;
/* Computing 2nd power */
	d__1 = sin(dv1);
	s1sq = d__1 * d__1;
/* Computing 2nd power */
	d__1 = cos(dv2);
	c2sq = d__1 * d__1;
	stdv1 = sin(tdv1);
	stdv2 = sin(tdv2);
	if (*ifflag == 1) {
	    fuvals[ielemn] = s1sq * c2sq;
	} else {
	    fuvals[igstrt + 1] = dens * stdv1 * c2sq;
	    fuvals[igstrt + 2] = -dens * s1sq * stdv2;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = tdl2 * cos(tdv1) * c2sq;
		fuvals[ihstrt + 2] = -dens * dens * stdv1 * stdv2;
		fuvals[ihstrt + 3] = -tdl2 * s1sq * cos(tdv2);
	    }
	}
	goto L4;

/*  ELEMENT TYPE : DCUP */

L2:
	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	smooth = epvalu[ipstrt + 1];
	v = v1 - v2;
	vsq = v * v;
	arg = smooth + vsq;
	sqarg = sqrt(arg);
	den = 1.f / sqarg;
	if (*ifflag == 1) {
	    fuvals[ielemn] = sqarg;
	} else {
	    fuvals[igstrt + 1] = v * den;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = (1.f - vsq / arg) * den;
	    }
	}
	goto L4;

/*  ELEMENT TYPE : 1CUP */

L3:
	v = xvalue[ielvar[ilstrt + 1]];
	smooth = epvalu[ipstrt + 1];
	vsq = v * v;
	arg = smooth + vsq;
	sqarg = sqrt(arg);
	den = 1.f / sqarg;
	if (*ifflag == 1) {
	    fuvals[ielemn] = sqarg;
	} else {
	    fuvals[igstrt + 1] = v * den;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = (1.f - vsq / arg) * den;
	    }
	}
L4:
	;
    }
    return 0;
} /* elfun_ */

elfunType elfunHAIRY_=_elfunHAIRY_;

/* Subroutine */ int _groupHAIRY_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset;


/*  PROBLEM NAME : HAIRY */

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

groupType groupHAIRY_=_groupHAIRY_;












