/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunTOINTPSP_(doublereal *fuvals, doublereal *xvalue, 
	doublereal *epvalu, integer *ncalcf, integer *itypee, integer *istaev,
	 integer *ielvar, integer *intvar, integer *istadh, integer *istepa, 
	integer *icalcf, integer *ltypee, integer *lstaev, integer *lelvar, 
	integer *lntvar, integer *lstadh, integer *lstepa, integer *lcalcf, 
	integer *lfvalu, integer *lxvalu, integer *lepvlu, integer *ifflag, 
	integer *ifstat)
{/*  PROBLEM NAME : TOINTPSP */

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
    return 0;
} /* elfun_ */

elfunType elfunTOINTPSP_=_elfunTOINTPSP_;

int _groupTOINTPSP_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset, i__1;
    doublereal d__1;

    /* Local variables */
    static doublereal t, ff, gg, hh;
    static logical tpos;
    static integer jcalcg, igroup, igrtyp, ipstrt;


/*  PROBLEM NAME : TOINTPSP */

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
	    goto L3;
	}
	ipstrt = istgpa[igroup] - 1;
	switch (igrtyp) {
	    case 1:  goto L1;
	    case 2:  goto L2;
	}

/*  GROUP TYPE : ACT */

L1:
	t = fvalue[igroup];
	if (! (*derivs)) {
/* Computing 2nd power */
	    d__1 = t - 5.f;
	    gvalue[igroup + gvalue_dim1] = d__1 * d__1;
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = t * 2.f - 10.f;
	    gvalue[igroup + gvalue_dim1 * 3] = 2.f;
	}
	goto L3;

/*  GROUP TYPE : BBT */

L2:
	t = fvalue[igroup];
	tpos = t >= .1f;
	if (tpos) {
	    ff = 1.f / t;
	}
	if (! tpos) {
	    ff = 20.f - t * 100.f;
	}
	if (tpos) {
/* Computing 2nd power */
	    d__1 = t;
	    gg = -1.f / (d__1 * d__1);
	}
	if (! tpos) {
	    gg = -100.f;
	}
	if (tpos) {
/* Computing 3rd power */
	    d__1 = t;
	    hh = 2.f / (d__1 * (d__1 * d__1));
	}
	if (! tpos) {
	    hh = 0.f;
	}
	if (! (*derivs)) {
	    gvalue[igroup + gvalue_dim1] = ff;
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = gg;
	    gvalue[igroup + gvalue_dim1 * 3] = hh;
	}
L3:
	;
    }
    return 0;
} /* group_ */

groupType groupTOINTPSP_=_groupTOINTPSP_;
