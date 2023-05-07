

#include "../common_data/common_data.h"
#include "../common_data/vector.h"
#include "../RealMatrix/RealMatrix.h"
#include "../Complex/Complex.h"
#include "../ComplexMatrix/ComplexMatrix.h"
#include "../StringMatrix/StringMatrix.h"
#include "../real/real.h"
//#include "../DoubleComparisons/DoubleComparisons.h"
#include "../integrate/Integrate.h"
//#include "../MyString/MyString.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

//
//bool test_DoubleComparisons (void)
//{
//   Real a(3.141514151415);
//   Real b(3.141514151415);
//   Real inc(1.141514151415e-13);
//
//
//   for ( int c=0; c<2000; c++, a+=inc, b+=inc )
//   {
//      if(b<a)
//         {return false;}
//      if(b>a)
//         {return false;}
//      if( ! (b<=a) )
//         {return false;}
//      if(! (b>=a) )
//         {return false;}
//      if(! (b==a) )
//         {return false;}
//   }
//   return true;
//}
//





bool Complex_test (void)
{
   Numerics::Complex z(3.1415);
   Numerics::Complex w = z.SinhC ();
   w = z.CoshC ();

   z.set_Re (1.57);
   w = z.LnC ();

   z.set_Re (0.78);
   w = z.SinC ();

   return true;
}


int printC (Numerics::Complex & z)
{
   return printf ( "\n\t%lf +i*( %lf)", z.Re(), z.Im() );
}



double f (double x)
   {return 3.5*x*x-3.5;}

void test_Newton_Raphson (void)
{
   const double epsilon = 5.0e-16;
   const double dx      = 5.0e-15;
   double  x, x0 = -2.14;
   unsigned short Max_Steps = 100;

   x = Newton_Raphson::Newton_Raphson ( x0, f, dx, Max_Steps, epsilon );
   printf ( "\n\n\tfound : x = %lf \t   f(x) = %le\n\n\tStrike Enter to leave",
            x, f(x));
}



double myf (double x)
{
   const double a = 3;
   const double b = 4;
   return 6.0*(a-x)*(x-b);
}



const double mi = 7.0;
const double s  = 22.0;

double Normale (double x)
{
   double exponent, res;
   const double pi = PI; //3.1415;

   exponent = pow ( (x-mi)/s, 2.0 );

   res = 1.0/(s*sqrt(2.0*pi)) * exp (-exponent/2.0);

   return res;
}

void test_Integrate (void)
{
   Numerics::Integrate  ist ( Normale, mi-3.0*s );
   //double measure = ist.equi_simpson (3.0, 3.0+1.0/6.0, 100);return; // used for TESTING
   const unsigned int hm = 4;
   double unifm[hm];
   double other[hm];
   //srand(31415);
   //time_t t = time ( NULL );
   srand( (unsigned)time( NULL ) );
   for (unsigned int k=0; k<hm; k++)
   {
      unifm[k] = (double)rand() / (double)RAND_MAX;
      other[k] = ist.percentile (unifm[k]);
   }

   double mu, mo;
   mu = mo = 0.0;
   unsigned short unifm_k, other_k;
   unifm_k = other_k = 0;
   for (unsigned int j=0; j<hm; j++)
   {
      mu += unifm[j];
      mo += other[j];
      if ( unifm[j]>=0.0 && unifm[j]<=1.0/6.0 ) {unifm_k++;}
      if ( other[j]<= mi-1.0*s+1.0/6.0 ) {other_k++;}
   }
   mu /=(double)hm;
   mo /=(double)hm;
}




/// starts Matrix section
void test_Matrix_real_1 (void)
{
   bool randomized = true;

   typedef Numerics::RealMatrix   ActualType;
   ActualType  mat (2,3, randomized);
   mat.insert (1, 0,0);
   mat.insert (2, 0,1);
   mat.insert (1, 0,2);
   mat.insert (3, 1,0);
   mat.insert (4, 1,1);
   mat.insert (1, 1,2);
   mat.show();
   ActualType  second (3,2, randomized);
   second.insert (1, 0,0);
   second.insert (2, 0,1);
   second.insert (1, 1,0);
   second.insert (3, 1,1);
   second.insert (4, 2,0);
   second.insert (1, 2,1);
   second.show();

   try
   {
      ActualType  product = mat * second;
      product.show();
      ActualType  www (2,2, randomized);
      www.insert (1, 0,0);
      www.insert (2, 0,1);
      www.insert (3, 1,0);
      www.insert (4, 1,1);
      www.show();
      std::cout << "\n\n\t determinante = "<< www.det()<<"\n\n";
      ActualType   inverse = www.inverse();
      inverse.show();
      ActualType  identity = www * inverse;
      identity.show();
      // this is dimensionally uncompatible
      ActualType  product1 = www * www;
      product1.show();
   }
   catch(Crash &cr)
   {
      std::cout << "\n\n\t An Exception has been raised by the Matrix<> class \n\n";
   }
   catch(...)
   {
      std::cout << "\n\n\t An UNKNOWN Exception has been raised and caught \n\n";
   }
}


