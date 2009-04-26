#include "nlp.h"
#include "r_opn.hd"
#ifndef f_MINLIST	/* TEMPORARY: now in updated r_opn.hd */
#define f_MINLIST	r_ops[11]
#define f_MAXLIST	r_ops[12]
#define f_ABS		r_ops[15]
#endif
#include "getstub.h"

static real objsign;

void obj(np, j, x, fj, cd) int np; int j; real *x; real *fj; char *cd;
{
	*fj = objsign * objval(i, x, NULL);
}

double constr(int np, int j, real *x, real *c, void *cd)
{
	ASL *asl = (ASL*)cd;

	if (j < nNLOppositeConstraints) 
        return RHSNLConstraint[j] - conival(j+iBaseNLContraints, x, NULL);
	return conival(j+iBaseNLContraints, x, NULL) - RHSNLConstraint[j];
}

void gradcon(int np, int j, real *x, real *J, Dummy d, void *cd)
{
	ASL *asl = (ASL*)cd;
    congrd(j+iBaseNLContraints, x, J, 0);
	if (j < nNLOppositeConstraints) J.multiply(-1.0); 
}

/*
static int maxiter = 200, objno = 1;
static int always, iprint, monotone;
static real eps = 1e-8, epseqn = 1e-8;

 static keyword
keywds[] = {	//
 KW("always",	I_val,	&always,	"always check feasibility in linesearch  (0 = no) [mode: C]"),
 KW("eps",	D_val,	&eps,		"final Newton dir. norm (1e-8) [eps]"),
 KW("epseqn",	D_val,	&epseqn,	"max. nonlin. equality constraint violation (1e-8) [epseqn]"),
 KW("glgeps",	D_val,	&gLgeps,	"tol. for norm of Lagrangian (-1) [gLeps]"),
 KW("iprint",	I_val,	&iprint,	"0, 1, 2, 3, or 10*M+{2,3} (0) [iprint]"),
 KW("maxiter",	I_val,	&maxiter,	"maximum iterations (default 200) [miter]"),
 KW("monotone",	I_val,	&monotone,	"monotone iterates (0 = no) [mode: B]"),
 KW("objeps",	D_val,	&objeps,	"abs func change tol for probs with no nonlin eqns (-1) [objeps]"),
 KW("objno",	I_val,	&objno, 	"objective (1 = first)"),
 KW("objrep",	D_val,	&objrep,	"rel func change tol for probs with no nonlin eqns (-1) [objrep]"),
 KW("outlev",	I_val,	&iprint,	"synonym for iprint [iprint]"),
 KW("wantsol",	WS_val,	0,		WS_desc_ASL+5)
 };
*/

static char *usage_msg[] = {
  "\n	See Sections 4 and 5 of the CFSQP manual for details on",
  "	many of the  name=value  assignment possibilities; names",
  "	in square [brackets] when you say \"fsqp -=\" are used in",
  "	the CFSQP manual.",
  0};

static Option_Info Oinfo = {
	"fsqp", "CFSQP 2.5", "fsqp_options", keywds, nkeywds, 1,
	fsqp_version, usage_msg };

void report(ASL *asl, int inform, int m, int nobj, real *y, real *f)
{
	int i, j;
	real F, *pi = pi0, t, t1;
	char msg[256];
	typedef struct { char *msg; int code, wantsol; } Sol_info;
	Sol_info *SI;
	static Sol_info solinfo[] = {
	 { "Optimal", 0, 1 },
	 { "Linear constraints are infeasible", 200, 0 },
	 { "Cannot satisfy the nonlinear constraints", 201, 0 },
	 { "Iteration limit", 400, 1 },
	 { "Line search failure", 501, 1 },
	 { "QP solver failed (d0)", 502, 1 },
	 { "QP solver failed (d1)", 503, 1 },
	 { "Input data are inconsistent (details with iprint > 0)", 202, 0 },
	 { "Overly tight stopping criteria (eps, epseqn)", 504, 1 },
	 { "Penalty parameter got too large", 505, 1 }
	 };

	t1 = -objsign;
	for (i = 0; i < m;) 
    {
		t = y[i];
		j = cperm[i++];
		if (j < 0) 
        {
			j = -(j+1);
			if (i < m && cperm[i] == j) 
            {
				if (t > y[i]) t = y[i];
				else t = -t;
				i++;
			} else t = -t;
		}
		pi[j] = t*t1;
	}

    if (inform >= 0 && inform < 10) 
    {
		SI = solinfo + inform;
		solve_result_num = SI->code;
		i = sprintf(msg, "%s: %s\n", Oinfo.bsname, SI->msg);
		if (SI->wantsol)
		    if (nobj > 0) 
            {
			    F = f[0];
			    j = 1;
			    if (F < 0. && abs_mode) F = -F;
			    while(j < nobj) 
                {
				    t = f[j++];
				    if (t < 0. && abs_mode) t = -t;
				    if (F < t) F = t;
			    }
			    i += sprintf(msg+i, "Objective %.*g\n", obj_prec(),
				    objsign*F);
			}
		    else i += sprintf(msg+i, "No objective.\n");
		sprintf(msg+i,"%d trial points and %d objective evaluations.",nx, nf);
	}
	else 
    {
		sprintf(msg, "%s: Unexpected value of inform = %d", inform);
		solve_result_num = 500;
	}
	write_sol(msg, X0, pi, &Oinfo);
}

