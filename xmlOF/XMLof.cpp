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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <float.h> // for _isnan and _isfinite
#include <windows.h>
#include <direct.h> // to have _getcwd
#include <process.h>
#include <crtdbg.h>

#else
//#include <signal.h>
//#include <setjmp.h>
#include <sys/wait.h>
#include <unistd.h>

#include <math.h>  // for  isnan and isfinite
static int _isnan (double d) { return isnan(d); }
static int _finite(double d) { return isfinite(d); }

#endif

#include "XMLof.h"
#include "../common/tools.h"
#include "../common/KeepBests.h"
#include "xmlParser.h"

void XMLof::DBTool(Vector rescale)
{
    printf("DBTool\n");
    if (debugOF==NULL)
    {
        printf("no output file! ('traceFile' undefined)\n");
        exit(255);
    }
    int i,j,jj,k,l, nl=dataFull.nLine(), lineSize=nToOptim*2+1+nSubAggrFunction+2, 
        err, N=(nToOptim+1)*(nToOptim+2)/2;
    Matrix mH(nToOptim,nToOptim), mRData(dataFull.nLine(),nToOptim+2);
    Vector vG(nToOptim), vLine(lineSize), vShort(nToOptim), 
            vBase(nToOptim), vInvertedRescale(nToOptim);
    VectorInt vError(nl);
    int *error=vError;
    double **dataf=(double**)dataFull,r, *pbase=vBase, *vs=vShort, 
           *vstart=vFullStart, norm, **datar=mRData,
    //    rho=rhoEnd*100;
          rho=rhostart*4*.9;//rhostart*.9;

    vLine.zero();
    KeepBests kb(N*2,nToOptim);

    for (j=0; j<nl; j++)
        aggregate(dataf[j], NULL, error+j);

    vInvertedRescale=rescale.clone();
    vInvertedRescale.oneByOneInvert();


    for (jj=0; jj<nl; jj++)
    {
        longXtoShortX(dataf[jj],pbase);
        memcpy((double*)vLine,pbase,nToOptim*sizeof(double));
        vLine[lineSize-2]=r=aggregate(dataf[jj], (double*)vIndexes, &err);
        vLine[lineSize-1]=err;
        if (nSubAggrFunction) vLine.setPart(nToOptim*2+1,vIndexes);
        vSigNoise.zero();
        
        vBase.oneByOneMutiply(vInvertedRescale);
        memcpy(datar[0],(double*)vBase,nToOptim*sizeof(double));
        datar[0][nToOptim]=r;
        datar[0][nToOptim+1]=err;

        i=nl; j=1;
        while (i--)
        {
            if (i==jj) continue;
            if (error[i]) continue;

            // means: longXtoShortX(dataf[i],vs); + subspace check
            l=0;
            for (k=0; k<fullDim; k++)
                if (vXToOptim[k]) { vs[l]=dataf[i][k]; l++; }
                else if (vstart[k]!=dataf[i][k]) break;

            if (k!=fullDim) continue;

            vShort.oneByOneMutiply(vInvertedRescale);

            norm=0; k=nToOptim;
            while (k--) norm+=sqr(vs[k]-pbase[k]);
            if (norm<=4.001*rho*rho)
            { 
                memcpy(datar[j],vs,nToOptim*sizeof(double)); 
                datar[j][nToOptim]=aggregate(dataf[i], NULL, &err);
                datar[j][nToOptim+1]=err;
                j++;
            }
        }

        if (j>=N)
        {
            mRData.setNLine(j);
            InterPolynomial poly(2, rho, vBase, mRData, NULL);
            if (poly.NewtonBasis!=NULL)
            {
                poly.gradientHessian(vBase,vG,mH);
                mH.multiplyByDiagonalMatrix(vInvertedRescale);
                vInvertedRescale.diagonalizeAndMultiply(mH);
                computeSigNoise(mH);
            }
        }
        vLine.setPart(nToOptim,vSigNoise);
        vLine.appendToMatrixFile(debugOF,names);
    }
    printf("Finished successfully\n");
}

double XMLof::evalNLConstraint(int j, Vector sv, int *ner)
{ 
   double *lx=vLx;
   shortXToLongX(sv,lx);
   return nlinequalities[j]->eval(lx,ner);
}

void XMLof::evalGradNLConstraint(int j, Vector sv, Vector result, int *ner)
{
    result.zero();
    double *grad=result, *lx=vLx;
    shortXToLongX(sv,lx);
    nlinequalities[j]->evalGrad(lx,grad,ner);
}

void launchObjFunction(double tt, char *configFile, char *objFunction,char *inputObjF, char *outputObjF)
{
    double t=tt;
    int k_old=-1, k=0;
    int er=0;
    FILE *stream;
#ifdef WIN32
    printf("running OF\n");
    remove(outputObjF);
    er=(int)_spawnlp(_P_NOWAIT,objFunction,
                               objFunction,
                               configFile,
                               inputObjF,
                               outputObjF,NULL);
#else
    unlink(outputObjF);
    if (fork()==0)
    {
        execlp(objFunction,
               objFunction,
               configFile,
               inputObjF,
               outputObjF,NULL);
        er=-1;
    }
#endif
    if (er==-1)
    {
		perror("\nError in evaluation of OF");
		printf("   Executable= %s\n"
               "   configFile= %s\n"
               "   inputObjF = %s\n"
               "   outputObjF= %s\n",objFunction,configFile,inputObjF,outputObjF);

        exit(1);
    }
    // wait until the whole file has been written to the disk
    // (if the disk is a network drive (mounted drive) it can take
    // some time)
    while (true)
    {
        #ifdef WIN32
        Sleep((int)(t*1000));
        #else
        sleep((int)t);
        #endif
        stream=fopen(outputObjF,"rb");
        if (stream==NULL) continue;     
        fseek(stream, 0, SEEK_END);
        k=ftell(stream);
        fclose(stream);
        t=mmin(1.0,tt);
        if (k==k_old) break;
        k_old=k;
    }
    #ifndef WIN32
    wait(NULL);
    #endif
    printf("OF Evaluation finished");
}

