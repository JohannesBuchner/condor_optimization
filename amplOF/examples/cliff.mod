# AMPL Model by Hande Y. Benson
#
# Copyright (C) 2001 Princeton University
# All Rights Reserved
#
# Permission to use, copy, modify, and distribute this software and
# its documentation for any purpose and without fee is hereby
# granted, provided that the above copyright notice appear in all
# copies and that the copyright notice and this
# permission notice appear in all supporting documentation.                     

#   Source:  problem 206 (p. 46) in
#   A.R. Buckley,
#   "Test functions for unconstrained minimization",
#   TR 1989CS-3, Mathematics, statistics and computing centre,
#   Dalhousie University, Halifax (CDN), 1989.

#   SIF input: Ph. Toint, Dec 1989.

#   classification OUR2-AN-2-0

param xinit{1..2};
var x{i in 1..2}:=xinit[i];

minimize f:
	(0.01*x[1]-0.03)^2 -x[1]+x[2]+exp(20*(x[1]-x[2]));

data;
param xinit:= 1 0.0 2 -1.0;

#solve;
#display f;
#display x;

write gcliff;
