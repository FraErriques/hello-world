
# include "real.h"

# ifndef NULL
   # define NULL 0
   # define _NULL__REDEFINED_
# endif

# ifndef size_t
   # define size_t unsigned int
   # define _size_t__REDEFINED_
# endif

double Fabs (double x)
{
   if ( x < 0.0 ) { x *= -1.0; }
   return x;
}

double Fact (size_t x) // factorial
{
   double res = 1.0;
   for (; x>1; x--)
      { res *= x; }
   return res;
}


bool is_odd (size_t index)
{
   double real = (double)index / 2.0;

   if ( (real-(size_t)real) > 1e-30 )
      return true;

   return false;
}


double general_rounder (double par)
{
    const double epsilon = 1e-16;
    bool is_negative = false;
    if ( par < 0.0 ) {is_negative = true; }
    double rounded;
    long ipart = (long) par;     // integer  part
    double fpart = par - ipart;  // floating part
    if ( Fabs(fpart)<epsilon )
     {rounded = (double)ipart;}
    else if ( Fabs(fpart)>(1.0-epsilon) )
    {
        if ( is_negative )
           {rounded = (double)ipart-1.0;} // growth downwards
        else
           {rounded = (double)ipart+1.0;} // growth upwards
    }
    else // no round needed
       {rounded = par;}

    return rounded;
}




double Nat_pow (double x, size_t n)
{
   double res = 1.0; // product invariant
   for (size_t j=0; j<n; j++)
      { res *= x; }
   return res;
}



double Pow (double base, double esp)  // power
{
   Domain domain = 1; // to be thrown before calling ln(0)
   double res;
   if ( Fabs(base) > 1e-50 )
      { res = Exp ( esp * Ln (base) ); }
   else
     throw domain;
   return res;
}






double Ln (double x)  // natural logarithm
{
   double   res = 1.0,
            admitted,
            c = 0.0, resto = 0.0,
            bi_res = 0.0;
   bool is_right_side;

   if ( x >= 1.0 ) // domain zone selector
      { admitted = 1.09; is_right_side = true; }
   else if ( x > 0.0   &&   x < 1.0 )
      { admitted = 0.9; is_right_side = false; }
   else  if ( x < 1e-60 )
      { Domain domain=1; throw domain; }

   for ( ; ; )
      {
         res *= admitted;  c++;
         if ( is_right_side )
            { if (res >= x) {break;} }
         else // x in (0, 1)
            { if (res <= x) {break;} }
      }
   resto = x/res;
   res = 0.0; // re-init and re-use for a reduct
   size_t k;
   double sign;

# define reduct_order 8e1
   for ( k=0; k < reduct_order;  k++ ) // reduct order
      {
         if (is_odd(k)) {sign=-1.0;}else{sign=1.0;}
         res    += sign/(k+1) * Nat_pow ( admitted -1.0, k+1);
         bi_res += sign/(k+1) * Nat_pow ( resto    -1.0, k+1);
      }
# undef  reduct_order
   return ( c * res + bi_res );
}



double Exp (double x)  // exponential
{
   double res = 0.0, admitted = 1.09,
            c = 0.0, resto    = 0.0;
   bool is_negative = false;

   if ( x < 0.0 ) { x *= -1.0; is_negative=true; }

   for ( ; ; )   // domain zone selector
      { // the argument "x" has been splitted in x = c * admitted + resto
         res += admitted;  c++;
         if ( res > x )
            { res-=admitted;  c--; break; } // i want resto>0
      }
   resto = x-res;

   size_t k;
   const size_t order = 70;

   double the_fact = 1.0; // factorial calculation
   const double e_admitted = 2.9742740725630653163119065891359;  //  e^admitted : CASHED_VALUE
   double e_resto=0.0; //  e^resto
   for (k=0; k<order; k++) //  ######## Kordell
      {
         if (k>1) {the_fact*=k;}
         // e^admitted is CASHED !
         e_resto  += Nat_pow (resto, k) / the_fact;    // e^resto
      }

   double result;
   double e_admitted_alla_c = 1.0; // product invariant

   for (k=0; k<c; k++) //  ######## Kordell
      { e_admitted_alla_c *= e_admitted; }
   result = e_admitted_alla_c * e_resto;

   if (is_negative) { result = 1.0/result; } // e^-x==1/(e^x)
   return result;
}



