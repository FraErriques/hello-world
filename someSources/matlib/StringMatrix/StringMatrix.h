
/**
 *  \class  Numerics::StringMatrix
 *  \brief  a Linear Algebra class
 *  \author F. Erriques
 *  \since  Thu Dec 27 16:27:37 CET 2001 ::Voghera.
 */


#ifndef __STRING_MATRIX_
#define __STRING_MATRIX_

#include<cstddef>
#include <string>
//
//# ifndef NULL
//   # define NULL 0
//   # define _NULL__REDEFINED_
//# endif
//
//# ifndef size_t
//   # define size_t unsigned int
//   # define _size_t__REDEFINED_
//# endif



class StringMatrix
{
public:
   /// Constructors and Destructors
   StringMatrix   ( const size_t Prows, const size_t Pcols ); // fill with zeroes
   // validate an existing file
   StringMatrix   ( const size_t Prows, const size_t Pcols, const char * where );
   // create in RAM and, if desired, dump on a file. (where==NULL means no dump desired)
   StringMatrix   ( const size_t Prows, const size_t Pcols, bool randomized, const char * where=NULL );
   /// copying methoda
   // Copy Constructor
   StringMatrix   ( const StringMatrix & original );
   // operator= only between isomorphic matrixes
   StringMatrix & operator=   ( const StringMatrix & second );
   // Destructor
   ~StringMatrix  ( void );

   /// algo ////////////////////////////////////////////////////////////////////////////////////////////////////
   StringMatrix   transpose   ( const StringMatrix & orig   )                    const;
   StringMatrix   operator+   ( const StringMatrix & second )                    const;
   bool           operator==  ( const StringMatrix & second )                    const;
   void           reductor    ( size_t i, size_t j, StringMatrix & smaller )     const;
   /// input-output ////////////////////////////////////////////////////////////////////////////////////////////
   void show   (void)                                                            const;
   bool insert (const std::string & what, const size_t row, const size_t col);   // cannot be const

private:
   /// data ////////////////////////////////////////////////////////////////////////////////////////////
   const size_t rows, cols;
   std::string ** m;
   /// memory management ///////////////////////////////////////////////////////////////////////////////
   std::string ** allocate   ( void );                                           // cannot be const
   void deallocate ( void );                                                     // cannot be const
   /// input-output ////////////////////////////////////////////////////////////////////////////////////
   void generator ( void );                                                      // cannot be const; generates in RAM
   void input (void);                                                            // cannot be const; reads in RAM
   bool readfile  ( const char * where );                                        // cannot be const; reads in RAM
   bool validate (const char * where)                                            const;
   bool writefile ( const char * where )                                         const;
};


//# ifdef _NULL__REDEFINED_
//   # undef NULL
//# endif
//
//# ifdef _size_t__REDEFINED_
//   # undef size_t
//# endif

#endif

