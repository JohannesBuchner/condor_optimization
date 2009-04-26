/* D:\my projects\sifReader\akiva\ELFUN.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "../SIFFunction.h"

/* Subroutine */ int A_elfunAkiva_(doublereal *fuvals, doublereal *xvalue, 
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
    extern doublereal f_(doublereal *, doublereal *), g_(doublereal *, 
	    doublereal *, integer *), h_(doublereal *, doublereal *, integer *
	    , integer *);
    static doublereal b1, b2;
    static integer jcalcf, ielemn, igstrt, ihstrt, ilstrt, ipstrt;


/*  PROBLEM NAME : AKIVA */

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

/*  ELEMENT TYPE : LIKE */

	b1 = xvalue[ielvar[ilstrt + 1]];
	b2 = xvalue[ielvar[ilstrt + 2]];
	if (*ifflag == 1) {
	    fuvals[ielemn] = f_(&b1, &b2);
    } else 
    {
        printf("error.\n"); exit(1);
    }
/* L2: */
    }
    return 0;
} /* elfun_ */

elfunType elfunAkiva_=A_elfunAkiva_;

/* D:\my projects\sifReader\akiva\EXTER.f -- translated by f2c (version 20030306).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

static logical c_false = FALSE_;


doublereal f_(doublereal *b1, doublereal *b2)
{
    /* System generated locals */
    doublereal ret_val;

    /* Local variables */
    static integer nc;
    static doublereal gr[2], beta[2], like, hess[4]	/* was [2][2] */;
    static logical compg, comph;
    extern /* Subroutine */ int hielow_(logical *, logical *, doublereal *, 
	    logical *, logical *, doublereal *, doublereal *, doublereal *, 
	    integer *);






    beta[0] = *b1;
    beta[1] = *b2;

    nc = 2;
    hielow_(&c_false, &c_false, beta, &compg, &comph, &like, gr, hess, &nc);
    compg = TRUE_;
    comph = TRUE_;
    ret_val = like;
    return ret_val;
} /* f_ */

/* ======================================================= */
/*     Copyright (c) FUNDP - Namur 1995 */

/*     Subroutine HieLoW */

/*     Author : Michel Bierlaire */
/*     Creation  : January 1995 */