void evalLarge(int nIndexInOF, Vector vLx, char *objFunction, char *configFile, char *inputObjF, 
                 char *outputObjF, Vector vIndexes, double timeToSleep, int *nerror)
{
	char c;
    FILE *stream;


    //	printf("CONDOR output:"); vLx.print(); fflush(0);
    vLx.save(inputObjF,2);
    
    launchObjFunction(timeToSleep, configFile, objFunction, inputObjF, outputObjF);

    stream=fopen(outputObjF,"rb");
    c=fgetc(stream);
    if (c=='A')
    {
// ascii input
        LPTSTR b=loadFile(stream), *b2=b;
        b=(LPTSTR)skipLine(b); // skip the line with the A
        *nerror=1-atol(b); // line of the error flag
        if (*nerror)
        {
		    printf("(failed)\n");
            free(b2);
            return;
        }
        b=vIndexes.getFromLine(removeAllEOL((LPTSTR)skipLine(b)));
        if ((int)vIndexes.sz()<nIndexInOF)
        {
            b[mmin((int)strlen(b),15)]=0;
            printf("(failed)(wrong %ith value in string: '%s')\n", vIndexes.sz(), b-5);
            free(b2);
            *nerror=1;
            vIndexes.setSize(nIndexInOF);
            return;
        }
        free(b2);
	    printf("(success)\n");
    } else
    {
// binary input
        fread(nerror,sizeof(int),1,stream);
        *nerror=1-*nerror;
        if (*nerror)
        {
		    printf("(failed)\n");
            fclose(stream);
            return;
        }
        double *v=vIndexes;
        int i=(int)fread(v, sizeof(double), nIndexInOF,stream);
        fclose(stream);
        if (i!=nIndexInOF)
        {
            printf("(failed)(return file too short)\n");
            *nerror=1; return;
        }
        while (i--)
            if ((_isnan(v[i]))||(!_finite(v[i]))) 
            { 
                printf("(failed)(strange return value for %ith index variable)\n",i+1);
                *nerror=1; return; 
            }
	    printf("(success)\n");

    }

//	printf("OF condor Value:%e\n",s); fflush(0);
    
    // aggregation into one (and only one) value.
}

void XMLof::shortXToLongX(double *sx, double *llx)
{
    int i=fullDim,j=nToOptim, fake;
    double *xf=vFullStart;
    char *xToOptim=vXToOptim;
    while (i--)
        if (xToOptim[i]) llx[i]=sx[--j]; 
        else llx[i]=xf[i];
    for (i=0; i<neq; i++)
        llx[equalities[i].ivar]=equalities[i].me->eval(llx,&fake);
}

void XMLof::longXtoShortX(double *llx, double *sx)
{
    int i,j=0;
    for (i=0; i<fullDim; i++)
        if (vXToOptim[i]) { sx[j]=llx[i]; j++; }
}

//void userFonction(double *t, double s)
//{
//    FILE *stream2;
//    computationNumber++;
//    stream2=fopen("/home/fvandenb/L6/progress.txt","a");
//    fprintf(stream2,"%i %f %f %f %f %f %f %f\n",computationNumber, t[18],t[19],t[23],t[16+25],t[18+25],t[19+25],s);
//    fclose(stream2);
//}

XMLof::~XMLof()
{
    if (dbname) free(dbname);
    free(*dbcolnames); free(dbcolnames); free(charMemSpace);
    if (debugOF) 
    {
        free(debugOF);
        free(names[nToOptim]); free(names);
    }
    free(resultsFile);
    free(configFile);
    if (objFunction) 
    {
        free(objFunction);
        free(outputObjF);
        free(inputObjF);
    }
    // free matheval
    int i;
    if (neq)
    {
        for (i=0; i<neq; i++) delete equalities[i].me;
        free(equalities);
    }
    if (nSubAggrFunction)
    {
        for (i=0; i<nSubAggrFunction; i++) { delete af[i].me; free(af[i].name); }
        free(af);
    }
    if (nNLConstraints)
    {
        for (i=0; i<nNLConstraints; i++) delete nlinequalities[i];
        free(nlinequalities);
    }
}

const char *indexVar(int *r, const char *to,char **names,int fullDim)
{
    int i,j=0,k;
    char *t=(LPTSTR)skipSpaces(to);
    while ((t[j]!=' ')&&(t[j]!='\t')&&(t[j]!='=')&&(t[j])) j++;
    if (t[j]==0) return NULL;
    k=j;
    if (t[k]!='=')
        while ((t[k]!='=')&&(t[k])) k++;
    if (t[k]==0) return NULL;
    to=t+k+1;
    for (i=0; i<fullDim; i++)
        if (memcmp(t,names[i],j)==0) { *r=i; return to; }
    return NULL;
}

void XMLof::initXMLBounds()
{    
    int dim=fullDim;
    bl.setSize(dim);
    bu.setSize(dim);
    double *dbl=bl,*dbu=bu;
    while (dim--)
    {
        *(dbl++)=-INF;
        *(dbu++)=INF;
    }
}

char *getFullPath(const char *filename)
{
    int i=0;
	char line[30000];
#ifdef WIN32
    _getcwd(line,300000); 
    if (line[1]!=':')
    {
        i=(int)strlen(line)-1;
        if (line[i]!='\\') { i++; line[i]='\\'; } i++;
    }
#else
    getcwd(line,300000); 
    if (line[1]!='/')
    {
        i=(int)strlen(line)-1;
        if (line[i]!='/') { i++; line[i]='/'; } i++;
    }
#endif
    strcpy(line+i,filename);
    return stringDuplicate(line);
}

