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
#include <stdio.h>
#include <memory.h>

#include <crtdbg.h>

#include "ObjectiveFunction.h"
#include "Solver.h"
#include "Matrix.h"
#include "tools.h"
#include "KeepBests.h"
#include "IntPoly.h"
#include "parallel.h"
#include "MultInd.h"
#include "VectorChar.h"

// from CTRSSolver: 
char isFeasible(Vector vx,ConstrainedObjectiveFunction *of, double *d=NULL);
extern Vector FullLambda;
Vector ConstrainedL2NormMinimizer(char isConstrained, InterPolynomial poly, int k, 
                                  double delta, int *info, int iterMax, double *lambda1, 
                                  Vector vOBase, ObjectiveFunction *uof);
void projectionIntoFeasibleSpace(Vector vFrom, Vector vBase, ObjectiveFunction *uof);

int findBest(Matrix data, char isConstrained, ObjectiveFunction *of)
{
    // find THE best point in the datas.
    int i=data.nLine(), k=-1, dim=data.nColumn()-2;
    if (i==0) 
    {
        Vector b(dim);
        if (isConstrained)
        {
            Vector a(dim); a.zero();
            projectionIntoFeasibleSpace(a, b,(ConstrainedObjectiveFunction *)of);
        }
        data.extendLine();
        data.setLine(0,b);
        data[0][dim]=of->eval(b);
        return 0;
    }

    double *p=*((double**)(data))+dim-1, best=INF;
    if (isConstrained)
    {
        Vector r(dim);
        int j;
        double best2=INF;
        while (i--)
        {
            data.getLine(i,r,dim);
            if (*p<best2) {j=i; best2=*p; }
            if (isFeasible(r,(ConstrainedObjectiveFunction *)of)&&(*p<best)) { k=i; best=*p; }
            p+=dim+1;
        }

        if (k==-1)
        {
            data.getLine(j,r,dim);
            Vector b(dim);
            projectionIntoFeasibleSpace(r, b,(ConstrainedObjectiveFunction *)of);
            i=data.nLine();
            data.extendLine();
            if (!isFeasible(b,(ConstrainedObjectiveFunction*)of,&best))
            {
                printf("unable to start (violation=%e).\n",best);
            }
            data.setLine(i,b);
            data[i][dim]=of->eval(b);
            return i;
        }
        return k;
    }
    
    while (i--)
    {
        if (*p<best) { k=i; best=*p; }
        p+=dim+1;
    }
    return k;
}

Vector *getFirstPoints(double **ValuesFF, int *np, double rho, char isConstrained,
                       ObjectiveFunction *of)
{
    Matrix data=of->data;
    int k=findBest(data, isConstrained, of);
    if (k==-1)
    {
        printf("Matrix Data must at least contains one line.\n"); getchar(); exit(255);
    }
    int dim=data.nColumn()-2, n=(dim+1)*(dim+2)/2, nl=data.nLine(), i=nl,j=0;
    Vector Base=data.getLine(k,dim);
    double vBase=((double**)data)[k][dim];
    double *p,norm, *pb=Base;
    KeepBests kb(n*2,dim);
    Vector *points;
    double *valuesF;

    fprintf(stderr,"Value Objective=%e\n", vBase);

    while (j<n)
    {
        i=data.nLine(); kb.reset(); j=0;
        while (i--)
        {
            p=data[i];
            norm=0; k=dim;
            while (k--) norm+=sqr(p[k]-pb[k]);
            norm=sqrt(norm);
            if (norm<=2.001*rho) { kb.add(norm,p[dim], p); j++; }
        }
        if (j>=n)
        {
            // we have retained only the 2*n best points:
            j=mmin(j,2*n);
            points=new Vector[j];
            valuesF=(double*)malloc(j*sizeof(double));
            for (i=0; i<j; i++)
            {
                valuesF[i]=kb.getValue(i);
                points[i]=Vector(dim, kb.getOptValue(i));
            }
        } else
        {
            points=GenerateData(&valuesF, rho, Base, vBase, of);
            for (i=0; i<n-1; i++) of->saveValue(points[i],valuesF[i]);
            delete[] points;
            free(valuesF);
        }
    }
    *np=j;
    *ValuesFF=valuesF;
    return points;
}

int findK(double *ValuesF, int n, char isConstrained, ObjectiveFunction *of, Vector *points)
{
    // find index k of the best value of the function 
    double minimumValueF=INF;
    int i,k=-1; 
    for (i=0; i<n; i++)
        if ((ValuesF[i]<minimumValueF)&&
            ((!isConstrained)||isFeasible(points[i],(ConstrainedObjectiveFunction*)of))) { k=i; minimumValueF=ValuesF[i]; }
    if (k==-1)
    {
        k=0;
//        printf("no feasible point found.\nPress return\n"); getchar(); exit(255);
    }
    return k;
}

