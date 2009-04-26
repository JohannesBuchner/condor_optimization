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


#include "SIFFunction.h"

// extern elfunType elfunPARKCH_; extern groupType groupPARKCH_; 
extern elfunType elfunAkiva_;    extern groupType groupAkiva_;
extern elfunType elfunRosen_;    extern groupType groupRosen_;
extern elfunType elfunALLINITU_; extern groupType groupALLINITU_;
extern elfunType elfunSTRATEC_;  extern groupType groupSTRATEC_;
extern elfunType elfunTOINTGOR_; extern groupType groupTOINTGOR_;
extern elfunType elfunTOINTPSP_; extern groupType groupTOINTPSP_;
extern elfunType elfun3PK_;      extern groupType group3PK_;
extern elfunType elfunBIGGS6_;   extern groupType groupBIGGS6_;
extern elfunType elfunBROWNDEN_; extern groupType groupBROWNDEN_;
extern elfunType elfunDECONVU_;  extern groupType groupDECONVU_;
extern elfunType elfunHEART_;    extern groupType groupHEART_;
extern elfunType elfunOSBORNEB_; extern groupType groupOSBORNEB_;
extern elfunType elfunVIBRBEAM_; extern groupType groupVIBRBEAM_;
extern elfunType elfunKOWOSB_;   extern groupType groupKOWOSB_;
extern elfunType elfunHELIX_;    extern groupType groupHELIX_;

extern elfunType elfunCRAGGLVY_; extern groupType groupCRAGGLVY_;
extern elfunType elfunEIGENALS_; extern groupType groupEIGENALS_;
extern elfunType elfunHAIRY_;    extern groupType groupHAIRY_;
extern elfunType elfunPFIT1LS_;  extern groupType groupPFIT1LS_;
extern elfunType elfunVARDIM_;   extern groupType groupVARDIM_;
extern elfunType elfunMANCINO_;  extern groupType groupMANCINO_;
extern elfunType elfunPOWER_;    extern groupType groupPOWER_;
extern elfunType elfunHATFLDE_;  extern groupType groupHATFLDE_;
extern elfunType elfunWATSON_;   extern groupType groupWATSON_;
extern elfunType elfunFMINSURF_; extern groupType groupFMINSURF_;
extern elfunType elfunDIXMAANK_; extern groupType groupDIXMAANK_;
extern elfunType elfunMOREBV_;   extern groupType groupMOREBV_;
extern elfunType elfunBRYBND_;   extern groupType groupBRYBND_;
extern elfunType elfunSCHMVETT_; extern groupType groupSCHMVETT_;
extern elfunType elfunHEART6LS_; extern groupType groupHEART6LS_;
extern elfunType elfunBROWNAL_;  extern groupType groupBROWNAL_;
extern elfunType elfunDQDRTIC_;  extern groupType groupDQDRTIC_;
extern elfunType elfunGROWTHLS_; extern groupType groupGROWTHLS_;
extern elfunType elfunSISSER_;   extern groupType groupSISSER_;
extern elfunType elfunCLIFF_;    extern groupType groupCLIFF_;
extern elfunType elfunGULF_;     extern groupType groupGULF_;
extern elfunType elfunSNAIL_;    extern groupType groupSNAIL_;
extern elfunType elfunHART6_;    extern groupType groupHART6_;

