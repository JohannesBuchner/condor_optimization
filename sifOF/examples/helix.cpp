/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunHELIX_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal r2, t2, v1, v2, t4, hd, sqt, v1sq, v2sq, r2sqt;
    static integer jcalcf, ielemn, ieltyp, igstrt, ihstrt;
    static doublereal twopii;
    static integer ilstrt, ipstrt;


/*  PROBLEM NAME : HELIX */

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
    twopii = .15915494f;
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

/*  ELEMENT TYPE : THETA */

L1:
	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	v1sq = v1 * v1;
	v2sq = v2 * v2;
	r2 = v1sq + v2sq;
	t2 = twopii / r2;
	t4 = t2 / r2;
	hd = t4 * 2.f * v1 * v2;
	if (*ifflag == 1) {
	    fuvals[ielemn] = twopii * atan2(v2, v1);
	} else {
	    fuvals[igstrt + 1] = -t2 * v2;
	    fuvals[igstrt + 2] = t2 * v1;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = hd;
		fuvals[ihstrt + 2] = t4 * (v2sq - v1sq);
		fuvals[ihstrt + 3] = -hd;
	    }
	}
	goto L3;

/*  ELEMENT TYPE : TWONRM */

L2:
	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	v1sq = v1 * v1;
	v2sq = v2 * v2;
	r2 = v1sq + v2sq;
	sqt = sqrt(r2);
	r2sqt = r2 * sqt;
	if (*ifflag == 1) {
	    fuvals[ielemn] = sqt;
	} else {
	    fuvals[igstrt + 1] = v1 / sqt;
	    fuvals[igstrt + 2] = v2 / sqt;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = v2sq / r2sqt;
		fuvals[ihstrt + 2] = -v1 * v2 / r2sqt;
		fuvals[ihstrt + 3] = v1sq / r2sqt;
	    }
	}
L3:
	;
    }
    return 0;
} /* elfun_ */

elfunType elfunHELIX_=_elfunHELIX_;

/* Subroutine */ int _groupHELIX_(doublereal *gvalue, integer *lgvalu, doublereal *
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


/*  PROBLEM NAME : HELIX */

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

groupType groupHELIX_=_groupHELIX_;
