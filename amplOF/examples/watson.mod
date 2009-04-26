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

#   Source:  problem 20 in
#   J.J. More', B.S. Garbow and K.E. Hillstrom,
#   "Testing Unconstrained Optimization Software",
#   ACM Transactions on Mathematical Software, vol. 7(1), pp. 17-41, 1981.

#   See also Buckley#128 (p. 100).

#   SIF input: Ph. Toint, Dec 1989.

#   classification SUR2-AN-V-0

param N:=10;
var x{1..N} := 0;

param t{i in 1..(N-2)} := i/(N-2);

minimize f:
	sum {i in 1..(N-2)} (sum {j in 2..N} (j-1)*x[j]*t[i]^(j-2) - (sum {j in 1..N} x[j]*t[i]^(j-1))^2 - 1)^2 +
	x[1]^2 + (x[2]-x[1]^2-1)^2;

#solve;
#display f;
#display x;
write gwatson;
