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
#include <string.h>
#include <stdlib.h>

#if HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <assert.h>
#include <stdarg.h>
#include "common.h"
#include "node.h"

extern char  **variableNames;
extern int numberOfVariables,meMaxIndexVar;

Node           *
node_create(char type,...)
{
	Node           *node;	/* New node.  */
	va_list         ap;	/* Variable argument list.  */

	/*
	 * Allocate memory for node and initialize its type.
	 */
	node = XMALLOC(Node, 1);
	node->type = type;

	/*
	 * According to node type, initialize rest of the node from variable
	 * argument list.
	 */
	va_start(ap, type);
	switch (node->type) {
	case 'c':
		/*
		 * Initialize constant value.
		 */
		node->data.constant = va_arg(ap, double);

		break;

	case 'v':
		/*
		 * Remember pointer to symbol table record describing
		 * variable.
		 */
		node->data.variable = va_arg(ap, Record *);
        {
            int i;
            char *t=node->data.variable->name;
            for (i=0; i<numberOfVariables; i++)
                if (strcmp(t,variableNames[i])==0) break;
            if (meMaxIndexVar<i) meMaxIndexVar=i;
            if (i==numberOfVariables)
            {
                printf("Variable %s not recognised.\nKnown variables are:\n", t);
                for (i=0; i<numberOfVariables; i++)
                    printf("%s ",variableNames[i]);
                printf("\n");
                exit(255);
            }
        }

		break;

	case 'f':
		/*
		 * Remember pointer to symbol table record describing
		 * function and initialize function argument.
		 */
		node->data.function.record = va_arg(ap, Record *);
		node->data.function.child = va_arg(ap, Node *);
		break;

	case 'u':
		/*
		 * Initialize operation type and operand.
		 */
		node->data.un_op.operation = (char)va_arg(ap, int);

		node->data.un_op.child = va_arg(ap, Node *);
		break;

	case 'b':
		/*
		 * Initialize operation type and operands.
		 */
		node->data.un_op.operation = (char)va_arg(ap, int);

		node->data.bin_op.left = va_arg(ap, Node *);
		node->data.bin_op.right = va_arg(ap, Node *);
		break;

	default:
		assert(0);
	}
	va_end(ap);

	return node;
}

void
node_destroy(Node * node)
{
	/*
	 * Skip if node already null (this may occur during simplification).
	 */
	if (!node)
		return;

	/*
	 * If necessary, destroy subtree rooted at node.
	 */
	switch (node->type) {
	case 'c':
	case 'v':
		break;

	case 'f':
		node_destroy(node->data.function.child);
		break;

	case 'u':
		node_destroy(node->data.un_op.child);
		break;

	case 'b':
		node_destroy(node->data.bin_op.left);
		node_destroy(node->data.bin_op.right);
		break;
	}

	/*
	 * Deallocate memory used by node.
	 */
	XFREE(node);
}

Node           *
node_copy(Node * node)
{
	/*
	 * According to node type, create (deep) copy of subtree rooted at
	 * node.
	 */
	switch (node->type) {
		case 'c':
		return node_create('c', node->data.constant);

	case 'v':
		return node_create('v', node->data.variable);

	case 'f':
		return node_create('f', node->data.function.record, node_copy(node->data.function.child));

	case 'u':
		return node_create('u', node->data.un_op.operation, node_copy(node->data.un_op.child));

	case 'b':
		return node_create('b', node->data.bin_op.operation, node_copy(node->data.bin_op.left), node_copy(node->data.bin_op.right));
	}
    return NULL;
}

