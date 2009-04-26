/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Table of constant values */

static doublereal c_b3 = .66666666666666663;

/* Subroutine */ int _elfunGULF_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal a, t, v1, v2, v3, am1, aln, v1sq, ymv2, expma;
    static integer jcalcf;
    static doublereal lnymv2, ymv2sq;
    static integer ielemn;
    static doublereal aexpma;
    static integer igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : GULF */

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

/*  ELEMENT TYPE : GLF */

	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	v3 = xvalue[ielvar[ilstrt + 3]];
	t = epvalu[ipstrt + 1];
	v1sq = v1 * v1;
	d__1 = log(t) * -50.f;
	ymv2 = pow_dd(&d__1, &c_b3) + 25.f - v2;
	am1 = a - 1.f;
	ymv2sq = ymv2 * ymv2;
	lnymv2 = log((abs(ymv2)));
	d__1 = abs(ymv2);
	a = pow_dd(&d__1, &v3) / v1;
	aln = a * lnymv2;
	expma = exp(-a);
	aexpma = a * expma;
	if (*ifflag == 1) {
	    fuvals[ielemn] = expma;
	} else {
	    fuvals[igstrt + 1] = aexpma / v1;
	    fuvals[igstrt + 2] = v3 * aexpma / ymv2;
	    fuvals[igstrt + 3] = -aexpma * lnymv2;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = (a - 2.f) * aexpma / v1sq;
		fuvals[ihstrt + 2] = v3 * am1 * aexpma / (v1 * ymv2);
		fuvals[ihstrt + 4] = -aln * aexpma / v1;
		fuvals[ihstrt + 3] = v3 * aexpma * (v3 * am1 + 1.f) / ymv2sq;
		fuvals[ihstrt + 5] = aexpma * (v3 * aln + 1.f) / ymv2;
		fuvals[ihstrt + 6] = aln * lnymv2 * expma * am1;
	    }
	}
/* L2: */
    }
    return 0;
} /* elfun_ */

elfunType elfunGULF_=_elfunGULF_;

/* Subroutine */ int _groupGULF_(doublereal *gvalue, integer *lgvalu, doublereal *
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


/*  PROBLEM NAME : GULF */

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

groupType groupGULF_=_groupGULF_;

