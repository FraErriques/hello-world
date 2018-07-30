
/**
 *  \class  Numerics::ComplexMatrix
 *  \brief  a Linear Algebra class
 *  \author F. Erriques
 *  \since  Thu Dec 27 16:27:37 CET 2001
 */



#ifndef __COMPLEX_MATRIX_
#define __COMPLEX_MATRIX_

#include<cstddef>

#include "../Complex/Complex.h"

//# ifndef NULL
//   # define NULL 0
//   # define _NULL__REDEFINED_
//# endif
//
//# ifndef size_t
//   # define size_t unsigned int
//   # define _size_t__REDEFINED_
//# endif

namespace Numerics   {


class ComplexMatrix
{
public:
   /// Constructors and Destructors
   ComplexMatrix   ( const size_t Prows, const size_t Pcols ); // fill with zeroes
   // validate an existing file
   ComplexMatrix   ( const size_t Prows, const size_t Pcols, const char * where );
   // create in RAM and, if desired, dump on a file. (where==NULL means no dump desired)
   ComplexMatrix   ( const size_t Prows, const size_t Pcols, bool randomized, const char * where=NULL );
   /// copying methoda
   // Copy Constructor
   ComplexMatrix   ( const ComplexMatrix & original );
   // operator= only between isomorphic matrixes
   ComplexMatrix & operator=   ( const ComplexMatrix & second );
   // Destructor
   ~ComplexMatrix  ( void );

   /// algo /////////////////////////////////////////////////////////////////////////////////////////////////////
   Complex         det         ( ComplexMatrix * & diagonal )                          const;
   Complex         det         ( void )                                                const;
   ComplexMatrix   transpose   ( const ComplexMatrix & orig   )                        const;
   Complex         scalar      ( Complex * first, Complex * second, const size_t dim ) const;// scalar product
   ComplexMatrix   operator*   ( const ComplexMatrix & second )                        const;
   ComplexMatrix   operator+   ( const ComplexMatrix & second )                        const;
   ComplexMatrix   operator-   ( const ComplexMatrix & second )                        const;
   ComplexMatrix   operator/   ( const ComplexMatrix & second )                        const;
   bool            operator==  ( const ComplexMatrix & second )                        const;
   void            reductor    ( size_t i, size_t j, ComplexMatrix & smaller )         const;
   ComplexMatrix   inverse     ( void )                                                const;
   /// input-output ////////////////////////////////////////////////////////////////////////////////////////////
   void show   (size_t set_precision=4)                                                const;
   bool insert (const Complex & what, const size_t row, const size_t col);             // cannot be const

private:
   /// data /////////////////////////////////////////////////////////////////////////////////////////////////////
   const size_t rows, cols;
   Complex ** m;
   /// memory management /////////////////////////////////////////////////////////////////////////////////////////
   Complex ** allocate   ( void );                                                     // cannot be const
   void deallocate ( void );                                                           // cannot be const
   /// input-output //////////////////////////////////////////////////////////////////////////////////////////////
   void generator ( void );                                                            // non const method; generates in RAM
   void input (void);                                                                  // non const method; reads in RAM
   bool readfile  ( const char * where );                                              // non const method; reads in RAM
   bool validate (const char * where)                                                  const;
   bool writefile ( const char * where )                                               const; // generate on a file
};

} // closing namespace Numerics

//# ifdef _NULL__REDEFINED_
//   # undef NULL
//# endif
//
//# ifdef _size_t__REDEFINED_
//   # undef size_t
//# endif

#endif