ObjectiveFunction *getObjectiveFunction(int i, double *rho)
{
    ObjectiveFunction *of=NULL;
    double rhoEnd=-1;

    switch (i)
    {

    // use SIF file
    case 104: of= new SIFFunction(i,"sifOF/examples/akiva.d"     ,elfunAkiva_    ,groupAkiva_   ); break; // 2
    case 105: of= new SIFFunction(i,"sifOF/examples/allinitu.d"  ,elfunALLINITU_ ,groupALLINITU_); break; // 4
    case 106: of= new SIFFunction(i,"sifOF/examples/stratec.d"   ,elfunSTRATEC_  ,groupSTRATEC_ ); break; // 10 
    case 107: of= new SIFFunction(i,"sifOF/examples/heart.d"     ,elfunHEART_    ,groupHEART_   ); break; // 8
    case 108: of= new SIFFunction(i,"sifOF/examples/osborneb.d"  ,elfunOSBORNEB_ ,groupOSBORNEB_); break; // 11
    case 109: of= new SIFFunction(i,"sifOF/examples/vibrbeam.d"  ,elfunVIBRBEAM_ ,groupVIBRBEAM_); break; // 8
    case 110: of= new SIFFunction(i,"sifOF/examples/kowosb.d"    ,elfunKOWOSB_   ,groupKOWOSB_  ); break; // 4
    case 111: of= new SIFFunction(i,"sifOF/examples/helix.d"     ,elfunHELIX_    ,groupHELIX_   ); break; // 3

    case 112: of= new SIFFunction(i,"sifOF/examples/rosenbrock.d",elfunRosen_    ,groupRosen_   ); rhoEnd= 5e-3; break; // 2
    case 114: of= new SIFFunction(i,"sifOF/examples/sisser.d"    ,elfunSISSER_   ,groupSISSER_  ); rhoEnd= 1e-2; break; // 2
    case 115: of= new SIFFunction(i,"sifOF/examples/cliff.d"     ,elfunCLIFF_    ,groupCLIFF_   ); rhoEnd= 1e-3; break; // 2
    case 116: of= new SIFFunction(i,"sifOF/examples/hairy.d"     ,elfunHAIRY_    ,groupHAIRY_   ); rhoEnd= 2e-3; break; // 2
    case 117: of= new SIFFunction(i,"sifOF/examples/pfit1ls.d"   ,elfunPFIT1LS_  ,groupPFIT1LS_ ); rhoEnd= 1e-2; break; // 3
    case 118: of= new SIFFunction(i,"sifOF/examples/hatflde.d"   ,elfunHATFLDE_  ,groupHATFLDE_ ); rhoEnd=12e-3; break; // 3
    case 119: of= new SIFFunction(i,"sifOF/examples/schmvett.d"  ,elfunSCHMVETT_ ,groupSCHMVETT_); rhoEnd= 1e-2; break; // 3
    case 120: of= new SIFFunction(i,"sifOF/examples/growthls.d"  ,elfunGROWTHLS_ ,groupGROWTHLS_); rhoEnd= 5e-3; break; // 3
    case 121: of= new SIFFunction(i,"sifOF/examples/gulf.d"      ,elfunGULF_     ,groupGULF_    ); rhoEnd= 5e-2; break; // 3
    case 122: of= new SIFFunction(i,"sifOF/examples/brownden.d"  ,elfunBROWNDEN_ ,groupBROWNDEN_); rhoEnd=57e-2; break; // 4
    case 123: of= new SIFFunction(i,"sifOF/examples/eigenals.d"  ,elfunEIGENALS_ ,groupEIGENALS_); rhoEnd= 1e-2; break; // 6
    case 124: of= new SIFFunction(i,"sifOF/examples/heart6ls.d"  ,elfunHEART6LS_ ,groupHEART6LS_); rhoEnd= 5e-2; break; // 6
    case 125: of= new SIFFunction(i,"sifOF/examples/biggs6.d"    ,elfunBIGGS6_   ,groupBIGGS6_  ); rhoEnd= 6e-2; break; // 6
    case 126: of= new SIFFunction(i,"sifOF/examples/hart6.d"     ,elfunHART6_    ,groupHART6_   ); rhoEnd= 2e-1; break; // 6
    case 127: of= new SIFFunction(i,"sifOF/examples/cragglvy.d"  ,elfunCRAGGLVY_ ,groupCRAGGLVY_); rhoEnd= 6e-2; break; // 10
    case 128: of= new SIFFunction(i,"sifOF/examples/vardim.d"    ,elfunVARDIM_   ,groupVARDIM_  ); rhoEnd= 1e-3; break; // 10
    case 129: of= new SIFFunction(i,"sifOF/examples/mancino.d"   ,elfunMANCINO_  ,groupMANCINO_ ); rhoEnd= 1e-6; break; // 10
    case 130: of= new SIFFunction(i,"sifOF/examples/power.d"     ,elfunPOWER_    ,groupPOWER_   ); rhoEnd= 2e-2; break; // 10
    case 131: of= new SIFFunction(i,"sifOF/examples/morebv.d"    ,elfunMOREBV_   ,groupMOREBV_  ); rhoEnd= 1e-1; break; // 10
    case 132: of= new SIFFunction(i,"sifOF/examples/brybnd.d"    ,elfunBRYBND_   ,groupBRYBND_  ); rhoEnd= 6e-3; break; // 10
    case 133: of= new SIFFunction(i,"sifOF/examples/brownal.d"   ,elfunBROWNAL_  ,groupBROWNAL_ ); rhoEnd= 8e-3; break; // 10
    case 134: of= new SIFFunction(i,"sifOF/examples/dqdrtic.d"   ,elfunDQDRTIC_  ,groupDQDRTIC_ ); rhoEnd= 1e-3; break; // 10
    case 135: of= new SIFFunction(i,"sifOF/examples/watson.d"    ,elfunWATSON_   ,groupWATSON_  ); rhoEnd= 4e-2; break; // 12
    case 137: of= new SIFFunction(i,"sifOF/examples/fminsurf.d"  ,elfunFMINSURF_ ,groupFMINSURF_); rhoEnd= 1e-1; break; // 16

    case 138: of= new SIFFunction(i,"sifOF/examples/tointgor.d"  ,elfunTOINTGOR_ ,groupTOINTGOR_); break; // 50
    case 139: of= new SIFFunction(i,"sifOF/examples/tointpsp.d"  ,elfunTOINTPSP_ ,groupTOINTPSP_); break; // 50
    case 140: of= new SIFFunction(i,"sifOF/examples/3pk.d"       ,elfun3PK_      ,group3PK_     ); break; // 30
    case 141: of= new SIFFunction(i,"sifOF/examples/deconvu.d"   ,elfunDECONVU_  ,groupDECONVU_ ); break; // 61
//  case 142: of= new SIFFunction(i,"sifOF/examples/parkch.d"    ,elfunPARKCH_   ,groupPARKCH_  ); break; // 15 

#ifdef WIN32
    case 113: of= new SIFFunction(i,"sifOF/examples/snail.d"     ,elfunSNAIL_    ,groupSNAIL_   ); rhoEnd= 2e-4; break; // 2
    case 136: of= new SIFFunction(i,"sifOF/examples/dixmaank.d"  ,elfunDIXMAANK_ ,groupDIXMAANK_); rhoEnd= 3e-1; break; // 15
#else
    case 113: of= new SIFFunction(i,"sifOF/examples/snail.d"     ,elfunSNAIL_    ,groupSNAIL_   ); rhoEnd= 7e-4; break; // 2
    case 136: of= new SIFFunction(i,"sifOF/examples/dixmaank.d"  ,elfunDIXMAANK_ ,groupDIXMAANK_); rhoEnd= 4e-1; break; // 15
#endif

    }

    if ((rho)&&(rhoEnd!=-1)) *rho=rhoEnd;
    if (of==NULL)
    {
        printf("No objective function defined under index %i\n",i);
        exit(255);
    }
    return of;
}

void testCONDORonSIF()
{
    double rhoStart=1e-1, rhoEnd=1e-4;
    int niter=1000, t=0, total=0, total2=0;
    ObjectiveFunction *of;
    
    FILE *ff=fopen("results.txt","w");

    for (t=112; t<=137; t++)
    {        
        of=getObjectiveFunction(t,&rhoEnd);

        // to debug objective function:
       // double d=of->eval(of->xStart);
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
        testCONDORonSIF();
   //     system("cls");
        printf("finished.\nPress return"); getchar();
    }
    return 0;
}

