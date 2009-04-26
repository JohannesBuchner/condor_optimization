/*

CONDOR 1.06 - COnstrained, Non-linear, Direct, parallel Optimization 
              using trust Region method for high-computing load, 
              noisy functions
Copyright (C) 2004 Frank Vanden Berghen

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

If you want to include this tools in any commercial product, 
you can contact the author at fvandenb@iridia.ulb.ac.be

*/
#ifndef XML_OBJECTIVEFUNCTION_INCLUDE
#define XML_OBJECTIVEFUNCTION_INCLUDE

#include "../common/ObjectiveFunction.h"
#include "../common/Vector.h"
#include "../common/VectorChar.h"
#include "../common/IntPoly.h"
#include "../MathEval/MathEvalClass.h"
#include "xmlParser.h"

class XMLof : public ObjectiveFunction 
{
public:
    int fullDim, maxIter,nToOptim;
    char scalingFactorIsGiven;
	double rhostart;
    Vector vScale,vFullStart,vSigma;

    XMLof(int _t,char *argv, double *rhoEnd);
    ~XMLof();

    double eval(Vector v, int *ner=NULL);
    double evalNLConstraint(int j, Vector v, int *ner=NULL);
    void evalGradNLConstraint(int j, Vector v, Vector result, int *ner=NULL);
    virtual void finalize(Vector vG, Matrix mH, Vector vLambda);

// quick and dirty hack:
    void setSigma(Matrix mH, Vector rescale);
    void DBTool(Vector r);

//    virtual void saveValue(Vector tmp,double valueOF, int nerror);

    //    void printStats() {ConstrainedObjectiveFunction::printStats();}
private:
    typedef struct equalityFunction
    {
        MathEvalClass *me;
        int ivar;
    } equalityFunction;
    equalityFunction *equalities;
    typedef struct aggregationFunction
    {
        MathEvalClass *me;
        LPTSTR name;
    } aggregationFunction;
    aggregationFunction *af;
    MathEvalClass **nlinequalities;

    Matrix dataFull;
    char *configFile, *objFunction, *inputObjF, *outputObjF, *dbname,
         **dbcolnames, *charMemSpace,
         *resultsFile, *debugOF, sigmaIsSet, **names; //, *aggregationFunction;
    Vector vIndexes, vLx, vLxIndexes, vSigNoise;
    VectorChar vXToOptim;
    int nIndexInOF, neq, nSubAggrFunction;
	double timeToSleep;
    double rhoEnd;

    void shortXToLongX(double *sx, double *llx);
	void longXtoShortX(double *llx, double *sx);
    void loadconstraints(int nineq, FILE *stream,char *line);
    void loadData(char *line, char ascii);
    void init(char *filame);
    void initXMLBounds();
    void reductionDimension();
    void setNamesForDBfile(int nIndexNames, char **indexNames, char **allVarNames);
    void setNamesForTraceFile();
    void computeSigNoise(Matrix mH);
    double aggregate(double *vLxIndexes, double *vIndexes, int *nerr);
};

/*
class ClientXMLof : public ObjectiveFunction 
{
public:

    ClientXMLof(int _t, char *, Vector v);
    ~ClientXMLof();

    double eval(Vector v, int *ner=NULL);
    double evalNLConstraint(int j, Vector v, int *ner=NULL){return 0;};
    void evalGradNLConstraint(int j, Vector v, Vector result, int *ner=NULL){};

private:
    char *configFile,*objFunction, *inputObjF, *outputObjF;
    int fullDim,nIndexInOF;
    Vector weight,center,exponent,vTmpOF,vFullStart,vLx,vXToOptim;
};
*/

#endif
