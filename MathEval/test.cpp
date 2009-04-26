#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "matheval.h"
/* Size of input buffer. */
#define BUFFER_SIZE 256
/* Program is demonstrating use of GNU libmatheval library of procedures
for evaluating mathematical functions. */
int main (int argc, char **argv)
{
char buffer[BUFFER_SIZE]; /* Input buffer. */
int length; /* Length of above buffer. */
void *f, *f_prim; /* Evaluators for function and function derivative. */
char *string; /* Textual representation of function derivative. */
double x; /* Variable x value. */
/* Read function. Function has to be over variable x, or result may
be undetermined. Size of textual represenatation of function is
bounded here to 256 characters, in real conditions one should
probably use GNU readline() instead of fgets() to overcome this
limit. */
printf ("f(x) = ");
fgets (buffer, BUFFER_SIZE, stdin);
length = (int)strlen (buffer);
if (length > 0 && buffer[length - 1] == '\n')
buffer[length - 1] = '\0';
/* Create evaluator for function. */
f = evaluator_create (buffer);
assert (f);
/* Create evaluator for function derivative and print textual
representation of derivative. */
f_prim = evaluator_derivative_x (f);
string =
(char *) malloc ((evaluator_calculate_length (f_prim) + 1) *
sizeof (char));
evaluator_write (f_prim, string);
printf (" f'(x) = %s\n", string);
free (string);
/* Read variable x value. */
printf ("x = ");
//Chapter 1: Introduction 5
scanf ("%lf", &x);
/* Calculate and print values of function and its derivative for given
value of x. */

char *names[] = { "x_1" };


printf (" f(%g) = %g\n", x, evaluator_evaluate (f, 1,names, &x)); //evaluator_evaluate_x (f, x));
printf (" f'(%g) = %g\n", x, evaluator_evaluate_x (f_prim, x));
/* Destroy evaluators. */
evaluator_destroy (f);
evaluator_destroy (f_prim);
exit (EXIT_SUCCESS);
}