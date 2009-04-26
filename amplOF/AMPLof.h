#ifndef AMPL_OBJECTIVEFUNCTION_INCLUDE
#define AMPL_OBJECTIVEFUNCTION_INCLUDE


#include "../common/ObjectiveFunction.h"
#include "../common/VectorInt.h"
#include "asl.h"

class AMPLObjectiveFunction : public ObjectiveFunction 
{
public:
	long nerror;
	ASL * asl;
    AMPLObjectiveFunction(int _t,char **argv, double _objectiveConst=0.0, Vector _xOptimal=Vector::emptyVector);
    AMPLObjectiveFunction(int _t,char *argv, double _objectiveConst=0.0, Vector _xOptimal=Vector::emptyVector);
    ~AMPLObjectiveFunction();

    double eval(Vector v, int *ner=NULL);
    double evalNLConstraint(int j, Vector v, int *ner=NULL);
    void evalGradNLConstraint(int j, Vector v, Vector result, int *ner=NULL);
//    void printStats() {ConstrainedObjectiveFunction::printStats();}
    virtual void finalize();
private:
    char *stub;
    double objsign;
    static Option_Info Oinfo;
    VectorInt indexNLConstraint;
    Vector RHSNLConstraint;
    void init(int _t, double _objectiveConst, Vector _xOptimal);

};

#endif

