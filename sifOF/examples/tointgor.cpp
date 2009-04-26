/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunTOINTGOR_(doublereal *fuvals, doublereal *xvalue, 
	doublereal *epvalu, integer *ncalcf, integer *itypee, integer *istaev,
	 integer *ielvar, integer *intvar, integer *istadh, integer *istepa, 
	integer *icalcf, integer *ltypee, integer *lstaev, integer *lelvar, 
	integer *lntvar, integer *lstadh, integer *lstepa, integer *lcalcf, 
	integer *lfvalu, integer *lxvalu, integer *lepvlu, integer *ifflag, 
	integer *ifstat)
{

/*  PROBLEM NAME : TOINTGOR */

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

elfunType elfunTOINTGOR_=_elfunTOINTGOR_;

/* Subroutine */ int _groupTOINTGOR_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
 /* System generated locals */
    integer gvalue_dim1, gvalue_offset, i__1;
    doublereal d__1;

    /* Local variables */
    static doublereal t, aa, ag, at, at1, lat, one, tneg, zero, tpos;
    static integer jcalcg, igroup, igrtyp, ipstrt;


/*  PROBLEM NAME : TOINTGOR */

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
    one = 1.;
    zero = 0.;
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
	at = abs(t);
	at1 = at + one;
	lat = log(at1);
	aa = at / at1;
	ag = aa + lat;
	if (! (*derivs)) {
	    gvalue[igroup + gvalue_dim1] = at * lat;
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = d_sign(&ag, &t);
	    gvalue[igroup + gvalue_dim1 * 3] = (2.f - aa) / at1;
	}
	goto L3;

/*  GROUP TYPE : BBT */

L2:
	t = fvalue[igroup];
	at = abs(t);
	at1 = at + one;
	lat = log(at1);
	aa = at / at1;
/* Computing MAX */
	d__1 = d_sign(&one, &t);
	tpos = max(d__1,zero);
	tneg = one - tpos;
	if (! (*derivs)) {
	    gvalue[igroup + gvalue_dim1] = t * t * (tneg + tpos * lat);
	} else {
	    gvalue[igroup + (gvalue_dim1 << 1)] = tneg * 2.f * t + tpos * t * 
		    (aa + lat * 2.f);
	    gvalue[igroup + gvalue_dim1 * 3] = tneg * 2.f + tpos * (aa * (4.f 
		    - aa) + lat * 2.f);
	}
L3:
	;
    }
    return 0;
}

groupType groupTOINTGOR_=_groupTOINTGOR_;
