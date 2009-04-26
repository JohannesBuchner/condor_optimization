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
// for sleep:
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../common/Solver.h"
#include "../common/tools.h"
#include "../common/ObjectiveFunction.h"

#ifdef WIN32
#include <crtdbg.h>
#endif


#include "XMLof.h"

#ifdef WIN32
extern void (__cdecl *quickHack)(InterPolynomial,int);
#else
extern void (*quickHack)(InterPolynomial,int);
#endif
XMLof *of;
CorrectScaleOF *ofc;    
void quickHackBurgmannFunction(InterPolynomial poly, int k)
{
    if (!of->vSigma.sz()) return;
    Matrix mH(of->nToOptim,of->nToOptim);
    Vector vG(of->nToOptim);
    poly.gradientHessian(poly.NewtonPoints[k],vG,mH);
    of->setSigma(mH,ofc->rescaling);
}

void protect(char*);

void runXML(int argc, char **argv)
{
	printf("CONDOR optimizer v1.06\n"
		   "======================\n"
		   "         This is a full registered version.\n"
		   "         Free unconstrained version available from the web at the\n"
		   "         URL: http://iridia.ulb.ac.be/~fvandenb/work/optimization \n\n");

    double rhoStart=1e-1, rhoEnd=1e-4;
    int niter=1000;
//
    if (argc>1) of=new XMLof(31,argv[1],&rhoEnd); 
    else of=new XMLof(31,"optim.xml",&rhoEnd);
    
	if (of->rhostart!=0.0) rhoStart=of->rhostart;
    if (of->maxIter!=0)    niter=of->maxIter;

//    of->setSaveFile();
//    ObjectiveFunction *ofc=new CorrectScaleOF(30,of,of->bu);

    if (of->scalingFactorIsGiven) ofc=new CorrectScaleOF(31,of, of->vScale);
    else ofc=new CorrectScaleOF(31,of);

    quickHack=quickHackBurgmannFunction;
    
    CONDOR(rhoStart, rhoEnd, niter, ofc);

    of->printStats();

    //ff=fopen("resultsOptim.bin","wb");
    //fwrite(of->xBest.d->p, of->fullDim*sizeof(double), 1, ff);
    //fwrite(&of->valueBest, sizeof(double), 1, ff);
    //fclose(ff);

    delete of;
}

int main(int argc, char **argv)
{
    {
        runXML(argc,argv);
   //     system("cls");
//        printf("finished.\nPress return"); getchar();
    }
    return 0;
}