double traslatore_dominio_seni (double x)
{
   double entita_di_molteplicita, parte_sottomultipla, traslato;
   entita_di_molteplicita = x / (2.0*PI);
   parte_sottomultipla = entita_di_molteplicita - (int)entita_di_molteplicita;
   traslato = (2.0*PI) * parte_sottomultipla;
   return traslato;
}


double Sin (double x)
{
   x = traslatore_dominio_seni (x);
   double res = 0.0;
   size_t k;
   double the_fact = 1.0; // factorial calculation
   double sign;
   for (k=0; k<29; k++) // reduct order set at 29
      {
         if (k>1) {the_fact*=k;}
         if (is_odd(k)) {sign=-1.0;}else{sign=1.0;}
         res += sign/the_fact * Nat_pow(x, 2*k+1);
      }
   return res;
}

double Cos (double x)
{
   x = traslatore_dominio_seni (x);
   double res = 0.0;
   size_t k;
   double the_fact = 1.0; // factorial calculation
   double sign;
   for (k=0; k<29; k++) // reduct order set at 29
      {
         if (k>1) {the_fact*=k;}
         if (is_odd(k)) {sign=-1.0;}else{sign=1.0;}
         res += sign/the_fact * Nat_pow(x, 2*k);
      }
   return res;
}


double Tan (double x)
{
   Domain domain = 1; // to be thrown before performing division by zero
   double denominator = Cos(x);
   if ( Fabs(denominator) < 1e-60 )
     { throw domain; }
   return Sin(x) / denominator;
}







double Arctan (double y)  // the arc(in radiants) whose tangent measures "y"
{
   double res = 0.0, x;
   bool is_negative = false;
   if ( y < 0.0 ) // the function is odd. I map only R+
   {
      y *= -1.0;
      is_negative = true;
   }
   bool is_transformed = false;
   if ( y > 1.0 )
      {
         x = 1.0 / y;            // arctan(x)+arctan(1/x)=PI/2
         is_transformed = true;
      }
   else
      { x = y; }
   double the_distance = Fabs (1-x);
   size_t k, order;
   double sign;
   if (Fabs(the_distance)>1e-7)
      {
        order = 26 * (size_t)(1.0 / the_distance);
        for (k=0; k<order; k++) // reduct order not-constant: OPTIMIZED
          {
             if (is_odd(k)) {sign=-1.0;}else{sign=1.0;}
             res += sign/(2*k+1) * Nat_pow(x, 2*k+1);
          }
      }
   else
      { res = PI/4.0; }

   if (is_transformed) {res =  PI/2.0-res;}    // arctan(x)+arctan(1/x)=PI/2  ->  arctan(x)=PI/2-arctan(1/x)
   if (is_negative)    {res *= -1.0;}          // the function is odd. I map only R+
   return res;
}





double Sinh (double x)
{
   double res = 0.0;
   size_t k;
   double the_fact = 1.0; // factorial calculation
   for (k=0; k<7e1; k++) // reduct order set at 7e1
      {
         if (k>1) {the_fact*=k;}
         res += 1.0/the_fact * Nat_pow(x, 2*k+1);
      }
   return res;
}

double Cosh (double x)
{
   double res = 0.0;
   size_t k;
   double the_fact = 1.0; // factorial calculation
   for (k=0; k<7e1; k++) // reduct order set at 7e1
      {
         if (k>1) {the_fact*=k;}
         res += 1.0/the_fact * Nat_pow(x, 2*k);
      }
   return res;
}

double Tanh (double x)
{
   return Sinh(x)/Cosh(x);
}





