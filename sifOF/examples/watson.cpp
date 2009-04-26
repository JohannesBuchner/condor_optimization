/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunWATSON_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal u, t1, t2, v1, v2, v3, v4, v5, v6, v7, v8, v9, t3, t4, 
	    t5, t6, t7, t8, t9, t10, t11, v10, v11, v12, t12, twot1, twot2, 
	    twot3, twot4, twot5, twot6, twot7, twot8, twot9, twot10, twot11, 
	    twot12;
    static integer jcalcf, ielemn, ieltyp, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : WATSON */

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

/*  ELEMENT TYPE : MSQ */

L2:
	v1 = xvalue[ielvar[ilstrt + 1]];
	if (*ifflag == 1) {
	    fuvals[ielemn] = -v1 * v1;
	} else {
	    fuvals[igstrt + 1] = -v1 - v1;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = -2.f;
	    }
	}
	goto L3;

/*  ELEMENT TYPE : MWSQ */

L1:
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
	v11 = xvalue[ielvar[ilstrt + 11]];
	v12 = xvalue[ielvar[ilstrt + 12]];
	t1 = epvalu[ipstrt + 1];
	t2 = epvalu[ipstrt + 2];
	t3 = epvalu[ipstrt + 3];
	t4 = epvalu[ipstrt + 4];
	t5 = epvalu[ipstrt + 5];
	t6 = epvalu[ipstrt + 6];
	t7 = epvalu[ipstrt + 7];
	t8 = epvalu[ipstrt + 8];
	t9 = epvalu[ipstrt + 9];
	t10 = epvalu[ipstrt + 10];
	t11 = epvalu[ipstrt + 11];
	t12 = epvalu[ipstrt + 12];
	u = t1 * v1 + t2 * v2 + t3 * v3 + t4 * v4 + t5 * v5 + t6 * v6 + t7 * 
		v7 + t8 * v8 + t9 * v9 + t10 * v10 + t11 * v11 + t12 * v12;
	twot1 = t1 + t1;
	twot2 = t2 + t2;
	twot3 = t3 + t3;
	twot4 = t4 + t4;
	twot5 = t5 + t5;
	twot6 = t6 + t6;
	twot7 = t7 + t7;
	twot8 = t8 + t8;
	twot9 = t9 + t9;
	twot10 = t10 + t10;
	twot11 = t11 + t11;
	twot12 = t12 + t12;
	if (*ifflag == 1) {
	    fuvals[ielemn] = -u * u;
	} else {
	    fuvals[igstrt + 1] = -twot1 * u;
	    fuvals[igstrt + 2] = -twot2 * u;
	    fuvals[igstrt + 3] = -twot3 * u;
	    fuvals[igstrt + 4] = -twot4 * u;
	    fuvals[igstrt + 5] = -twot5 * u;
	    fuvals[igstrt + 6] = -twot6 * u;
	    fuvals[igstrt + 7] = -twot7 * u;
	    fuvals[igstrt + 8] = -twot8 * u;
	    fuvals[igstrt + 9] = -twot9 * u;
	    fuvals[igstrt + 10] = -twot10 * u;
	    fuvals[igstrt + 11] = -twot11 * u;
	    fuvals[igstrt + 12] = -twot12 * u;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = -twot1 * t1;
		fuvals[ihstrt + 2] = -twot1 * t2;
		fuvals[ihstrt + 4] = -twot1 * t3;
		fuvals[ihstrt + 7] = -twot1 * t4;
		fuvals[ihstrt + 11] = -twot1 * t5;
		fuvals[ihstrt + 16] = -twot1 * t6;
		fuvals[ihstrt + 22] = -twot1 * t7;
		fuvals[ihstrt + 29] = -twot1 * t8;
		fuvals[ihstrt + 37] = -twot1 * t9;
		fuvals[ihstrt + 46] = -twot1 * t10;
		fuvals[ihstrt + 56] = -twot1 * t11;
		fuvals[ihstrt + 67] = -twot1 * t12;
		fuvals[ihstrt + 3] = -twot2 * t2;
		fuvals[ihstrt + 5] = -twot2 * t3;
		fuvals[ihstrt + 8] = -twot2 * t4;
		fuvals[ihstrt + 12] = -twot2 * t5;
		fuvals[ihstrt + 17] = -twot2 * t6;
		fuvals[ihstrt + 23] = -twot2 * t7;
		fuvals[ihstrt + 30] = -twot2 * t8;
		fuvals[ihstrt + 38] = -twot2 * t8;
		fuvals[ihstrt + 47] = -twot2 * t10;
		fuvals[ihstrt + 57] = -twot2 * t11;
		fuvals[ihstrt + 68] = -twot2 * t12;
		fuvals[ihstrt + 6] = -twot3 * t3;
		fuvals[ihstrt + 9] = -twot3 * t4;
		fuvals[ihstrt + 13] = -twot3 * t5;
		fuvals[ihstrt + 18] = -twot3 * t6;
		fuvals[ihstrt + 24] = -twot3 * t7;
		fuvals[ihstrt + 31] = -twot3 * t8;
		fuvals[ihstrt + 39] = -twot3 * t8;
		fuvals[ihstrt + 48] = -twot3 * t10;
		fuvals[ihstrt + 58] = -twot3 * t11;
		fuvals[ihstrt + 69] = -twot3 * t12;
		fuvals[ihstrt + 10] = -twot4 * t4;
		fuvals[ihstrt + 14] = -twot4 * t5;
		fuvals[ihstrt + 19] = -twot4 * t6;
		fuvals[ihstrt + 25] = -twot4 * t7;
		fuvals[ihstrt + 32] = -twot4 * t8;
		fuvals[ihstrt + 40] = -twot4 * t8;
		fuvals[ihstrt + 49] = -twot4 * t10;
		fuvals[ihstrt + 59] = -twot4 * t11;
		fuvals[ihstrt + 70] = -twot4 * t12;
		fuvals[ihstrt + 15] = -twot5 * t5;
		fuvals[ihstrt + 20] = -twot5 * t6;
		fuvals[ihstrt + 26] = -twot5 * t7;
		fuvals[ihstrt + 33] = -twot5 * t8;
		fuvals[ihstrt + 41] = -twot5 * t8;
		fuvals[ihstrt + 50] = -twot5 * t10;
		fuvals[ihstrt + 60] = -twot5 * t11;
		fuvals[ihstrt + 71] = -twot5 * t12;
		fuvals[ihstrt + 21] = -twot6 * t6;
		fuvals[ihstrt + 27] = -twot6 * t7;
		fuvals[ihstrt + 34] = -twot6 * t8;
		fuvals[ihstrt + 42] = -twot6 * t8;
		fuvals[ihstrt + 51] = -twot6 * t10;
		fuvals[ihstrt + 61] = -twot6 * t11;
		fuvals[ihstrt + 72] = -twot6 * t12;
		fuvals[ihstrt + 28] = -twot7 * t7;
		fuvals[ihstrt + 35] = -twot7 * t8;
		fuvals[ihstrt + 43] = -twot7 * t8;
		fuvals[ihstrt + 52] = -twot7 * t10;
		fuvals[ihstrt + 62] = -twot7 * t11;
		fuvals[ihstrt + 73] = -twot7 * t12;
		fuvals[ihstrt + 36] = -twot8 * t8;
		fuvals[ihstrt + 44] = -twot8 * t8;
		fuvals[ihstrt + 53] = -twot8 * t10;
		fuvals[ihstrt + 63] = -twot8 * t11;
		fuvals[ihstrt + 74] = -twot8 * t12;
		fuvals[ihstrt + 45] = -twot9 * t9;
		fuvals[ihstrt + 54] = -twot9 * t10;
		fuvals[ihstrt + 64] = -twot9 * t11;
		fuvals[ihstrt + 75] = -twot9 * t12;
		fuvals[ihstrt + 55] = -twot10 * t10;
		fuvals[ihstrt + 65] = -twot10 * t11;
		fuvals[ihstrt + 76] = -twot10 * t12;
		fuvals[ihstrt + 66] = -twot11 * t11;
		fuvals[ihstrt + 77] = -twot11 * t12;
		fuvals[ihstrt + 78] = -twot12 * t12;
	    }
	}
L3:
	;
    }
    return 0;
} /* elfun_ */

elfunType elfunWATSON_=_elfunWATSON_;

/* Subroutine */ int _groupWATSON_(doublereal *gvalue, integer *lgvalu, doublereal *
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


/*  PROBLEM NAME : WATSON */

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

groupType groupWATSON_=_groupWATSON_;
