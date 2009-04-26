/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/
#include "../SIFFunction.h"

/* Subroutine */ int _elfunVIBRBEAM_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal b, q, y, a0, a1, a2, a3, y2, y3, y4, y5, y6, phi, bcos, 
	    bsin;
    static integer jcalcf, ielemn;
    static doublereal cosphi, sinphi;
    static integer igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : VIBRBEAM */

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

/*  ELEMENT TYPE : fun */

	a0 = xvalue[ielvar[ilstrt + 1]];
	a1 = xvalue[ielvar[ilstrt + 2]];
	a2 = xvalue[ielvar[ilstrt + 3]];
	a3 = xvalue[ielvar[ilstrt + 4]];
	b = xvalue[ielvar[ilstrt + 5]];
	y = epvalu[ipstrt + 1];
	q = epvalu[ipstrt + 2];
	y2 = y * y;
	y3 = y * y2;
	y4 = y2 * y2;
	y5 = y2 * y3;
	y6 = y3 * y3;
	phi = a0 + y * (a1 + y * (a2 + y * a3)) - q;
	cosphi = cos(phi);
	sinphi = sin(phi);
	bcos = b * cosphi;
	bsin = b * sinphi;
	if (*ifflag == 1) {
	    fuvals[ielemn] = bcos;
	} else {
	    fuvals[igstrt + 1] = -bsin;
	    fuvals[igstrt + 2] = -bsin * y;
	    fuvals[igstrt + 3] = -bsin * y2;
	    fuvals[igstrt + 4] = -bsin * y3;
	    fuvals[igstrt + 5] = cosphi;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = -bcos;
		fuvals[ihstrt + 2] = -bcos * y;
		fuvals[ihstrt + 4] = -bcos * y2;
		fuvals[ihstrt + 7] = -bcos * y3;
		fuvals[ihstrt + 11] = -sinphi;
		fuvals[ihstrt + 3] = -bcos * y2;
		fuvals[ihstrt + 5] = -bcos * y3;
		fuvals[ihstrt + 8] = -bcos * y4;
		fuvals[ihstrt + 12] = -sinphi * y;
		fuvals[ihstrt + 6] = -bcos * y4;
		fuvals[ihstrt + 9] = -bcos * y5;
		fuvals[ihstrt + 13] = -sinphi * y2;
		fuvals[ihstrt + 10] = -bcos * y6;
		fuvals[ihstrt + 14] = -sinphi * y3;
		fuvals[ihstrt + 15] = 0.;
	    }
	}
/* L2: */
    }
    return 0;
} /* elfun_ */

elfunType elfunVIBRBEAM_=_elfunVIBRBEAM_;

/* Subroutine */ int _groupVIBRBEAM_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset, i__1;

    /* Local variables */
    static doublereal x;
    static integer jcalcg, igroup, igrtyp, ipstrt;


/*  PROBLEM NAME : VIBRBEAM */

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

/*  GROUP TYPE : square */

	x = fvalue[igroup];
	if (! (*derivs)) {
	    gvalue[igroup + gvalue_dim1] = x * x;
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = x + x;
	    gvalue[igroup + gvalue_dim1 * 3] = 2.f;
	}
L2:
	;
    }
    return 0;
} /* group_ */

groupType groupVIBRBEAM_=_groupVIBRBEAM_;

