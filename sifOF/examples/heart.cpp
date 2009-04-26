/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"
#include "../f2c.h"

/* Subroutine */ int _elfunHEART_(doublereal *fuvals, doublereal *xvalue, 
	doublereal *epvalu, integer *ncalcf, integer *itypee, integer *istaev,
	 integer *ielvar, integer *intvar, integer *istadh, integer *istepa, 
	integer *icalcf, integer *ltypee, integer *lstaev, integer *lelvar, 
	integer *lntvar, integer *lstadh, integer *lstepa, integer *lcalcf, 
	integer *lfvalu, integer *lxvalu, integer *lepvlu, integer *ifflag, 
	integer *ifstat)
{
    /* System generated locals */
    integer i__1;
    doublereal d__1, d__2;

    /* Local variables */
    static doublereal x, y, z__, six, two, diff, dfsq, twod, zero, twox, 
	    alpha, three;
    static integer jcalcf, ielemn, ieltyp, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : HEART8LS */

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
	    case 5:  goto L5;
	    case 6:  goto L6;
	    case 7:  goto L7;
	    case 8:  goto L8;
	}

/*  ELEMENT TYPE : 2PROD */

L1:
	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	zero = 0.f;
	if (*ifflag == 1) {
	    fuvals[ielemn] = x * y;
	} else {
	    fuvals[igstrt + 1] = y;
	    fuvals[igstrt + 2] = x;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = zero;
		fuvals[ihstrt + 3] = zero;
		fuvals[ihstrt + 2] = 1.f;
	    }
	}
	goto L9;

/*  ELEMENT TYPE : 3PROD */

L2:
	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	z__ = xvalue[ielvar[ilstrt + 3]];
	zero = 0.f;
	if (*ifflag == 1) {
	    fuvals[ielemn] = x * y * z__;
	} else {
	    fuvals[igstrt + 1] = y * z__;
	    fuvals[igstrt + 2] = x * z__;
	    fuvals[igstrt + 3] = x * y;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = zero;
		fuvals[ihstrt + 3] = zero;
		fuvals[ihstrt + 6] = zero;
		fuvals[ihstrt + 2] = z__;
		fuvals[ihstrt + 4] = y;
		fuvals[ihstrt + 5] = x;
	    }
	}
	goto L9;

/*  ELEMENT TYPE : VPV */

L3:
	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	alpha = epvalu[ipstrt + 1];
	zero = 0.f;
	diff = alpha - y;
	if (*ifflag == 1) {
	    fuvals[ielemn] = x * diff;
	} else {
	    fuvals[igstrt + 1] = diff;
	    fuvals[igstrt + 2] = -x;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = zero;
		fuvals[ihstrt + 3] = zero;
		fuvals[ihstrt + 2] = -1.f;
	    }
	}
	goto L9;

/*  ELEMENT TYPE : ADFSQ */

L4:
	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	z__ = xvalue[ielvar[ilstrt + 3]];
	zero = 0.f;
	two = 2.f;
/* Computing 2nd power */
	d__1 = y;
/* Computing 2nd power */
	d__2 = z__;
	dfsq = d__1 * d__1 - d__2 * d__2;
	twox = two * x;
	if (*ifflag == 1) {
	    fuvals[ielemn] = x * dfsq;
	} else {
	    fuvals[igstrt + 1] = dfsq;
	    fuvals[igstrt + 2] = twox * y;
	    fuvals[igstrt + 3] = -twox * z__;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = zero;
		fuvals[ihstrt + 3] = twox;
		fuvals[ihstrt + 6] = -twox;
		fuvals[ihstrt + 2] = two * y;
		fuvals[ihstrt + 4] = -two * z__;
		fuvals[ihstrt + 5] = zero;
	    }
	}
	goto L9;

/*  ELEMENT TYPE : PDFSQ */

L5:
	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	z__ = xvalue[ielvar[ilstrt + 3]];
	alpha = epvalu[ipstrt + 1];
	zero = 0.f;
	two = 2.f;
	diff = alpha - x;
/* Computing 2nd power */
	d__1 = y;
/* Computing 2nd power */
	d__2 = z__;
	dfsq = d__1 * d__1 - d__2 * d__2;
	twox = two * x;
	twod = two * diff;
	if (*ifflag == 1) {
	    fuvals[ielemn] = diff * dfsq;
	} else {
	    fuvals[igstrt + 1] = -dfsq;
	    fuvals[igstrt + 2] = twod * y;
	    fuvals[igstrt + 3] = -twod * z__;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = zero;
		fuvals[ihstrt + 3] = twod;
		fuvals[ihstrt + 6] = -twod;
		fuvals[ihstrt + 2] = -two * y;
		fuvals[ihstrt + 4] = two * z__;
		fuvals[ihstrt + 5] = zero;
	    }
	}
	goto L9;

