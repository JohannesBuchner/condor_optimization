#makefile for the CONDOR project.

#all : simpleOptimizer
all :  simpleOptimizer \
        sifOptimizer \
        amplOptimizer \
        xmlStuff 
xmlStuff: xmlOptimizer matconvert testFunctions dbtool

OPTIONS= -g -Wall -static
#OPTIONS= -O3

SIFEXAMPLES = sifOF/examples/3pk.cpp sifOF/examples/akiva.cpp sifOF/examples/allinitu.cpp sifOF/examples/biggs6.cpp \
   sifOF/examples/brownal.cpp sifOF/examples/brownden.cpp sifOF/examples/brybnd.cpp sifOF/examples/cliff.cpp \
   sifOF/examples/cragglvy.cpp sifOF/examples/deconvu.cpp sifOF/examples/dixmaank.cpp sifOF/examples/dqdrtic.cpp \
   sifOF/examples/eigenals.cpp sifOF/examples/fminsurf.cpp sifOF/examples/growthls.cpp sifOF/examples/gulf.cpp \
   sifOF/examples/hairy.cpp sifOF/examples/hatflde.cpp sifOF/examples/heart.cpp sifOF/examples/heart6ls.cpp \
   sifOF/examples/helix.cpp sifOF/examples/kowosb.cpp sifOF/examples/mancino.cpp sifOF/examples/morebv.cpp \
   sifOF/examples/osborneb.cpp sifOF/examples/parkch.cpp sifOF/examples/pfit1ls.cpp sifOF/examples/power.cpp \
   sifOF/examples/rosenbock.cpp sifOF/examples/schmvett.cpp sifOF/examples/sisser.cpp sifOF/examples/snail.cpp \
   sifOF/examples/stratec.cpp sifOF/examples/tointgor.cpp sifOF/examples/tointpsp.cpp sifOF/examples/vardim.cpp \
   sifOF/examples/vibrbeam.cpp sifOF/examples/watson.cpp sifOF/examples/hart6.cpp

COMMONCPPFILES = common/Matrix.cpp common/MatrixTriangle.cpp \
   common/MultInd.cpp common/ObjectiveFunction.cpp common/Poly.cpp \
   common/Vector.cpp common/VectorInt.cpp common/VectorChar.cpp common/tools.cpp \
   common/IntPoly.cpp common/KeepBests.cpp  common/parallel.cpp \
   common/MSSolver.cpp common/UTRSSolver.cpp common/CTRSSolver.cpp \
   common/CNLSolver.cpp common/QPSolver.cpp 

COMMONHFILES = common/Matrix.h common/MatrixTriangle.h common/MultInd.h \
   common/ObjectiveFunction.h common/Poly.h common/Vector.h common/VectorInt.h \
   common/VectorChar.h common/tools.h common/IntPoly.h common/KeepBests.h \
   common/parallel.h common/Solver.h

AMPLFILES = amplOF/AMPLof.cpp amplOF/amplsolver.a -ldl amplOF/amplMain.cpp

XMLFILESH = xmlOF/XMLof.h xmlOF/xmlParser.h
XMLFILESCPP = xmlOF/XMLof.cpp xmlOF/xmlParser.cpp 

MATHEVALCPP= MathEval/MathEvalClass.cpp \
   MathEval/lib/error.c  MathEval/lib/matheval.c  MathEval/lib/node.c \
   MathEval/lib/parser.c  MathEval/lib/scanner.c  MathEval/lib/symbol_table.c  \
   MathEval/lib/xmalloc.c  MathEval/lib/xmath.c

matconvert: matConvert/matConvert.cpp common/Matrix.cpp common/Matrix.h \
    common/Vector.cpp common/Vector.h common/tools.cpp common/tools.h
	g++ $(OPTIONS) -o matconvert -DNOMATRIXTRIANGLE \
    matConvert/matConvert.cpp common/Matrix.cpp common/Vector.cpp common/tools.cpp
	cp matconvert xmlOF/testCondor
        
xmlOptimizer: $(COMMONCPPFILES) $(COMMONHFILES) \
     $(XMLFILESH) $(XMLFILESCPP) $(MATHEVALCPP)
	g++ -D_REENTRANT $(OPTIONS) -o xmlOptimizer \
      -DHAVE_MATH_H -DYY_NO_UNPUT -DYY_NEVER_INTERACTIVE \
     $(COMMONCPPFILES) xmlOF/xmlMain.cpp \
     $(XMLFILESCPP) $(MATHEVALCPP) -lm -lfl
# -DNOPROTECT
	cp xmlOptimizer xmlOF/testCondor/condor

dbtool: $(COMMONCPPFILES) $(COMMONHFILES) \
     $(XMLFILESH) $(XMLFILESCPP) $(MATHEVALCPP)
	g++ -D_REENTRANT $(OPTIONS) -o dbtool \
      -DHAVE_MATH_H -DYY_NO_UNPUT -DYY_NEVER_INTERACTIVE \
     $(COMMONCPPFILES) xmlOF/dbtool/DBToolMain.cpp \
     $(XMLFILESCPP) $(MATHEVALCPP) -lm -lfl

testFunctions: xmlOF/testCondor/OF/testOF.cpp
	g++ $(OPTIONS) -o xmlOF/testCondor/OF/testOF \
		xmlOF/testCondor/OF/testOF.cpp
	g++ $(OPTIONS) -o xmlOF/testCondor/OF/testOFF \
	        -DFAILURE xmlOF/testCondor/OF/testOF.cpp

sifOptimizer : $(SIFEXAMPLES) sifOF/SIFFunction.h sifOF/f2c.h \
      $(COMMONCPPFILES) $(COMMONHFILES) sifOF/SIFFunction.cpp \
      sifOF/sifMain.cpp
	g++ -D_REENTRANT  $(OPTIONS)  \
        -o sifOptimizer \
        -lpthread $(SIFEXAMPLES) $(COMMONCPPFILES) \
	sifOF/sifMain.cpp sifOF/SIFFunction.cpp -lm 

amplOptimizer : $(AMPLFILES) $(COMMONCPPFILES) \
       $(COMMONHFILES) amplOF/AMPLof.h 
	g++ -D_REENTRANT  $(OPTIONS)  \
        -o amplOptimizer -lpthread $(AMPLFILES) $(COMMONCPPFILES) -lm

simpleOptimizer : $(COMMONCPPFILES) $(COMMONHFILES) \
       simpleOF/simpleMain.cpp  simpleOF/simpleObjFunctions.cpp  \
       simpleOF/simpleObjFunctions.h
	g++  $(OPTIONS) -o simpleOptimizer \
        $(COMMONCPPFILES) simpleOF/simpleMain.cpp  -lm \
         simpleOF/simpleObjFunctions.cpp 

clean:
	rm xmlOptimizer
	rm matconvert
	rm sifOptimizer
	rm amplOptimizer
	rm simpleOptimizer
	rm dbtool
	rm resultsConstrained.txt
	rm result.txt
	rm test.dat
	rm *~
