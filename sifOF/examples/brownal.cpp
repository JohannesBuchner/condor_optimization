/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunBROWNAL_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v12, v34, v56, 
	    v78, v910;
    static integer jcalcf, ielemn, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : BROWNAL */

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

/*  ELEMENT TYPE : PROD */

	v1 = xvalue[ielvar[ilstrt + 1]];
	v2 = xvalue[ielvar[ilstrt + 2]];
	v3 = xvalue[ielvar[ilstrt + 3]];
	v4 = xvalue[ielvar[ilstrt + 4]];
	v5 = xvalue[ielvar[ilstrt + 5]];
	v6 = xvalue[ielvar[ilstrt + 6]];
	v7 = xvalue[ielvar[ilstrt + 7]];
	v8 = xvalue[ielvar[ilstrt + 8]];
	v9 = xvalue[ielvar[ilstrt + 9]];
	v10 = xvalue[ielvar[ilstrt + 10]];
	v12 = v1 * v2;
	v34 = v3 * v4;
	v56 = v5 * v6;
	v78 = v7 * v8;
	v910 = v9 * v10;
	if (*ifflag == 1) {
	    fuvals[ielemn] = v12 * v34 * v56 * v78 * v910;
	} else {
	    fuvals[igstrt + 1] = v2 * v34 * v56 * v78 * v910;
	    fuvals[igstrt + 2] = v1 * v34 * v56 * v78 * v910;
	    fuvals[igstrt + 3] = v12 * v4 * v56 * v78 * v910;
	    fuvals[igstrt + 4] = v12 * v3 * v56 * v78 * v910;
	    fuvals[igstrt + 5] = v12 * v34 * v6 * v78 * v910;
	    fuvals[igstrt + 6] = v12 * v34 * v5 * v78 * v910;
	    fuvals[igstrt + 7] = v12 * v34 * v56 * v8 * v910;
	    fuvals[igstrt + 8] = v12 * v34 * v56 * v7 * v910;
	    fuvals[igstrt + 9] = v12 * v34 * v56 * v78 * v10;
	    fuvals[igstrt + 10] = v12 * v34 * v56 * v78 * v9;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 2] = v34 * v56 * v78 * v910;
		fuvals[ihstrt + 4] = v2 * v4 * v56 * v78 * v910;
		fuvals[ihstrt + 7] = v2 * v3 * v56 * v78 * v910;
		fuvals[ihstrt + 11] = v2 * v34 * v6 * v78 * v910;
		fuvals[ihstrt + 16] = v2 * v34 * v5 * v78 * v910;
		fuvals[ihstrt + 22] = v2 * v34 * v56 * v8 * v910;
		fuvals[ihstrt + 29] = v2 * v34 * v56 * v7 * v910;
		fuvals[ihstrt + 37] = v2 * v34 * v56 * v78 * v10;
		fuvals[ihstrt + 46] = v2 * v34 * v56 * v78 * v9;
		fuvals[ihstrt + 5] = v1 * v4 * v56 * v78 * v910;
		fuvals[ihstrt + 8] = v1 * v3 * v56 * v78 * v910;
		fuvals[ihstrt + 12] = v1 * v34 * v6 * v78 * v910;
		fuvals[ihstrt + 17] = v1 * v34 * v5 * v78 * v910;
		fuvals[ihstrt + 23] = v1 * v34 * v56 * v8 * v910;
		fuvals[ihstrt + 30] = v1 * v34 * v56 * v7 * v910;
		fuvals[ihstrt + 38] = v1 * v34 * v56 * v78 * v10;
		fuvals[ihstrt + 47] = v1 * v34 * v56 * v78 * v9;
		fuvals[ihstrt + 9] = v12 * v56 * v78 * v910;
		fuvals[ihstrt + 13] = v12 * v4 * v6 * v78 * v910;
		fuvals[ihstrt + 18] = v12 * v4 * v5 * v78 * v910;
		fuvals[ihstrt + 24] = v12 * v4 * v56 * v8 * v910;
		fuvals[ihstrt + 31] = v12 * v4 * v56 * v7 * v910;
		fuvals[ihstrt + 39] = v12 * v4 * v56 * v78 * v10;
		fuvals[ihstrt + 48] = v12 * v4 * v56 * v78 * v9;
		fuvals[ihstrt + 14] = v12 * v3 * v6 * v78 * v910;
		fuvals[ihstrt + 19] = v12 * v3 * v5 * v78 * v910;
		fuvals[ihstrt + 25] = v12 * v3 * v56 * v8 * v910;
		fuvals[ihstrt + 32] = v12 * v3 * v56 * v7 * v910;
		fuvals[ihstrt + 40] = v12 * v3 * v56 * v78 * v10;
		fuvals[ihstrt + 49] = v12 * v3 * v56 * v78 * v9;
		fuvals[ihstrt + 20] = v12 * v34 * v78 * v910;
		fuvals[ihstrt + 26] = v12 * v34 * v6 * v8 * v910;
		fuvals[ihstrt + 33] = v12 * v34 * v6 * v7 * v910;
		fuvals[ihstrt + 41] = v12 * v34 * v6 * v78 * v10;
		fuvals[ihstrt + 50] = v12 * v34 * v6 * v78 * v9;
		fuvals[ihstrt + 27] = v12 * v34 * v5 * v8 * v910;
		fuvals[ihstrt + 34] = v12 * v34 * v5 * v7 * v910;
		fuvals[ihstrt + 42] = v12 * v34 * v5 * v78 * v10;
		fuvals[ihstrt + 51] = v12 * v34 * v5 * v78 * v9;
		fuvals[ihstrt + 35] = v12 * v34 * v56 * v910;
		fuvals[ihstrt + 43] = v12 * v34 * v56 * v8 * v10;
		fuvals[ihstrt + 52] = v12 * v34 * v56 * v8 * v9;
		fuvals[ihstrt + 44] = v12 * v34 * v56 * v7 * v10;
		fuvals[ihstrt + 53] = v12 * v34 * v56 * v7 * v9;
		fuvals[ihstrt + 54] = v12 * v34 * v56 * v78;
		fuvals[ihstrt + 1] = 0.;
		fuvals[ihstrt + 3] = 0.;
		fuvals[ihstrt + 6] = 0.;
		fuvals[ihstrt + 10] = 0.;
		fuvals[ihstrt + 15] = 0.;
		fuvals[ihstrt + 21] = 0.;
		fuvals[ihstrt + 28] = 0.;
		fuvals[ihstrt + 36] = 0.;
		fuvals[ihstrt + 45] = 0.;
		fuvals[ihstrt + 55] = 0.;
	    }
	}
/* L2: */
    }
    return 0;
} /* elfun_ */

elfunType elfunBROWNAL_=_elfunBROWNAL_;

/* Subroutine */ int _groupBROWNAL_(doublereal *gvalue, integer *lgvalu, doublereal *
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


/*  PROBLEM NAME : BROWNAL */

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

groupType groupBROWNAL_=_groupBROWNAL_;
