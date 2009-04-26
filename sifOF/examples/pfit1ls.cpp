/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunPFIT1LS_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal b, c__, d__, y, a1, aa, ba, da, cc, dr, bx, dx, rr, xx, 
	    baa, daa, ccc, dar, bax, dax, arx, drx, bxx, dxx, logy;
    static integer jcalcf, ielemn, ieltyp, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : PFIT1LS */

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
	}

/*  ELEMENT TYPE : T1 */

L1:
	aa = xvalue[ielvar[ilstrt + 1]];
	rr = xvalue[ielvar[ilstrt + 2]];
	xx = xvalue[ielvar[ilstrt + 3]];
	if (*ifflag == 1) {
	    fuvals[ielemn] = aa * rr * xx;
	} else {
	    fuvals[igstrt + 1] = rr * xx;
	    fuvals[igstrt + 2] = aa * xx;
	    fuvals[igstrt + 3] = aa * rr;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 2] = xx;
		fuvals[ihstrt + 4] = rr;
		fuvals[ihstrt + 5] = aa;
		fuvals[ihstrt + 1] = 0.;
		fuvals[ihstrt + 3] = 0.;
		fuvals[ihstrt + 6] = 0.;
	    }
	}
	goto L6;

/*  ELEMENT TYPE : T2 */

L2:
	aa = xvalue[ielvar[ilstrt + 1]];
	rr = xvalue[ielvar[ilstrt + 2]];
	xx = xvalue[ielvar[ilstrt + 3]];
	a1 = aa + 1.f;
	y = xx + 1.f;
	logy = log(y);
	d__1 = -a1;
	c__ = pow_dd(&y, &d__1);
	cc = c__ / y;
	ccc = cc / y;
	b = 1.f - c__;
	ba = logy * c__;
	bx = a1 * cc;
	baa = -logy * logy * c__;
	bax = -logy * bx + cc;
	bxx = -a1 * (a1 + 1.f) * ccc;
	arx = aa * rr * xx;
	if (*ifflag == 1) {
	    fuvals[ielemn] = arx * b;
	} else {
	    fuvals[igstrt + 1] = rr * xx * b + arx * ba;
	    fuvals[igstrt + 2] = aa * xx * b;
	    fuvals[igstrt + 3] = aa * rr * b + arx * bx;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = rr * 2.f * xx * ba + arx * baa;
		fuvals[ihstrt + 2] = xx * b + aa * xx * ba;
		fuvals[ihstrt + 4] = rr * b + rr * xx * bx + aa * rr * ba + 
			arx * bax;
		fuvals[ihstrt + 5] = aa * b + aa * xx * bx;
		fuvals[ihstrt + 6] = aa * 2.f * rr * bx + arx * bxx;
		fuvals[ihstrt + 3] = 0.;
	    }
	}
	goto L6;

/*  ELEMENT TYPE : T3 */

L3:
	aa = xvalue[ielvar[ilstrt + 1]];
	rr = xvalue[ielvar[ilstrt + 2]];
	xx = xvalue[ielvar[ilstrt + 3]];
	if (*ifflag == 1) {
	    fuvals[ielemn] = aa * (aa + 1.f) * rr * xx * xx;
	} else {
	    fuvals[igstrt + 1] = (aa * 2.f + 1.f) * rr * xx * xx;
	    fuvals[igstrt + 2] = aa * (aa + 1.f) * xx * xx;
	    fuvals[igstrt + 3] = aa * 2.f * (aa + 1.f) * rr * xx;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = rr * 2.f * xx * xx;
		fuvals[ihstrt + 2] = (aa * 2.f + 1.f) * xx * xx;
		fuvals[ihstrt + 4] = (aa * 2.f + 1.f) * 2.f * rr * xx;
		fuvals[ihstrt + 5] = aa * 2.f * (aa + 1.f) * xx;
		fuvals[ihstrt + 6] = aa * 2.f * (aa + 1.f) * rr;
		fuvals[ihstrt + 3] = 0.;
	    }
	}
	goto L6;

/*  ELEMENT TYPE : T4 */

L4:
	aa = xvalue[ielvar[ilstrt + 1]];
	rr = xvalue[ielvar[ilstrt + 2]];
	xx = xvalue[ielvar[ilstrt + 3]];
	y = xx + 1.f;
	logy = log(y);
	d__1 = -aa;
	c__ = pow_dd(&y, &d__1);
	cc = c__ / y;
	ccc = cc / y;
	b = 1.f - c__;
	ba = logy * c__;
	bx = aa * cc;
	baa = -logy * logy * c__;
	bax = -logy * bx + cc;
	bxx = -aa * (aa + 1.f) * ccc;
	if (*ifflag == 1) {
	    fuvals[ielemn] = rr * b;
	} else {
	    fuvals[igstrt + 1] = rr * ba;
	    fuvals[igstrt + 2] = b;
	    fuvals[igstrt + 3] = rr * bx;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = rr * baa;
		fuvals[ihstrt + 2] = ba;
		fuvals[ihstrt + 4] = rr * bax;
		fuvals[ihstrt + 5] = bx;
		fuvals[ihstrt + 6] = rr * bxx;
		fuvals[ihstrt + 3] = 0.;
	    }
	}
	goto L6;

/*  ELEMENT TYPE : T5 */

L5:
	aa = xvalue[ielvar[ilstrt + 1]];
	rr = xvalue[ielvar[ilstrt + 2]];
	xx = xvalue[ielvar[ilstrt + 3]];
	a1 = aa + 2.f;
	y = xx + 1.f;
	logy = log(y);
	d__1 = -a1;
	c__ = pow_dd(&y, &d__1);
	cc = c__ / y;
	ccc = cc / y;
	b = 1.f - c__;
	ba = logy * c__;
	bx = a1 * cc;
	baa = -logy * logy * c__;
	bax = -logy * bx + cc;
	bxx = -a1 * (a1 + 1.f) * ccc;
	d__ = aa * (aa + 1.f) * rr * xx * xx;
	da = (aa * 2.f + 1.f) * rr * xx * xx;
	dr = aa * (aa + 1.f) * xx * xx;
	dx = aa * 2.f * (aa + 1.f) * rr * xx;
	daa = rr * 2.f * xx * xx;
	dar = (aa * 2.f + 1.f) * xx * xx;
	dax = (aa * 2.f + 1.f) * 2.f * rr * xx;
	drx = aa * 2.f * (aa + 1.f) * xx;
	dxx = aa * 2.f * (aa + 1.f) * rr;
	if (*ifflag == 1) {
	    fuvals[ielemn] = d__ * b;
	} else {
	    fuvals[igstrt + 1] = da * b + d__ * ba;
	    fuvals[igstrt + 2] = dr * b;
	    fuvals[igstrt + 3] = dx * b + d__ * bx;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = daa * b + da * 2.f * ba + d__ * baa;
		fuvals[ihstrt + 2] = dar * b + dr * ba;
		fuvals[ihstrt + 4] = dax * b + da * bx + dx * ba + d__ * bax;
		fuvals[ihstrt + 5] = drx * b + dr * bx;
		fuvals[ihstrt + 6] = dxx * b + dx * 2.f * bx + d__ * bxx;
		fuvals[ihstrt + 3] = 0.;
	    }
	}
L6:
	;
    }
    return 0;
} /* elfun_ */

elfunType elfunPFIT1LS_=_elfunPFIT1LS_;

/* Subroutine */ int _groupPFIT1LS_(doublereal *gvalue, integer *lgvalu, doublereal *
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


/*  PROBLEM NAME : PFIT1LS */

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

groupType groupPFIT1LS_=_groupPFIT1LS_;
