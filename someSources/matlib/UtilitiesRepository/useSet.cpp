//
//# ifdef WIN32
//   #pragma warning(disable:4786)
//# endif
//
//#include <map>
//#include <set>
//#include <string>
//#include <iostream>
//#include <cstdio>
//#include <cstdlib>
//
//
//struct Entry
//{
//   long     maturity;
//   double   real_time_quote;
//   bool operator< (const Entry & second) const {return second.maturity > maturity;}
//};
//typedef std::set < Entry >    EntrySet;
//typedef EntrySet * EntrySetPtr;
//
//
//
//struct Family
//{
//   std::string    family_name;
//   short          priority;
//   short          fill_mode;
//   short          stub_mode;
//   short          can_extra;
//};
//typedef Family * FamilyPtr;
//struct Compare
//{
//   bool operator() (const FamilyPtr& first, const FamilyPtr& second) const {return first->priority < second->priority;}
//};
//typedef std::map < FamilyPtr, EntrySetPtr, Compare >    FamilyMap;
//
//
///// IO services
//void printEntry (EntrySet::const_iterator & entry)
//{
//   std::cout << "\n" << "\t Maturity:        " << entry->maturity
//                     << "\t Real Time Quote: " << entry->real_time_quote;
//}
//
//void printFamily (FamilyMap::const_iterator family)
//{
//   std::cout << "\n\n" << "\t #####  Family Name: \t" << family->first->family_name
//                       << "\n\t priority : " << family->first->priority
//                       << "\n\t fill_mode: " << family->first->fill_mode
//                       << "\n\t stub_mode: " << family->first->stub_mode
//                       << "\n\t can_extra: " << family->first->can_extra;
//   typedef EntrySet::const_iterator    EntrySetConstIter;
//   EntrySetConstIter    p, e;
//   p = family->second->begin();
//   e = family->second->end();
//   for(;p!=e;++p) {printEntry(p);}
//   std::cout << "\n" << "\t #####  End of Family: \t" << family->first->family_name;
//}
//void printFamily (const Family & family, const EntrySet & entry)
//{
//   std::cout << "\n\n" << "\t #####  Family Name: \t" << family.family_name
//                       << "\n\t priority : " << family.priority
//                       << "\n\t fill_mode: " << family.fill_mode
//                       << "\n\t stub_mode: " << family.stub_mode
//                       << "\n\t can_extra: " << family.can_extra;
//   typedef EntrySet::const_iterator    EntrySetConstIter;
//   EntrySetConstIter    p, e;
//   p = entry.begin();
//   e = entry.end();
//   for(;p!=e;++p) {printEntry(p);}
//   std::cout << "\n" << "\t #####  End of Family: \t" << family.family_name;
//}
//
//
//void printFamilyMap (const FamilyMap & family)
//{
//   std::cout << "\n\n\n" << "\t @@@@@@  ######  All of the Families   ###### @@@@@@ ";
//   FamilyMap::const_iterator p, e;
//   p = family.begin();
//   e = family.end();
//   for(;p!=e;++p) {printFamily(p);}
//   std::cout << "\n\n" << "\t @@@@@@  ######  END All of the Families   ###### @@@@@@ ";
//}
///// IO services
//
//
//
//
//FamilyPtr retrieve_family_membership (const std::string & par)
//{
//   static bool first_time = true;
//   static FamilyPtr   dep;
//   static FamilyPtr   swp;
//   static FamilyPtr   fut;
//
//   if (first_time) // init only once
//   {
//      // will be in the DataBase tomorrow
//      dep = new Family;
//      swp = new Family;
//      fut = new Family;
//      // will be in the DataBase tomorrow
//
//      ///  Deposit
//      dep->family_name = "Deposit";
//      dep->priority  = 2;
//      dep->fill_mode = 3;
//      dep->stub_mode = 3;
//      dep->can_extra = 0;
//      ///  END Deposit
//
//      ///  Future
//      fut->family_name = "Future";
//      fut->priority  = 1;
//      fut->fill_mode = 3;
//      fut->stub_mode = 3;
//      fut->can_extra = 0;
//      ///  END Future
//
//      ///  Swap
//      swp->family_name = "Swap";
//      swp->priority  = 3;
//      swp->fill_mode = 3;
//      swp->stub_mode = 3;
//      swp->can_extra = 1;
//      ///  END Swap
//
//      first_time = false; // init only once
//   }
//
//   if ( par.find("DEP") != std::string::npos ) return  dep;
//   if ( par.find("FUT") != std::string::npos ) return  fut;
//   if ( par.find("SWP") != std::string::npos ) return  swp;
//   return NULL;
//}
//
//
//
//
//short CurveGenPrototype
//(
//   double      * real_time_rates,
//   long        * maturities,
//   std::string * names,
//   short       * is_active,
//   short         cardinality
//)
//{
//   struct CurveGenLocalData
//   {
//      typedef Family * FamilyPtr;
//      typedef std::map < FamilyPtr, EntrySetPtr, Compare >    FamilyMap;
//      FamilyMap                                               familymap;
//      FamilyPtr                                               family_ptr;
//      CurveGenLocalData()
//      {}
//      ~CurveGenLocalData()
//      {}
//   };
//
//   CurveGenLocalData  curvegenlocaldata;
//   for (short i=0; i<cardinality; i++)
//   {
//      if ( is_active[i] )
//	   {
//	      curvegenlocaldata.family_ptr =
//	         retrieve_family_membership ( names[i] );
//
//	      CurveGenLocalData::FamilyMap::iterator  p, e;
//         p = curvegenlocaldata.familymap.find ( curvegenlocaldata.family_ptr );
//         e = curvegenlocaldata.familymap.end();
//     //    if(p==NULL) continue;  // it's NOT an allowed family
//         if ( e == p ) // it's an allowed family, but not yet inserted
//         {
//            // Family Map
//            //typedef std::map < FamilyPtr, EntrySetPtr, Compare >    FamilyMap;
//            std::pair < FamilyPtr, EntrySetPtr >   family_tmp;
//            family_tmp.first  =  curvegenlocaldata.family_ptr; // Push a Family (a visitor *)
//            family_tmp.second = new EntrySet;
//            curvegenlocaldata.familymap.insert (family_tmp);
//         }// else the considered family has already got enties
//         Entry tmp_entry;
//         tmp_entry.maturity = maturities[i];
//         tmp_entry.real_time_quote = real_time_rates[i];
//         curvegenlocaldata.familymap[curvegenlocaldata.family_ptr]->insert (tmp_entry);
//      } // end if (is_active[i])
//   } // end for (each_element
//
//   // have a report
//   printFamilyMap (curvegenlocaldata.familymap);
//
///*
//   /// starts semaphore management
//   // ...
//*/
//   return 0;
//}
//
//
//int main ()
//{
//   double real_time_rates[]= {.0335,    .0349,    .0348,      .0347,    .0346,    .0345};
//   long   maturities[]=      {52000,    51001,    51900,      51800,    51100,    51700};
//   std::string names[]=      {"EUR_SWP","EUR_DEP","EUR_FUT",  "EUR_SWP","EUR_DEP","EUR_FUT"};
//   short is_active[]=        {1,1,1,1,1,1};
//   short cardinality=        6;
//
//   CurveGenPrototype (real_time_rates,maturities,names,is_active,cardinality);
//
//   getchar();
//   return 0;
//}
