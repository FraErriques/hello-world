
# include "DoubleComparisons.h"
# define epsilon 1e-50

bool Real::operator== (const Real & second) const
{
   Real result (*this - second);
   if ( result.Fabs() >epsilon)
      {return false;}
   return true;
}


bool Real::operator< (const Real & second) const
{
   if (*this==second)
      {return false;}
   if (*this-second>epsilon)
      return false;
   return true;
}

bool Real::operator> (const Real & second) const
{
   if (*this==second)
      {return false;}
   if (*this-second<epsilon)
      return false;
   return true;
}

bool Real::operator<= (const Real & second) const
{
   if (*this>second)
      {return false;}
   return true;
}


bool Real::operator>= (const Real & second) const
{
   if (*this<second)
      {return false;}
   return true;
}


Real  Real::operator- (const Real & second) const
{
   Real result (this->x - second.x);
   return result;
}

Real  Real::operator+ (const Real & second) const
{
   Real result (this->x + second.x);
   return result;
}

Real  Real::operator* (const Real & second) const
{
   Real result (this->x * second.x);
   return result;
}

Real  Real::operator/ (const Real & second) const
{
   Real result (this->x / second.x);
   return result;
}

Real & Real::operator-= (const Real & second)
{
   *this = *this - second;
   return *this;
}

Real & Real::operator+= (const Real & second)
{
   *this = *this + second;
   return *this;
}

Real & Real::operator*= (const Real & second)
{
   *this = *this * second;
   return *this;
}

Real & Real::operator/= (const Real & second)
{
   *this = *this / second;
   return *this;
}


double Real::Fabs (void) const
{
   if ( x < epsilon  && x>-epsilon ) // zero
      { return 0.0;}
   if ( x < -epsilon ) { return  (-1.0 * x); } // negative
   return  x; // positive
}
# undef  epsilon
