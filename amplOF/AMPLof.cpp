// #include<crtdbg.h>

#include "AMPLof.h"
#include "../common/tools.h"
#include "getstub.h"

//#include "asl.h"
//#include "nlp.h"
//#include "r_opn.hd"
//#ifndef f_MINLIST	// TEMPORARY: now in updated r_opn.hd 
//#define f_MINLIST	r_ops[11]
//#define f_MAXLIST	r_ops[12]
//#define f_ABS		r_ops[15]
//#endif
//#include "getstub.h"

static char *usage_msg[] = {
  "\n	Here will be the usage",
  "	messages",
  0};

static char *condor_version="CONDOR v1.0";

Option_Info AMPLObjectiveFunction::Oinfo = {
	"CONDOR", "COnstrained Non-linear Direct Optimization by trust Region", "condor_options", NULL, 0, 1,
	condor_version, usage_msg };

// What's inside structure "Option_Info" ?
//      char *sname;            // invocation name of solver 
//      char *bsname;           // solver name in startup "banner" 
//      char *opname;           // name of solver_options environment var 
//      keyword *keywds;        // key words 
//      int n_keywds;           // number of key words 
//      int want_funcadd;       // whether funcadd will be called 
//      char *version;          // for -v and Ver_key_ASL() 
//      char **usage;           // solver-specific usage message 
//      Solver_KW_func *kwf;    // solver-specific keyword function 
//      Fileeq_func *feq;       // for n=filename 
//      keyword *options;       // command-line options (with -) before stub 
//      int n_options;          // number of options 

double AMPLObjectiveFunction::eval(Vector v, int *ner)
{
    if (ner) *ner=1;
	double r=objsign * objval(0, (double*)v, (fint*)ner);
    updateCounter(r,v);
    return r;
}

double AMPLObjectiveFunction::evalNLConstraint(int j, Vector v, int *ner)
{
    int i=indexNLConstraint[j];
    if (ner) *ner=1;
    if (i<0) 
        return RHSNLConstraint[j] - conival(-i-1, (double*)v, (fint*)ner);
    return conival(i, (double*)v, (fint*)ner) - RHSNLConstraint[j];
}

void AMPLObjectiveFunction::evalGradNLConstraint(int j, Vector v, Vector r, int *ner)
{
    int i=indexNLConstraint[j];
    if (ner) *ner=1;
    if (i <0)
    {
        congrd(-i-1, (double*)v, (double*)r, (fint*)ner);
        r.multiply(-1.0);
        return;
    }
    congrd(i, (double*)v, (double*)r, (fint*)ner);
}

/*
static int maxiter = 200, objno = 1;
static int always, iprint, monotone;
static real eps = 1e-8, epseqn = 1e-8;

 static keyword keywds[] = {	
 KW("always",	I_val,	&always,	"always check feasibility in linesearch  (0 = no) [mode: C]"),
 KW("eps",	    D_val,	&eps,		"final Newton dir. norm (1e-8) [eps]"),
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
*/
AMPLObjectiveFunction::AMPLObjectiveFunction(int _t, char **argv, double _objectiveConst, Vector _xOptimal)
{
    asl = ASL_alloc(ASL_read_fg);
    stub=getstops(argv, &Oinfo);
    init(_t, _objectiveConst, _xOptimal);
}

AMPLObjectiveFunction::AMPLObjectiveFunction(int _t, char *argv, double _objectiveConst, Vector _xOptimal)
{
    asl = ASL_alloc(ASL_read_fg);
    stub=argv;
    init(_t, _objectiveConst, _xOptimal);
}

void AMPLObjectiveFunction::init(int _t, double _objectiveConst, Vector _xOptimal)
{
    t=_t;
    FILE *F;
    int i,j;
    double *bb;

        // If you want to compute nonlinear functions but will never
        // compute derivatives, reduce overhead by setting
        // want_derivs = 0 (before calling fg_read).
        //
        // we want derivatives of the constraints, so:
    want_derivs=1;

    F = jac0dim(stub, (ftnlen)strlen(stub));

    setName(asl->i.filename_);    

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

//    X0[0]=1.0;     X0[1]=1.0;    X0[2]=1.0;

    objectiveConst=_objectiveConst; // objconst(0);
    if (!(_xOptimal==Vector::emptyVector)&&((int)_xOptimal.sz()==n_var)) xOptimal.copyFrom(_xOptimal);

    if (nlnc!=0)
    {
        printf("nonlinear network constraints not handled.\n"); exit(255);
    }
    if (lnc!=0)
    {
        printf("linear network not handled.\n"); exit(255);
    }

    // nlc: number of non-linear constraints
    if (nlc>0)
    {
        bb=LUrhs; nNLConstraints=0;
        for (i = 0; i < nlc; i++, bb += 2) 
        {
            if (bb[0] == bb[1] )
            {
                printf("equality constraint not handled.\n"); exit(255);
            }
            if (bb[0] > negInfinity)  nNLConstraints++; 
            if (bb[1] <    Infinity)  nNLConstraints++;
        }
        RHSNLConstraint.setSize(nNLConstraints);
        double *nlbb=RHSNLConstraint;
        indexNLConstraint.setSize(nNLConstraints);
        int *inlc=indexNLConstraint;

        bb=LUrhs; j=0;
        for (i = 0; i < nlc; i++, bb += 2) 

        {
            if (bb[0] > negInfinity) { nlbb[j]=bb[0]; inlc[j]= i; j++; }
            if (bb[1] <    Infinity) { nlbb[j]=bb[1]; inlc[j]=-i-1; j++; }
        }
    }

//    _CrtCheckMemory();

    int iBaseNLContraints=n_con-nlc;
    if (iBaseNLContraints>0)
    {
        bb = LUrhs+nlc*2; j=0;
        for (i = 0; i < iBaseNLContraints; i++, bb += 2) 
        {
            if (bb[0] == bb[1] )
            {
                printf("equality constraint not handled.\n"); exit(255);
            }
            if (bb[0] > negInfinity) j++;
            if (bb[1] < Infinity) j++;
        }

        A.setSize(j, n_var); A.zero();
        b.setSize(j); 
        
        double *db=b, **a=A;
        cgrad *cg;

        bb = LUrhs+nlc*2; j=0;
        for (i=0; i < iBaseNLContraints; i++, bb += 2) 
        {
            if (bb[0] > negInfinity)
            {
                for(cg = Cgrad[i+nlc]; cg; cg = cg->next) a[j][cg->varno]=cg->coef;
                db[j]=bb[0];
                j++;
            }
            if (bb[1] < Infinity) 
            {
                for(cg = Cgrad[i+nlc]; cg; cg = cg->next) a[j][cg->varno]=-cg->coef;
                db[j]=-bb[1];
                j++;
            }
        }
    }
    free(LUrhs);

    // minimization problem:
    objsign = 1.;
    // maximization problem:
    //objsign = -1.;
    endInit();
}

AMPLObjectiveFunction::~AMPLObjectiveFunction()
{
    ASL_free(&asl);
//    report(asl, inform, m, nobj, y + n_var, f);
};

void AMPLObjectiveFunction::finalize()
{
    write_sol("CONOQA finished.", (double*)xBest, NULL, &Oinfo);
}