/*  ELEMENT TYPE : P3PRD */

L6:
	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	z__ = xvalue[ielvar[ilstrt + 3]];
	alpha = epvalu[ipstrt + 1];
	zero = 0.f;
	two = 2.f;
	diff = alpha - x;
	if (*ifflag == 1) {
	    fuvals[ielemn] = diff * y * z__;
	} else {
	    fuvals[igstrt + 1] = -y * z__;
	    fuvals[igstrt + 2] = diff * z__;
	    fuvals[igstrt + 3] = diff * y;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = zero;
		fuvals[ihstrt + 3] = zero;
		fuvals[ihstrt + 6] = zero;
		fuvals[ihstrt + 2] = -z__;
		fuvals[ihstrt + 4] = -y;
		fuvals[ihstrt + 5] = diff;
	    }
	}
	goto L9;

/*  ELEMENT TYPE : 3DPRD */

L7:
	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	z__ = xvalue[ielvar[ilstrt + 3]];
	zero = 0.f;
	two = 2.f;
	three = 3.f;
	six = 6.f;
/* Computing 2nd power */
	d__1 = y;
/* Computing 2nd power */
	d__2 = z__;
	diff = d__1 * d__1 - three * (d__2 * d__2);
	if (*ifflag == 1) {
	    fuvals[ielemn] = x * y * diff;
	} else {
	    fuvals[igstrt + 1] = y * diff;
/* Computing 2nd power */
	    d__1 = y;
	    fuvals[igstrt + 2] = x * diff + two * x * (d__1 * d__1);
	    fuvals[igstrt + 3] = -six * x * y * z__;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = zero;
		fuvals[ihstrt + 3] = six * x * y;
		fuvals[ihstrt + 6] = -six * x * y;
/* Computing 2nd power */
		d__1 = y;
		fuvals[ihstrt + 2] = diff + two * (d__1 * d__1);
		fuvals[ihstrt + 4] = -six * y * z__;
		fuvals[ihstrt + 5] = -six * x * z__;
	    }
	}
	goto L9;

/*  ELEMENT TYPE : D3PRD */

L8:
	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	z__ = xvalue[ielvar[ilstrt + 3]];
	alpha = epvalu[ipstrt + 1];
	zero = 0.f;
	two = 2.f;
	three = 3.f;
	six = 6.f;
/* Computing 2nd power */
	d__1 = y;
/* Computing 2nd power */
	d__2 = z__;
	dfsq = d__1 * d__1 - three * (d__2 * d__2);
	diff = alpha - x;
	if (*ifflag == 1) {
	    fuvals[ielemn] = diff * y * dfsq;
	} else {
	    fuvals[igstrt + 1] = -y * dfsq;
/* Computing 2nd power */
	    d__1 = y;
	    fuvals[igstrt + 2] = diff * (dfsq + two * (d__1 * d__1));
	    fuvals[igstrt + 3] = -six * y * z__ * diff;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = zero;
		fuvals[ihstrt + 3] = six * y * diff;
		fuvals[ihstrt + 6] = -six * y * diff;
/* Computing 2nd power */
		d__1 = y;
		fuvals[ihstrt + 2] = -dfsq - two * (d__1 * d__1);
		fuvals[ihstrt + 4] = six * y * z__;
		fuvals[ihstrt + 5] = -six * diff * z__;
	    }
	}
L9:
	;
    }
    return 0;
} /* elfun_ */

elfunType elfunHEART_=_elfunHEART_;

/* Subroutine */ int _groupHEART_(doublereal *gvalue, integer *lgvalu, doublereal *
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


/*  PROBLEM NAME : HEART8LS */

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
	    goto L2;
	}
	ipstrt = istgpa[igroup] - 1;

/*  GROUP TYPE : L2 */

	gvar = fvalue[igroup];
	if (! (*derivs)) {
	    gvalue[igroup + gvalue_dim1] = gvar * gvar;
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = gvar + gvar;
	    gvalue[igroup + gvalue_dim1 * 3] = 2.f;
	}
L2:
	;
    }
    return 0;
} /* group_ */

groupType groupHEART_=_groupHEART_;
