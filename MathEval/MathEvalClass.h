#ifndef AMPL_OBJECT_INCLUDE
#define AMPL_OBJECT_INCLUDE

#include "../common/Vector.h"

class MathEvalClass
{
public:
    MathEvalClass::MathEvalClass(char *filename, char **dbcolnames, int _dim, char isGradientNeeded);
    ~MathEvalClass();
    double eval(Vector v, int *ner);
    double eval(double *d, int *ner);
    void evalGrad(Vector sv, Vector result, int *ner);
    void evalGrad(double *v, double *res, int *ner);
    int maxIndexVarUsed() { return maxIndexVar; }

private:
    char **varnames;
    int dim, maxIndexVar;
    void *aggrFunct;
    void **gradFunct;
};

#endif

