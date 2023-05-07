#ifndef _MY__String_
#define _MY__String_


class MyString
{
public:
   // typedefS and Data
   typedef unsigned int size_type;
   static const size_type npos;// = -1;

   // Constructors Destructors and assignement operators
   MyString (void);
   MyString (const MyString & that);
   MyString (const char * that);
   MyString & operator=(const MyString & that);
   ~MyString (void);
   // ==
   bool operator==(const MyString & that) const;
   // +
   MyString operator+(const MyString & that) const; // creates a new class
   // +=
   MyString & operator+=(const MyString & that); // modifies "this"
   // []
   char & operator[](const size_type where);
   // substr
   MyString substr(size_type pos = 0, size_type n = npos) const; // creates a new class
   // find
   size_type find(const MyString & that, size_type pos=0) const;
   // c_str
   const char * c_str (void) const {return data;}

private:
   char * data;
   size_type length;
   bool has_ownership;
};

#endif