Node           *
node_simplify(Node * node)
{
	/*
	 * According to node type, apply further simplifications.
	 */
	switch (node->type) {
		case 'c':
		case 'v':
		return node;

	case 'f':
		/*
		 * Simplify function argument and if constant evaluate
		 * function and replace function node with constant node.
		 */
		node->data.function.child = node_simplify(node->data.function.child);
		if (node->data.function.child->type == 'c') {
			double          value = node_evaluate(node);

			node_destroy(node);
			return node_create('c', value);
		} else
			return node;

	case 'u':
		/*
		 * Simplify unary operation operand and if constant apply
		 * operation and replace operation node with constant node.
		 */
		node->data.un_op.child = node_simplify(node->data.un_op.child);
		if (node->data.un_op.operation == '-' && node->data.un_op.child->type == 'c') {
			double          value = node_evaluate(node);

			node_destroy(node);
			return node_create('c', value);
		} else
			return node;

	case 'b':
		/*
		 * Simplify binary operation operands.
		 */
		node->data.bin_op.left = node_simplify(node->data.bin_op.left);
		node->data.bin_op.right = node_simplify(node->data.bin_op.right);

		/*
		 * If operands constant apply operation and replace operation
		 * node with constant node.
		 */
		if (node->data.bin_op.left->type == 'c' && node->data.bin_op.right->type == 'c') {
			double          value = node_evaluate(node);

			node_destroy(node);
			return node_create('c', value);
		}
		/*
		 * Eliminate 0 as neutral addition operand.
		 */
		else if (node->data.bin_op.operation == '+')
			if (node->data.bin_op.left->type == 'c' && node->data.bin_op.left->data.constant == 0) {
				Node           *right;

				right = node->data.bin_op.right;
				node->data.bin_op.right = NULL;
				node_destroy(node);
				return right;
			} else if (node->data.bin_op.right->type == 'c' && node->data.bin_op.right->data.constant == 0) {
				Node           *left;

				left = node->data.bin_op.left;
				node->data.bin_op.left = NULL;
				node_destroy(node);
				return left;
			} else
				return node;
		/*
		 * Eliminate 0 as neutral subtraction right operand.
		 */
		else if (node->data.bin_op.operation == '-')
			if (node->data.bin_op.right->type == 'c' && node->data.bin_op.right->data.constant == 0) {
				Node           *left;

				left = node->data.bin_op.left;
				node->data.bin_op.left = NULL;
				node_destroy(node);
				return left;
			} else
				return node;
		/*
		 * Eliminate 1 as neutral multiplication operand.
		 */
		else if (node->data.bin_op.operation == '*')
			if (node->data.bin_op.left->type == 'c' && node->data.bin_op.left->data.constant == 1) {
				Node           *right;

				right = node->data.bin_op.right;
				node->data.bin_op.right = NULL;
				node_destroy(node);
				return right;
			} else if (node->data.bin_op.right->type == 'c' && node->data.bin_op.right->data.constant == 1) {
				Node           *left;

				left = node->data.bin_op.left;
				node->data.bin_op.left = NULL;
				node_destroy(node);
				return left;
			} else
				return node;
		/*
		 * Eliminate 1 as neutral division right operand.
		 */
		else if (node->data.bin_op.operation == '/')
			if (node->data.bin_op.right->type == 'c' && node->data.bin_op.right->data.constant == 1) {
				Node           *left;

				left = node->data.bin_op.left;
				node->data.bin_op.left = NULL;
				node_destroy(node);
				return left;
			} else
				return node;
		/*
		 * Eliminate 0 and 1 as both left and right exponentiation
		 * operands.
		 */
		else if (node->data.bin_op.operation == '^')
			if (node->data.bin_op.left->type == 'c' && node->data.bin_op.left->data.constant == 0) {
				node_destroy(node);
				return node_create('c', 0.0);
			} else if (node->data.bin_op.left->type == 'c' && node->data.bin_op.left->data.constant == 1) {
				node_destroy(node);
				return node_create('c', 1.0);
			} else if (node->data.bin_op.right->type == 'c' && node->data.bin_op.right->data.constant == 0) {
				node_destroy(node);
				return node_create('c', 1.0);
			} else if (node->data.bin_op.right->type == 'c' && node->data.bin_op.right->data.constant == 1) {
				Node           *left;

				left = node->data.bin_op.left;
				node->data.bin_op.left = NULL;
				node_destroy(node);
				return left;
			} else
				return node;
		else
			return node;
	}
    return NULL;
}

