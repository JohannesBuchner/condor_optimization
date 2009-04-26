/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunHART6_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal v1, pij;
    static integer jcalcf, ielemn, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : HART6 */

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

/*  ELEMENT TYPE : SQ */

	v1 = xvalue[ielvar[ilstrt + 1]];
	pij = epvalu[ipstrt + 1];
	if (*ifflag == 1) {
	    fuvals[ielemn] = (v1 - pij) * (v1 - pij);
	} else {
	    fuvals[igstrt + 1] = (v1 - pij) * 2.f;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = 2.f;
	    }
	}
/* L2: */
    }
    return 0;
} /* elfun_ */

elfunType elfunHART6_=_elfunHART6_;

/* Subroutine */ int _groupHART6_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset, i__1;

    /* Local variables */
    static doublereal ci, alpha;
    static integer jcalcg, igroup, igrtyp, ipstrt;


/*  PROBLEM NAME : HART6 */

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

/*  GROUP TYPE : NEXP */

	alpha = fvalue[igroup];
	ci = gpvalu[ipstrt + 1];
	if (! (*derivs)) {
	    gvalue[igroup + gvalue_dim1] = ci * exp(-alpha);
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = -ci * exp(-alpha);
	    gvalue[igroup + gvalue_dim1 * 3] = ci * exp(-alpha);
	}
L2:
	;
    }
    return 0;
} /* group_ */

groupType groupHART6_=_groupHART6_;

