void unConstraintedQPOptim( double rhoStart, double rhoEnd, int niter, 
              ObjectiveFunction *of, int nnode)
{
    QPOptim( rhoStart, rhoEnd, niter, of, nnode, 0);
}

void QPOptim( double rhoStart, double rhoEnd, int niter, 
              ObjectiveFunction *of, int nnode, char isConstrained)
{
    rhoStart=mmax(rhoStart,rhoEnd);
    int dim=of->dim(), n=(dim+1)*(dim+2)/2, info, k, t, nPtsTotal;
    double rho=rhoStart, delta=rhoStart, rhoNew,
           lambda1, normD, modelStep, reduction, r, valueOF, valueFk, bound, noise;
    double *ValuesF;
    Vector Base, d, tmp, *points;
    bool improvement, forceTRStep=true, evalNeeded, bCActive=false;
    
    if (isConstrained)
    {
        ConstrainedObjectiveFunction *cof=(ConstrainedObjectiveFunction *)of;
        FullLambda.setSize(dim*2+cof->A.nLine()+cof->nNLConstraints);
        FullLambda.zero();
    }

    // pre-create the MultInd indexes to prevent multi-thread problems:

    cacheMultInd.get(dim,1); cacheMultInd.get(dim,2);    

    parallelInit(nnode, dim, of);
    of->initDataFromXStart();
    points=getFirstPoints(&ValuesF, &nPtsTotal, rhoStart, isConstrained, of);
//    of->nfe=n;
    fprintf(stderr,"init part 1 finished.\n");

    // find index k of the best (lowest) value of the function 
    k=findK(ValuesF, nPtsTotal, isConstrained, of, points);
    Base=points[k].clone();
//    Base=of->xStart.clone();
    valueFk=ValuesF[k];
    // translation:
    t=nPtsTotal; while (t--) points[t]-=Base;

    // exchange index 0 and index k (to be sure best point is inside poly):
    tmp=points[k];
    points[k]=points[0];
    points[0]=tmp;
    ValuesF[k]=ValuesF[0];
    ValuesF[0]=valueFk;
    k=0;

    InterPolynomial poly(2, nPtsTotal, points, ValuesF );
    
    // update M:
    for (t=n; t<nPtsTotal; t++) poly.updateM(points[t], ValuesF[t]);
    
    fprintf(stderr,"init part 2 finished.\n");
    fprintf(stderr,"init finished.\n");

    // first of init all variables:
    parallelImprove(&poly, &k, rho, &valueFk, Base);

    // really start in parallel:
    startParallelThread();

    while (true)
    {
//        fprintf(stderr,"rho=%e; fo=%e; NF=%i\n", rho,valueFk,QP_NF);
        while (true)
        {
            // trust region step
            while (true)
            {
//                poly.print();
                parallelImprove(&poly, &k, rho, &valueFk, Base);
   
                // to debug:
                fprintf(stderr,"Best Value Objective=%e (nfe=%i)\n", valueFk, of->nfe);
                
                d=ConstrainedL2NormMinimizer(isConstrained,poly,k,delta,&info,1000,&lambda1,Base,of);

//                if (!isConstrained) 
//                    d=L2NormMinimizer(poly, poly.NewtonPoints[k], delta, &info, 1000, &lambda1);
//                else
//                {
//                    if (!bCActive) 
////                    if (1)
//                        d=L2NormMinimizer(poly, poly.NewtonPoints[k], delta, &info, 1000, &lambda1);
//                    else
//                    {
//                        poly.gradient(Vector::emptyVector,d);
//                        double dno=d.euclidianNorm();
//                        if (dno>delta) d.multiply(delta/dno);
//                    }
//                    bCActive=projectionIntoFeasibleSpace(d, Base+poly.NewtonPoints[k],
//                                (ConstrainedObjectiveFunction*)of) || bCActive;
//                } 

                if (d.euclidianNorm()>delta)
                {
                    printf("Warning d to long: (%e > %e)\n", d.euclidianNorm(), delta);
                }

                normD=mmin(d.euclidianNorm(), delta);
                d+=poly.NewtonPoints[k];

//              next line is equivalent to reduction=valueFk-poly(d); 
//              BUT is more precise (no rounding error)
                reduction=-poly.shiftedEval(d,valueFk);

                //if (normD<0.5*rho) { evalNeeded=true; break; }
                if ((normD<0.5*rho)&&(!forceTRStep)) { evalNeeded=true; break; }

                //  IF THE MODEL REDUCTION IS SMALL, THEN WE DO NOT SAMPLE FUNCTION
                //  AT THE NEW POINT. WE THEN WILL TRY TO IMPROVE THE MODEL.

                noise=0.5*mmax(of->noiseAbsolute*(1+of->noiseRelative), abs(valueFk)*of->noiseRelative);
                if ((reduction<noise)&&(!forceTRStep)) { evalNeeded=true; break; }
                forceTRStep=false; evalNeeded=false;

                if (!isFeasible(tmp,(ConstrainedObjectiveFunction *)of, &r))
                {
                    printf("error: %e\n",r);
                }
                tmp=Base+d; valueOF=of->eval(tmp); of->saveValue(tmp,valueOF); 

                // update of delta:
                r=(valueFk-valueOF)/reduction;
                if (r<=0.1) delta=0.5*normD;
                else if (r<0.7) delta=mmax(0.5*delta, normD);
                     else delta=mmax(rho+ normD, mmax(1.25*normD, delta));
            // powell's heuristics:
                if (delta<1.5*rho) delta=rho;

                if (valueOF<valueFk) 
                {
                    t=poly.findAGoodPointToReplace(-1, rho, d,&modelStep);
                    k=t; valueFk=valueOF; 
                    improvement=true;
//                    fprintf(stderr,"Value Objective=%e\n", valueOF);
                } else
                {
                    t=poly.findAGoodPointToReplace(k, rho, d,&modelStep);
                    improvement=false;
//                    fprintf(stderr,".");
                };

                if (t<0) { poly.updateM(d, valueOF); break; }

                // If we are along constraints, it's more important to update
                // the polynomial with points which increase its quality.
                // Thus, we will skip this update to use only points coming
                // from checkIfValidityIsInBound

                if ((improvement)||(reduction>0.0)||(normD<rho)) poly.replace(t, d, valueOF); 

                if (improvement) continue;
//                if (modelStep>4*rho*rho) continue;
                if (modelStep>2*rho) continue;
                if (normD>=2*rho) continue;
                break;
            }
            // model improvement step
            forceTRStep=true;

//            fprintf(stderr,"improvement step\n");
            bound=0.0;
            if (normD<0.5*rho) 
            {
                bound=0.5*sqr(rho)*lambda1;
                if (poly.nUpdateOfM<10) bound=0.0;
            }

            parallelImprove(&poly, &k, rho, &valueFk, Base);

            // !! change d (if needed):
            t=poly.checkIfValidityIsInBound(d, k, bound, rho );
            if (t>=0)
            {
                tmp=Base+d; valueOF=of->eval(tmp); of->saveValue(tmp,valueOF); 
                poly.replace(t, d, valueOF); 
                if ((valueOF<valueFk)&&
                    ((!isConstrained)||isFeasible(tmp,(ConstrainedObjectiveFunction*)of))) { k=t; valueFk=valueOF; };
                continue;
            }

            // the model is perfect for this value of rho:
            // OR
            // we have crossed a non_linear constraint which prevent us to advance
            if ((normD<=rho)||(reduction<0.0)) break;
        }

        // change rho because no improvement can now be made:
        if (rho<=rhoEnd) break;

        fprintf(stderr,"rho=%e; fo=%e; NF=%i\n", rho,valueFk,of->nfe);

        if (rho<16*rhoEnd) rhoNew=rhoEnd;
        else if (rho<250*rhoEnd) rhoNew=sqrt(rho*rhoEnd);
             else rhoNew=0.1*rho;
        delta=mmax(0.5*rho,rhoNew);
        rho=rhoNew;

        
        // update of the polynomial: translation of x[k].
        // replace BASE by BASE+x[k]
        if (!poly.NewtonPoints[k].equals(Vector::emptyVector))
        {
            Base+=poly.NewtonPoints[k];
            poly.translate(poly.NewtonPoints[k]);
        }
    }
    parallelFinish();
    
    if (evalNeeded)
    {
        tmp=Base+d; valueOF=of->eval(tmp); of->saveValue(tmp,valueOF); 
        if (valueOF<valueFk) { valueFk=valueOF; Base=tmp; }
        else Base+=poly.NewtonPoints[k];
    } else Base+=poly.NewtonPoints[k];


//    delete[] points; :necessary: not done in destructor of poly which is called automatically:
    fprintf(stderr,"rho=%e; fo=%e; NF=%i\n", rho,valueFk,of->nfe);
    
    of->valueBest=valueFk;
    of->xBest=Base;
}

