
# ifndef _COMMON_DATA_
# define _COMMON_DATA_

#include <iostream>
#include <cmath>

typedef short Domain; // type for throwing exceptions

#define PI M_PI

//const double PI = 3.1415926535897932384626433832795; // double PI = 6.0 * arctan ( pow (3.0, -0.5) );
//const double E  = 2.7182818284590452353602874713527;


struct Crash // a type for Exception-Handling
{
   Crash (const char * message) {std::cout << "\n\t" << message << "\n\t";}
   Crash (void)                 {char  message[] = "\n\tMemory Fault\n\n"; std::cout<<message;}
};

struct Couple // the generic vector of R2, used by the interpolation methods
{
   double argument; // abscissa
   double image;    // ordinate
};
# ifdef _USE_VECTOR_
   # include "vector.h"
   typedef Vector < Couple > Table; // Class for a vector of R2 points
# endif

# endif

