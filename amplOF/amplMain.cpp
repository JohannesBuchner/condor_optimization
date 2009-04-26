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


#include "AMPLof.h"


ObjectiveFunction *getObjectiveFunction(int i, double *rho)
{
    ObjectiveFunction *of=NULL;
    double rhoEnd=-1;

    switch (i)
    {

    case 200: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs022.nl",1.0); break;
    case 201: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs023.nl"); break;
    case 202: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs026.nl"); break;
    case 203: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs034.nl"); break;
    case 204: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs038.nl"); break;
    case 205: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs044.nl"); break;
    case 206: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs065.nl"); break;
    case 207: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs076.nl"); break;
    case 208: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs100.nl"); break;
    case 209: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs106.nl"); break;
    case 210: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs108.nl"); rhoEnd= 1e-5; break;
    case 211: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs116.nl"); break;
    case 212: of= new AMPLObjectiveFunction(i,"amplOF/examples/hs268.nl"); break;
    
    case 250: of= new AMPLObjectiveFunction(i,"amplOF/examples/rosenbr.nl" ); rhoEnd= 5e-3; break; // 2
    case 251: of= new AMPLObjectiveFunction(i,"amplOF/examples/sisser.nl"  ); rhoEnd= 1e-2; break; // 2
    case 252: of= new AMPLObjectiveFunction(i,"amplOF/examples/cliff.nl"   ); rhoEnd= 1e-3; break; // 2
    case 253: of= new AMPLObjectiveFunction(i,"amplOF/examples/hairy.nl"   ); rhoEnd= 2e-2; break; // 2
    case 254: of= new AMPLObjectiveFunction(i,"amplOF/examples/pfit1ls.nl" ); rhoEnd= 1e-2; break; // 3
    case 255: of= new AMPLObjectiveFunction(i,"amplOF/examples/hatflde.nl" ); rhoEnd=12e-3; break; // 3
    case 256: of= new AMPLObjectiveFunction(i,"amplOF/examples/growthls.nl"); rhoEnd= 5e-3; break; // 3
    case 257: of= new AMPLObjectiveFunction(i,"amplOF/examples/gulf.nl"    ); rhoEnd= 5e-2; break; // 3
    case 258: of= new AMPLObjectiveFunction(i,"amplOF/examples/brownden.nl"); rhoEnd=57e-2; break; // 4
    case 259: of= new AMPLObjectiveFunction(i,"amplOF/examples/eigenals.nl"); rhoEnd= 1e-2; break; // 6
    case 260: of= new AMPLObjectiveFunction(i,"amplOF/examples/heart6ls.nl"); rhoEnd= 1e-2; break; // 6
    case 261: of= new AMPLObjectiveFunction(i,"amplOF/examples/biggs6.nl"  ); rhoEnd= 6e-2; break; // 6
    case 262: of= new AMPLObjectiveFunction(i,"amplOF/examples/hart6.nl"   ); rhoEnd= 2e-1; break; // 6
    case 263: of= new AMPLObjectiveFunction(i,"amplOF/examples/cragglvy.nl"); rhoEnd= 1e-2; break; // 10
    case 264: of= new AMPLObjectiveFunction(i,"amplOF/examples/vardim.nl"  ); rhoEnd= 1e-3; break; // 10
    case 265: of= new AMPLObjectiveFunction(i,"amplOF/examples/mancino.nl" ); rhoEnd= 1e-6; break; // 10
    case 266: of= new AMPLObjectiveFunction(i,"amplOF/examples/power.nl"   ); rhoEnd= 2e-2; break; // 10
    case 267: of= new AMPLObjectiveFunction(i,"amplOF/examples/morebv.nl"  ); rhoEnd= 1e-1; break; // 10
    case 268: of= new AMPLObjectiveFunction(i,"amplOF/examples/brybnd.nl"  ); rhoEnd= 3e-3; break; // 10
    case 269: of= new AMPLObjectiveFunction(i,"amplOF/examples/brownal.nl" ); rhoEnd= 8e-3; break; // 10
    case 270: of= new AMPLObjectiveFunction(i,"amplOF/examples/dqdrtic.nl" ); rhoEnd= 1e-3; break; // 10
    case 271: of= new AMPLObjectiveFunction(i,"amplOF/examples/watson.nl"  ); rhoEnd= 4e-2; break; // 12
    case 272: of= new AMPLObjectiveFunction(i,"amplOF/examples/dixmaank.nl"); rhoEnd= 3e-1; break; // 15
    case 273: of= new AMPLObjectiveFunction(i,"amplOF/examples/fminsurf.nl"); rhoEnd= 1e-1; break; // 16

    }
    if ((i>=250)&&(i<=273)) of->isConstrained=0;

    if ((rho)&&(rhoEnd!=-1)) *rho=rhoEnd;
    if (of==NULL)
    {
        printf("No objective function defined under index %i\n",i);
        exit(255);
    }
    return of;
}

void runAMPL()
{
    double rhoStart=1e-0, rhoEnd=1e-4;
    int niter=1000, t=0, total=0, total2=0;
    ObjectiveFunction *of;
    
    FILE *ff=fopen("resultsConstrained.txt","w");
    for (t=200; t<=212; t++)
    {
        if (t==209) continue;
//        if (t==211) continue;

        of=getObjectiveFunction(t,&rhoEnd);
        printf("Problem Name: %s\nDimension of the search space: %i\n",of->name,of->dim());
        CONDOR(rhoStart, rhoEnd, niter, of, 0);
        of->printStats();
        fprintf(ff,"%s & %i & %i & (%i) & %e \\\\\n", of->name, of->dim(), of->getNFE(), of->getNFE2(), of->valueBest);
        fflush(ff);
        total+=of->getNFE(); total2+=of->getNFE2();
        delete of;
    }
    fprintf(ff,"\n total number of function evaluation :%i (%i)\n", total, total2);
    fclose(ff);
}

int main(int argc, char **argv)
{
    {
       runAMPL();
   //     system("cls");
        printf("finished.\nPress return"); getchar();
    }
    return 0;
}

