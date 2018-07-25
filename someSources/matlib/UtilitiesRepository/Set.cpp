
#pragma warning(disable:4786)


# include <set>
# include <iostream>
# include <algorithm>

struct Entry
{
   long   time;
   double value;

   bool operator<(const Entry& x) const
      {return x.time < time;}
};

struct Comparison : public std::binary_function<Entry, Entry, bool>
{
   bool operator()(const Entry& x,const Entry& y) const
      {return x.time < y.time;}
};

typedef std::set < Entry, Comparison > Curve;


void write_entry (const Curve & par)
{
   Curve::const_iterator p,e;
   p = par.begin();
   e = par.end();
   for (;p!=e; ++p)
      {  std::cout << "\n\t" << (*p).time << "\t" << (*p).value;  }
   std::cout << "\n\n";
}


template <class T>
bool smaller ( T & a,  T & b)
{
   return a.time < b.time;
}



int main ()
{
   Curve curve;

   Entry tmp;
   long t;
   double y;

   for (t=52020, y=2.0; t>52000; t--, y-=0.1)
   {
      tmp.time = t;
      tmp.value = y;
      curve.insert ( tmp );
   }


   Curve::iterator p, e;
   write_entry (curve); // show

   p = curve.begin();
   e = curve.end();
//   std::random_shuffle (p, e);
   write_entry (curve); // show

   p = curve.begin();
   e = curve.end();
   Curve::iterator r = curve.begin();
  
   // make a istance of the Predicate; since it's a template it has no linkage  !!!!
   smaller<Entry>(*p,*(r));  //   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//   std::sort (p, e, smaller<Entry> ); // !!! otherwise this doesn't work
   write_entry (curve); // show

   getchar();
   return 0;
}
