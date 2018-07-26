#include "MyString.h"

#define NULL 0

const MyString::size_type  MyString::npos = -1;

// Constructor
MyString::MyString (void)
{// only builds the class, without contained data
   data   = NULL;
   length = 0;
   has_ownership = false; // nothing to delete[]
}
// copy Constructor from another class MyString
MyString::MyString (const MyString & that)
{
   data = new char[that.length+1];
   length = that.length;
   for(size_type c=0; c<length; c++){data[c] = that.data[c];}data[length]=0; // terminate
   has_ownership = true; // the ownership is mine. I copied the pointee of "that" to another pointee
}
// copy Constructor from a raw char *
MyString::MyString (const char * that)
{
   for (length=0; that[length]!=0; length++) ; // a hand-made "strlen", that gives value to this->length
   data = new char[length+1];
   for(size_type c=0; c<length; c++){data[c] = that[c];}data[length]=0; // terminate
   has_ownership = true; // the ownership is mine. I copied the pointee of "that" to another pointee
}
// Destructor
MyString::~MyString (void)
{
   if (has_ownership)
   {
      delete[] data;
      data = NULL;
   }
}
// =   assignement operator
MyString & MyString::operator=(const MyString & that)
{
   if (has_ownership)
   {
      delete[] data;
      data = NULL;
   }
   data = new char[that.length+1];
   length = that.length;
   for(size_type c=0; c<length; c++){data[c] = that.data[c];}data[length]=0; // terminate
   has_ownership = true; // the ownership is mine. I copied the pointee of "that" to another pointee
   return *this;
}

// ==
bool MyString::operator==(const MyString & that) const // just a boolean check
{
   if ( length != that.length )
      {return false;}
   for (size_type j=0; j<that.length; j++)
   {
      if ( that.data[j] != data[j] )
         {return false;}
   }
   return true;
}

// +
MyString MyString::operator+(const MyString & that) const // creates a new class MyString
{
   size_type new_syze = length + that.length;
   char * tmp = new char[new_syze+1];
   for (size_type j=0; j<new_syze; j++)
   {
      if (j<length)
      {
         tmp[j] = data[j]; // take from the first operand
      }
      else
      {
         tmp[j] = that.data[j-length]; // take from the second operand
      }
   }
   tmp[new_syze]=0;
   MyString  Return(tmp);// the copy Constructor from char * will set this->length
   delete[] tmp;
   return Return;
}

// []
char & MyString::operator[](const size_type where) // NOT const; can be both left and right value
{ // N.B. the returned reference is NOT const. So the operator[] can be both left and right value. es. s[2]='a', char c=s[2]
   if (where >= length)
      {throw 0;}
   return data[where];
}

// +=
MyString & MyString::operator+=(const MyString & that) // NOT const: modifies "this"
{
   size_type new_length = length + that.length;
   char * tmp = new char[new_length+1];
   for (size_type j=0; j<new_length; j++)
   {
      if (j<length)
      {
         tmp[j] = data[j]; // take from the first operand
      }
      else
      {
         tmp[j] = that.data[j-length]; // take from the second operand
      }
   }
   tmp[new_length]=0; // terminate the temporary
   length = new_length; // change of this->length
   if (has_ownership) // if "this" had ownership on "data", delete[] it. Soon "this" will point to new data
   {
      delete[] data;
      data = NULL;
   }
   data = tmp;// this->data points to tmp (i.e. has ownership). tmp is not delete[]
   return *this;
}

// substr
MyString MyString::substr (size_type pos, size_type n) const // creates a new class MyString
{// pos is where i start; n are how many characters i take.
   // default for pos==0
   // default for   n==npos. This default needs to be catched and corrected to this->length
   if (npos==n){n=length-pos;} // means: take all the remaining characters
   if ( pos>=length || n>=length )
      {throw 0;}
   char * tmp = new char[n+1];
   size_type j;
   for (j=pos; j<pos+n; j++)
   {
      tmp[j-pos] = data[j];
   }
   tmp[j-pos]=0;
   MyString   Return(tmp);
   delete[] tmp;
   return Return;
}

// find
MyString::size_type MyString::find (const MyString & that, size_type pos) const // returns an index
{
   if ( pos>=length ) // default for pos==0
      {throw 0;}
   size_type container = pos;
   bool is_complete = false;
   size_type partial_accumulation=0;
   size_type searched=0;

   for ( ; container < length; )
   {
      for ( searched=partial_accumulation=0; searched<that.length; searched++ )
      {
         if ( that.data[searched] != data[container] )
         {
            container -= partial_accumulation;
            break;
         }
         if (that.length-1 == searched)// if ALL THE SEARCHED TOKEN WAS FOUND
         {
            is_complete=true; // increment container another time before breaking !
         }        // then the research is COMPLETE
         partial_accumulation++;
         container++;// while the comparison succeeds step forwards BOTH on the container and on the searched
      }// end loop on the SEARCHED TOKEN
      if (is_complete)
      {
         break; // if ALL the searched token was found, then break parsing the container and return "container"
      }// "container" holds just the position, in the container, where the found "searched" starts
      else
      {
         container++;
      }
   }// end loop on the CONTAINER

   size_type Return;
   if ( length == container ) // all the CONTAINER was UNUSEFULLY parsed
      {Return = npos;} // means return "NOT FOUND"
   else
      {Return = container-that.length;} // (where the found token ends-its length)==(where it starts)
   return Return;
}

#undef NULL
