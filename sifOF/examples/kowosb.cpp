/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/
#include "../SIFFunction.h"
#include "../f2c.h"

/* Subroutine */ int _elfunKOWOSB_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal u, b1, b2, t1, t2, v1, v2, v3, v4, ub1, uv1, usq, b2cb, 
	    b2sq;
    static integer jcalcf, ielemn, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : KOWOSB */

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

/*  ELEMENT TYPE : KWO */

	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	v3 = xvalue[ielvar[ilstrt + 3]];
	v4 = xvalue[ielvar[ilstrt + 4]];
	u = epvalu[ipstrt + 1];
	usq = u * u;
	b1 = usq + u * v2;
	b2 = usq + u * v3 + v4;
	b2sq = b2 * b2;
	b2cb = b2 * b2sq;
	uv1 = u * v1;
	ub1 = u * b1;
	t1 = b1 / b2sq;
	t2 = 2.f / b2cb;
	if (*ifflag == 1) {
	    fuvals[ielemn] = v1 * b1 / b2;
	} else {
	    fuvals[igstrt + 1] = b1 / b2;
	    fuvals[igstrt + 2] = uv1 / b2;
	    fuvals[igstrt + 3] = -uv1 * t1;
	    fuvals[igstrt + 4] = -v1 * t1;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 2] = u / b2;
		fuvals[ihstrt + 4] = -ub1 / b2sq;
		fuvals[ihstrt + 7] = -t1;
		fuvals[ihstrt + 5] = -uv1 * u / b2sq;
		fuvals[ihstrt + 8] = -uv1 / b2sq;
		fuvals[ihstrt + 6] = t2 * uv1 * ub1;
		fuvals[ihstrt + 9] = t2 * uv1 * b1;
		fuvals[ihstrt + 10] = t2 * v1 * b1;
		fuvals[ihstrt + 1] = 0.;
		fuvals[ihstrt + 3] = 0.;
	    }
	}
/* L2: */
    }
    return 0;
} /* elfun_ */

elfunType elfunKOWOSB_=_elfunKOWOSB_;

/* Subroutine */ int _groupKOWOSB_(doublereal *gvalue, integer *lgvalu, doublereal *
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


/*  PROBLEM NAME : KOWOSB */

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

groupType groupKOWOSB_=_groupKOWOSB_;