double
node_evaluate(Node * node)
{
	/*
	 * According to node type, evaluate subtree rooted at node.
	 */
	switch (node->type) {
		case 'c':
		return node->data.constant;

	case 'v':
		/*
		 * Variable values are used from symbol table.
		 */
		return node->data.variable->data.value;

	case 'f':
		/*
		 * Functions are evaluated through symbol table.
		 */
		return (*node->data.function.record->data.function) (node_evaluate(node->data.function.child));

	case 'u':
		/*
		 * Unary operation node is evaluated according to operation
		 * type.
		 */
		switch (node->data.un_op.operation) {
		case '-':
			return -node_evaluate(node->data.un_op.child);
		}

	case 'b':
		/*
		 * Binary operation node is evaluated according to operation
		 * type.
		 */
		switch (node->data.un_op.operation) {
		case '+':
			return node_evaluate(node->data.bin_op.left) + node_evaluate(node->data.bin_op.right);

		case '-':
			return node_evaluate(node->data.bin_op.left) - node_evaluate(node->data.bin_op.right);

		case '*':
			return node_evaluate(node->data.bin_op.left) * node_evaluate(node->data.bin_op.right);

		case '/':
			return node_evaluate(node->data.bin_op.left) / node_evaluate(node->data.bin_op.right);

		case '^':
			return pow(node_evaluate(node->data.bin_op.left), node_evaluate(node->data.bin_op.right));
		}
	}

	return 0;
}

