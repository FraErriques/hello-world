
#ifndef _MY_REAL
#define _MY_REAL

#include "../common_data/common_data.h"

# ifndef NULL
   # define NULL 0
   # define _NULL__REDEFINED_
# endif

# ifndef size_t
   # define size_t unsigned int
   # define _size_t__REDEFINED_
# endif


/// utilities
double Fabs (double x); // floating absolute value
double Fact (size_t x); // factorial

/// functions related to powers, logarithms, exponentials, ...
double Ln      (double x);                   // natural logarithm
double Nat_pow (double x, size_t n);         // integer exponent power
double Pow     (double base, double esp);    // power
double Exp     (double x);                   // exponential

/// real trigonometry
double Sin     (double x);
double Cos     (double x);
double Tan     (double x);
double Arctan  (double x);  // the arc(in radiants) whose tangent measures "x"

/// hyperbolic trigonometry
double Sinh (double x);
double Cosh (double x);
double Tanh (double x);








/// varieties between two points
namespace Two_Points_Interpolation {

struct Linear_Variety_Coefficients // line between two points
{
   // y = a*x+b = (y1-y0)/(x1-x0)*x + (y0-x0*(y1-y0)/(x1-x0))
   // a = (y1-y0)/(x1-x0)
   // b = (y0-x0*(y1-y0)/(x1-x0)) = y0 - x0 * a
   double alpha;
   double beta;
};

struct Exponential_Variety_Coefficients // exponential between two points
{
   /// double gamma == y0
   /// double chi   == y1/y0
   /// double psi   == x0
   /// double fi    == x1-x0
   /// y = y0*(y1/y0)^((x-x0)/(x1-x0)) = gamma*chi^((x-psi)/fi) = IMMAGINE
   /// x = fi * log_base_chi(y/gamma) + psi                     = ANTE_IMMAGINE
   double gamma; ///   y0
   double chi;   ///   y1/y0
   double psi;   ///   x0
   double fi;    ///   x1-x0
};

Linear_Variety_Coefficients       linear     (Couple left, Couple right);
Exponential_Variety_Coefficients  log_linear (Couple left, Couple right);
double linear_image        (Linear_Variety_Coefficients, double);
double linear_ante_image   (Linear_Variety_Coefficients, double);
double log_linear_image        (Exponential_Variety_Coefficients, double);
double log_linear_ante_image   (Exponential_Variety_Coefficients, double);

} // end of namespace Two_Points_Interpolation






/// will be used for least-squares varieties
namespace Many_Points_Interpolation {
struct Linear_Variety_Coefficients // will be used for least-squares line
{
   double alpha;
   double beta;
};
} // end of namespace Many_Points_Interpolation



namespace Newton_Raphson {
typedef double (*Fp) (double);
double f1 (double x , Fp f, const double dx=2.0e-9);
double NR (double xn, Fp f, const double dx=2.0e-9);
double Newton_Raphson ( double x0, Fp f, const double dx,
                        const unsigned short Max_Steps, const double epsilon);
}

# ifdef _NULL__REDEFINED_
   # undef NULL
# endif

# ifdef _size_t__REDEFINED_
   # undef size_t
# endif

#endif

