
# include "StringMatrix.h"
# include "../common_data/common_data.h"

#include<ctime>      // for srand(time())
#include<fstream>    // for the data files
//#include<cstdlib>    // for rand() but already included from <string>



///   Constructors and Destructors   BEGIN
StringMatrix::StringMatrix   ( const size_t Prows, const size_t Pcols )
 : rows(Prows), cols(Pcols)
{
   m = allocate(); // prepare memory locations
   size_t  row, col;
   for (row=0; row<rows; row++)  // fill with zeroes
   {
      for (col=0; col<cols; col++)
         { m[row][col] = 0.0; }
   }
}

StringMatrix::StringMatrix   ( const size_t Prows, const size_t Pcols,
                                const char * where )
 : rows(Prows), cols(Pcols)
{
   m = allocate(); // prepare
   if ( ! validate(where) ) // validate the data file on suggested dimensions (rows, cols)
      {throw Crash("suggested dimensions (rows, cols) are inconsistent with the matrix contained in the data file");}
   readfile  (where);
}

// create in RAM and, if desired, dump on a file
StringMatrix::StringMatrix   ( const size_t Prows, const size_t Pcols,
                                bool randomized,
                                const char * where ) // NULL==where means "no dump desired"
 : rows(Prows), cols(Pcols) // init dimensions
{
   m = allocate();  // prepare the memory locations
   if (randomized)
   {
      generator (); // fill in RAM with random numbers
   }
   else
   {
      input();      // fill in RAM getting each entry from stdin
   }
   if ( NULL != where ) // means "no dump desired"
      {writefile (where);} // dump on a file
   else
      {return;}
}

// Copy Constructor
StringMatrix::StringMatrix ( const StringMatrix & original )
 : rows(original.rows), cols(original.cols) // init dimensions
{
   m = allocate();  // prepare the memory locations
   size_t  row, col;
   for (row=0; row<rows; row++)  //  input  the matrix, copying the original
   {
      for (col=0; col<cols; col++)
         { m[row][col] = original.m[row][col]; }
   }
}

StringMatrix::~StringMatrix  ( void )  //destructor
   { deallocate(); }
/// Constructors and Destructors   END

/// START memory management routines

// called by the Constructors
std::string ** StringMatrix::allocate ( void )
{
   m = new std::string*[rows];
   if ( NULL == m ) {return NULL;}
   for (size_t  j=0; j<rows; j++)
   {
      m[j] = new std::string[cols];
      if ( NULL == m[j] ) {return NULL;}
   }
   return m;
}

// called by the Destructor
void StringMatrix::deallocate ( void )
{
   size_t  row;
   if ( NULL == m ) return;
   for (row=0; row<rows; row++)  //  free  the matrix
      { delete[] m[row]; }
   delete[] m;
}
/// END memory management routines






// transpose a matrix
StringMatrix   StringMatrix::transpose
                  ( const StringMatrix & orig ) const
{
   StringMatrix   copy(orig.cols, orig.rows); // create [cols][rows] to transpose [row][cols]
   size_t  row, col;
   for (row=0; row<orig.rows; row++)
   {
      for (col=0; col<orig.cols; col++)
      {
         copy.m[col][row] = orig.m[row][col];
      }
   }
   return copy;
}





StringMatrix   StringMatrix::operator+ ( const StringMatrix  & second ) const
{
   if (rows!=second.rows || cols!=second.cols){throw Crash("incompatible indexes for sum");}
   StringMatrix   res(rows, cols);
   size_t rA, rB;
   for ( rA=0; rA<rows; rA++ )
   {
      for ( rB=0; rB<cols; rB++ )
         {  res.m[rA][rB] = m[rA][rB] + second.m[rA][rB]; }
   }
   return res;
}






bool StringMatrix::operator== ( const StringMatrix  & second ) const
{
   size_t  row, col;
   for (row=0; row<rows; row++)
   {
      for (col=0; col<cols; col++)
      {
         if ( m[row][col] == second.m[row][col] )
            { return false; }
      }
   }
   return true;
}



