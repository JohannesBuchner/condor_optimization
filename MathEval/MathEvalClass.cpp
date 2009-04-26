#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "MathEvalClass.h"
#include "lib/matheval.h"
#include "../common/tools.h"

#ifdef WIN32
#include <float.h> // for  isnan and isfinite
#else
#include <math.h>  // for  isnan and isfinite
static int _isnan (double d) { return isnan(d); }
static int _finite(double d) { return isfinite(d); }
#endif

// #include<crtdbg.h>
MathEvalClass::~MathEvalClass()
{
    evaluator_destroy(aggrFunct);
    if (gradFunct)
    {
        int i;
        for (i=0; i<dim; i++) evaluator_destroy(gradFunct[i]);
        free(gradFunct);
    }
}

double MathEvalClass::eval(Vector v, int *ner)
{
    return eval((double*)v, ner);
}

double MathEvalClass::eval(double *v, int *ner)
{
    *ner=0;
    double r=evaluator_evaluate(aggrFunct, dim,varnames, v);
    if ((_isnan(r))||(!_finite(r))) *ner=1;
    return r;
}

void MathEvalClass::evalGrad(Vector sv, Vector result, int *ner)
{
    evalGrad((double*)sv,(double*)result,ner);
}

void MathEvalClass::evalGrad(double *v, double *res, int *ner)
{
    if (gradFunct==NULL)
    {
        printf("You must explicitely ask for gradient computation when initializing MathEval Object.\n");
        exit(255);
    }
    double r;
    int i=dim; *ner=0; 
    while (i--)
    {
        r=evaluator_evaluate(gradFunct[i], dim, varnames, v);
        if ((_isnan(r))||(!_finite(r))) { *ner=1; return; }
        res[i]=r;
    }
}

MathEvalClass::MathEvalClass(char *expression, char **dbcolnames, int _dim, char isGradientNeeded)
{
    varnames=dbcolnames;
    dim=_dim;
    aggrFunct=evaluator_create(expression, varnames, dim, &maxIndexVar);

    if (!isGradientNeeded) { gradFunct=NULL; return; }

    gradFunct=(void **)malloc(dim*sizeof(void*));
    int i;
    for (i=0; i<dim; i++) gradFunct[i]=evaluator_derivative(aggrFunct,varnames[i]);
}