void XMLof::init(char *filename)
{
	int i=0,j;

    // we need the full path because it will be given as first
    // parameter to the objective function
    configFile=getFullPath(filename);

    XMLResults pResults;
    XMLNode xMainNode=XMLNode::parseFile(configFile,&pResults);
    if (pResults.error)
    {
        printf(
            "XML Parsing error inside config file '%s'.\n"
            "Error: %s\n"
            "At line %i, column %i\n",configFile,
            XMLNode::getError(pResults.error),pResults.nLine,pResults.nColumn);
        free(configFile);
        exit(255);
    }

    XMLNode nodeTmp,xnode=xMainNode; i=0;
    while (i<xnode.nChildNode())
    {
        nodeTmp=xnode.getChildNode(i); 
        if (strcmp(nodeTmp.getName(),"configCONDOR")==0) break;
        if (nodeTmp.isDeclaration()) { xnode=nodeTmp; i=0; } else i++;
    }
    if (i==xnode.nChildNode())
    {
        printf("Cannot find 'configCONDOR' tag\n.");
        exit(255);
    }
    xMainNode=nodeTmp;

    // get name of all the variables.
    // get dimension of the problem
    char **allVarNames=NULL;

    nodeTmp=xMainNode.getChildNode(_T("varNames"));
    if (nodeTmp.isEmpty())
    {
        printf("Cannot find 'varNames'.\n");
        exit(255);
    }
    LPCTSTR t=(LPTSTR)nodeTmp.getText();
    if (t)
    {
        fullDim=0; 
        allVarNames=getNameTable(t, &fullDim);
        t=nodeTmp.getAttribute(_T("dimension"));

        if ((t)&&(fullDim!=atol(removeQuotes((LPTSTR)t))))
        {
            printf("Error in 'varNames': dimension attribute and number of named variables does not match.\n");
            exit(255);
        }
    } else
    {
        t=nodeTmp.getAttribute(_T("dimension"));
        if (!t)
        {
            printf("Error: no content and no 'dimension' attribute to tag 'varNames'.\n");
            exit(255);
        }
        fullDim=atol(removeQuotes((LPTSTR)t));
    }
    if (fullDim<2)
    {
        printf("Error: dimension of the search space must be at least 2.\n");
        exit(255);
    }

    xnode=xMainNode.getChildNode(_T("objectiveFunction"));
    if (xnode.isEmpty())
    {
        printf("Cannot find 'objectiveFunction'.\n");
        exit(255);
    }

    int nIndexNames=0;
    char **indexNames=NULL;
    nIndexInOF=0;
    t=xnode.getAttribute("nIndex");
    if (t)
    {
        nIndexInOF=atol(removeQuotes((LPTSTR)t));
        if (nIndexInOF<1)
        {
            printf("Error: 'nIndex' attribute must be at least 1.\n");
            exit(255);
        }
        nodeTmp=xnode.getChildNode(_T("indexNames"));
        if ((nodeTmp.isEmpty())||(nodeTmp.getText()==NULL))
        {
            nIndexNames=1;
            indexNames=(char**)malloc(sizeof(char*));
            indexNames[0]=stringDuplicate("Y");
        } else
        {
            indexNames=getNameTable(nodeTmp.getText(), &nIndexNames);
            if (nIndexNames>nIndexInOF)
            {
                printf("Error: to many 'indexNames' compared to attribute 'nIndex'.\n");
                exit(255);
            }
        }
    } else
    {
        nodeTmp=xnode.getChildNode(_T("indexNames"));
        if ((!nodeTmp.isEmpty())&&(nodeTmp.getText()!=NULL))
        {
            nIndexNames=0;
            indexNames=getNameTable(t, &nIndexNames);
            nIndexInOF=nIndexNames;
        }
    }
    // vector used when reading back values given at each evaluation of the OF:
    vIndexes.setSize(nIndexInOF);
    vLxIndexes.setSize(nIndexInOF+fullDim);
    // create dbcolnames
    setNamesForDBfile(nIndexNames, indexNames, allVarNames);

    printf("Dimension of the search space=%i.\nDesign Variable Names: %s",fullDim,dbcolnames[0]);
    for (i=1; i<fullDim; i++) printf(" ,%s",dbcolnames[i]);
    printf("\n");

    nodeTmp=xnode.getChildNode(_T("aggregationFunction"));
    nSubAggrFunction=0;
    char OFExeRequired=TRUE;
    if (!nodeTmp.isEmpty()) 
    {

        nSubAggrFunction=nodeTmp.nChildNode(_T("subAggregationFunction"));
        if (nSubAggrFunction)
        {
            af=(aggregationFunction*)malloc(nSubAggrFunction*sizeof(aggregationFunction));
            XMLNode nodeTmp2;
            int nindex=0;
            OFExeRequired=FALSE;
            for (i=0; i<nSubAggrFunction; i++)
            {
                nodeTmp2=nodeTmp.getChildNode(_T("subAggregationFunction"),&nindex);
                t=nodeTmp2.getAttribute("name");
                if (t) af[i].name=stringDuplicate(t); else 
                {
                    char *buffer[30];
                    sprintf((char*)buffer,"SUBOBJ_%i",i);
                    af[i].name=stringDuplicate((char*)buffer);
                }
                if (nodeTmp2.nText()==0)
                {
                    printf("Subobjective '%s' is empty.\n",af[i].name);
                    exit(255);
                }
                t=nodeTmp2.getText();
                af[i].me=new MathEvalClass((LPTSTR)t,dbcolnames,fullDim+nIndexInOF, FALSE);
                if (af[i].me->maxIndexVarUsed()>=fullDim) OFExeRequired=TRUE;
            }
        } else
        {
            if (nodeTmp.nText())
            {
                nSubAggrFunction=1;
                af=(aggregationFunction*)malloc(sizeof(aggregationFunction));
                t=nodeTmp.getText();
                af[0].me=new MathEvalClass((LPTSTR)t,dbcolnames,fullDim+nIndexInOF, FALSE);
                if (af[0].me->maxIndexVarUsed()<fullDim) OFExeRequired=FALSE;
                af[0].name=NULL;
            }
        }
    }

    if (OFExeRequired)
    {
        if (nIndexInOF==0)
        {
            printf("Error: you must give a value for 'nIndex attibute' or give a 'indexNames' tag.\n");
            exit(255);
        }
        printf("Executable for evaluation of the objective function is required.\n"
               "Number of Index variables: %i\nIndex variables names: %s",nIndexInOF, dbcolnames[fullDim]);
        for (i=1; i<nIndexInOF; i++) printf(" ,%s",dbcolnames[fullDim+i]);
        printf("\n");
    } else printf("No executable required to evaluate the Objective Function.\n");

    switch (nSubAggrFunction)
    {
    case 0: printf("Aggregation function is the sum of all the index variables (default case)\n"); break;
    case 1: printf("Aggregation function is '%s'.\n", t); break;
    default: 
        printf("Number of Aggregation functions: %i\nNames of aggregation functions: %s", nSubAggrFunction,af[0].name);

        for (i=0; i<nSubAggrFunction; i++) printf(" ,%s",af[i].name);
        printf("\n");
    }

    objFunction=NULL;
    if (OFExeRequired)
    {
        nodeTmp=xnode.getChildNode(_T("executableFile"));
        if (nodeTmp.isEmpty())
        {
            printf("cannot find 'executableFile'.");
            exit(255);
        }
        t=nodeTmp.getText();
        if (!t)
        {
            printf("no 'executableFile' content.\n");
            exit(255);
        }
        objFunction=getFullPath(t);
        printf("ExecutableFile: '%s'.\n", objFunction);

        nodeTmp=xnode.getChildNode(_T("inputObjectiveFile"));
        if (nodeTmp.isEmpty())
        {
            printf("cannot find 'inputObjectiveFile'.");
            exit(255);
        }
        t=nodeTmp.getText();
        if (!t)
        {
            printf("no 'inputObjectiveFile' content.\n");
            exit(255);
        }
        inputObjF=getFullPath(t);
        printf("InputObjectiveFile: '%s'.\n", inputObjF);

        nodeTmp=xnode.getChildNode(_T("outputObjectiveFile"));
        if (nodeTmp.isEmpty())
        {
            printf("cannot find 'outputObjectiveFile'.");
            exit(255);
        }
        t=nodeTmp.getText();
        if (!t)
        {
            printf("no 'outputObjectiveFile' content.\n");
            exit(255);
        }
        outputObjF=getFullPath(t);
        printf("OutputObjectiveFile: '%s'.\n", outputObjF);
    }

    nodeTmp=xnode.getChildNode(_T("variablesToOptimize"));

    Vector vTmp(fullDim);
    double *tmp=vTmp;
    vTmp.set(1.0);
    if (!nodeTmp.isEmpty()) 
    {
        t=nodeTmp.getText();
        if (*t) 
        {
            t=vTmp.getFromLine((LPTSTR)t);
            if (((int)vTmp.sz()!=fullDim)||(!isEmpty(t)))
            {
                printf("error in 'variablesToOptimize': not the right dimension.\n");
                exit(255);
            }
        }
    }

    vXToOptim.setSize(fullDim); 
    char   *xToOptim=vXToOptim;
    nToOptim=0;
	for (j=0; j<fullDim; j++)
	{
		xToOptim[j]=(tmp[j]!=0.0);
		if (xToOptim[j]) nToOptim++;
	}
	if (nToOptim==0)
	{
        printf("error in 'variablesToOptimize': no variables to optimize.\n");
		exit(255);
	};
    printf("Optimizing %i variables in a global search space of dimension %i.\n"
        "The variables which are currently optimized are: ",nToOptim,fullDim);
    i=0;
	for (j=0; j<fullDim; j++)
		if (xToOptim[j]) 
        {
            if (i) printf(" ,%s",dbcolnames[j]);
            else { printf("%s",dbcolnames[j]); i=1; }
        }
    printf("\n");

    nodeTmp=xMainNode.getChildNode(_T("startingPoint"));
    if ((nodeTmp.isEmpty())||(nodeTmp.getText()==NULL)) 
    {
        startPointIsGiven=0;
        printf("No starting point: using best from DB file.\n");
    }
    else
    {
        startPointIsGiven=1;
        vFullStart.setSize(fullDim);
        t=vFullStart.getFromLine((LPTSTR)nodeTmp.getText());
        if (((int)vFullStart.sz()!=fullDim)||(!isEmpty(t)))
        {
            printf("Error in 'startingpoint': wrong dimension.\n");
            exit(255);
        }
        printf("Starting point is: "); vFullStart.print();
    }

    initXMLBounds();
    neq=0;
    nNLConstraints=0;
    xnode=xMainNode.getChildNode(_T("constraints"));
    if (xnode.isEmpty()) printf("No Constraints.\n");
    else
    {
        nodeTmp=xnode.getChildNode(_T("lowerBounds"));
        if ((!nodeTmp.isEmpty())&&(nodeTmp.getText()!=NULL))
        {
            bl.setSize(fullDim);
            t=bl.getFromLine((LPTSTR)nodeTmp.getText());
            if (((int)bl.sz()!=fullDim)||(!isEmpty(t)))
            {
                printf("Error in lower bound.\n");
                exit(255);
            }
            printf("Lower bounds are: "); bl.print();            
        } else printf("No Lower Bounds.\n");

        nodeTmp=xnode.getChildNode(_T("upperBounds"));
        if ((!nodeTmp.isEmpty())&&(nodeTmp.getText()!=NULL))
        {
            bu.setSize(fullDim);
            t=bu.getFromLine((LPTSTR)nodeTmp.getText());
            if (((int)bu.sz()!=fullDim)||(!isEmpty(t)))
            {
                printf("Error in upper bound.\n");
                exit(255);
            }
            printf("Upper bounds are: "); bu.print();            
        } else printf("No Upper Bounds.\n");

        nodeTmp=xnode.getChildNode(_T("linearInequalities"));
        if (nodeTmp.isEmpty()) printf("No Linear Inequalities.\n");
        else
        {
            int nineq=nodeTmp.nChildNode(_T("eq"));
            if (nineq)
            {
                if (nodeTmp.getText())
                {
                    printf("Error in linear inequalities: both notations are in use.\n");
                    exit(255);
                }
                A.setSize(nineq,fullDim);
                b.setSize(nineq);
                vTmp.setSize(fullDim+1); tmp=(double*)vTmp;
                int nIndex=0;
                for (i=0; i<nineq; i++)
                {
                    t=nodeTmp.getChildNode(_T("eq"),&nIndex).getText();
                    if (t==NULL)
                    {
                        printf("Error in linear inequality %i: empty line.\n",i+1);
                        exit(255);
                    }
                    t=vTmp.getFromLine((LPTSTR)t);
                    if ((int)vTmp.sz()!=fullDim+1)
                    {
                        printf("Error in linear inequality %i: line too short.\n", i+1);
                        exit(255);
                    }
                    // skip until End Of String
                    if (!isEmpty(t))
                    {
                        printf("Error in linear inequality %i: line too long.\n", i+1);
                        exit(255);
                    }
                    b[i]=tmp[fullDim];
                    A.setLine(i,vTmp,fullDim);
                }
            } else
                if ((t=nodeTmp.getText()))
                {
                    A.setSize(0,fullDim);
                    vTmp.setSize(fullDim+1); tmp=(double*)vTmp;

                    while (*t)
                    {
                        t=vTmp.getFromLine((LPTSTR)t);
                        if ((int)vTmp.sz()!=fullDim+1)
                        {
                            printf("Error in linear inequality %i: line too short.\n", i+1);
                            exit(255);
                        }

                        // skip until EOL
                        t=skipSpaces(t);

                        // if EOL, skip EOL, else error
                        if (*t=='\r') t++;
                        else if (*t=='\n') { t++; if (*t=='\r') t++; }
                            else
                            {
                                printf("Error in linear inequality %i: line too long.\n", i+1);
                                exit(255);
                            }

                        A.extendLine();
                        b.extend();
                        b[nineq]=tmp[fullDim];
                        A.setLine(nineq,vTmp,fullDim);
                        nineq++;
                    }
                }
            printf("Number of linear Inequalities: %i\n", A.nLine());
        }
        nodeTmp=xnode.getChildNode(_T("nonLinearInequalities"));
        if (nodeTmp.isEmpty()) printf("No Non-Linear Inequalities.\n");
        else
        {
            nNLConstraints=nodeTmp.nChildNode(_T("eq"));
            if (nNLConstraints)
            {
                if (nodeTmp.getText())
                {
                    printf("Error in non-linear inequalities: both notations are in use.\n");
                    exit(255);
                }
                nlinequalities=(MathEvalClass**)malloc(nNLConstraints*sizeof(MathEvalClass*));
                int nIndex=0;
                for (i=0; i<nNLConstraints; i++)
                {
                    t=nodeTmp.getChildNode(_T("eq"),&nIndex).getText();
                    if (t==NULL)
                    {
                        printf("error in non-linear inequality %i", i+1);
                        exit(255);
                    }
                    nlinequalities[i]=new MathEvalClass((LPTSTR)t,dbcolnames,fullDim, TRUE);
                }
            } else
                if ((t=nodeTmp.getText()))
                {
                    nNLConstraints=1;
                    nlinequalities=(MathEvalClass**)malloc(sizeof(MathEvalClass*));
                    nlinequalities[0]=new MathEvalClass((LPTSTR)t,dbcolnames,fullDim, TRUE);
                }
            printf("Number of non-linear Inequalities: %i\n", nNLConstraints);
        }

        nodeTmp=xnode.getChildNode(_T("equalities"));
        if (nodeTmp.isEmpty()) printf("No Non-Linear Equalities.\n");
        else
        {
            neq=nodeTmp.nChildNode(_T("eq"));
            if (neq)
            {
                if (nodeTmp.getText())
                {
                    printf("Error in non-linear inequalities: both notations are in use.\n");
                    exit(255);
                }
                equalities=(equalityFunction*)malloc(neq*sizeof(equalityFunction));
                int nIndex=0;
                for (i=0; i<neq; i++)
                {
                    t=nodeTmp.getChildNode(_T("eq"),&nIndex).getText();
                    if (t==NULL)
                    {
                        printf("Error: non-linear equality %i is empty.", i+1);
                        exit(255);
                    }
                    t=indexVar(&j,t,dbcolnames,fullDim);
                    if (t==NULL)
                    {
                        printf("error in non-linear equality %i.", i+1);
                        exit(255);
                    }                    
                    equalities[i].ivar=j;
                    equalities[i].me=new MathEvalClass((LPTSTR)t,dbcolnames,fullDim, FALSE);
                    if (equalities[i].me==NULL)
                    {
                        printf("parsing error in non-linear equality %i.", i+1);
                        exit(255);
                    }
                    if (xToOptim[j])
                    {
                        nToOptim--;
                        xToOptim[j]=0;
                    }
                }
            } else
                if ((t=nodeTmp.getText()))
                {
                    neq=1;
                    t=indexVar(&j,t,dbcolnames,fullDim);
                    if (t==NULL)
                    {
                        printf("error in non-linear equality.");
                        exit(255);
                    }
                    equalities=(equalityFunction*)malloc(sizeof(equalityFunction));
                    equalities[0].ivar=j;
                    equalities[0].me=new MathEvalClass((LPTSTR)t,dbcolnames,fullDim, FALSE);
                    if (equalities[0].me==NULL)
                    {
                        printf("parsing error in non-linear equality.");
                        exit(255);
                    }
                    if (xToOptim[j])
                    {
                        nToOptim--;
                        xToOptim[j]=0;
                    }
                }
            if (nToOptim==0)
            {
                printf("Error: no variable to optimize due to equality constraints.\n");
                exit(255);
            }
            printf("Number of non-linear equalities: %i\nThe design variables determined by equalities are: %s", 
                neq,dbcolnames[equalities[0].ivar]);
            for (i=1; i<neq; i++) printf(" ,%s",dbcolnames[equalities[i].ivar]);
            printf("\n");
        }
    }

    scalingFactorIsGiven=1;
    vScale.setSize(fullDim);
    vScale.set(1.0);
    nodeTmp=xMainNode.getChildNode(_T("scalingFactor"));
    if (nodeTmp.isEmpty()) printf("No re-scaling.\n");
    else
    {
        if (nodeTmp.isAttributeSet("Auto"))
        {
            if (nodeTmp.getText()!=NULL)
            {
                printf("Error: Cannot define auto scaling and user-defined scaling at the same time.\n");
                exit(255);
            }
            scalingFactorIsGiven=0;
            printf("Auto scaling (factors will be based on lower/upper bounds)\n");
            // todo: check lower/upper bounds
        } else
            if (nodeTmp.getText()!=NULL)
            {
                t=vScale.getFromLine((LPTSTR)nodeTmp.getText());
                if (((int)vScale.sz()!=fullDim)||(!isEmpty(t)))
                {
                    printf("Error in scaling factor.\n");
                    exit(255);
                }
                printf("Scaling Factors: "); vScale.print();
            }
    } 


    xnode=xMainNode.getChildNode(_T("optimizationParameters"));
    if (xnode.isEmpty())
    {
        printf("Error: no 'optimizationParameters'.\n");
        exit(255);
    }

    t=xnode.getAttribute("rhostart");
    if (t==NULL)
    {
        printf("Error: no 'rhostart' attribute inside 'optimizationParameters' tag.\n");
        exit(255);
    }
    rhostart=atof(removeQuotes((LPTSTR)t));


    t=xnode.getAttribute("rhoend");
    if (t==NULL)
    {
        printf("Error: no 'rhoend' attribute inside 'optimizationParameters' tag.\n");
        exit(255);
    }
    rhoEnd=atof(removeQuotes((LPTSTR)t));

    if (OFExeRequired)
    {
        t=xnode.getAttribute("timeToSleep");
        if (t==NULL)
        {
            printf("Error: no 'timeToSleep' attribute inside 'optimizationParameters' tag.\n");
            exit(255);
        }
        timeToSleep=atof(removeQuotes((LPTSTR)t));
    } else timeToSleep=0;

    t=xnode.getAttribute("maxIteration");
    if (t==NULL)
    {
        printf("Error: no 'maxIteration' attribute inside 'optimizationParameters' tag.\n");
        exit(255);
    }
    maxIter=atol(removeQuotes((LPTSTR)t));
    i=(nToOptim+1)*(nToOptim+2)/2;

    if (maxIter<i+1)
    {
        printf("'maxIteration' parameter must be greater than %i.\n",i+1);
        exit(255);
    }
    printf("Optimization parameters:\n"
           "    rhostart    =%e\n"
           "    rhoend      =%e\n"
           "    timeToSleep =%f\n"
           "    maxIteration=%i\n",rhostart,rhoEnd,timeToSleep,maxIter);

    dataFull.setSize(0,fullDim+nIndexInOF);
    dataFull.setColNames(dbcolnames);
    debugOF=NULL;
    dbname=NULL;

    xnode=xMainNode.getChildNode(_T("dataFiles"));

    if (xnode.isEmpty()) printf("No Data Files\n");
    else
    {
        t=xnode.getAttribute("binaryDatabaseFile");
        if (t&&OFExeRequired)
        {
            dbname=stringDuplicate(t);

            // to debug/prevent warm start:
            //        printf("Warning deleting DB file (no hot-start).\n"); deleteFile(dbname);

            FILE *f=fopen(dbname,"rb");
            if (f!=NULL)
            {
                fclose(f);
                Matrix dataInter(dbname,0);
                dataFull.merge(dataInter);
            }
            i=dataFull.nLine();
            printf("The binary database filename is: '%s' (%i lines loaded).\n",dbname,i);
        } else printf("No binary database.\n");

        t=xnode.getAttribute("asciiDatabaseFile");
        if (t&&OFExeRequired)
        {
            t=removeQuotes((LPTSTR)t);
            FILE *f=fopen(t,"rb");
            if (f!=NULL)
            {
                fclose(f); 
                Matrix dataInter(t,1);
                dataFull.merge(dataInter);
                if (dbname) dataFull.save(dbname,0);
            }
            printf("The ascii database filename is: '%s' (%i lines loaded).\n",
                removeQuotes((LPTSTR)t),dataFull.nLine()-i);
        } else printf("No ascii database.\n");

        t=xnode.getAttribute("traceFile");
        if (t)
        {
            debugOF=stringDuplicate(t);
            deleteFile(debugOF);
            setNamesForTraceFile();
            printf("The optimization trace file is '%s'.\n",debugOF);
        } else printf("No optimization trace file.\n");
    }

    xnode=xMainNode.getChildNode(_T("resultFile"));
    if ((xnode.isEmpty())||(xnode.getText()==NULL))
    {
        printf("Error: no 'resultFile'");
        exit(255);
    }
    resultsFile=stringDuplicate(xnode.getText());
    printf("The result file is '%s'.\n",resultsFile);
    FILE *ff=fopen(resultsFile,"w");
    if (ff==NULL)
    {
        printf("cannot write into result file '%s'.\n",resultsFile);
        exit(255);
    }
    fclose(ff);

    sigmaIsSet=0;
    nodeTmp=xMainNode.getChildNode(_T("sigmaVector"));
    if ((!nodeTmp.isEmpty())&&(nodeTmp.getText()!=NULL))
    {
        vSigma.setSize(fullDim);
        t=vSigma.getFromLine((LPTSTR)nodeTmp.getText());
        if (((int)vSigma.sz()!=fullDim)||(!isEmpty(t)))
        {
            printf("Error in 'sigmavector'.\n");
            exit(255);
        }
        sigmaIsSet=1;
        printf("Sigma Vector is "); vSigma.print();
    } else printf("No sigma Vector\n");

    vSigNoise.setSize(nToOptim+1); vSigNoise.zero();
};