Node           *
node_derivative(Node * node, char *name, SymbolTable * symbol_table)
{
	/*
	 * According to node type, derivative tree for subtree rooted at node
	 * is created.
	 */
	switch (node->type) {
		case 'c':
		/*
		 * Derivative of constant equals 0.
		 */
		return node_create('c', 0.0);

	case 'v':
		/*
		 * Derivative of variable equals 1 if variable is derivative
		 * variable, 0 otherwise.
		 */
		return node_create('c', (!strcmp(name, node->data.variable->name)) ? 1.0 : 0.0);

	case 'f':
		/*
		 * Apply rule of exponential function derivative.
		 */
		if (!strcmp(node->data.function.record->name, "exp"))
			return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_copy(node));
		/*
		 * Apply rule of logarithmic function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "log"))
			return node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_copy(node->data.function.child));
		/*
		 * Apply rule of square root function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "sqrt"))
			return node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('b', '*', node_create('c', 2.0), node_copy(node)));
		/*
		 * Apply rule of sine function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "sin"))
			return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('f', symbol_table_lookup(symbol_table, "cos"), node_copy(node->data.function.child)));
		/*
		 * Apply rule of cosine function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "cos"))
			return node_create('u', '-', node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('f', symbol_table_lookup(symbol_table, "sin"), node_copy(node->data.function.child))));
		/*
		 * Apply rule of tangent function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "tan"))
			return node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('b', '^', node_create('f', symbol_table_lookup(symbol_table, "cos"), node_copy(node->data.function.child)), node_create('c', 2.0)));
		/*
		 * Apply rule of cotangent function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "cot"))
			return node_create('u', '-', node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('b', '^', node_create('f', symbol_table_lookup(symbol_table, "sin"), node_copy(node->data.function.child)), node_create('c', 2.0))));
                /*
		 * Apply rule of secant function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "sec"))
                        return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('b', '*', node_create('f', symbol_table_lookup(symbol_table, "sec"), node_copy(node->data.function.child)), node_create('f', symbol_table_lookup(symbol_table, "tan"), node_copy(node->data.function.child))));
                /*
		 * Apply rule of cosecant function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "csc"))
                        return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('u', '-', node_create('b', '*', node_create('f', symbol_table_lookup(symbol_table, "cot"), node_copy(node->data.function.child)), node_create('f', symbol_table_lookup(symbol_table, "csc"), node_copy(node->data.function.child)))));
		/*
		 * Apply rule of inverse sine function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "asin"))
			return node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('f', symbol_table_lookup(symbol_table, "sqrt"), node_create('b', '-', node_create('c', 1.0), node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0)))));
		/*
		 * Apply rule of inverse cosine function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "acos"))
			return node_create('u', '-', node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('f', symbol_table_lookup(symbol_table, "sqrt"), node_create('b', '-', node_create('c', 1.0), node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0))))));
		/*
		 * Apply rule of inverse tangent function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "atan"))
			return node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('b', '+', node_create('c', 1.0), node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0))));
		/*
		 * Apply rule of inverse cotanget function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "acot"))
			return node_create('u', '-', node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('b', '+', node_create('c', 1.0), node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0)))));
                /*
		 * Apply rule of inverse secant function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "asec"))
			return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('b', '/', node_create('c', 1.0), node_create('b', '*', node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0)), node_create('f', symbol_table_lookup(symbol_table, "sqrt"), node_create('b', '-', node_create('c', 1.0), node_create('b', '/', node_create('c', 1.0), node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0))))))));
                /*
		 * Apply rule of inverse cosecant function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "acsc"))
			return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('u', '-', node_create('b', '/', node_create('c', 1.0), node_create('b', '*', node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0)), node_create('f', symbol_table_lookup(symbol_table, "sqrt"), node_create('b', '-', node_create('c', 1.0), node_create('b', '/', node_create('c', 1.0), node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0)))))))));
		/*
		 * Apply rule of hyperbolic sine function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "sinh"))
			return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('f', symbol_table_lookup(symbol_table, "cosh"), node_copy(node->data.function.child)));
		/*
		 * Apply rule of hyperbolic cosine function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "cosh"))
			return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('f', symbol_table_lookup(symbol_table, "sinh"), node_copy(node->data.function.child)));
		/*
		 * Apply rule of hyperbolic tangent function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "tanh"))
			return node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('b', '^', node_create('f', symbol_table_lookup(symbol_table, "cosh"), node_copy(node->data.function.child)), node_create('c', 2.0)));
		/*
		 * Apply rule of hyperbolic cotangent function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "coth"))
			return node_create('u', '-', node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('b', '^', node_create('f', symbol_table_lookup(symbol_table, "sinh"), node_copy(node->data.function.child)), node_create('c', 2.0))));
                /*
		 * Apply rule of hyperbolic secant function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "sech"))
                        return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('u', '-', node_create('b', '*', node_create('f', symbol_table_lookup(symbol_table, "sech"), node_copy(node->data.function.child)), node_create('f', symbol_table_lookup(symbol_table, "tanh"), node_copy(node->data.function.child)))));
                /*
		 * Apply rule of hyperbolic cosecant function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "csch"))
                        return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('u', '-', node_create('b', '*', node_create('f', symbol_table_lookup(symbol_table, "coth"), node_copy(node->data.function.child)), node_create('f', symbol_table_lookup(symbol_table, "csch"), node_copy(node->data.function.child)))));
		/*
		 * Apply rule of inverse hyperbolic sine function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "asinh"))
			return node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('f', symbol_table_lookup(symbol_table, "sqrt"), node_create('b', '-', node_create('c', 1.0), node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0)))));
		/*
		 * Apply rule of inverse hyperbolic cosine function
		 * derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "acosh"))
			return node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('f', symbol_table_lookup(symbol_table, "sqrt"), node_create('b', '-', node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0)), node_create('c', 1.0))));
		/*
		 * Apply rule of inverse hyperbolic tangent function
		 * derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "atanh"))
			return node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('b', '-', node_create('c', 1.0), node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0))));
		/*
		 * Apply rule of inverse hyperbolic cotangent function
		 * derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "acoth"))
			return node_create('b', '/', node_derivative(node->data.function.child, name, symbol_table), node_create('b', '-', node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0)), node_create('c', 1.0)));
                /*
		 * Apply rule of inverse hyperbolic secant function
		 * derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "asech"))
			return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('u', '-', node_create('b', '*', node_create('b', '/', node_create('c', 1.0), node_create('b', '*', node_copy(node->data.function.child), node_create('f', symbol_table_lookup(symbol_table, "sqrt"), node_create('b', '-', node_create('c', 1.0), node_copy(node->data.function.child))))), node_create('f', symbol_table_lookup(symbol_table, "sqrt"), node_create('b', '/', node_create('c', 1.0), node_create('b', '+', node_create('c', 1.0), node_copy(node->data.function.child)))))));
                /*
		 * Apply rule of inverse hyperbolic cosecant function
		 * derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "acsch"))
                        return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('u', '-', node_create('b', '/', node_create('c', 1.0), node_create('b', '*', node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0)), node_create('f', symbol_table_lookup(symbol_table, "sqrt"), node_create('b', '+', node_create('c', 1.0), node_create('b', '/', node_create('c', 1.0), node_create('b', '^', node_copy(node->data.function.child), node_create('c', 2.0)))))))));
		/*
		 * Apply rule of absolute value function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "abs"))
			return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('b', '-', node_create('b', '*', node_create('c', 2.0), node_create('f', symbol_table_lookup(symbol_table, "step"), node_copy(node->data.function.child))), node_create('c', 1.0)));
                /*
		 * Apply rule of step function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "step"))
			return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('f', symbol_table_lookup(symbol_table, "delta"), node_copy(node->data.function.child)));
                /*
		 * Apply rule of delta function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "delta"))
			return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('f', symbol_table_lookup(symbol_table, "nandelta"), node_copy(node->data.function.child)));
                /*
		 * Apply rule of nandelta function derivative.
		 */
		else if (!strcmp(node->data.function.record->name, "nandelta"))
			return node_create('b', '*', node_derivative(node->data.function.child, name, symbol_table), node_create('f', symbol_table_lookup(symbol_table, "nandelta"), node_copy(node->data.function.child)));

	case 'u':
		switch (node->data.un_op.operation) {
		case '-':
			/*
			 * Apply (-f)'=-f' derivative rule.
			 */
			return node_create('u', '-', node_derivative(node->data.un_op.child, name, symbol_table));
		}

	case 'b':
		switch (node->data.bin_op.operation) {
		case '+':
			/*
			 * Apply (f+g)'=f'+g' derivative rule.
			 */
			return node_create('b', '+', node_derivative(node->data.bin_op.left, name, symbol_table), node_derivative(node->data.bin_op.right, name, symbol_table));

		case '-':
			/*
			 * Apply (f-g)'=f'-g' derivative rule.
			 */
			return node_create('b', '-', node_derivative(node->data.bin_op.left, name, symbol_table), node_derivative(node->data.bin_op.right, name, symbol_table));

		case '*':
			/*
			 * Apply (f*g)'=f'*g+f*g' derivative rule.
			 */
			return node_create('b', '+', node_create('b', '*', node_derivative(node->data.bin_op.left, name, symbol_table), node_copy(node->data.bin_op.right)), node_create('b', '*', node_copy(node->data.bin_op.left), node_derivative(node->data.bin_op.right, name, symbol_table)));

		case '/':
			/*
			 * Apply (f/g)'=(f'*g-f*g')/g^2 derivative rule.
			 */
			return node_create('b', '/', node_create('b', '-', node_create('b', '*', node_derivative(node->data.bin_op.left, name, symbol_table), node_copy(node->data.bin_op.right)), node_create('b', '*', node_copy(node->data.bin_op.left), node_derivative(node->data.bin_op.right, name, symbol_table))), node_create('b', '^', node_copy(node->data.bin_op.right), node_create('c', 2.0)));

		case '^':
			/*
			 * If right operand of exponentiation constant apply
			 * (f^n)'=n*f^(n-1)*f' derivative rule.
			 */
			if (node->data.bin_op.right->type == 'c')
				return node_create('b', '*', node_create('b', '*', node_create('c', node->data.bin_op.right->data.constant), node_derivative(node->data.bin_op.left, name, symbol_table)), node_create('b', '^', node_copy(node->data.bin_op.left), node_create('c', node->data.bin_op.right->data.constant - 1.0)));
			/*
			 * Otherwise, apply logarithmic derivative rule:
			 * (log(f^g))'=(f^g)'/f^g =>
			 * (f^g)'=f^g*(log(f^g))'=f^g*(g*log(f))'
			 */
			else {
				Node           *log_node, *derivative;

				log_node = node_create('b', '*', node_copy(node->data.bin_op.right), node_create('f', symbol_table_lookup(symbol_table, "log"), node_copy(node->data.bin_op.left)));
				derivative = node_create('b', '*', node_copy(node), node_derivative(log_node, name, symbol_table));
				node_destroy(log_node);
				return derivative;
			}
		}
	}
    return NULL;
}

