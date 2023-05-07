# ifndef _MY_DOUBLE_COMPARISONS_
# define _MY_DOUBLE_COMPARISONS_

class Real
{
 private:
    double x;
    double Fabs (void) const;
    void copy (const Real & second){ this->x = second.x; }

public:
   Real (double par) {x=par;}
   Real (const Real & second) {copy(second);}
   Real & operator=(const Real & second) { copy(second); return *this;}
   virtual ~Real (void) {}


   bool operator== (const Real & second) const;
   bool operator<  (const Real & second) const;
   bool operator>  (const Real & second) const;
   bool operator<= (const Real & second) const;
   bool operator>= (const Real & second) const;

   Real  operator- (const Real & second) const;
   Real  operator+ (const Real & second) const;
   Real  operator* (const Real & second) const;
   Real  operator/ (const Real & second) const;

   Real & operator-= (const Real & second);
   Real & operator+= (const Real & second);
   Real & operator*= (const Real & second);
   Real & operator/= (const Real & second);
};

# endif

