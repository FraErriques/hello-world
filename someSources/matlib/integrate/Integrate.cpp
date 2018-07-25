#include"Integrate.h"
#include<ctime>
#include <cmath>

namespace Numerics {

/// systemistic
void  Integrate::reset_parameters
     (double left_P, double right_P, const unsigned long parts_P)
  {
     left  = left_P ;
     right = right_P;
     parts = parts_P;
  }

/// wrapper
inline double Integrate::equi_trapezium ()
{
   return equi_trapezium ( left, right, parts );
}

/// wrapper
inline double  Integrate::equi_log ()
{
   return equi_log ( left, right, parts );
}

/// wrapper
inline double  Integrate::profile ( Integrator ist_par )
{
   return profile ( ist_par, left, right, parts );
}


/// constructor
// NB left_infinity MUST be set anyway. If You don't need it, set it to any value
// a constructor that sets data members, to do not touch them any more
Integrate::Integrate
(
   Functional_Form initializer,
   double left_P, double right_P,
   const unsigned long parts_P,
   const double left_infinity_P
)
     : functional_form ( initializer ),
       left_infinity   ( left_infinity_P )
{
   reset_parameters (left_P, right_P, parts_P);
   Trapezium          = & Integrate::equi_trapezium;
   Log                = & Integrate::equi_log;
   percentile_address = & Integrate::percentile;
}


//NB left_infinity MUST be set anyway. If You don't need it, set it to any value
// a constructor that doesn't influence data members
/// constructor
Integrate::Integrate
(
   Functional_Form initializer,
   const double left_infinity_P
)
     : functional_form ( initializer ),
       left_infinity   ( left_infinity_P )
{
   Trapezium          = & Integrate::equi_trapezium;
   Log                = & Integrate::equi_log;
   percentile_address = & Integrate::percentile;
}



/// numerical method
double Integrate::equi_trapezium   ( double a, double b, unsigned long n )
{
   double  delta = (b-a) / (double)n  ,
          res   = 0.0                ,
	       x     = a+delta 		        ;

   for ( ;  x < b ;   x += delta )
      {res += functional_form(x);} // sum all the internal sides
   res *= delta; // multiply them for the common base
   res += (functional_form(a)+functional_form(b))*0.5*delta; // add extrema * base/2

   return res;
}


/// numerical method
double Integrate::percentile ( double measure )
{
   double  delta = 1e-3 , // constant step
          res   = 0.0 ,
	       x     = left_infinity   ;

   for ( ;  res <= measure ;   x += delta )
      {res += 0.5 * delta * (functional_form(x+delta) + functional_form(x));}

   return x; // return the value, until which I integrated
}


/// numerical method
double Integrate::equi_log ( double a, double b, unsigned long n )
{
   double   res   = 0.0     		     ,
	        delta = (b-a) / (double)n  ,
           f_x_delta, f_x             ,
	        x = a                      ;

   for ( ;  x < b ;   x += delta )
   {
      f_x_delta = functional_form(x+delta);
      f_x       = functional_form(x);
      res += delta * (f_x_delta-f_x) / (std::log(f_x_delta)-std::log(f_x)) ;
   }

   return res;
}


/// systemistic
double  Integrate::profile ( Integrator ist_par ,
                             double a, double b ,
                             unsigned long  n    )
{
   const double magnifier = 1e4;
   std::clock_t start = std::clock();
   result = (this->*ist_par)(a,b,n);// point to the right calee
   std::clock_t finish = std::clock();
    // if there are too few steps, the spent time becomes imperceptible
   if(n<(unsigned long)9e3)
      finish *= (unsigned long)magnifier;

   double duration = (double)(finish-start) / CLOCKS_PER_SEC;
   return duration;
}

} // closing namespace Numerics

