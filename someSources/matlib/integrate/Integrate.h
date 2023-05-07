/*
 *  \author  F. Erriques
 *  \since   August 2001
 *  \notes   The integration methoda are validated by the standard proofs; the Trapezi
 *           method gives analytic results on linear functions. The logarithmic method
 *           gives analytic results on exponential functions. These tests are contained
 *           in the integrate_test.c file of the fmropen test project. Obviously these analytic
 *           results are obtained in one step; this means that the interpolating
 *           function adopted by the method is the same as the choosen integrand.
 */


#ifndef __INTEGRATE_
#define __INTEGRATE_


#include <cmath>


namespace Numerics {


class     Integrate
{
public:
   typedef double (Integrate::* Integrator)(double ,double, unsigned long);
   typedef double (Integrate::* Percentile)(double);
   typedef double (* Functional_Form)(double);
   const Functional_Form  functional_form;
   double result; // used to publish the result through the "profile method"
   Integrator Trapezium, Log;// pointers to member functions
   Percentile  percentile_address;
   /// numerical methods
   double equi_trapezium ( double a, double b, unsigned long n );
   double percentile ( double measure );
   double equi_log ( double a, double b, unsigned long n );
   /// systemistic
   inline double profile ( Integrator ist_par );
   double profile ( Integrator ist_par, double a, double b, unsigned long  n );
   void reset_parameters  ( double left_P, double right_P, const unsigned long parts_P );
   /// wrappers
   inline double equi_trapezium ();
   inline double equi_log ();
   /// constructors
   Integrate ( Functional_Form initializer, double left_P, double right_P, const unsigned long parts_P, const double left_infinity_P );
   Integrate ( Functional_Form initializer, const double left_infinity_P );

private:
   double left, right;         // integration domain extrem
   unsigned long parts;        // partitioning intervals
   const double left_infinity; // conveniently set -inf
};


} // closing namespace Numerics

#endif

