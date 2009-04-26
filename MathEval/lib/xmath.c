/*
 * Copyright (C) 1999, 2002, 2003, 2004  Free Software Foundation, Inc.
 * 
 * This file is part of GNU libmatheval
 * 
 * GNU libmatheval is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2, or (at your option) any later
 * version.
 * 
 * GNU libmatheval is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * program; see the file COPYING. If not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "xmath.h"

double
math_cot(double x)
{
	/*
	 * Calculate cotangent value.
	 */
	return 1 / tan(x);
}

double
math_sec(double x)
{
	/*
	 * Calculate secant value.
	 */
	return 1 / cos(x);
}

double
math_csc(double x)
{
	/*
	 * Calculate cosecant value.
	 */
	return 1 / sin(x);
}

double
math_acot(double x)
{
	/*
	 * Calculate inverse cotangent value.
	 */
	return atan(1 / x);
}

double
math_asec(double x)
{
	/*
	 * Calculate inverse secant value.
	 */
	return acos(1 / x);
}

double
math_acsc(double x)
{
	/*
	 * Calculate inverse cosecant value.
	 */
	return asin (1 / x);
}

double
math_coth(double x)
{
	/*
	 * Calculate hyperbolic cotangent value.
	 */
	return 1 / tanh(x);
}

double
math_sech(double x)
{
	/*
	 * Calculate hyperbolic secant value.
	 */
	return 1 / cosh(x);
}

double
math_csch(double x)
{
	/*
	 * Calculate hyperbolic cosecant value.
	 */
	return 1 / sinh(x);
}

double
math_asinh(double x)
{
	/*
	 * Calculate inverse hyperbolic sine value.
	 */
	return log(x + sqrt(x * x + 1));
}

double
math_acosh(double x)
{
	/*
	 * Calculate inverse hyperbolic cosine value.
	 */
	return log(x + sqrt(x * x - 1));
}

double
math_atanh(double x)
{
	/*
	 * Calculate inverse hyperbolic tangent value.
	 */
	return 0.5 * log((1 + x) / (1 - x));
}

double
math_acoth(double x)
{
	/*
	 * Calculate inverse hyperbolic cotangent value.
	 */
	return 0.5 * log((x + 1) / (x - 1));
}

double
math_asech(double x)
{
	/*
	 * Calculate inverse hyperbolic secant value.
	 */
	return math_acosh(1 / x);
}

double
math_acsch(double x)
{
	/*
	 * Calculate inverse hyperbolic cosecant value.
	 */
        return math_asinh(1 / x);
}

double          math_step(double x)
{
        /*
	 * Calculate step function value.
	 */
        return (x < 0) ? 0 : 1;
}

double          math_delta(double x)
{
        /*
	 * Calculate delta function value.
	 */
        return (x == 0) ? MATH_INFINITY : 0;
}

double          math_nandelta(double x)
{
        /*
	 * Calculate modified delta function value.
	 */
#ifdef WIN32
    unsigned long __nan[2]={0xffffffff, 0x7fffffff};
    double g = *( double* )__nan;
        return (x == 0) ? g: 0;
#else
        return (x == 0) ? MATH_NAN : 0;
#endif
}
