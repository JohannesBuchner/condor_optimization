Condor 1.06
===========

You are free to use this software for any non-commercial applications.
You can contact the author (Frank Vanden Berghen) to obtain a license
for commerical use.

This version of CONDOR is NOT using any constraints and is uniprocessor
only.

You can obtain a full-featured executable (binary) of CONDOR at the 
website of Frank Vanden Berghen.

The simplest way to interface CONDOR with an external objective function
is to use the XML interface. A full documentation of this interface is 
available at the website of Frank Vanden Berghen.

If you want to optimize an industrial design or process using an 
objective function based on a windows executable which is running in 
interactive mode, you can use the WinBatch (www.winbatch.com)
tool to transform your interactive window application into a batch process.
Once you have a batch executable, you can easily use CONDOR via the XML
interface. The combination of WinBatch with the xml-based version of CONDOR
allows you to optimize nearly everything you can dream of! Have fun!

The .jcc and .jst files are Code Crusader project files. Code crusader(jcc)
is a powerfull and STABLE development environnement for c++ under linux.

INSTALLATION
============

Under UNIX: type make.
Under windows: double_click "condor106.sln": it will open visual studio.Net.
               Then click on "rebuild all".

It will generate on the root of the install directory the following
executables:
 - simpleOptimizer: 
	This is where I suggest you to go if you want to use Condor 
	directly through the C++ interface. A can copy/paste the code
	of the 'simpleOptimizer' and obtain rapidly want you want.
 - xmlOptimizer:
	It's the most complex executable. Yoy can see it in action
	an small tests problems inside directory xmlOF/testCondor.
	There is inside this directory many small scripts running 
	many different examples
 - dbtool, matconvert:
	This two executables are not optimizers. They are some small
	tools usefull to manipulate outputs from xmlOptimizer. 
	dbtool looks inside the database of all the evaluations which
	have been done and compute the sigma's and the (sub)aggregation
	functions for each points. It's usefull to see the impact of the
	adjustement of an aggregation function.
	matconvert convert saved condor-matrices from binary format to
	ascii format.
 - amplOptimizer:
	Condor with ampl input. This is currently a very primitive ampl
	reader. You must first generate by hand an .nl file and submit
	it to condor. Condor then computes the solution and write back
	the result in a standard ascii test file. 
 - sifOptimizer: 
	Condor with SIF input. To be able to use SIF files, you must 
	first use SIFDECODE to transform the SIF file into .d file and
	fortran code. Then after you must use F2C to tranform fortran 
	code to C code. Then, finally, you can link Condor to the C code 
	and load the SIF and .d file via this interface. See the makefile
	and the examples to see clearly how to do it.
                  
DIRECTORY STRUCTURE
===================

common: The heart of the Condor optimizer. The files in this directory
        are used by all the generated executables.
amplOF: files for ampl interface.
matconvert: file to implement matconvert
MathEval: the GNU MathEval library. Usefull for the XML interface.
sifOF: files for sif interface.
simpleOF: files for the simpleOptimizer.
xmlOF: files for the xml interface. The xml inteface includes
       a simple, fast and powerfull cross-platform xml parser.
xmlOF/testCondor: some small test problems for the xml interface.

FILES
=====

windows specific: amplOF\amplsolv.lib, *.vcproj, *.vcproj.old, 
condor106.sln, condor106.suo, MathEval\lib\libfl.lib, 
xmlOF\testCondorwin32\*.*, win32clean.bat, amplOF\examples\ampl.exe

linux specific: amplOF/amplsolver.a, condor.jcc, condor.jst, 
makefile, xmlOF/testCondor/*

All the binaries will appear in the root directory after compilation
(it's the case for both windows and linux systems).