StringMatrix &  StringMatrix::operator= ( const StringMatrix  & second )
{
   if ( rows != second.rows || cols != second.cols )
      {throw Crash("only isomorphic matrixes can be copied");}

   size_t  row, col;
   for (row=0; row<rows; row++) // copy element by element
   {
      for (col=0; col<cols; col++)
         { m[row][col] = second.m[row][col]; }
   }
   return *this;
}




void StringMatrix::reductor ( size_t i, size_t j, StringMatrix  & smaller ) const
{
   size_t  row_this, col_this,  row_smaller, col_smaller;
   for ( row_this=0, row_smaller=0;
         row_this<rows;
         row_this++, row_smaller++  )
   {
      if ( row_this == i ) // "i" is the row to eliminate
         { row_smaller--; continue; } // skip the selected row
      for ( col_this=0, col_smaller=0;
            col_this<cols;
            col_this++, col_smaller++  )
      {
         if ( col_this == j ) // "j" is the col to eliminate
            { col_smaller--; continue; } // skip the selected row
         smaller.m[row_smaller][col_smaller] = m[row_this][col_this]; // main task
      }
   }
}




/// input, output and memory management  BEGIN
void StringMatrix::show ( void ) const
{
   size_t  row, col;
   if ( m == NULL ) return;
   std::cout<<"\n\n";
   for (row=0; row<rows; row++)  //  show matrix
   {
      if ( m[row] == NULL ) return;
      for (col=0; col<cols; col++)
      {
         std::cout << "\t" << m[row][col];
         if (col==cols-1)
            {std::cout<<"\n";}
      }
   }
}



// input each element of the matrix
void StringMatrix::input ( void )
{
   size_t  row, col;
   for (row=0; row<rows; row++)  //  input  the matrix
   {
      for (col=0; col<cols; col++)
      {
         std::cout<<"Enter element["<<row<<"]["<<col<<"]\t";
         std::cin >> m[row][col];
      }
   }
}


// insert a single element [row][col]
bool StringMatrix::insert (const std::string & what, const size_t row, const size_t col) // cannot be const
{
   if ( row>=rows || col>=cols )
      {return false;}
   m[row][col] = what;
   return true;
}



bool StringMatrix::readfile ( const char * where )
{  // File format is NOT compulsory:
   //      The only costraint is that the number of elements must be rows*cols
   //      Entries can be divided both by spaces, or tabs (one or more of each)

   std::ifstream  in;
   in.open (where);
   size_t row, col;
   for ( row=0; row<rows; row++)
   {
      for ( col=0; col<cols; col++ )
      {
         in >> m[row][col];
      }
   }
   in.close();
   return true;
}



// validate an existing file
bool StringMatrix::validate ( const char * where ) const
{  // File format is NOT compulsory:
   //      The only costraint is that the number of elements must be rows*cols
   //      Entries can be divided both by spaces, or tabs (one or more of each)

   std::ifstream  in;
   in.open (where);

   size_t elements;
   std::string  tmp;

   for ( elements=0; ; elements++ )
   {
      in >> tmp;
      if (""==tmp || elements >= rows*cols){break;}
   }
   in.close();
   if ( elements != rows*cols )
      {return false;}
   return true;
}


bool StringMatrix::writefile ( const char * where  ) const
{
   size_t  row, col;
   std::ofstream  out;
   out.open (where);
   srand ((unsigned)time(NULL)); // seed with clock: different seed each time.

   for (row=0; row<rows; row++)  //  fill a matrix with randomized entries
   {
      for (col=0; col<cols; col++)
      {
         out << m[row][col];
         if (col==cols-1)
            { out << "\n"; }
         else
            { out << "\t"; }
      }
   }
   out.close();
   return true;
}



void StringMatrix::generator ( void )
{
   const double rand_reductor = (122.0-97.0)/(double)RAND_MAX;

   size_t  row, col;
   srand( (unsigned)time( NULL ) ); // seed with clock: different seed each time.
   for (row=0; row<rows; row++)
   {  //  fill a matrix with randomized entries
      for (col=0; col<cols; col++)
      {
         for (short c=0; c<4; c++)
         {
            double extracted = (double)rand();
            extracted *= rand_reductor;
            extracted += 97.0;
            char tmp =  (char) (extracted);
            m[row][col] += tmp;
         }
      }
   }
}


