/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"
#include "../f2c.h"

/* Subroutine */ int _elfunDECONVU_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal x, y, idx, scal;
    static integer jcalcf, ielemn;
    static logical negidx;
    static integer igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : DECONVU */

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

/*  ELEMENT TYPE : PR */

	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	idx = epvalu[ipstrt + 1];
	negidx = idx <= 0.f;
	if (negidx) {
	    scal = 0.f;
	}
	if (! negidx) {
	    scal = 1.f;
	}
	if (*ifflag == 1) {
	    fuvals[ielemn] = scal * x * y;
	} else {
	    fuvals[igstrt + 1] = scal * y;
	    fuvals[igstrt + 2] = scal * x;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 2] = scal;
		fuvals[ihstrt + 1] = 0.;
		fuvals[ihstrt + 3] = 0.;
	    }
	}
/* L2: */
    }
    return 0;
} /* elfun_ */

elfunType elfunDECONVU_=_elfunDECONVU_;

/* Subroutine */ int _groupDECONVU_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset, i__1;
    doublereal d__1;

    /* Local variables */
    static doublereal alpha;
    static integer jcalcg, igroup, igrtyp, ipstrt;


/*  PROBLEM NAME : DECONVU */

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

/*  GROUP TYPE : SQ */

	alpha = fvalue[igroup];
	if (! (*derivs)) {
/* Computing 2nd power */
	    d__1 = alpha;
	    gvalue[igroup + gvalue_dim1] = d__1 * d__1;
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = alpha * 2;
	    gvalue[igroup + gvalue_dim1 * 3] = 2.;
	}
L2:
	;
    }
    return 0;
} /* group_ */

groupType groupDECONVU_=_groupDECONVU_;