void test_Matrix_real_2 (void) // test operators (+,-,*,=)
{
   typedef Numerics::RealMatrix   ActualType;

   try
   {
      ActualType  first__operand (2,2, true, "./first__operand.txt");
      ActualType  second_operand (2,2, true, "./second_operand.txt");
      first__operand.show();
      second_operand.show();

      ActualType  sum  = first__operand + second_operand;
      sum.show();
      ActualType  diff = first__operand - second_operand;
      diff.show();
      ActualType  div  = first__operand / second_operand;
      div.show();
      ActualType  prod = first__operand * second_operand;
      prod.show();

      ActualType * triang; // only declare a pointer; do not construct yet
      second_operand.det ( triang ); // this method will construct the diagonalized matrix
      triang->show();
      delete triang; // destroy dinamic matrix

      ActualType   inverse = second_operand.inverse();
      inverse.show();
      ActualType  identity = second_operand * inverse;
      identity.show();
   }
   catch(Crash)
   {
      std::cout << "\n\n\t An Exception has been raised by the Matrix<> class \n\n";
   }
   catch(...)
   {
      std::cout << "\n\n\t An UNKNOWN Exception has been raised and caught \n\n";
   }
}


void test_Matrix_Complex (void)
{
   bool randomized = true;
   typedef Numerics::ComplexMatrix   ActualType;
   ActualType  mat (2,3, randomized);
   mat.insert (1, 0,0);
   mat.insert (2, 0,1);
   mat.insert (1, 0,2);
   mat.insert (3, 1,0);
   mat.insert (4, 1,1);
   mat.insert (1, 1,2);
   mat.show();
   ActualType  second (3,2, randomized);
   second.insert (1, 0,0);
   second.insert (2, 0,1);
   second.insert (1, 1,0);
   second.insert (3, 1,1);
   second.insert (4, 2,0);
   second.insert (1, 2,1);
   second.show();

   try
   {
      ActualType  product = mat * second;
      product.show();

      ActualType  www (2,2, true, "./mat.txt");
      www.show();

      Numerics::Complex det = www.det(); // determinant for Complex type
      std::cout<<"\n\n\t determinante = \t"<< det.Re()<<" +i*( "<< det.Im()<<")";

      ActualType   inverse = www.inverse();
      inverse.show();
      ActualType  identity = www * inverse;
      identity.show();
      // this is dimensionally uncompatible
//      ActualType  product1 = mat * www; // throws
//      product1.show();
   }
   catch(Crash)
   {
      std::cout << "\n\n\t An Exception has been raised by the Matrix<> class \n\n";
   }
   catch(...)
   {
      std::cout << "\n\n\t An UNKNOWN Exception has been raised and caught \n\n";
   }
}



void test_Matrix_real_3 (void)
{
   typedef  Numerics::RealMatrix   ActualType;
   ActualType  first (3,3);
   first.show();
   ActualType  second (3,3);
   second.show();
   second.insert (1.1, 0,0);
   second.insert (2.2, 0,1);
   second.insert (3.3, 1,0);
   second.insert (4.4, 1,1);
   second.show();

   ActualType  third (3,3, true);
   third.show();

   try
   {
      ActualType  fifth (3,3, true, "./dump_manual_insertions.txt");
      fifth.show();
   }
   catch(Crash)
   {
      std::cout << "\n\n\t An Exception has been raised by the Matrix<> class \n\n";
   }
   catch(...)
   {
      std::cout << "\n\n\t An UNKNOWN Exception has been raised and caught \n\n";
   }

   // tests operator=
   first = second;
   first.show();

   // scalar product
   double v1[] = {1.0, 2.0, 3.0};
   double v2[] = {6.0, 5.0, 4.0};
   double scal = first.scalar ( v1,v2, 3 );
   std::cout << "\n\t Scalar product result {1.0, 2.0, 3.0}_{6.0, 5.0, 4.0}= " << scal << "\n";

   try
   {

   }
   catch(Crash)
   {
      std::cout << "\n\n\t An Exception has been raised by the Matrix<> class \n\n";
   }
   catch(...)
   {
      std::cout << "\n\n\t An UNKNOWN Exception has been raised and caught \n\n";
   }
}


