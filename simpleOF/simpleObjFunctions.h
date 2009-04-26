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
#ifndef SIMPLE_OBJECTIVEFUNCTION_INCLUDE
#define SIMPLE_OBJECTIVEFUNCTION_INCLUDE

#include "../common/ObjectiveFunction.h"

class FletcherTest: public ObjectiveFunction 
{
    // practical method of optimization
    // page 199 equation 9.1.15
    // page 142 figure 7.1.3
  public:
    FletcherTest(int _t);
    ~FletcherTest(){};

    double eval(Vector v, int *nerror=NULL);
    virtual double evalNLConstraint(int j, Vector v, int *nerror=NULL);
    virtual void evalGradNLConstraint(int j, Vector v, Vector result, int *nerror=NULL);
};

class FletcherTest2: public ObjectiveFunction 
{
    // practical method of optimization
    // page 199 equation 9.1.15
    // page 142 figure 7.1.3
  public:
    FletcherTest2(int _t);
    ~FletcherTest2(){};

    double eval(Vector v, int *nerror=NULL);
    virtual double evalNLConstraint(int j, Vector v, int *nerror=NULL){return 0.0;};
    virtual void evalGradNLConstraint(int j, Vector v, Vector result, int *nerror=NULL){};
};

class SuperSimpleConstrainedObjectiveFunction: public ObjectiveFunction 
{
  public:
    SuperSimpleConstrainedObjectiveFunction(int _t);
    ~SuperSimpleConstrainedObjectiveFunction(){};

    double eval(Vector v, int *nerror=NULL);
    virtual double evalNLConstraint(int j, Vector v, int *nerror=NULL){ return 0; };
    virtual void evalGradNLConstraint(int j, Vector v, Vector result, int *nerror=NULL) {};
};

class Rosenbrock : public UnconstrainedObjectiveFunction
{
  public:
    Rosenbrock(int _t);
    ~Rosenbrock(){};
    double eval(Vector v, int *nerror=NULL);
};

class NoisyRosenbrock : public UnconstrainedObjectiveFunction
{
  public:
    NoisyRosenbrock(int _t);
    ~NoisyRosenbrock(){};
    double eval(Vector v, int *nerror=NULL);
};

class NoisyQuadratic : public UnconstrainedObjectiveFunction
{
  public:
    NoisyQuadratic(int _t, int _n=0);
    ~NoisyQuadratic(){};
    double eval(Vector v, int *nerror=NULL);
  private:
    int n;
};

class SimpleQuadratic : public UnconstrainedObjectiveFunction
{
  public:
    SimpleQuadratic(int _t, int _n=0);
    ~SimpleQuadratic(){};
    double eval(Vector v, int *nerror=NULL);
  private:
    int n;
};

class BADScaleRosenbrock : public UnconstrainedObjectiveFunction
{
  public:
    BADScaleRosenbrock(int _t);
    ~BADScaleRosenbrock(){};
    double eval(Vector v, int *nerror=NULL);
    virtual void finalize(Vector v, Matrix H, Vector l){saveStats("results.txt",v,H,l);};
};

class RandomOF: public UnconstrainedObjectiveFunction
{
  public:
    RandomOF(int _t,int n);
    RandomOF(int _t,char *);
    ~RandomOF(){};
    double eval(Vector v, int *nerror=NULL);
    double ff(Vector v);
    void save(char *);

  private:
    Vector A;
    Matrix S,C;
    void alloc(int n);
};

#endif

