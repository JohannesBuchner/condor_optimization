/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"
#include "../f2c.h"

/* Subroutine */ int _elfunEIGENALS_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal d__, q1, q2;
    static integer jcalcf, ielemn, ieltyp, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : EIGENALS */

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

/*  ELEMENT TYPE : 2PROD */

L1:
	q1 = xvalue[ielvar[ilstrt + 1]];
	q2 = xvalue[ielvar[ilstrt + 2]];
	if (*ifflag == 1) {
	    fuvals[ielemn] = q1 * q2;
	} else {
	    fuvals[igstrt + 1] = q2;
	    fuvals[igstrt + 2] = q1;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 2] = 1.;
		fuvals[ihstrt + 1] = 0.;
		fuvals[ihstrt + 3] = 0.;
	    }
	}
	goto L3;

/*  ELEMENT TYPE : 3PROD */

L2:
	q1 = xvalue[ielvar[ilstrt + 1]];
	q2 = xvalue[ielvar[ilstrt + 2]];
	d__ = xvalue[ielvar[ilstrt + 3]];
	if (*ifflag == 1) {
	    fuvals[ielemn] = q1 * q2 * d__;
	} else {
	    fuvals[igstrt + 1] = q2 * d__;
	    fuvals[igstrt + 2] = q1 * d__;
	    fuvals[igstrt + 3] = q1 * q2;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 2] = d__;
		fuvals[ihstrt + 4] = q2;
		fuvals[ihstrt + 5] = q1;
		fuvals[ihstrt + 1] = 0.;
		fuvals[ihstrt + 3] = 0.;
		fuvals[ihstrt + 6] = 0.;
	    }
	}
L3:
	;
    }
    return 0;
} /* elfun_ */

elfunType elfunEIGENALS_=_elfunEIGENALS_;

/* Subroutine */ int _groupEIGENALS_(doublereal *gvalue, integer *lgvalu, doublereal *
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


/*  PROBLEM NAME : EIGENALS */

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
	    gvalue[igroup + gvalue_dim1 * 3] = 2.;
	}
L2:
	;
    }
    return 0;
} /* group_ */

groupType groupEIGENALS_=_groupEIGENALS_;
