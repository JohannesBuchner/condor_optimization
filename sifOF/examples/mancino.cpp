/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunMANCINO_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal b, x, a1, a2, a3, v2, al, ii, jj, cij, lij, sal, sij, 
	    vij, dbdx, dcdx, dsal, scij, dvij, dsdx, dscij, invij, sumal;
    static integer jcalcf, ielemn;
    static doublereal dsumal;
    static integer igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : MANCINO */

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

/*  ELEMENT TYPE : MANC */

	x = xvalue[ielvar[ilstrt + 1]];
	ii = epvalu[ipstrt + 1];
	jj = epvalu[ipstrt + 2];
	al = epvalu[ipstrt + 3];
	a1 = al - 1.f;
	a2 = al - 2.f;
	a3 = al - 3.f;
	invij = x * x + ii / jj;
	vij = sqrt(invij);
	v2 = vij * vij;
	dvij = x / vij;
	lij = log(vij);
	sij = sin(lij);
	dsdx = cij * dvij / vij;
	cij = cos(lij);
	dcdx = -sij * dvij / vij;
	sumal = pow_dd(&sij, &al) + pow_dd(&cij, &al);
	dsumal = al * (dsdx * pow_dd(&sij, &a1) + dcdx * pow_dd(&cij, &a1));
	scij = sij * cij;
	dscij = sij * dcdx + dsdx * cij;
	sal = pow_dd(&sij, &a2) - pow_dd(&cij, &a2);
	dsal = a2 * (dsdx * pow_dd(&sij, &a3) - dcdx * pow_dd(&cij, &a3));
	b = sumal + al * scij * sal;
	dbdx = dsumal + al * (dscij * sal + scij * dsal);
	if (*ifflag == 1) {
	    fuvals[ielemn] = vij * sumal;
	} else {
	    fuvals[igstrt + 1] = x * b / vij;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = (b + x * dbdx) / vij - b * x * dvij / v2;
	    }
	}
/* L2: */
    }
    return 0;
} /* elfun_ */

elfunType elfunMANCINO_=_elfunMANCINO_;

/* Subroutine */ int _groupMANCINO_(doublereal *gvalue, integer *lgvalu, doublereal *
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


/*  PROBLEM NAME : MANCINO */

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

groupType groupMANCINO_=_groupMANCINO_;
