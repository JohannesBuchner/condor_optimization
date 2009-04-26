/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunGROWTHLS_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal u1, u2, u3, rn, logrn, power;
    static integer jcalcf, ielemn, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : GROWTHLS */

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

/*  ELEMENT TYPE : FIT */

	u1 = xvalue[ielvar[ilstrt + 1]];
	u2 = xvalue[ielvar[ilstrt + 2]];
	u3 = xvalue[ielvar[ilstrt + 3]];
	rn = epvalu[ipstrt + 1];
	logrn = log(rn);
	d__1 = u2 + logrn * u3;
	power = pow_dd(&rn, &d__1);
	if (*ifflag == 1) {
	    fuvals[ielemn] = u1 * power;
	} else {
	    fuvals[igstrt + 1] = power;
	    fuvals[igstrt + 2] = u1 * power * logrn;
/* Computing 2nd power */
	    d__1 = logrn;
	    fuvals[igstrt + 3] = u1 * power * (d__1 * d__1);
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = 0.f;
		fuvals[ihstrt + 2] = power * logrn;
/* Computing 2nd power */
		d__1 = logrn;
		fuvals[ihstrt + 4] = power * (d__1 * d__1);
/* Computing 2nd power */
		d__1 = logrn;
		fuvals[ihstrt + 3] = u1 * power * (d__1 * d__1);
/* Computing 3rd power */
		d__1 = logrn;
		fuvals[ihstrt + 5] = u1 * power * (d__1 * (d__1 * d__1));
/* Computing 4th power */
		d__1 = logrn, d__1 *= d__1;
		fuvals[ihstrt + 6] = u1 * power * (d__1 * d__1);
	    }
	}
/* L2: */
    }
    return 0;
} /* elfun_ */

elfunType elfunGROWTHLS_=_elfunGROWTHLS_;

/* Subroutine */ int _groupGROWTHLS_(doublereal *gvalue, integer *lgvalu, doublereal *
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


/*  PROBLEM NAME : GROWTHLS */

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

groupType groupGROWTHLS_=_groupGROWTHLS_;
