/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/
#include "../SIFFunction.h"
#include "../f2c.h"

/* Subroutine */ int _elfunOSBORNEB_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal a, t, v1, v2, v3, t3, tmv2, fval, expa;
    static integer jcalcf;
    static doublereal tmv2sq;
    static integer ielemn, ieltyp, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : OSBORNEB */

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

/*  ELEMENT TYPE : PEXP */

L1:
	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	t = epvalu[ipstrt + 1];
	expa = exp(-t * v2);
	fval = v1 * expa;
	if (*ifflag == 1) {
	    fuvals[ielemn] = fval;
	} else {
	    fuvals[igstrt + 1] = expa;
	    fuvals[igstrt + 2] = -t * fval;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 2] = -t * expa;
		fuvals[ihstrt + 3] = t * t * fval;
		fuvals[ihstrt + 1] = 0.;
	    }
	}
	goto L3;

/*  ELEMENT TYPE : PEXP3 */

L2:
	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	v3 = xvalue[ielvar[ilstrt + 3]];
	t3 = epvalu[ipstrt + 1];
	tmv2 = t3 - v2;
	tmv2sq = tmv2 * tmv2;
	expa = exp(-tmv2sq * v3);
	fval = v1 * expa;
	a = tmv2 * 2.f * v3;
	if (*ifflag == 1) {
	    fuvals[ielemn] = fval;
	} else {
	    fuvals[igstrt + 1] = expa;
	    fuvals[igstrt + 2] = a * fval;
	    fuvals[igstrt + 3] = -tmv2sq * fval;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 2] = a * expa;
		fuvals[ihstrt + 4] = -tmv2sq * expa;
		fuvals[ihstrt + 3] = (a * a - v3 * 2.f) * fval;
		fuvals[ihstrt + 5] = (tmv2 * 2.f - a * tmv2sq) * fval;
		fuvals[ihstrt + 6] = tmv2sq * tmv2sq * fval;
		fuvals[ihstrt + 1] = 0.;
	    }
	}
L3:
	;
    }
    return 0;
} /* elfun_ */

elfunType elfunOSBORNEB_=_elfunOSBORNEB_;

/* Subroutine */ int _groupOSBORNEB_(doublereal *gvalue, integer *lgvalu, doublereal *
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


/*  PROBLEM NAME : OSBORNEB */

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

groupType groupOSBORNEB_=_groupOSBORNEB_;