void XMLof::setNamesForDBfile(int nIndexNames, char **indexNames, char **allVarNames)
{
    int i;

    // set Name for db file
    dbcolnames=(char**)malloc((fullDim+nIndexInOF)*sizeof(char*));
    if (allVarNames) 
    {
        memcpy(dbcolnames, allVarNames, fullDim*sizeof(char*));
        free(allVarNames);
    }
    else
    {
        char *t2=(char*)malloc(fullDim*5);

        for (i=0; i<fullDim; i++)
        {
            dbcolnames[i]=t2; sprintf(t2,"X_%02i",i+1); t2+=5;
        }
    }

    if (indexNames)
    {
        if (nIndexNames>=nIndexInOF) 
        {

            memcpy(dbcolnames+fullDim, indexNames, nIndexInOF*sizeof(char*));
            charMemSpace=*indexNames;
            free(indexNames);
        }
        else
        {
            int i,j=0,k=1,l=0;
            for (i=1; i<=nIndexInOF; i++)
            {
                l+=(int)strlen(indexNames[j]);
                if (k<10) l+=3;
                else { if (k<100) l+=4;
                       else if (k<1000) l+=5; }
                j++; if (j==nIndexNames) { j=0; k++; }
            }
            charMemSpace=(char*)malloc(l);
            char *t2=charMemSpace; 
            j=0; k=1;
            for (i=0; i<nIndexInOF; i++)
            {
                dbcolnames[fullDim+i]=t2;
                t2+=sprintf(t2,"%s_%i",indexNames[j],k)+1;
                j++; if (j==nIndexNames) { j=0; k++; }
            }
            free(*indexNames); free(indexNames);
        }
    } else
    {
        charMemSpace=(char*)malloc(nIndexInOF*6);
        char *t2=charMemSpace;
        for (i=0; i<nIndexInOF; i++)
        {
            dbcolnames[fullDim+i]=t2; sprintf(t2,"Y_%03i",i); t2+=6;
        }
    }

    // check if there is not two times the same name.

    int j;
    for (i=0; i<nIndexInOF+fullDim; i++)
        for (j=i+1; j<nIndexInOF+fullDim; j++)
        {
            if (strcmp(dbcolnames[i],dbcolnames[j])==0)
            {
                printf("Error: Two (design variable)/(index variable) have the same name: '%s'.\n",dbcolnames[i]);
                exit(255);
            }
        }
}

