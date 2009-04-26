/* D:\my projects\L2parallel\SIF\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int _elfunSNAIL_(doublereal *fuvals, doublereal *xvalue, 
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
    static doublereal a, b, c__, d__, r__, s, u, v, x, y, d2, d3, r2, r3, r4, 
	    x2, y2, cl, cu, arg, dcdx, dcdy, drdx, drdy, dtdx, dtdy, dudx, 
	    dudy, dvdx, dvdy, d2cdx2, d2cdy2, d2rdx2, d2rdy2, theta, d2tdx2, 
	    d2tdy2, d2udx2, d2udy2, d2vdx2, d2vdy2, d2cdxy, d2rdxy, d2tdxy, 
	    d2udxy, d2vdxy;
    static integer jcalcf, ielemn, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : SNAIL */

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

/*  ELEMENT TYPE : SPIRAL */

	x = xvalue[ielvar[ilstrt + 1]];
	y = xvalue[ielvar[ilstrt + 2]];
	cl = epvalu[ipstrt + 1];
	cu = epvalu[ipstrt + 2];
	a = (cu + cl) * .5f;
	b = (cu - cl) * .5f;
	x2 = x * x;
	y2 = y * y;
	r2 = x2 + y2;
	d__ = r2 + 1.f;
	d2 = d__ * d__;
	d3 = d2 * d__;
	u = r2 / d__;
	dudx = (x + x) / d2;
	dudy = (y + y) / d2;
	d2udx2 = (d__ - x2 * 4.f) * 2.f / d3;
	d2udy2 = (d__ - y2 * 4.f) * 2.f / d3;
	d2udxy = x * -8.f * y / d3;
	theta = atan2(y, x);
	dtdx = -y / r2;
	dtdy = x / r2;
	r4 = r2 * r2;
	d2tdx2 = x * 2.f * y / r4;
	d2tdy2 = y * -2.f * x / r4;
	d2tdxy = (y2 - x2) / r4;
	r__ = sqrt(r2);
	r3 = r__ * r2;
	drdx = x / r__;
	drdy = y / r__;
	d2rdx2 = y2 / r3;
	d2rdy2 = x2 / r3;
	d2rdxy = -x * y / r3;
	arg = r__ - theta;
	s = b * sin(arg);
	c__ = b * cos(arg);
	dcdx = -s * (drdx - dtdx);
	dcdy = -s * (drdy - dtdy);
/* Computing 2nd power */
	d__1 = drdx - dtdx;
	d2cdx2 = -c__ * (d__1 * d__1) - s * (d2rdx2 - d2tdx2);
/* Computing 2nd power */
	d__1 = drdy - dtdy;
	d2cdy2 = -c__ * (d__1 * d__1) - s * (d2rdy2 - d2tdy2);
	d2cdxy = -c__ * (drdx - dtdx) * (drdy - dtdy) - s * (d2rdxy - d2tdxy);
	v = a * r__ + 1.f - r__ * c__;
	dvdx = a * drdx - drdx * c__ - r__ * dcdx;
	dvdy = a * drdy - drdy * c__ - r__ * dcdy;
	d2vdx2 = a * d2rdx2 - d2rdx2 * c__ - drdx * 2.f * dcdx - r__ * d2cdx2;
	d2vdy2 = a * d2rdy2 - d2rdy2 * c__ - drdy * 2.f * dcdy - r__ * d2cdy2;
	d2vdxy = a * d2rdxy - d2rdxy * c__ - drdx * dcdy - drdy * dcdx - r__ *
		 d2cdxy;
	if (*ifflag == 1) {
	    fuvals[ielemn] = u * v;
	} else {
	    fuvals[igstrt + 1] = dudx * v + u * dvdx;
	    fuvals[igstrt + 2] = dudy * v + u * dvdy;
	    if (*ifflag == 3) {
		fuvals[ihstrt + 1] = d2udx2 * v + dudx * 2.f * dvdx + u * 
			d2vdx2;
		fuvals[ihstrt + 2] = d2udxy * v + dudx * dvdy + dudy * dvdx + 
			u * d2vdxy;
		fuvals[ihstrt + 3] = d2udy2 * v + dudy * 2.f * dvdy + u * 
			d2vdy2;
	    }
	}
/* L2: */
    }
    return 0;
} /* elfun_ */

elfunType elfunSNAIL_=_elfunSNAIL_;

/* Subroutine */ int _groupSNAIL_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset;


/*  PROBLEM NAME : SNAIL */

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
    return 0;
} /* group_ */

groupType groupSNAIL_=_groupSNAIL_;












