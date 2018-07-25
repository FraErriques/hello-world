
# ifndef _MY_VECTOR_
# define _MY_VECTOR_

# ifndef NULL
   # define NULL 0
   # define _NULL__REDEFINED_
# endif

# ifndef size_t
   # define size_t unsigned int
   # define _size_t__REDEFINED_
# endif

template <class T> class Vector
{
   public:
      Vector            (void);
      ~Vector           (void);
      bool reserve      (size_t hm);
      bool resize       (size_t hm);
      bool push_back    (const T & entry);
      T & operator[]    (const size_t index);
      T * at            (const size_t index);
      size_t size(void) const;
      size_t capacity() const;

   private:
      T *      store;
      size_t   cardinality;
      size_t   last;
	   size_t   chunk_resizer (void);
};



template <class T>
Vector<T>::Vector (void)
{ 
   store = NULL;
   cardinality = 0;
   last = 0;
}



template <class T>
Vector<T>::~Vector (void)
{ 
   delete[] store;
}


template <class T>
size_t Vector<T>::chunk_resizer (void)
{
   return (size_t)(2.0*(cardinality * cardinality +1)/(cardinality+1));
}


template <class T>
bool
Vector<T>::reserve (size_t hm)
{
   if (hm<=cardinality) {return true;} // already able to contain the requested size
   delete[] store;
   store = new T[hm];
   if (NULL == store) {return false;} // failed
   cardinality = hm;// adjust the cardinality
   return true;
}


template <class T>
bool
Vector<T>::resize     (size_t hm)
{
   size_t elements_to_copy=0;
   if ( hm < last+1 ) // shorten the vector
      {elements_to_copy = hm;}
   else if (store)  // DO NOT shorten the vector (BUT CHECK THE EFFECTIVENESS OF ITS STORE ! )
      {elements_to_copy = last+1;}

   Vector<T> tmp;
   if ( ! tmp.reserve (elements_to_copy) ) // temporary
	   { return false; }
   size_t j;
   for ( j=0; j<elements_to_copy; j++ )
      { tmp.push_back(store[j]); } // feed the temporary

   delete[] store;// delete the old content; The temporary now has its own copy with size==elements_to_copy
   store = new T[hm];
   if ( NULL == store ) {return false;}
   cardinality = hm;// cardinality increased to "hm"; "last" unchanged
   for ( j=0; j<elements_to_copy; j++ )
      { store[j] = tmp[j]; } // feed back the "store"
   if (elements_to_copy) // something copyed
      {last = elements_to_copy-1;}

   return true;
}



template <class T>
size_t
Vector<T>::size (void) const
{
   return last;
}


template <class T>
size_t
Vector<T>::capacity() const
{
   return cardinality-last;
}



template <class T>
bool
Vector<T>::push_back    (const T & entry)
{
size_t chunk_size;
# define M_DEBUG
# ifdef M_DEBUG
static unsigned k = 0;
# endif
   if ( last >= cardinality )
      {
	      # ifdef M_DEBUG
	         printf("\n\t %d before : %d", ++k, cardinality);
         # endif

	      chunk_size = chunk_resizer(); // increase by a chunk;
         if ( ! resize ( chunk_size ) ){return false;}

	      # ifdef M_DEBUG
	         printf(" \t after : %d", cardinality);
         # endif
      }
   store[last++] = entry; // increase "last", after use
   return true;
}



template <class T>
T &
Vector<T>::operator[] ( const size_t index )
{
   return *(store+index); // unsafe; will crash, with invalid indexes
}




template <class T>
T *
Vector<T>::at (const size_t index)
{
   if ( cardinality > index )  // safe
      { return  (store+index); }
   return NULL;
}


# ifdef _NULL__REDEFINED_
   # undef NULL
# endif

# ifdef _size_t__REDEFINED_
   # undef size_t
# endif

# endif