static const char *colNamesForData="AGGR.OBJ.\0ERROR\0";

void XMLof::setNamesForTraceFile()
{
    int i,ib,l=0;
    i=nToOptim*2+1;
    if (nSubAggrFunction>1) i+=nSubAggrFunction;
    names=(char**)malloc((i+2)*sizeof(char*));
    names[i]=(char*)colNamesForData;
    names[i+1]=(char*)colNamesForData+10;

    ib=0;
    for (i=0; i<fullDim; i++)
        if (vXToOptim[i]) 
        {
            names[ib]=dbcolnames[i]; ib++;
            l+=(int)strlen(dbcolnames[i])+5;
        }
    char *t2=(char*)malloc(l+8);
    for (i=0; i<nToOptim; i++)
    {
        names[i+nToOptim]=t2; t2+=sprintf(t2,"SIG_%s",dbcolnames[i])+1;
    }
    names[i+nToOptim]=t2; strcpy(t2,"SIG_TOT");
    if (nSubAggrFunction>1)
    {
        for (i=0; i<nSubAggrFunction; i++) names[i+nToOptim*2+1]=af[i].name;
    }
}

void XMLof::reductionDimension()
{
    vLx.setSize(fullDim);

    int i,ib;
    char *xToOptim=vXToOptim;
    // get the starting point

    int nerror;
    double d;
    int nl=dataFull.nLine();
    if (startPointIsGiven)
    {
        i=dataFull.lineIndex(vFullStart);
        if (i!=-1)
        {
            dataFull.swapLines(0,i);
        } else
        {
            Vector vshort(nToOptim);
            longXtoShortX((double*)vFullStart,(double*)vshort);
            eval(vshort,&nerror);
            if (nerror)
            {
                printf("Evaluation of the Obj. Funct. at the starting point as failed.\n");
                exit(255);
            }            
            dataFull.swapLines(0,nl);
        }
    } else
    {
        double best=INF,p;
        int n=-1,err;
        if (nl==0)
        {
            printf("No starting point (nothing in XML file and nothing in DB)\n."); exit(255);
        }
        for (i=0; i<nl; i++)
        {
            p=aggregate(((double**)dataFull)[i],NULL,&err);
            if ((!err)&&(best>p)) {best=p; n=i;}
        }
        if (n==-1)
        {
            printf("evaluation of aggregation function failed on all points of database.\n"); exit(255);
        }
        dataFull.swapLines(0,n);
        dataFull.getLine(0,vFullStart,fullDim);
    }

    // applying equality constraints to starting point:
    double *x=vFullStart;
    for (i=0; i<neq; i++) x[equalities[i].ivar]=equalities[i].me->eval(x,&ib);

    int l,j;
    double sum, *pb=b;
    nl=0;

    // reduce linear constraints

    double **p=A;
    for (j=0; j<A.nLine(); j++)

	{
        // count number of non-null on the current line
        l=0; sum=pb[j]; ib=0;
	    for (i=0; i<fullDim; i++) 
	        if ((xToOptim[i]) && (p[j][i]!=0.0)) 
            {
                l++; ib=i;
                if (l==2) break;
            }
            else sum-=p[j][i]*x[i];
	    if (l==0) continue;
        if (l==2)
        {
            ib=0; sum=0;
	        for (i=0; i<fullDim; i++) 
	            if (xToOptim[i])
                {
                    p[nl][ib]=p[j][i]; ib++;
                } 
                else sum-=p[j][i]*x[i];
            pb[j]+=sum;
            nl++;
            continue;
        }
	    if (l==1)
	    {
            d=p[j][ib];
		    if (d<0) 
		    {
		        bu[ib]=mmin(bu[ib],sum/d);
		        if (x[ib]>bu[ib])
		        {
        	        fprintf(stderr,"error(2) on linear constraints %i.\n",j+1);
	                exit(254);
		        }
		    }
		    else 
		    {
		        bl[ib]=mmax(bl[ib],sum/d);
		        if (x[ib]<bl[ib])
		        {
        	        fprintf(stderr,"error(3) on linear constraints %i.\n",j+1);
	                exit(254);
		        }
            }
		    continue;
		}
    }
    if (nl) A.setSize(nl,nToOptim); else A.setSize(0,0);


    // reduce upper and lower bound, vScale
    // compute xStart
    xStart.setSize(nToOptim);
    ib=0;
    double *xs=xStart, *xf=vFullStart, *s=vScale;
    for (i=0; i<fullDim; i++)
        if (xToOptim[i]) 
        {  
            bl[ib]=bl[i];
            bu[ib]=bu[i];
            xs[ib]=xf[i];
            if (scalingFactorIsGiven) s[ib]=s[i];
            ib++;
        }
    bl.setSize(nToOptim);
    bu.setSize(nToOptim);
    vScale.setSize(nToOptim);

    // build reduced dimension data matrix
    
    // no necessary since the function which will really by optimized is "correctScale": initTolLC(xStart);

    data.setSize(dataFull.nLine(),nToOptim+2);
    double **dataf=dataFull,**datar=data,v;
	Vector vShortX(nToOptim);
	int k=0, err;
    for (j=0; j<dataFull.nLine(); j++)
    {
        ib=0;
        for (i=0; i<fullDim; i++)
            if (xToOptim[i]) 
            {  
                datar[k][ib]=dataf[j][i];
                ib++;
            } else
			{
				if (xf[i]!=dataf[j][i]) break;
			}
		if (i!=fullDim) continue;
		longXtoShortX(dataf[j],(double*)vShortX);
        if (!isFeasible(vShortX)) continue;

        v=aggregate(dataf[j],NULL,&err);
        if (err) continue;
        datar[k][ib]=v;
        datar[k][ib+1]=0;
		k++;
    }
	data.setNLine(k);
}