/* Subroutine */ int hielow_(logical *cgrad, logical *chess, doublereal *beta,
	 logical *compg, logical *comph, doublereal *like, doublereal *gr, 
	doublereal *hess, integer *nc)
{
    /* Initialized data */

    static integer utcara[6]	/* was [3][2] */ = { 2,1,0,0,3 };
    static integer utleng[3] = { 1,2,0 };
    static integer theta[3] = { -1,-1,-2 };
    static logical disp[63]	/* was [21][3] */ = { TRUE_,TRUE_,TRUE_,TRUE_,
	    TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,
	    TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,
	    TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,
	    TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,
	    TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,TRUE_,
	    TRUE_,TRUE_,TRUE_,TRUE_ };
    static integer chosen[21] = { 1,1,2,1,1,2,2,1,1,1,1,2,2,1,2,2,1,2,2,1,2 };
    static doublereal caract[63]	/* was [21][3] */ = { 52.9,4.1,4.1,
	    56.2,51.8,.2,27.6,89.9,41.5,95.,99.1,18.5,82.,8.6,22.5,51.4,81.,
	    51.,62.2,95.1,41.6,4.4,28.5,86.9,31.6,20.2,91.2,79.7,2.2,24.5,
	    43.5,8.4,84.,38.,1.6,74.1,83.8,19.2,85.,90.1,22.2,91.5,1.,1.,1.,
	    1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,1. };
    static integer arbre[3] = { 3,3,-1 };
    static integer utcoef[6]	/* was [3][2] */ = { 1,1,0,0,2 };

    /* System generated locals */
    integer hess_dim1, hess_offset, i__1;

    /* Local variables */
    static integer i__, j, obs;
    static doublereal vij[3], l1ij[3], l2ij[3], util[3], logar[3], deriv[6]	
	    /* was [3][2] */;
    static integer noeud;
    static doublereal logit[3], loggra, derlog[6]	/* was [3][2] */, 
	    somexp[3], exputi[3];


/*     Computation of the loglikelihood function of a hierarchical logit */

/*     Data structures */








/*     Tree structure description */
/*     ARBRE(I) = J  <==> Node J is the father of node I */
/*     ARBRE(I) = -1 <==> I is the root of the tree */
/*     Assertion : ARBRE(I) = J ==> J > I */

    /* Parameter adjustments */
    hess_dim1 = *nc;
    hess_offset = 1 + hess_dim1;
    hess -= hess_offset;
    --gr;
    --beta;

    /* Function Body */

/*     Utility function */


/*     Node 1 : Transit */


/*     Node 2 : Auto */


/*     Node 3 : RACINE */


/*     Structural coefficients */

/*     THETA(I) > 0  ==> I structural node */
/*     THETA(I) = -1 ==> I elemental node */
/*     THETA(I) = -2 ==> I root */


/*     Observations */


/*     Observation 1 */


/*     Observation 2 */


/*     Observation 3 */


/*     Observation 4 */


/*     Observation 5 */


/*     Observation 6 */


/*     Observation 7 */


/*     Observation 8 */


/*     Observation 9 */


/*     Observation 10 */


/*     Observation 11 */


/*     Observation 12 */


/*     Observation 13 */


/*     Observation 14 */


/*     Observation 15 */


/*     Observation 16 */


/*     Observation 17 */


/*     Observation 18 */


/*     Observation 19 */


/*     Observation 20 */


/*     Observation 21 */


/*     Function computation */

/*     Initialize */

    loggra = log(1e308);
    *like = 0.;
    if (*cgrad || *chess) {
	for (i__ = 1; i__ <= 2; ++i__) {
	    gr[i__] = 0.;
	    if (*chess) {
		for (j = i__; j <= 2; ++j) {
		    hess[i__ + j * hess_dim1] = 0.;
/* L910: */
		}
	    }
/* L900: */
	}
    }

/*     Loop on the observations */

    for (obs = 1; obs <= 21; ++obs) {
	for (noeud = 1; noeud <= 3; ++noeud) {
	    somexp[noeud - 1] = 0.;
/* L1015: */
	}
	for (noeud = 1; noeud <= 3; ++noeud) {
	    if (disp[obs + noeud * 21 - 22]) {

/*     Utility function */

		if (arbre[noeud - 1] != -1) {
		    util[noeud - 1] = 0.;
		    i__1 = utleng[noeud - 1];
		    for (i__ = 1; i__ <= i__1; ++i__) {
			util[noeud - 1] += beta[utcoef[noeud + i__ * 3 - 4]] *
				 caract[obs + utcara[noeud + i__ * 3 - 4] * 
				21 - 22];
/* L1020: */
		    }

/*     Check overflow */

		    if (util[noeud - 1] >= loggra) {
			util[noeud - 1] = loggra;
		    }
		}

/*     Inclusive value - expected maximum utility */

		if (theta[noeud - 1] != -1) {
		    logar[noeud - 1] = log(somexp[noeud - 1]);
		    if (theta[noeud - 1] > 0) {
			util[noeud - 1] += beta[theta[noeud - 1]] * logar[
				noeud - 1];
		    }
		} else {
		    somexp[noeud - 1] = 0.;
		}

/*     Exp(utility) */

		if (util[noeud - 1] >= loggra) {
		    exputi[noeud - 1] = 1e308;
		} else {
		    exputi[noeud - 1] = exp(util[noeud - 1]);
		}

/*     Accumulate in the father node and check overflow */

		if (arbre[noeud - 1] != -1) {
		    if (somexp[arbre[noeud - 1] - 1] != 1e308) {
			if (exputi[noeud - 1] == 1e308) {
			    somexp[arbre[noeud - 1] - 1] = 1e308;
			} else {
			    somexp[arbre[noeud - 1] - 1] += exputi[noeud - 1];
			    if (somexp[arbre[noeud - 1] - 1] > 1e308) {
				somexp[arbre[noeud - 1] - 1] = 1e308;
			    }
			}
		    }
		}
	    }
/* L1010: */
	}

/*     Compute the function */

	noeud = chosen[obs - 1];
L1050:
	if (arbre[noeud - 1] != -1) {
	    *like = *like + util[noeud - 1] - logar[arbre[noeud - 1] - 1];
	    noeud = arbre[noeud - 1];
	    goto L1050;
	}

/*     Compute derivatives, if required */

	if (*cgrad || *chess) {

/*     Gradient */

/*     Initialize */

	    for (noeud = 1; noeud <= 3; ++noeud) {
		for (i__ = 1; i__ <= 2; ++i__) {
		    deriv[noeud + i__ * 3 - 4] = 0.;
		    derlog[noeud + i__ * 3 - 4] = 0.;
/* L1150: */
		}

/*     Derivatives of the elemental utility */

		if (arbre[noeud - 1] != -1) {
		    i__1 = utleng[noeud - 1];
		    for (i__ = 1; i__ <= i__1; ++i__) {
			deriv[noeud + utcoef[noeud + i__ * 3 - 4] * 3 - 4] = 
				caract[obs + utcara[noeud + i__ * 3 - 4] * 21 
				- 22];
/* L1170: */
		    }
		}
/* L1100: */
	    }

/*     Derivatives of the inclusive value */

	    for (noeud = 1; noeud <= 2; ++noeud) {
		if (disp[obs + noeud * 21 - 22]) {

/*     Equation (6) of Daly */

		    logit[noeud - 1] = exputi[noeud - 1] / somexp[arbre[noeud 
			    - 1] - 1];
		    if (theta[noeud - 1] > 0) {

/*     Derivatives wrt structural coefficient */

			deriv[noeud + theta[noeud - 1] * 3 - 4] += logar[
				noeud - 1];
			for (i__ = 1; i__ <= 2; ++i__) {

/*     Equation (23) */

			    if (i__ != theta[noeud - 1]) {
				deriv[noeud + i__ * 3 - 4] += beta[theta[
					noeud - 1]] * derlog[noeud + i__ * 3 
					- 4];
			    }

/*     Equation (21) */

			    derlog[arbre[noeud - 1] + i__ * 3 - 4] += logit[
				    noeud - 1] * deriv[noeud + i__ * 3 - 4];
/* L1250: */
			}
		    } else {

/*     Elemental node */

			for (i__ = 1; i__ <= 2; ++i__) {
			    derlog[arbre[noeud - 1] + i__ * 3 - 4] += logit[
				    noeud - 1] * deriv[noeud + i__ * 3 - 4];
/* L1300: */
			}
		    }
		}
/* L1200: */
	    }

/*     Derivatives */

	    if (*cgrad) {
		noeud = chosen[obs - 1];
L1350:
		if (arbre[noeud - 1] != -1) {
		    for (i__ = 1; i__ <= 2; ++i__) {
			gr[i__] = gr[i__] + deriv[noeud + i__ * 3 - 4] - 
				derlog[arbre[noeud - 1] + i__ * 3 - 4];
/* L1400: */
		    }
		    noeud = arbre[noeud - 1];
		    goto L1350;
		}
	    }
	}

/*     Compute Hessian matrix, if required */

	if (*chess) {
	    for (i__ = 1; i__ <= 2; ++i__) {
		for (j = i__; j <= 2; ++j) {

/*     Initialize */

		    for (noeud = 1; noeud <= 3; ++noeud) {
			l1ij[noeud - 1] = 0.;
			l2ij[noeud - 1] = 0.;
/* L2020: */
		    }
		    for (noeud = 1; noeud <= 2; ++noeud) {
			if (disp[obs + noeud * 21 - 22]) {
			    if (i__ == theta[noeud - 1]) {
				vij[noeud - 1] = derlog[noeud + j * 3 - 4];
			    } else if (j == theta[noeud - 1]) {
				vij[noeud - 1] = derlog[noeud + i__ * 3 - 4];
			    } else if (theta[noeud - 1] != -1) {
				vij[noeud - 1] = beta[theta[noeud - 1]] * (
					l1ij[noeud - 1] + l2ij[noeud - 1] - 
					derlog[noeud + i__ * 3 - 4] * derlog[
					noeud + j * 3 - 4]);
			    } else {
				vij[noeud - 1] = 0.;
			    }
			    l1ij[arbre[noeud - 1] - 1] += logit[noeud - 1] * 
				    deriv[noeud + i__ * 3 - 4] * deriv[noeud 
				    + j * 3 - 4];
			    l2ij[arbre[noeud - 1] - 1] += logit[noeud - 1] * 
				    vij[noeud - 1];
			}
/* L2030: */
		    }

/*     Hessian */

		    noeud = chosen[obs - 1];
L3350:
		    if (arbre[noeud - 1] != -1) {
			hess[i__ + j * hess_dim1] = hess[i__ + j * hess_dim1] 
				+ vij[noeud - 1] - l2ij[arbre[noeud - 1] - 1] 
				+ derlog[arbre[noeud - 1] + i__ * 3 - 4] * 
				derlog[arbre[noeud - 1] + j * 3 - 4] - l1ij[
				arbre[noeud - 1] - 1];
			noeud = arbre[noeud - 1];
			goto L3350;
		    }
/* L2010: */
		}
/* L2000: */
	    }
	}
/* L1000: */
    }
    return 0;
} /* hielow_ */

/* Subroutine */ int _groupAkiva_(doublereal *gvalue, integer *lgvalu, doublereal *
	fvalue, doublereal *gpvalu, integer *ncalcg, integer *itypeg, integer 
	*istgpa, integer *icalcg, integer *ltypeg, integer *lstgpa, integer *
	lcalcg, integer *lfvalu, integer *lgpvlu, logical *derivs, integer *
	igstat)
{
    /* System generated locals */
    integer gvalue_dim1, gvalue_offset;


/*  PROBLEM NAME : AKIVA */

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

groupType groupAkiva_=_groupAkiva_;