// ########## begin  of interpolation and extrapolation modes
namespace  Two_Points_Interpolation {

double linear_image ( Linear_Variety_Coefficients coefficients,
                      double x )
{
   double  y;
   ///  no need to compute coefficients; use the direct ones
   //   a = (y1-y0)/(x1-x0);
   //   b = y0-x0*a;
   y = coefficients.alpha * x + coefficients.beta; //   y = a*x + b;
   return y;
}


double linear_ante_image ( Linear_Variety_Coefficients coefficients,
                           double y )
{
   double x;
   double c,d;
   /// compute the inverse coefficients
   c = 1.0/coefficients.alpha;                    // c = (x1-x0)/(y1-y0) = 1/a
   d = -coefficients.beta / coefficients.alpha;   // d = -b/a;
   x = c*y + d;
   return x;
}


Linear_Variety_Coefficients linear (Couple left, Couple right)
{
   Linear_Variety_Coefficients  coefficients;
   double x0,x1, y0,y1;
   x0 = left.argument;
   x1 = right.argument;
   y0 = left.image;
   y1 = right.image;

   if (Fabs(x1-x0)<1.e-12)// vertical line x=x0
      { Domain domain = 1; throw domain; }
   // y = a*x+b = (y1-y0)/(x1-x0)*x + (y0-x0*(y1-y0)/(x1-x0))
   // a = (y1-y0)/(x1-x0)
   // b = (y0-x0*(y1-y0)/(x1-x0)) = y0 - x0 * a
   coefficients.alpha = (y1-y0)/(x1-x0);
   coefficients.beta  = y0 - x0 * coefficients.alpha;

   return  coefficients;
}



double log_linear_image ( Exponential_Variety_Coefficients coefficients,
                          double x )
{
   double  y;
   ///  no need to compute coefficients; use the direct ones

   /// double gamma == y0
   /// double chi   == y1/y0
   /// double psi   == x0
   /// double fi    == x1-x0

   /// y = y0*(y1/y0)^((x-x0)/(x1-x0)) = gamma*chi^((x-psi)/fi)
   y = coefficients.gamma * Pow ( coefficients.chi,((x-coefficients.psi)/coefficients.fi) );

   return y;
}


double log_linear_ante_image ( Exponential_Variety_Coefficients coefficients,
                               double y )
{
   double x;
   double log_base_chi;
   /// compute the inverse coefficients
   /// y = y0*(y1/y0)^((x-x0)/(x1-x0)) = gamma*chi^((x-psi)/fi) = immagine
   /// x = fi * log_base_chi(y/gamma) + psi = ante_immagine
   log_base_chi = Ln (y/coefficients.gamma) / Ln (coefficients.chi);
   x = coefficients.fi * log_base_chi + coefficients.psi;

   return x;
}




Exponential_Variety_Coefficients log_linear (Couple left, Couple right)
{
   Exponential_Variety_Coefficients  coefficients;
   double x0,x1, y0,y1;
   x0 = left.argument;
   x1 = right.argument;
   y0 = left.image;
   y1 = right.image;

   if (Fabs(x1-x0)<1.e-12)// vertical line x=x0
      { Domain domain = 1; throw domain; }
   if (Fabs(left.image)<1.e-12)
      { Domain domain = 1; throw domain; }// an exponential curve does not reach zeroes

   //   y = y0*(y1/y0)^((x-x0)/(x1-x0)) = gamma*chi^((x-psi)/fi)
   coefficients.gamma = y0;                   /// double gamma == y0
   coefficients.chi   = y1/y0;                /// double chi   == y1/y0
   coefficients.psi   = x0;                   /// double psi   == x0
   coefficients.fi    = x1-x0;                /// double fi    == x1-x0

   return  coefficients;
}
} /// end of namespace Two_Points_Interpolation
/// ########## end  of interpolation and extrapolation modes


namespace Newton_Raphson {

//static   const double dx = 2.0e-9;

double f1 (double x, Fp f, const double dx)
{
   return (f(x+dx)-f(x))/dx;
}

double NR (double xn, Fp f, const double dx)
{
   return xn - f(xn)/f1(xn,f,dx);
}

double Newton_Raphson ( double x0, Fp f, const double dx,
                        const unsigned short Max_Steps, const double epsilon)
{
   unsigned short steps;
   double x;
   for ( steps=0, x=x0;
         Fabs(f(x))>epsilon && steps<Max_Steps;
         steps++                                )
      { x = NR (x, f, dx); }
   return x;
}

} // end of namespace Newton_Raphson

# ifdef _NULL__REDEFINED_
   # undef NULL
# endif

# ifdef _size_t__REDEFINED_
   # undef size_t
# endif


