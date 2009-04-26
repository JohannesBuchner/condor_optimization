/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunSCHMVETT_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal a, e, g, t, u, u1, u2, v1, v2, v3, t2, usq, dad1, dad2, 
	    u2sq, deda, expe, d2ad12, d2ad22, d2eda2, halfu;
    static integer jcalcf;
    static doublereal dedasq;
    static integer ielemn;
    static doublereal shalfu;
    static integer ieltyp, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : SCHMVETT */

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

/*  ELEMENT TYPE : SCH1 */

L1:
	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	u = v1 - v2;
	usq = u * u;
	t = usq + 1.f;
	t2 = t * t;
	if (*ifflag == 1) {
	    fuvals[ielemn] = -1.f / t;
	} else {
	    fuvals[igstrt + 1] = u * 2.f / t2;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = (1.f - usq * 4.f / t) * 2.f / t2;
	    }
	}
	goto L4;

/*  ELEMENT TYPE : SCH2 */

L2:
	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	u = v1 * 3.14159f + v2;
	halfu = u * .5f;
	shalfu = sin(halfu);
	if (*ifflag == 1) {
	    fuvals[ielemn] = -shalfu;
	} else {
	    fuvals[igstrt + 1] = cos(halfu) * -.5f;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = shalfu * .25f;
	    }
	}
	goto L4;

/*  ELEMENT TYPE : SCH3 */

L3:
	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	v3 = xvalue[ielvar[ilstrt + 3]];
	u1 = v1 + v3;
	u2 = v2;
	u2sq = u2 * u2;
	a = u1 / u2 - 2.f;
	e = -a * a;
	expe = exp(e);
	deda = a * -2.f;
	d2eda2 = -2.f;
	dad1 = 1.f / u2;
	dad2 = -u1 / u2sq;
	d2ad12 = -1.f / u2sq;
	d2ad22 = u1 * 2.f / (u2sq * u2);
	dedasq = deda * deda;
	g = expe * deda;
	if (*ifflag == 1) {
	    fuvals[ielemn] = -expe;
	} else {
	    fuvals[igstrt + 1] = -g * dad1;
	    fuvals[igstrt + 2] = -g * dad2;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = -expe * (dedasq * dad1 * dad1 + d2eda2 * 
			dad1 * dad1);
		fuvals[ihstrt + 2] = -expe * (dedasq * dad1 * dad2 + d2eda2 * 
			dad1 * dad2 + deda * d2ad12);
		fuvals[ihstrt + 3] = -expe * (dedasq * dad2 * dad2 + d2eda2 * 
			dad2 * dad2 + deda * d2ad22);
	    }
	}
L4:
	;
    }
    return 0;
} /* elfun_ */

elfunType elfunSCHMVETT_=_elfunSCHMVETT_;

/* Subroutine */ int _groupSCHMVETT_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset;


/*  PROBLEM NAME : SCHMVETT */

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

groupType groupSCHMVETT_=_groupSCHMVETT_;