void initLinux();
XMLof::XMLof(int _t,char *argv, double *r): 
    maxIter(0), rhostart(0.0), sigmaIsSet(0),names(NULL)
{
    setName("XMLOF");
    t=30;
    init(argv);
    reductionDimension();
    *r=rhoEnd;
    initLinux();
    endInit();
}

double XMLof::aggregate(double *vLxIndexes, double *vIndexes, int *nerr)
{
    int i;
    double r=0.0;
    *nerr=0;

    if (nSubAggrFunction==0)
    {
        i=nIndexInOF;
        vLxIndexes+=fullDim;
        while (i--) r+=vLxIndexes[i];
    } else
    {
        i=nSubAggrFunction;
        if (vIndexes)
            while (i--)
            {
                r+=vIndexes[i]=af[i].me->eval(vLxIndexes,nerr);
                if (*nerr) break;
            }
        else
            while (i--)
            {
                r+=af[i].me->eval(vLxIndexes,nerr);
                if (*nerr) break;
            }
    }
    return r;
}

double XMLof::eval(Vector v, int *nerr)
{
    double r=0.0;
    shortXToLongX(v,vLx);
    int i;
    if (objFunction)
    {
        i=dataFull.lineIndex(vLx);
        if (i!=-1) 
        {
            dataFull.getLine(i,vLxIndexes,nIndexInOF+fullDim); 
            *nerr=0;
// we are keeping dataFull in memory to be able to compute vIndexes
            r=aggregate(vLxIndexes, vIndexes, nerr);
        }
        else
        {
            evalLarge(nIndexInOF,vLx,objFunction,configFile,inputObjF,outputObjF,vIndexes,timeToSleep,nerr);
            if (*nerr==0)
            {
                vLxIndexes.setPart(0,vLx);
                vLxIndexes.setPart(fullDim,vIndexes);
                dataFull.append(vLxIndexes);
                if (dbname) dataFull.updateSave(dbname);
                r=aggregate(vLxIndexes, vIndexes, nerr);
            }
            updateCounter(r,v,*nerr);
        }
    } else 
    {
        r=aggregate(vLx, vIndexes, nerr);
        updateCounter(r,v,*nerr);
    }

    if (debugOF)
    {
        Vector vsave;

        i=nToOptim*2+1;
        if (nSubAggrFunction>1) i+=nSubAggrFunction;
        vsave.setSize(i+2);
        vsave.setPart(0,vLx);
        if ((!sigmaIsSet)&&(vSigma.sz())) vSigNoise.zero();
        vsave.setPart(nToOptim,vSigNoise);
        if (nSubAggrFunction) vsave.setPart(nToOptim*2+1,vIndexes);
        sigmaIsSet=0;
        vsave[i]=r;
        vsave[i+1]=*nerr;
        vsave.appendToMatrixFile(debugOF,names);
    }
    return r;
}