void
node_flag_variables(Node * node)
{
        /*
	 * According to node type, flag variable in symbol table or
	 * proceed with calling function recursively on node children.
	 */
        switch (node->type) {
        case 'v':
                node->data.variable->flag = TRUE;
                break;

        case 'f':
                node_flag_variables(node->data.function.child);
                break;

        case 'u':
                node_flag_variables(node->data.un_op.child);
                break;

        case 'b':
                node_flag_variables(node->data.bin_op.left);
                node_flag_variables(node->data.bin_op.right);
                break;
        }
}

int
node_get_length(Node * node)
{
        FILE           *file;	/* Temporary file. */
        int             count;	/* Count of bytes written to above file. */
	int             length;	/* Length of above string. */

	/*
	 * According to node type, calculate length of string representing
	 * subtree rooted at node.
	 */
	switch (node->type) {
	case 'c':
		length = 0;
		if (node->data.constant < 0)
			length += 1;

                file = tmpfile();
                if (file) {
                        if ((count = fprintf(file, "%g", node->data.constant)) >= 0)
                                length += count;
                        fclose(file);
                }

		if (node->data.constant < 0)
			length += 1;
		return length;

	case 'v':
		return strlen(node->data.variable->name);

	case 'f':
		return strlen(node->data.function.record->name) + 1 + node_get_length(node->data.function.child) + 1;
		break;

	case 'u':
		return 1 + 1 + node_get_length(node->data.un_op.child) + 1;

	case 'b':
		return 1 + node_get_length(node->data.bin_op.left) + 1 + node_get_length(node->data.bin_op.right) + 1;
	}

	return 0;
}

