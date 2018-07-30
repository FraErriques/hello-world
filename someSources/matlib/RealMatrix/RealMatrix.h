
/**
 *  \class  Numerics::RealMatrix
 *  \brief  a Linear Algebra class
 *  \author F. Erriques
 *  \since  Thu Dec 27 16:27:37 CET 2001
 */


#ifndef __REAL_MATRIX_
#define __REAL_MATRIX_

#include <cstddef>

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


class RealMatrix
{
public:
   /// Constructors and Destructors ////////////////////////////////////////////////////////////////////////////
    // fill with zeroes
   RealMatrix   ( const size_t Prows, const size_t Pcols );
   // validate an existing file
   RealMatrix   ( const size_t Prows, const size_t Pcols, const char * where );
   // create in RAM and, if desired, dump on a file. (where==NULL means no dump desired)
   RealMatrix   ( const size_t Prows, const size_t Pcols, bool randomized, const char * where=NULL );
   /// copying methoda
   // Copy Constructor
   RealMatrix   ( const RealMatrix & original );
   // operator= only between isomorphic matrixes
   RealMatrix & operator=   ( const RealMatrix & second );
   // Destructor
   ~RealMatrix  ( void ); // make it virtual, if you derive from this class

   /// algo /////////////////////////////////////////////////////////////////////////////////////////////////////
   double       det         ( RealMatrix * & diagonal )                             const;
   double       det         ( void )                                                const;
   RealMatrix   transpose   ( const RealMatrix & orig   )                           const;
   double       scalar      ( double * first, double * second, const size_t dim )   const;// scalar product
   RealMatrix   operator*   ( const RealMatrix & second )                           const;
   RealMatrix   operator+   ( const RealMatrix & second )                           const;
   RealMatrix   operator-   ( const RealMatrix & second )                           const;
   RealMatrix   operator/   ( const RealMatrix & second )                           const;
   bool         operator==  ( const RealMatrix & second )                           const;
   void         reductor    ( size_t i, size_t j, RealMatrix & smaller )            const;
   RealMatrix   inverse     ( void )                                                const;
   /// input-output /////////////////////////////////////////////////////////////////////////////////////////////
   void show   (size_t set_precision=4)                                             const;
   bool insert (const double & what, const size_t row, const size_t col);           // cannot be const

private:
   /// data ////////////////////////////////////////////////////////////////////////////////////////////////////////
   const size_t   rows, cols;
   double ** m;
   /// memory management ///////////////////////////////////////////////////////////////////////////////////////////
   double ** allocate   ( void );                                                   // non const: allocates
   void      deallocate ( void );                                                   // non const: DE-allocates
   /// input-output /////////////////////////////////////////////////////////////////////////////////////////////////
   void generator          ( void ); // non const method; generates in RAM          // cannot be const
   void input              (void); // non const method; reads in RAM                // cannot be const
   bool readfile           ( const char * where );                                  // non const method; reads in RAM
   bool validate           (const char * where)                                     const;
   bool writefile          ( const char * where )                                   const; // generate on a file
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

