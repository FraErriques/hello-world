
#pragma warning(disable : 4786) // disable long name warning

#include <map>
#include <set>
#include <string>
#include <iostream>
#include <cstdio>


struct Entry
{
   long     maturity;
   double   real_time_quote;
   bool operator< (const Entry & second) const {return second.maturity > maturity;}
};
typedef std::set < Entry >    EntrySet;
typedef EntrySet * EntrySetPtr;


struct Family
{
   std::string    family_name;
   short    priority;
};

typedef Family * FamilyPtr;
struct Compare
{
   bool operator() (const FamilyPtr first, const FamilyPtr second){return first->priority < second->priority;}
};
typedef std::map < FamilyPtr, EntrySetPtr, Compare >    FamilyMap;

/// IO services
void printEntry (EntrySet::const_iterator & entry)
{
   std::cout << "\n" << "\t Maturity:        " << entry->maturity
                     << "\t Real Time Quote: " << entry->real_time_quote;
}

void printFamily (FamilyMap::const_iterator family)
{
   std::cout << "\n\n" << "\t #####  Family Name: \t" << family->first->family_name
                       << "\t Priority:    \t" << family->first->priority;
   typedef EntrySet::const_iterator    EntrySetConstIter;
   EntrySetConstIter    p, e;
   p = family->second->begin();
   e = family->second->end();
   for(;p!=e;++p) {printEntry(p);}
   std::cout << "\n" << "\t #####  End of Family: \t" << family->first->family_name;
}
void printFamily (const Family & family, const EntrySet & entry)
{
   std::cout << "\n\n" << "\t #####  Family Name: \t" << family.family_name
                       << "\t Priority:    \t" << family.priority;
   typedef EntrySet::const_iterator    EntrySetConstIter;
   EntrySetConstIter    p, e;
   p = entry.begin();
   e = entry.end();
   for(;p!=e;++p) {printEntry(p);}
   std::cout << "\n" << "\t #####  End of Family: \t" << family.family_name;
}


void printFamilyMap (const FamilyMap & family)
{
   std::cout << "\n\n\n" << "\t @@@@@@  ######  All of the Families   ###### @@@@@@ ";
   FamilyMap::const_iterator p, e;
   p = family.begin();
   e = family.end();
   for(;p!=e;++p) {printFamily(p);}
   std::cout << "\n\n" << "\t @@@@@@  ######  END All of the Families   ###### @@@@@@ ";
}
/// IO services
//
//int main ()
//{
//   FamilyMap    familymap;
//   Entry tmp;
//
//   ///  Deposit
//   Family  dep;
//   dep.family_name = "Deposit";
//   dep.priority = 2;
//   EntrySet  deposit_entry_set;
//
//   tmp.maturity = 52004;
//   tmp.real_time_quote = .0337;
//   deposit_entry_set.insert (tmp);
//
//   tmp.maturity = 52002;
//   tmp.real_time_quote = .0338;
//   deposit_entry_set.insert (tmp);
//
//   tmp.maturity = 52007;
//   tmp.real_time_quote = .0339;
//   deposit_entry_set.insert (tmp);
//
//   printFamily (dep, deposit_entry_set);
//   ///  END Deposit
//
//   ///  Swap
//   Family   swp;
//   swp.family_name = "Swap";
//   swp.priority = 3;
//   EntrySet  swap_entry_set;
//
//   tmp.maturity = 52006;
//   tmp.real_time_quote = .0347;
//   swap_entry_set.insert (tmp);
//
//   tmp.maturity = 52002;
//   tmp.real_time_quote = .0348;
//   swap_entry_set.insert (tmp);
//
//   tmp.maturity = 52008;
//   tmp.real_time_quote = .0349;
//   swap_entry_set.insert (tmp);
//
//   printFamily (swp, swap_entry_set);
//   ///  END Swap
//
//   ///  Future
//   Family   fut;
//   fut.family_name = "Future";
//   fut.priority = 1;
//   EntrySet  future_entry_set;
//
//   tmp.maturity = 52006;
//   tmp.real_time_quote = .0357;
//   future_entry_set.insert (tmp);
//
//   tmp.maturity = 52008;
//   tmp.real_time_quote = .0358;
//   future_entry_set.insert (tmp);
//
//   tmp.maturity = 52007;
//   tmp.real_time_quote = .0359;
//   future_entry_set.insert (tmp);
//
//   printFamily (fut, future_entry_set);
//   ///  END Future
//
//
//   // Family Map
//   //typedef std::map < FamilyPtr, EntrySetPtr, Compare >    FamilyMap;
//   std::pair < FamilyPtr, EntrySetPtr >   family_tmp;
//
//   family_tmp.first  = & dep; // Push DEPOSIT family into FamilyMap
//   family_tmp.second = & deposit_entry_set;
//   familymap.insert (family_tmp);
//
//   family_tmp.first  = & swp; // Push SWAP    family into FamilyMap
//   family_tmp.second = & swap_entry_set;
//   familymap.insert (family_tmp);
//
//   family_tmp.first  = & fut; // Push FUTURE  family into FamilyMap
//   family_tmp.second = & future_entry_set;
//   familymap.insert (family_tmp);
//
//   /// DUMP FamilyMap
//   printFamilyMap (familymap);
//
//   getchar();
//   return 0;
//}
