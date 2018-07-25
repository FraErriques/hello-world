
# include "../Real/DoubleComparisons.h"


bool test (void)
{
   Real a(3.141514151415);
   Real b(3.141514151415);
   Real inc(1.141514151415e-13);


   for ( int c=0; c<2000; c++, a+=inc, b+=inc )
   {
      if(b<a)
         {return false;}
      if(b>a)
         {return false;}
      if( ! (b<=a) )
         {return false;}
      if(! (b>=a) )
         {return false;}
      if(! (b==a) )
         {return false;}
   }
   return true;
}


int main (void)
{
   bool result = test();
   if ( ! result) {return 1;}
   return 0;
}