void
node_write(Node * node, char *string)
{
	/*
	 * According to node type, write subtree rooted at node to node
	 * string variable.  Always use parenthesis to resolve operation
	 * precedence.
	 */
	switch (node->type) {
		case 'c':
		if (node->data.constant < 0) {
			sprintf(string, "%c", '(');
			string += strlen(string);
		}
		sprintf(string, "%g", node->data.constant);
		string += strlen(string);
		if (node->data.constant < 0)
			sprintf(string, "%c", ')');
		break;

	case 'v':
		sprintf(string, "%s", node->data.variable->name);
		break;

	case 'f':
		sprintf(string, "%s%c", node->data.function.record->name, '(');
		string += strlen(string);
		node_write(node->data.function.child, string);
		string += strlen(string);
		sprintf(string, "%c", ')');
		break;

	case 'u':
		sprintf(string, "%c", '(');
		string += strlen(string);
		sprintf(string, "%c", node->data.un_op.operation);
		string += strlen(string);
		node_write(node->data.un_op.child, string);
		string += strlen(string);
		sprintf(string, "%c", ')');
		break;

	case 'b':
		sprintf(string, "%c", '(');
		string += strlen(string);
		node_write(node->data.bin_op.left, string);
		string += strlen(string);
		sprintf(string, "%c", node->data.bin_op.operation);
		string += strlen(string);
		node_write(node->data.bin_op.right, string);
		string += strlen(string);
		sprintf(string, "%c", ')');
		break;
	}
}