void XMLof::computeSigNoise(Matrix mH)
{
    double *d=vSigNoise, *si=vSigma, s=0.0;
    int ib=0,i;
    for (i=0; i<fullDim; i++)
        if (vXToOptim[i]) 
        {  
            s+=d[ib]=((double**)mH)[ib][ib]*sqr(si[i]);
            ib++;
        }
    d[ib]=s;
}

void XMLof::finalize(Vector vG, Matrix mH, Vector vLambda)
{

    // to do: convert vBest to long form
    shortXToLongX(xBest,vLx);
    xBest=vLx;

    FILE *ff=fopen(resultsFile,"w");
    if (ff==NULL)
    {
        printf("cannot write into result file '%s'.\n",resultsFile);
        exit(255);
    }
    fprintf(ff,";dimension of search-space, total NFE, NFE before best point found, value OF at solution\n"
               "%i\t%i\t(%i)\t%e\n"
               ";Solution vector\n", dim(), nfe, nfe2, valueBest);
    xBest.save(ff,2);
    if (objFunction)
    {
        fprintf(ff,";OF ouput at the solution\n"); fflush(ff);
        dataFull.getLine(dataFull.lineIndex(xBest)).save(ff,2);
    }
    fprintf(ff,";Hessian matrix at the solution\n");
    mH.save(ff,3);
    fprintf(ff,";Gradient vector at the solution (should be zero if no active constraints)\n");
    vG.save(ff,2);
    if (vLambda.sz())
    {
        fprintf(ff,";Lagrangian Vector at the solution (lower,upper,linear,non-linear)\n");
        vLambda.save(ff,2);
    }
    if (vSigma.sz())
    {
        computeSigNoise(mH);
        fprintf(ff,";Sensitivity Vector (+sum).\n");
        vSigNoise.save(ff,2);
    }
    fprintf(ff,"\n");
    fclose(ff);
}

