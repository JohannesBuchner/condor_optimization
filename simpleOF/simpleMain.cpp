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


#include "simpleObjFunctions.h"

ObjectiveFunction *getObjectiveFunction(int i, int n=0)
{
    ObjectiveFunction *of=NULL;

    switch (i)
    {
// first choice: internally coded functions:
    case  1: of=new Rosenbrock(i); break; // n=2;
    case  2: of=new BADScaleRosenbrock(i); break; // n=2;
    case  3: of=new FletcherTest(i); break; // n=2; (contraints!)
    case  4: of=new SuperSimpleConstrainedObjectiveFunction(i); break; //n=2; (contraints!)
    case  5: of=new FletcherTest2(i); break; // n=3; (contraints!)
    case  6: of=new NoisyRosenbrock(i); break; //n=2;
    case  7: of=new NoisyQuadratic(i,n); break; 
    case  8: of=new SimpleQuadratic(i,n); break; 
// second choice: create new random objective function
    case  20: of=new RandomOF(i+1,n); ((RandomOF *)of)->save("test.dat"); break; 

// third choice : reload from disk previous random objective function
    case  21: of=new RandomOF(i,"test.dat"); break; 
    }

    if (of==NULL)
    {
        printf("No objective function defined under index %i\n",i);
        exit(255);
    }
    return of;
}

void testRescale2()
{
    double rhoStart=1e-1, rhoEnd=1e-5;
    int niter=1000;

    Vector r(2);
    r[0]=1.0;
    r[1]=1e3;

    ObjectiveFunction *of=new BADScaleRosenbrock(2),
                      *rof=new CorrectScaleOF(2, of, r);
 
    CONDOR(rhoStart, rhoEnd, niter, rof);
    of->printStats();
    delete of; delete rof;
}

void testRescale1()
{
    double rhoStart=1e-1, rhoEnd=1e-5;
    int niter=1000;

    Vector r(2);
    r[0]=1.0;
    r[1]=1e3;

    ObjectiveFunction *of=new BADScaleRosenbrock(2);
    CONDOR(rhoStart, rhoEnd, niter, of);
    of->printStats();
    delete of;
}

void simpleTest(char c, int n)
{
    double rhoStart=1e-0, rhoEnd=1e-7;
    int niter=1000;

    ObjectiveFunction *of;
    switch (c)
    {
    case '1': of=new Rosenbrock(1); break;
    case '5': of=new RandomOF(6,n); ((RandomOF *)of)->save("test.dat"); break; 
    case '6': of=new RandomOF(6,"test.dat"); break; 
    }

    of->setSaveFile();
    CONDOR(rhoStart, rhoEnd, niter, of);
    of->printStats();

    FILE *ff=fopen("resultsOne.txt","w");
    fprintf(ff,"%s & %i & %i & (%i) & %e \\\\\n", of->name, of->dim(), of->getNFE(), of->getNFE2(), of->valueBest);
    fclose(ff);

    delete of;
}

void testNoise()
{
    const int ntest=50;
    double rhoStart=1e-0, rhoEnd=1e-4, nfet=0.0, nfe2t=0.0, f=0.0;
    int niter=200,i, maxnfe=0, minnfe=10000, maxnfe2=0, minnfe2=10000;

    for (i=0; i<ntest; i++)
    {
        ObjectiveFunction *of=new NoisyQuadratic(i,4); 
        of->setSaveFile("out.dat");
        CONDOR(rhoStart, rhoEnd, niter, of);
        of->printStats();
        maxnfe=mmax(maxnfe,of->getNFE());
        minnfe=mmin(minnfe,of->getNFE());
        maxnfe2=mmax(maxnfe2,of->getNFE2());
        minnfe2=mmin(minnfe2,of->getNFE2());
        nfet+=of->getNFE();
        nfe2t+=of->getNFE2();
        f+=of->valueBest;
        delete of;
    }
    nfet/=(double)ntest;
    nfe2t/=(double)ntest;
    f/=(double)ntest;
    printf("\naverage number of function evaluation: %f (%f)\n"
        "Maximum number of function evaluation: %i (%i)\n"
        "Minimum number of function evaluation: %i (%i)\n"
        "\naverage final function value: %e\n"
        , nfet, nfe2t,maxnfe,maxnfe2,minnfe,minnfe2,f);
}

int main(int argc, char **argv)
{
    if (argc<2)
    {
        printf("SYNTAX: simpleDemoOptimizer <problem_number> [<dimension>]\n"
               "You must specify <problem_number> on the command line.\n"
               "<problem_number> can be:\n"
               " 1 : The simple rosenbrock banana function will be solved.\n"
               " 2 : A badly scaled function based on the rosenbrock function will be solved.\n"
               "     Because of the bad scaling, a huge number of funtion evaluation is needed.\n"
               " 3 : The same function that was used in point '2' above is solved after\n"
               "     automatic rescaling. Note the reduced number of function evaluation.\n"
               " 4 : A quadratic in 4 dimention centered at (2,2,2,2) and perturbated with a\n"
               "     uniform noise of amplitude 1e-5 will be optimized 50 times. The average\n"
               "     final function value will be displayed. Note that this final averaging \n"
               "     value is always below the noise level. This means that the optimizer has\n"
               "     never been stucked into a local minima during this 50 runs (illustration\n"
               "     of the robustness against noise).\n"
               " 5 : A random objective function will be generated, saved on the disk and\n"
               "     solved. You can give on the command line the dimension of the search-\n"
               "     space (the default dimension is 4).\n"
               " 6 : An objective function previously generated will be loaded from he disk\n"
               "     and solved\n\n");
        exit(255);
    }
    switch (argv[1][0])
    {
    case '1': 
    case '5':
    case '6': 
        if (argc>3) simpleTest(argv[1][0],atol(argv[2])); 
        else simpleTest(argv[1][0],4); 
        break;
    case '2': testRescale1(); break;
    case '3': testRescale2(); break;
    case '4': testNoise(); break;
    }
//    printf("finished.\nPress return"); getchar();
    return 0;
}

