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
#include "../../common/tools.h"
#include "../XMLof.h"

#ifdef WIN32
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#endif

int main(int argc, char **argv)
{
    XMLof *of;
    CorrectScaleOF *ofc;    

    double fake;
    if (argc>1) of=new XMLof(30,argv[1],&fake); 
    else of=new XMLof(30,"optim.cfg",&fake);

    if (of->scalingFactorIsGiven) ofc=new CorrectScaleOF(31,of, of->vScale);
    else ofc=new CorrectScaleOF(31,of);

    ofc->rescaling.set(1.0);

    of->DBTool(ofc->rescaling);

    delete of;
    return 0;
}