// this method is called by 'quickHackBurgmannFunction' :
void XMLof::setSigma(Matrix mH, Vector rescale)
{
    Vector r=rescale.clone();
    r.oneByOneInvert();
    mH.multiplyByDiagonalMatrix(r);
    r.diagonalizeAndMultiply(mH);
    computeSigNoise(mH);
    sigmaIsSet=1;
}

#ifdef WIN32
void initLinux(){};
#else
void action2(int) { wait(NULL); }
void initLinux()
{
        // to prevent zombie processus:
    struct sigaction maction;
    maction.sa_handler=action2;
    sigemptyset(&maction.sa_mask);
    sigaction(SIGCHLD,&maction,NULL);

	signal(   SIGHUP, action2);
	signal(	  SIGINT, action2);
	signal(  SIGQUIT, action2);
	signal(	  SIGILL, action2);
	signal(  SIGTRAP, action2);
	signal(  SIGABRT, action2);
	signal(	  SIGIOT, action2);
	signal(	  SIGBUS, action2);
	signal(   SIGFPE, action2);
	signal(  SIGKILL, action2);
	signal(  SIGUSR1, action2);
	signal(  SIGSEGV, action2);
	signal(  SIGUSR2, action2);
	signal(  SIGPIPE, action2);
	signal(  SIGALRM, action2);
	signal(  SIGTERM, action2);
	signal(SIGSTKFLT, action2);
	signal(	 SIGCONT, action2);
	signal(	 SIGTSTP, action2);
	signal(	 SIGTTIN, action2);
	signal(	 SIGTTOU, action2);
	signal(	 SIGXCPU, action2);
	signal(  SIGXFSZ, action2);
	signal(SIGVTALRM, action2);
	signal(  SIGPROF, action2);
	signal(	   SIGIO, action2);
	signal(	  SIGPWR, action2);
	signal(   SIGSYS, action2);
}
#endif

//void XMLof::saveValue(Vector tmp,double valueOF, int nerror)
//{
//    shortXToLongX(tmp,vLx);
//    ObjectiveFunction::saveValue(vLx,valueOF, nerror);
//}

/***********************
  FOR PARALLEL ONLY
***********************/

/*
ClientXMLof::ClientXMLof(int _t, char *filename, Vector v)
{
    t=_t;
    vFullStart=v;
	FILE *stream;
	int i=0;
	char line[30000];
    fullPathToConfigFile(filename,&configFile);

    if ((stream=fopen(filename,"r"))==NULL)
	{
	   printf("optimization config file not found.\n"); exit(254);
	};

	while ((fgets(line,30000,stream)!=NULL))
	{
		if (emptyline(line)) continue;
		switch (i)
		{
		    case 0:  // name of blackbox flow solver
                     objFunction=(char*)malloc(strlen(line)+1);
		             strcpy(objFunction,line);
		             GetRidOfTheEOL(objFunction);	    
		    case 1:  inputObjF=(char*)malloc(strlen(line)+1);
		             strcpy(inputObjF,line);
		             GetRidOfTheEOL(inputObjF);	    
		    case 2:  outputObjF=(char*)malloc(strlen(line)+1);
		             strcpy(outputObjF,line);
		             GetRidOfTheEOL(outputObjF);
			case 3:  // number of parameters
			         fullDim=atol(line);
			         break;
			case 4:  // weight for each component of the objective function 
			           weight=Vector(line,0); 
                       vTmpOF.setSize(nIndexInOF); break;
            case 5:    center=Vector(line,weight.sz()); break;
            case 6:  exponent=Vector(line,weight.sz()); break;
            case 7:  vXToOptim=Vector(line,fullDim); break;
		};
		i++;
	};
	fclose(stream);    
    initLinux();
};

ClientXMLof::~ClientXMLof()
{
    free(objFunction);
    free(outputObjF);
    free(inputObjF);
}

double ClientXMLof::eval(Vector v, int *nerr)
{
    int i,j=0;
    double *xf=vFullStart,*xToOptim=vXToOptim,*sx=v,*llx=vLx;
    for (i=0; i<fullDim; i++)
        if (xToOptim[i]!=0.0) { llx[i]=sx[j]; j++; }
        else llx[i]=xf[i];

    double r=evalLarge(vLx,objFunction,configFile,inputObjF,outputObjF,NULL,NULL,
            vTmpOF,3,amplEvalObj,nerr);                 
    return r;
}
*/
