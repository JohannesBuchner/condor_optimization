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

Vector min;
Vector max;
Vector discrete;

#ifdef MAXIMIZE
#define SIGN -
#else
#define SIGN 
#endif

#define OUTPUTFORMAT "%.10f"

class Interactive : public ObjectiveFunction
{
  public:
    Interactive(int _t);
    ~Interactive(){};
    double eval(Vector v, int *nerror=NULL);
    virtual double evalNLConstraint(int j, Vector v, int *nerror=NULL){ return 0; };
    virtual void evalGradNLConstraint(int j, Vector v, Vector result, int *nerror=NULL) {};
};

double Interactive::eval(Vector X, int *nerror)
{
    double *x=X;
    unsigned int i;
	double realval;
    for(i = 0; i < X.sz(); i++) {
		if(x[i] < bl[i] || x[i] > bu[i]) {
			fprintf(stderr, "Rejecting (dim=%d): "OUTPUTFORMAT"\n", i, x[i]);
		    updateCounter(SIGN (1e300*((x[i]-0.5)*(x[i]-0.5))),X);
			return SIGN (1e300*((x[i]-0.5)*(x[i]-0.5)));
		}
	}

    for(i = 0; i < X.sz(); i++) {
		realval = x[i]*(max[i] - min[i]) + min[i];
		if(discrete[i] == 1)
			printf("%d", (int)realval); 
		else
			printf(OUTPUTFORMAT, realval);
    	if (i != X.sz() - 1) {
			printf("\t");
		} else {
			printf("\n");
		}
    }
   	fflush(NULL);

	if (scanf("%lf", &realval) != 1) {
		fprintf(stderr, "user aborted.\n");
		exit(1);
	}
	fprintf(stderr, OUTPUTFORMAT"\n", realval);

    updateCounter(realval,X);
    return realval;
}

Interactive::Interactive(int _t)
{
    t=_t;
    strcpy(name,"INTERACTIVE");
}

void usage(char * progname) {
	printf("%s: SYNOPSIS: <exactness> parameters...\n"
		"\n"
		"\texactness: \thow detailled should we refine the search\n"
		"\tparameters:\teach parameter is a quadrupel of type, min, max, start\n"
		"\t\ttype: i for integer, d for double\n"
		"\t\tmin : lower bound of parameter values\n"
		"\t\tmax : upperbound of parameter values\n"
		"\t\tstart : starting point (0..1)\n"
		"\n"
		"example: %s 0.001 i -10 100 0 d 12.3 54 20\n"
		"\n", progname, progname);
	exit(1);
}

int main(int argc, char ** argv) {
    double rhoStart=1e0;
    int niter=1000;
	unsigned int i;
	unsigned int ndim;
	double exactness;
	if (argc < 2 + 4 * 1) {
		usage(argv[0]);
		return 1;
	}
	exactness = atof(argv[1]);
	if ((argc - 2) % 4) {
		fprintf(stderr, "%s: wrong number of arguments\n", argv[0]);
		usage(argv[0]);
	}
	ndim = (argc - 2) / 4;
    ObjectiveFunction *of;
    Interactive * ia = new Interactive(1);
    ia->xStart.setSize(ndim);
    ia->initBounds();
    ia->bu.setSize(ndim);
    ia->bl.setSize(ndim);
    min.setSize(ndim);
    max.setSize(ndim);
    discrete.setSize(ndim);
    ia->nNLConstraints=0;
    
	for (i = 0; i < ndim; i++) {
		if (strcmp(argv[2 + i * 4], "i") == 0) {
			discrete[i] = 1;
		} else if (strcmp(argv[2 + i * 4], "d") == 0) {
			discrete[i] = 0;
		}else{
			usage(argv[0]);
		}
		min[i] = atof(argv[2 + i * 4 + 1]);
		ia->bl[i] = 0;
		max[i] = atof(argv[2 + i * 4 + 2]);
		ia->bu[i] = 1;
		ia->xStart[i] = (atof(argv[2 + i * 4 + 3]) - min[i]) / (max[i] - min[i]);
	}
	ia->endInit();

    of = ia;
    fprintf(stderr, "Starting condor ...\n");
    of->setSaveFile();
    CONDOR(rhoStart, exactness, niter, of);
    
    fprintf(stderr, "Value: %e\n", of->valueBest + of->objectiveConst);
    fprintf(stderr, "Number of function Evaluation: %i (%i)\n", of->getNFE(), 
    	of->getNFE2());
    fprintf(stderr, "Solution : \n[");
    for (i=0; i<ndim; i++) {
    	if(i != 0)
    		fprintf(stderr, ", ");
		if(discrete[i] == 1)
	    	fprintf(stderr, "%d",(int) (of->xBest[i]*(max[i] - min[i]) + min[i]));
	    else
	    	fprintf(stderr, ""OUTPUTFORMAT,of->xBest[i]*(max[i] - min[i]) + min[i]);
    }
    fprintf(stderr, "]\n"); 

    delete of;
}


