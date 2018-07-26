
# include "RealMatrix.h"
# include "../common_data/common_data.h"

#include<cmath>      // for fabs() and pow()
#include<ctime>      // for srand(time())
#include<fstream>    // for the data files
#include<cstdlib>    // for rand()
#include<string>

namespace Numerics {


///   Constructors and Destructors   BEGIN
RealMatrix::RealMatrix   ( const size_t Prows, const size_t Pcols )
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

RealMatrix::RealMatrix   ( const size_t Prows, const size_t Pcols,
                                const char * where )
 : rows(Prows), cols(Pcols)
{
   m = allocate(); // prepare
   if ( ! validate(where) ) // validate the data file on suggested dimensions (rows, cols)
      {throw Crash("suggested dimensions (rows, cols) are inconsistent with the matrix contained in the data file");}
   readfile  (where);
}

// create in RAM and, if desired, dump on a file
RealMatrix::RealMatrix   ( const size_t Prows, const size_t Pcols,
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
RealMatrix::RealMatrix ( const RealMatrix & original )
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

RealMatrix::~RealMatrix  ( void )  //destructor
   { deallocate(); }
/// Constructors and Destructors   END

/// START memory management routines

// called by the Constructors
double ** RealMatrix::allocate ( void )
{
   m = new double*[rows];
   if ( NULL == m ) {return NULL;}
   for (size_t  j=0; j<rows; j++)
   {
      m[j] = new double[cols];
      if ( NULL == m[j] ) {return NULL;}
   }
   return m;
}

// called by the Destructor
void RealMatrix::deallocate ( void )
{
   size_t  row;
   if ( NULL == m ) return;
   for (row=0; row<rows; row++)  //  free  the matrix
      { delete[] m[row]; }
   delete[] m;
}
/// END memory management routines





/// algorithms   BEGIN
double   RealMatrix::det ( RealMatrix * & copy ) const
{
   if (rows != cols) {throw Crash("determinant only on square matrices");} // determinant only on square matrices
   const size_t n = rows; // =cols
   size_t  row ,col, xrow;// xrow is a second row-index, required for the algorithm
   double k; // no init required; k is the normalization-coefficient
   copy = new RealMatrix (*this); // Copy Constructor call

   // ######################## Kordell begin ##################################
   for ( row=1; row<n; row++) // skip first row !
   {
         for (xrow=0; xrow<row; xrow++) // another loop on the rows
         {// the n^ row must be re-normalized n-1 times
            k = - copy->m[row][xrow] / copy->m[xrow][xrow];// each time with a new coefficient
            for (col=0; col<n; col++)
               {  copy->m[row][col] += k * copy->m[xrow][col]; }
         }
   }
   // ######################## Kordell end   ##################################

   size_t  index;
   double det = 1.0;  // init to product invariant (both for real and complex cases)!
   for (index=0; index<n; index++)
      { det *= copy->m[index][index];}// det on a triangular matrix
   if(n==0) {throw Crash("\nzero-size matrix is ""NO NUMBER\n");}
   return det;
}




double RealMatrix::det ( void ) const
{
   if (rows != cols) {throw Crash("determinant only on square matrices");} // determinant only on square matrices
   const size_t n = rows; // =cols
   size_t  row ,col, xrow;// xrow is a second row-index, required for the algorithm
   double k; // no init required; k is the normalization-coefficient
   RealMatrix  copy(*this);

   // ######################## Kordell begin ##################################
   for ( row=1; row<n; row++) // skip first row !
   {
         for (xrow=0; xrow<row; xrow++) // another loop on the rows
         {// the n^ row must be re-normalized n-1 times
            k = - copy.m[row][xrow] / copy.m[xrow][xrow];// each time with a new coefficient
            for (col=0; col<n; col++)
               { copy.m[row][col] += k * copy.m[xrow][col]; }
         }
   }

   // ######################## Kordell end   ##################################

   size_t  index;
   double det = 1.0;  // init to product invariant (both for real and complex cases)!
   for (index=0; index<n; index++)
      { det *= copy.m[index][index];}// det on a triangular matrix
   if(n==0) {throw Crash("\nzero-size matrix is ""NO NUMBER\n");}
   return det;
}




// transpose a matrix
RealMatrix   RealMatrix::transpose
                  ( const RealMatrix & orig ) const
{
   RealMatrix   copy(orig.cols, orig.rows); // create [cols][rows] to transpose [row][cols]
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

// the scalar product between vectors (isomorphic obviously)
double RealMatrix::scalar ( double * first, double * second, const size_t dim ) const
{
   double res = 0.0;
   size_t index;
   for ( index=0; index<dim; index++ )
      {  res += first[index] * second[index];   }
   return res;
}






RealMatrix  RealMatrix::operator* ( const RealMatrix  & second ) const
{
   if (cols!=second.rows){throw Crash("incompatible indexes for product");}
   RealMatrix  trasp_second = transpose ( second );
   RealMatrix  res(rows, second.cols);
   size_t rA, rB;
   for ( rA=0; rA<rows; rA++ )
   {
      for ( rB=0; rB<second.cols; rB++ )
         {  res.m[rA][rB] = scalar ( m[rA], trasp_second.m[rB], cols ); }
   }
   return res;
}



RealMatrix   RealMatrix::operator+ ( const RealMatrix  & second ) const
{
   if (rows!=second.rows || cols!=second.cols){throw Crash("incompatible indexes for sum");}
   RealMatrix   res(rows, cols);
   size_t rA, rB;
   for ( rA=0; rA<rows; rA++ )
   {
      for ( rB=0; rB<cols; rB++ )
         {  res.m[rA][rB] = m[rA][rB] + second.m[rA][rB]; }
   }
   return res;
}



RealMatrix   RealMatrix::operator- ( const RealMatrix & second ) const
{
   if (rows!=second.rows || cols!=second.cols){throw Crash("incompatible indexes for sum");}
   RealMatrix  res(rows, cols);
   size_t rA, rB;
   for ( rA=0; rA<rows; rA++ )
   {
      for ( rB=0; rB<cols; rB++ )
         {  res.m[rA][rB] = m[rA][rB] - second.m[rA][rB]; }
   }
   return res;
}




RealMatrix   RealMatrix::operator/ ( const RealMatrix  & second ) const
{
   if (second.rows != second.cols) {throw Crash("the matrix to be inverted must be square");}
   if (cols != second.rows) {throw Crash("indexes are incompatible for division; must be: [a][n]*[n][n]");}
   RealMatrix   inverse = second.inverse();
   RealMatrix   res = *this * inverse;
   return res;
}




bool RealMatrix::operator== ( const RealMatrix  & second ) const
{
# define epsilon 1.0e-12
   size_t  row, col;
   for (row=0; row<rows; row++)
   {
      for (col=0; col<cols; col++)
      {
         if (fabs ( m[row][col] - second.m[row][col] ) > epsilon)
            { return false; }
      }
   }
   return true;
# undef epsilon
}



RealMatrix &  RealMatrix::operator= ( const RealMatrix  & second )
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




void RealMatrix::reductor ( size_t i, size_t j, RealMatrix  & smaller ) const
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




RealMatrix   RealMatrix::inverse (void) const
{
   if(rows!=cols){throw Crash("only square matrices can be inverted (if non singular)");}
   RealMatrix   cof(rows, rows); // square !
   RealMatrix   smaller(rows-1,rows-1);
   size_t  row, col;
   double coefficient;
   for (row=0; row<rows; row++)  // build the cofactor matrix
      {
         for (col=0; col<rows; col++) // cols==rows !  square !
            {
               reductor ( row, col,  smaller );
               coefficient = pow ( -1.0, row+1.0+col+1.0 );
               // each element of cof matrix is the det of the appropriate minor * -1^(i+j)
               cof.m[row][col] = coefficient * smaller.det();
            }
      }

   RealMatrix   trasp_cof = transpose (cof);
   double det_orig = det();   // det of original matrix
   if (0.0==det_orig)
      {throw Crash("a square matrix can be inverted only if NON SINGULAR");}
   for (row=0; row<rows; row++)  // normalize the transposed of cofactor
      {
         for (col=0; col<rows; col++) // cols==rows !  square !
            {
               trasp_cof.m[row][col] /= det_orig; // normalize the transposed of cofactor
            }
      }
   return trasp_cof; // already normalized
}
/// algorithms  END





/// input, output and memory management  BEGIN
void RealMatrix::show ( size_t set_precision ) const
{
   size_t  row, col;
   if ( m == NULL ) return;
   if ( 20 < set_precision )
      { set_precision = 20; }
   std::cout.precision (set_precision);
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
void RealMatrix::input ( void )
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
bool RealMatrix::insert (const double & what, const size_t row, const size_t col) // cannot be const
{
   if ( row>=rows || col>=cols )
      {return false;}
   m[row][col] = what;
   return true;
}



bool RealMatrix::readfile ( const char * where )
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
bool RealMatrix::validate ( const char * where ) const
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
      if ("i"==tmp) {throw Crash("a Complex file was passed to a real istance: exit!");}
      if (""==tmp || elements >= rows*cols){break;}
   }
   in.close();
   if ( elements != rows*cols )
      {return false;}
   return true;
}


bool RealMatrix::writefile ( const char * where  ) const
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



void RealMatrix::generator ( void )
{
   const double rand_reductor = (9.0)/(double)RAND_MAX;
   size_t  row, col;
   srand( (unsigned)time( NULL ) ); // seed with clock: different seed each time.
   for (row=0; row<rows; row++)
   {  //  fill a matrix with randomized entries
      for (col=0; col<cols; col++)
         { m[row][col] = rand()* rand_reductor; }
   }
}

} //closing namespace Numerics