//
//
//void  test_MyString (void)
//{
//   MyString kstr("Pippuzzo");
//   // copy Constructor
//   MyString  alpha (kstr);
//   bool bres;
//   // ==, =
//   if (alpha==kstr)
//      {bres = true;}
//   else
//      {bres = false;}
//   alpha = "Toro";
//   if (alpha==kstr)
//      {bres = true;}
//   else
//      {bres = false;}
//   if (alpha=="Toro")
//      {bres = true;}
//   else
//      {bres = false;}
//
//   // substr
//   char c = kstr[4];
//   kstr[4] = 'a';
//   MyString  sub = kstr.substr (2,1);
//   std::cout<< "\n\t "<< kstr.c_str() << " substr(2,1) = " << sub.c_str() << "\n\n";
//
//   // find
//   MyString::size_type  res;
//   res = kstr.find ( MyString("p") );       //  2
//   res = kstr.find ( MyString("p"), res );  //  3
//   res = kstr.find ( MyString("pu"), res ); // 11
//   res = kstr.find ( "p"       ); //  2
//   res = kstr.find ( "p" , res ); //  3
//   res = kstr.find ( "pu", res ); // 11
//
//   MyString  str;
//   str = "PippuzzoPippuzzoPippuzzo";
//
//   // +, +=
//   MyString  sum = str + "_zxz";
//   sum += str;
//   sum += "xyxyxy";
//
//   // substr
//   char jc = kstr[4];
//   kstr[4] = 'a';
//   MyString  jsub = kstr.substr (2,4);
//   std::cout<< "\n\t "<< kstr.c_str() << " substr(2,4) = " << jsub.c_str() << "\n\n";
//
//   // find
//   MyString::size_type  jkakres;
//   jkakres = str.find ( MyString("p")         ); //  2
//   jkakres = str.find ( MyString("p") , jkakres+1 ); //  3
//   jkakres = str.find ( MyString("pu"), jkakres   ); // 11
//   jkakres = str.find ( "p"         ); //  2
//   jkakres = str.find ( "p" , jkakres+1 ); //  3
//   jkakres = str.find ( "pu", jkakres   ); // 11
//}
//
//
//
//
//void test_vector_1 (void)
//{
//   Vector <double> v;
//   for (double d=0.0; d<3000.0; d+=1.0)
//   {
//      v.push_back ( d );
//   }
//
//   std::cout << "\n\n\tresizing to 2";
//   size_t hm = 2;
//   size_t kk;
//   v.resize(hm);
//   for (kk=0; kk<hm; kk++)
//   {
//      std::cout << "\n\t"<< v[kk];
//   }
//
//   std::cout << "\n\n\tresizing to 4";
//   hm = 4;
//   v.resize(hm);
//   for (kk=0; kk<hm; kk++)
//   {
//      std::cout << "\n\t"<< v[kk];
//   }
//
//   std::cout << "\n\n\treserving to 3";
//   hm = 4;
//   v.reserve(3);
//   for (kk=0; kk<hm; kk++)
//   {
//      std::cout << "\n\t"<< v[kk];
//   }
//}
//
//void test_vector_2 (void)
//{
//   Vector <double> v;
//   v.reserve (2999);
//   for (double d=0.0; d<3000.0; d+=1.0)
//   {
//      v.push_back ( d );
//   }
//}


/*
#include<vector>
void test_vector_3 (void)
{
   std::vector <double> v;

   v.reserve (6);
   std::cout <<"\n\nreserve (6)";
   for (double d=0.0; d<6.0; d+=1.0)
   {
      v.push_back ( d );
   }
   size_t ds;
   for (ds=0; ds<6; ds++)
   {
      std::cout <<"\n\t"<<v[ds];
   }

   v.resize(9);
   std::cout <<"\n\nresize(9)";
   for (ds=0; ds<9; ds++)
   {
      std::cout <<"\n\t"<<v[ds];
   }

   v.resize(3);
   std::cout <<"\n\nresize(3)";
   for (ds=0; ds<3; ds++)
   {
      std::cout <<"\n\t"<<v[ds];
   }

   v.reserve(5);
   std::cout <<"\n\nreserve(5)";
   for (ds=0; ds<5; ds++)
   {
      std::cout <<"\n\t"<<v[ds];
   }

   v.reserve(35);
   std::cout <<"\n\nreserve(35)";
   for (ds=0; ds<35; ds++)
   {
      std::cout <<"\n\t"<<v[ds];
   }

   v.resize(36);
   std::cout <<"\n\nresize(36)";
   for (ds=0; ds<36; ds++)
   {
      std::cout <<"\n\t"<<v[ds];
   }
}
*/


int main (void)
{

   test_Integrate();
//   test_Newton_Raphson ();
   test_Matrix_Complex();
   test_Matrix_real_1();
   test_Matrix_real_2(); // test operators (+,-,*,=)
   test_Matrix_real_3();

//   test_vector_1 ();
   std::cout << "\n\n";
//   test_vector_2 ();
   std::cout << "\n\n";
/*
   test_vector_3 ();
   std::cout << "\n\n";
*/
   getchar();
   return 0;
}
