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

#   classification  SUR2-AN-3-0

param cf := -8.0;
param cg := -18-(2/3);
param ch := -23-(1/9);

var a:=1.0;
var r:=0.0;
var h:=1.0;

minimize f:
	(-0.5*(a*(a+1)*r*h^2)+a*r*h-r*(1-(1+h)^-a)-cf)^2 +
	(-a*(a+1)*r*h^2+a*r*h*(1-(1+h)^-(a+1))-cg)^2 +
	(-a*(a+1)*r*h^2*(1-(1+h)^-(a+2))-ch)^2;

#display f;
#solve;
#display f;
#display a, r, h;
write gpfit1ls;