int main(int argc, char **argv)
{
	ASL *asl;
	char *stub;
	FILE *F;
	int inform, m, mode, n, neq, neqn, nineq, nineqn, nobj;
	real *f, *g, *y;
	static int mesh_pts[1];
	Not_Used(argc);

	asl = ASL_alloc(ASL_read_fg);
	stub = getstops(argv, &Oinfo);
        //If you want to compute nonlinear functions but will never
        // compute derivatives, reduce overhead by setting
        // want_derivs = 0 (before calling fg_read).
    want_derivs=0;

    F = jac0dim(stub, (ftnlen)strlen(stub));

    xStart.setSize(n_var);
    bl.setSize(n_var);
    bu.setSize(n_var);

    LUv = (double*)bl;
	Uvx = (double*)bu;
    X0  = (double*)xStart;
	LUrhs = (double*)malloc(2*n_con*sizeof(double));
	Urhsx = NULL;
	pi0 = NULL;

    fg_read(F, 0);

    if (nlnc!=0)
    {
        printf("nonlinear network constraints not handled.\n"); exit(255);
    }
    if (lnc!=0)
    {
        printf("linear network not handled.\n"); exit(255);
    }

    iBaseNLContraints=n_con-nlc;
    if (nl>0)
    {
        b = LUrhs; j=0;
        for (i = 0; i < iBaseNLContraints; i++, b += 2) 
        {
		    if (b[0] == b[1] )
            {
                printf("equality constraint not handled.\n"); exit(255);
            }
            if (b[0] > negInfinity) j++;
            if (b[1] < Infinity) j++;
        }

        A.setSize(j, n_var);
        b.setSize(j);

        b = LUrhs;
	    for (i=0; i < iBaseNLContraints; i++, b += 2) 
        {
		    if (b[0] > negInfinity)
            {
    		    for(cg = Cgrad[i]; cg; cg = cg->next) a[j][cg->varno]=-cg->coef;
                db[j]=-b[0];
                j++;
            }
		    if (b[1] < Infinity) 
            {
    		    for(cg = Cgrad[i]; cg; cg = cg->next) a[j][cg->varno]=cg->coef;
                db[j]=b[0];
                j++;
		    }
	    }
    }
    if (nlc>0)
    {
        b=LUrhs+iBaseNLContraints*2; nNLOppositeCosntraints=0; nNLConstraints=0;
        for (i = iBaseNLContraints; i < n_con; i++, b += 2) 
        {
		    if (b[0] == b[1] )
            {
                printf("equality constraint not handled.\n"); exit(255);
            }
            if (b[0] > negInfinity) { nNLOppositeConstraints++; nNLConstraints++; }
            if (b[1] < Infinity) nNLConstraints++;
        }
        RHSNLConstraint.setsize(nNLConstraints);
        double *bb=RHSNLConstraint;

        b=LUrhs+iBaseNLContraints*2; j=0; k=nNLOppositeConstraints;
        for (i = iBaseNLContraints; i < n_con; i++, b += 2) 
        {
            if (b[0] > negInfinity)
            {
                bb[j]=b[0]; j++;
            }
            if (b[1] < Infinity)
            {
                bb[k]=b[1]; k++;
            }
        }
    }

	if (objno <= 0 || objno > n_obj) objno = 0;
	objsign = objtype[obj_no] ? -1. : 1.;

	cperm = (int*)(f + nobj);

//call to solver

    report(asl, inform, m, nobj, y + n_var, f);
	return 0;
}
