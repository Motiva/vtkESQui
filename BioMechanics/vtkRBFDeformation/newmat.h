//$$ newmat.h           definition file for new version of matrix package

// Copyright (C) 2004: R B Davies

#ifndef NEWMAT_LIB
#define NEWMAT_LIB 0

//$$ include.h           include files required by various versions of C++

#ifndef INCLUDE_LIB
#define INCLUDE_LIB

//#define use_namespace                   // define name spaces

//#define SETUP_C_SUBSCRIPTS              // allow element access via A[i][j]

//#define OPT_COMPATIBLE                  // for use with opt++

// Activate just one of the following 3 statements

//#define SimulateExceptions              // use simulated exceptions
#define UseExceptions                   // use C++ exceptions
//#define DisableExceptions               // do not use exceptions


//#define TEMPS_DESTROYED_QUICKLY         // for compilers that delete
// temporaries too quickly

//#define TEMPS_DESTROYED_QUICKLY_R       // the same thing but applied
// to return from functions only

//#define DO_FREE_CHECK                   // check news and deletes balance

#define USING_DOUBLE                    // elements of type double
//#define USING_FLOAT                   // elements of type float

#define bool_LIB 0                      // for compatibility with my older libraries

//#define ios_format_flags ios::fmtflags  // for Gnu 3 and Intel for Linux


//#define _STANDARD_                    // using standard library

//#define use_float_h                   // use float.h for precision data


//#define HAS_INT64                     // if unsigned _int64 is recognised
// used by newran03

// comment out next line if Exception causes a problem
#define TypeDefException

//*********************** end of options set by user ********************


// for Gnu C++ version 3
#if defined __GNUG__ && __GNUG__ >= 3
#define _STANDARD_                   // use standard library
#define ios_format_flags ios::fmtflags
#endif

// for Intel C++ for Linux
#if defined __ICC
#define _STANDARD_                   // use standard library
#define ios_format_flags ios::fmtflags
#endif

// for Microsoft Visual C++ 7 and above (and Intel simulating these)
#if defined _MSC_VER && _MSC_VER >= 1300
#define _STANDARD_                   // use standard library
#endif


#ifdef _STANDARD_                       // using standard library
#include <cstdlib>
#if defined _MSC_VER && _MSC_VER == 1200
#include <limits>              // for VC++6
#endif
#ifdef WANT_STREAM
#include <iostream>
#include <iomanip>
#endif
#ifdef WANT_MATH
#include <cmath>
#endif
#ifdef WANT_STRING
#include <cstring>
#endif
#ifdef WANT_TIME
#include <ctime>
#endif
#ifdef WANT_FSTREAM
#include <fstream>
#endif
using namespace std;
#else

#define DEFAULT_HEADER                  // use AT&T style header
// if no other compiler is recognised

#ifdef _MSC_VER                         // Microsoft
#include <stdlib.h>

//   reactivate these statements to run under MSC version 7.0
//   typedef int jmp_buf[9];
//   extern "C"
//   {
//      int __cdecl setjmp(jmp_buf);
//      void __cdecl longjmp(jmp_buf, int);
//   }

#ifdef WANT_STREAM
#include <iostream.h>
#include <iomanip.h>
#endif
#ifdef WANT_MATH
#include <math.h>
#include <float.h>
#endif
#ifdef WANT_STRING
#include <string.h>
#endif
#ifdef WANT_TIME
#include <time.h>
#endif
#ifdef WANT_FSTREAM
#include <fstream.h>
#endif
#undef DEFAULT_HEADER
#endif

#ifdef __ZTC__                          // Zortech
#include <stdlib.h>
#ifdef WANT_STREAM
#include <iostream.hpp>
#include <iomanip.hpp>
#define flush ""                  // not defined in iomanip?
#endif
#ifdef WANT_MATH
#include <math.h>
#include <float.h>
#endif
#ifdef WANT_STRING
#include <string.h>
#endif
#ifdef WANT_TIME
#include <time.h>
#endif
#ifdef WANT_FSTREAM
#include <fstream.h>
#endif
#undef DEFAULT_HEADER
#endif

#if defined __BCPLUSPLUS__ || defined __TURBOC__  // Borland or Turbo
#include <stdlib.h>
#ifdef WANT_STREAM
#include <iostream.h>
#include <iomanip.h>
#endif
#ifdef WANT_MATH
#include <math.h>
#include <float.h>            // Borland has both float and values
// but values.h returns +INF for
// MAXDOUBLE in BC5
#endif
#ifdef WANT_STRING
#include <string.h>
#endif
#ifdef WANT_TIME
#include <time.h>
#endif
#ifdef WANT_FSTREAM
#include <fstream.h>
#endif
#undef DEFAULT_HEADER
#endif

#ifdef __GNUG__                         // Gnu C++
#include <stdlib.h>
#ifdef WANT_STREAM
#include <iostream.h>
#include <iomanip.h>
#endif
#ifdef WANT_MATH
#include <math.h>
#include <float.h>
#endif
#ifdef WANT_STRING
#include <string.h>
#endif
#ifdef WANT_TIME
#include <time.h>
#endif
#ifdef WANT_FSTREAM
#include <fstream.h>
#endif
#undef DEFAULT_HEADER
#endif

#ifdef __WATCOMC__                      // Watcom C/C++
#include <stdlib.h>
#ifdef WANT_STREAM
#include <iostream.h>
#include <iomanip.h>
#endif
#ifdef WANT_MATH
#include <math.h>
#include <float.h>
#endif
#ifdef WANT_STRING
#include <string.h>
#endif
#ifdef WANT_TIME
#include <time.h>
#endif
#ifdef WANT_FSTREAM
#include <fstream.h>
#endif
#undef DEFAULT_HEADER
#endif


#ifdef macintosh                        // MPW C++ on the Mac
#include <stdlib.h>
#ifdef WANT_STREAM
#include <iostream.h>
#include <iomanip.h>
#endif
#ifdef WANT_MATH
#include <float.h>
#include <math.h>
#endif
#ifdef WANT_STRING
#include <string.h>
#endif
#ifdef WANT_TIME
#include <time.h>
#endif
#ifdef WANT_FSTREAM
#include <fstream.h>
#endif
#undef DEFAULT_HEADER
#endif

#ifdef use_float_h                      // use float.h for precision values
#include <stdlib.h>
#ifdef WANT_STREAM
#include <iostream.h>
#include <iomanip.h>
#endif
#ifdef WANT_MATH
#include <float.h>
#include <math.h>
#endif
#ifdef WANT_STRING
#include <string.h>
#endif
#ifdef WANT_TIME
#include <time.h>
#endif
#ifdef WANT_FSTREAM
#include <fstream.h>
#endif
#undef DEFAULT_HEADER
#endif


#ifdef DEFAULT_HEADER                   // for example AT&T
#define ATandT
#include <stdlib.h>
#ifdef WANT_STREAM
#include <iostream.h>
#include <iomanip.h>
#endif
#ifdef WANT_MATH
#include <math.h>
#define SystemV                         // use System V
#include <values.h>
#endif
#ifdef WANT_STRING
#include <string.h>
#endif
#ifdef WANT_TIME
#include <time.h>
#endif
#ifdef WANT_FSTREAM
#include <fstream.h>
#endif
#endif                                  // DEFAULT_HEADER

#endif                                  // _STANDARD_

#ifdef use_namespace
namespace RBD_COMMON {
#endif


#ifdef USING_FLOAT                      // set precision type to float
  typedef float Real;
  typedef double long_Real;
#endif

#ifdef USING_DOUBLE                     // set precision type to double
  typedef double Real;
  typedef long double long_Real;
#endif


  // This is for (very old) compilers that do not have bool automatically defined

#ifndef bool_LIB
#define bool_LIB 0

  class bool
  {
    int value;
  public:
    bool(const int b) { value = b ? 1 : 0; }
    bool(const void* b) { value = b ? 1 : 0; }
    bool() {}
    operator int() const { return value; }
    int operator!() const { return !value; }
  };


  const bool true = 1;
  const bool false = 0;

#endif


#ifdef use_namespace
}
#endif


#ifdef use_namespace
namespace RBD_COMMON {}
namespace RBD_LIBRARIES                 // access all my libraries
{
  using namespace RBD_COMMON;
}
#endif


#endif

//$$ myexcept.h                                  Exception handling classes


// A set of classes to simulate exceptions in C++
//
//   Partially copied from Carlos Vidal s article in the C users  journal
//   September 1992, pp 19-28
//
//   Operations defined
//      Try {     }
//      Throw ( exception object )
//      ReThrow
//      Catch ( exception class ) {      }
//      CatchAll {      }
//      CatchAndThrow
//
//   All catch lists must end with a CatchAll or CatchAndThrow statement
//   but not both.
//
//   When exceptions are finally implemented replace Try, Throw(E), Rethrow,
//   Catch, CatchAll, CatchAndThrow by try, throw E, throw, catch,
//   catch(...), and {}.
//
//   All exception classes must be derived from BaseException, have no
//   non-static variables and must include the statement
//
//      static unsigned long Select;
//
//   Any constructor in one of these exception classes must include
//
//      Select = BaseException::Select;
//
//   For each exceptions class, EX_1, some .cpp file must include
//
//      unsigned long EX_1::Select;
//


#ifndef EXCEPTION_LIB
#define EXCEPTION_LIB

//#include "include.h"

#ifdef use_namespace
namespace RBD_COMMON {
#endif


  void MatrixTerminate();


  //********** classes for setting up exceptions and reporting ************//

  class BaseException;

  class Tracer                             // linked list showing how
  {                                        // we got here
    const char* entry;
    Tracer* previous;
  public:
    Tracer(const char*);
    ~Tracer();
    void ReName(const char*);
    static void PrintTrace();             // for printing trace
    static void AddTrace();               // insert trace in exception record
    static Tracer* last;                  // points to Tracer list
    friend class BaseException;
  };


  class BaseException                          // The base exception class
  {
  protected:
    static char* what_error;              // error message
    static int SoFar;                     // no. characters already entered
    static int LastOne;                   // last location in error buffer
  public:
    static void AddMessage(const char* a_what);
    // messages about exception
    static void AddInt(int value);        // integer to error message
    static unsigned long Select;          // for identifying exception
    BaseException(const char* a_what = 0);
    static const char* what() { return what_error; }
    // for getting error message
  };

#ifdef TypeDefException
  typedef BaseException Exception;        // for compatibility with my older libraries
#endif

  inline Tracer::Tracer(const char* e)
    : entry(e), previous(last) { last = this; }

  inline Tracer::~Tracer() { last = previous; }

  inline void Tracer::ReName(const char* e) { entry=e; }

#ifdef SimulateExceptions                // SimulateExceptions

#include <setjmp.h>


  //************* the definitions of Try, Throw and Catch *****************//


  class JumpItem;
  class Janitor;

  class JumpBase         // pointer to a linked list of jmp_buf s
  {
  public:
    static JumpItem *jl;
    static jmp_buf env;
  };

  class JumpItem         // an item in a linked list of jmp_buf s
  {
  public:
    JumpItem *ji;
    jmp_buf env;
    Tracer* trace;                     // to keep check on Tracer items
    Janitor* janitor;                  // list of items for cleanup
    JumpItem() : ji(JumpBase::jl), trace(0), janitor(0)
    { JumpBase::jl = this; }
    ~JumpItem() { JumpBase::jl = ji; }
  };

  void Throw();

  inline void Throw(const BaseException&) { Throw(); }

#define Try                                             \
  if (!setjmp( JumpBase::jl->env )) {                  \
  JumpBase::jl->trace = Tracer::last;               \
  JumpItem JI387256156;

#define ReThrow Throw()

#define Catch(EXCEPTION)                                \
  } else if (BaseException::Select == EXCEPTION::Select) {

#define CatchAll } else

#define CatchAndThrow  } else Throw();


  //****************** cleanup heap following Throw ***********************//

  class Janitor
  {
  protected:
    static bool do_not_link;                  // set when new is called
    bool OnStack;                             // false if created by new
  public:
    Janitor* NextJanitor;
    virtual void CleanUp() {}
    Janitor();
    virtual ~Janitor();
  };


  // The tiresome old trick for initializing the Janitor class
  // this is needed for classes derived from Janitor which have objects
  // declared globally

  class JanitorInitializer
  {
  public:
    JanitorInitializer();
  private:
    static int ref_count;
  };

  static JanitorInitializer JanInit;

#endif                                // end of SimulateExceptions

#ifdef UseExceptions

#define Try try
#define Throw(E) throw E
#define ReThrow throw
#define Catch catch
#define CatchAll catch(...)
#define CatchAndThrow {}

#endif                                // end of UseExceptions


#ifdef DisableExceptions              // Disable exceptions

#define Try {
#define ReThrow Throw()
#define Catch(EXCEPTION) } if (false) {
#define CatchAll } if (false)
#define CatchAndThrow }

  inline void Throw() { MatrixTerminate(); }
  inline void Throw(const BaseException&) { MatrixTerminate(); }


#endif                                // end of DisableExceptions

#ifndef SimulateExceptions            // ! SimulateExceptions

  class Janitor                         // a dummy version
  {
  public:
    virtual void CleanUp() {}
    Janitor() {}
    virtual ~Janitor() {}
  };

#endif                                // end of ! SimulateExceptions


  //******************** FREE_CHECK and NEW_DELETE ***********************//

#ifdef DO_FREE_CHECK                          // DO_FREE_CHECK
  // Routines for tracing whether new and delete calls are balanced

  class FreeCheck;

  class FreeCheckLink
  {
  protected:
    FreeCheckLink* next;
    void* ClassStore;
    FreeCheckLink();
    virtual void Report()=0;                   // print details of link
    friend class FreeCheck;
  };

  class FCLClass : public FreeCheckLink         // for registering objects
  {
    char* ClassName;
    FCLClass(void* t, char* name);
    void Report();
    friend class FreeCheck;
  };

  class FCLRealArray : public FreeCheckLink     // for registering real arrays
  {
    char* Operation;
    int size;
    FCLRealArray(void* t, char* o, int s);
    void Report();
    friend class FreeCheck;
  };

  class FCLIntArray : public FreeCheckLink     // for registering int arrays
  {
    char* Operation;
    int size;
    FCLIntArray(void* t, char* o, int s);
    void Report();
    friend class FreeCheck;
  };


  class FreeCheck
  {
    static FreeCheckLink* next;
    static int BadDelete;
  public:
    static void Register(void*, char*);
    static void DeRegister(void*, char*);
    static void RegisterR(void*, char*, int);
    static void DeRegisterR(void*, char*, int);
    static void RegisterI(void*, char*, int);
    static void DeRegisterI(void*, char*, int);
    static void Status();
    friend class FreeCheckLink;
    friend class FCLClass;
    friend class FCLRealArray;
    friend class FCLIntArray;
  };

#define FREE_CHECK(Class)                                                  \
public:                                                                    \
  void* operator new(size_t size)                                         \
  {                                                                       \
  void* t = ::operator new(size); FreeCheck::Register(t,#Class);       \
  return t;                                                            \
  }                                                                       \
  void operator delete(void* t)                                           \
  { FreeCheck::DeRegister(t,#Class); ::operator delete(t); }


#ifdef SimulateExceptions         // SimulateExceptions

#define NEW_DELETE(Class)                                                  \
public:                                                                    \
  void* operator new(size_t size)                                         \
  {                                                                       \
  do_not_link=true;                                                    \
  void* t = ::operator new(size); FreeCheck::Register(t,#Class);       \
  return t;                                                            \
  }                                                                       \
  void operator delete(void* t)                                           \
  { FreeCheck::DeRegister(t,#Class); ::operator delete(t); }


#endif                           // end of SimulateExceptions


#define MONITOR_REAL_NEW(Operation, Size, Pointer)                         \
  FreeCheck::RegisterR(Pointer, Operation, Size);
#define MONITOR_INT_NEW(Operation, Size, Pointer)                          \
  FreeCheck::RegisterI(Pointer, Operation, Size);
#define MONITOR_REAL_DELETE(Operation, Size, Pointer)                      \
  FreeCheck::DeRegisterR(Pointer, Operation, Size);
#define MONITOR_INT_DELETE(Operation, Size, Pointer)                       \
  FreeCheck::DeRegisterI(Pointer, Operation, Size);

#else                            // DO_FREE_CHECK not defined

#define FREE_CHECK(Class) public:
#define MONITOR_REAL_NEW(Operation, Size, Pointer) {}
#define MONITOR_INT_NEW(Operation, Size, Pointer) {}
#define MONITOR_REAL_DELETE(Operation, Size, Pointer) {}
#define MONITOR_INT_DELETE(Operation, Size, Pointer) {}


#ifdef SimulateExceptions         // SimulateExceptions


#define NEW_DELETE(Class)                                                  \
public:                                                                    \
  void* operator new(size_t size)                                    \
  { do_not_link=true; void* t = ::operator new(size); return t; }    \
  void operator delete(void* t) { ::operator delete(t); }

#endif                            // end of SimulateExceptions

#endif                            // end of ! DO_FREE_CHECK

#ifndef SimulateExceptions        // ! SimulateExceptions

#define NEW_DELETE(Class) FREE_CHECK(Class)

#endif                            // end of ! SimulateExceptions


  //********************* derived exceptions ******************************//

  class Logic_error : public BaseException
  {
  public:
    static unsigned long Select;
    Logic_error(const char* a_what = 0);
  };

  class Runtime_error : public BaseException
  {
  public:
    static unsigned long Select;
    Runtime_error(const char* a_what = 0);
  };

  class Domain_error : public Logic_error
  {
  public:
    static unsigned long Select;
    Domain_error(const char* a_what = 0);
  };

  class Invalid_argument : public Logic_error
  {
  public:
    static unsigned long Select;
    Invalid_argument(const char* a_what = 0);
  };

  class Length_error : public Logic_error
  {
  public:
    static unsigned long Select;
    Length_error(const char* a_what = 0);
  };

  class Out_of_range : public Logic_error
  {
  public:
    static unsigned long Select;
    Out_of_range(const char* a_what = 0);
  };

  //class Bad_cast : public Logic_error
  //{
  //public:
  //   static unsigned long Select;
  //   Bad_cast(const char* a_what = 0);
  //};

  //class Bad_typeid : public Logic_error
  //{
  //public:
  //   static unsigned long Select;
  //   Bad_typeid(const char* a_what = 0);
  //};

  class Range_error : public Runtime_error
  {
  public:
    static unsigned long Select;
    Range_error(const char* a_what = 0);
  };

  class Overflow_error : public Runtime_error
  {
  public:
    static unsigned long Select;
    Overflow_error(const char* a_what = 0);
  };

  class Bad_alloc : public BaseException
  {
  public:
    static unsigned long Select;
    Bad_alloc(const char* a_what = 0);
  };

#ifdef use_namespace
}
#endif


#endif                            // end of EXCEPTION_LIB


// body file: myexcept.cpp





#ifdef use_namespace
namespace NEWMAT { using namespace RBD_COMMON; }
namespace RBD_LIBRARIES { using namespace NEWMAT; }
namespace NEWMAT {
#endif

//#define DO_REPORT                     // to activate REPORT

#ifdef NO_LONG_NAMES
#define UpperTriangularMatrix UTMatrix
#define LowerTriangularMatrix LTMatrix
#define SymmetricMatrix SMatrix
#define DiagonalMatrix DMatrix
#define BandMatrix BMatrix
#define UpperBandMatrix UBMatrix
#define LowerBandMatrix LBMatrix
#define SymmetricBandMatrix SBMatrix
#define BandLUMatrix BLUMatrix
#endif

// ************************** general utilities ****************************/

class GeneralMatrix;

void MatrixErrorNoSpace(const void*);                 // no space handler

class LogAndSign
// Return from LogDeterminant function
//    - value of the log plus the sign (+, - or 0)
{
   Real log_val;
   int sign_val;
public:
   LogAndSign() { log_val=0.0; sign_val=1; }
   LogAndSign(Real);
   void operator*=(Real);
   void pow_eq(int k);  // raise to power of k
   void PowEq(int k) { pow_eq(k); }
   void ChangeSign() { sign_val = -sign_val; }
   void change_sign() { sign_val = -sign_val; }
   Real LogValue() const { return log_val; }
   Real log_value() const { return log_val; }
   int Sign() const { return sign_val; }
   int sign() const { return sign_val; }
   Real value() const;
   Real Value() const { return value(); }
   FREE_CHECK(LogAndSign)
};

// the following class is for counting the number of times a piece of code
// is executed. It is used for locating any code not executed by test
// routines. Use turbo GREP locate all places this code is called and
// check which ones are not accessed.
// Somewhat implementation dependent as it relies on "cout" still being
// present when ExeCounter objects are destructed.

#ifdef DO_REPORT

class ExeCounter
{
   int line;                                    // code line number
   int fileid;                                  // file identifier
   long nexe;                                   // number of executions
   static int nreports;                         // number of reports
public:
   ExeCounter(int,int);
   void operator++() { nexe++; }
   ~ExeCounter();                               // prints out reports
};

#endif


// ************************** class MatrixType *****************************/

// Is used for finding the type of a matrix resulting from the binary operations
// +, -, * and identifying what conversions are permissible.
// This class must be updated when new matrix types are added.

class GeneralMatrix;                            // defined later
class BaseMatrix;                               // defined later
class MatrixInput;                              // defined later

class MatrixType
{
public:
   enum Attribute {  Valid     = 1,
                     Diagonal  = 2,             // order of these is important
                     Symmetric = 4,
                     Band      = 8,
                     Lower     = 16,
                     Upper     = 32,
                     Square    = 64,
                     Skew      = 128,
                     LUDeco    = 256,
                     Ones      = 512 };

   enum            { US = 0,
                     UT = Valid + Upper + Square,
                     LT = Valid + Lower + Square,
                     Rt = Valid,
                     Sq = Valid + Square,
                     Sm = Valid + Symmetric + Square,
                     Sk = Valid + Skew + Square,
                     Dg = Valid + Diagonal + Band + Lower + Upper + Symmetric
                        + Square,
                     Id = Valid + Diagonal + Band + Lower + Upper + Symmetric
                        + Square + Ones,
                     RV = Valid,     //   do not separate out
                     CV = Valid,     //   vectors
                     BM = Valid + Band + Square,
                     UB = Valid + Band + Upper + Square,
                     LB = Valid + Band + Lower + Square,
                     SB = Valid + Band + Symmetric + Square,
                     KB = Valid + Band + Skew + Square,
                     Ct = Valid + LUDeco + Square,
                     BC = Valid + Band + LUDeco + Square,
                     Mask = ~Square
                   };


   static int nTypes() { return 13; }          // number of different types
                 // exclude Ct, US, BC
public:
   int attribute;
   bool DataLossOK;                            // true if data loss is OK when
                                               // this represents a destination
public:
   MatrixType () : DataLossOK(false) {}
   MatrixType (int i) : attribute(i), DataLossOK(false) {}
   MatrixType (int i, bool dlok) : attribute(i), DataLossOK(dlok) {}
   MatrixType (const MatrixType& mt)
      : attribute(mt.attribute), DataLossOK(mt.DataLossOK) {}
   void operator=(const MatrixType& mt)
      { attribute = mt.attribute; DataLossOK = mt.DataLossOK; }
   void SetDataLossOK() { DataLossOK = true; }
   int operator+() const { return attribute; }
   MatrixType operator+(MatrixType mt) const
      { return MatrixType(attribute & mt.attribute); }
   MatrixType operator*(const MatrixType&) const;
   MatrixType SP(const MatrixType&) const;
   MatrixType KP(const MatrixType&) const;
   MatrixType operator|(const MatrixType& mt) const
      { return MatrixType(attribute & mt.attribute & Valid); }
   MatrixType operator&(const MatrixType& mt) const
      { return MatrixType(attribute & mt.attribute & Valid); }
   bool operator>=(MatrixType mt) const
      { return ( attribute & ~mt.attribute & Mask ) == 0; }
   bool operator<(MatrixType mt) const         // for MS Visual C++ 4
      { return ( attribute & ~mt.attribute & Mask ) != 0; }
   bool operator==(MatrixType t) const
      { return (attribute == t.attribute); }
   bool operator!=(MatrixType t) const
      { return (attribute != t.attribute); }
   bool operator!() const { return (attribute & Valid) == 0; }
   MatrixType i() const;                       // type of inverse
   MatrixType t() const;                       // type of transpose
   MatrixType AddEqualEl() const               // Add constant to matrix
      { return MatrixType(attribute & (Valid + Symmetric + Square)); }
   MatrixType MultRHS() const;                 // type for rhs of multiply
   MatrixType sub() const                      // type of submatrix
      { return MatrixType(attribute & Valid); }
   MatrixType ssub() const                     // type of sym submatrix
      { return MatrixType(attribute); }        // not for selection matrix
   GeneralMatrix* New() const;                 // new matrix of given type
   GeneralMatrix* New(int,int,BaseMatrix*) const;
                                               // new matrix of given type
   const char* value() const;                  // to print type
   const char* Value() const { return value(); }
   friend bool Rectangular(MatrixType a, MatrixType b, MatrixType c);
   friend bool Compare(const MatrixType&, MatrixType&);
                                               // compare and check conv.
   bool is_band() const { return (attribute & Band) != 0; }
   bool is_diagonal() const { return (attribute & Diagonal) != 0; }
   bool is_symmetric() const { return (attribute & Symmetric) != 0; }
   bool CannotConvert() const { return (attribute & LUDeco) != 0; }
                                               // used by operator== 
   FREE_CHECK(MatrixType)
};


// *********************** class MatrixBandWidth ***********************/

class MatrixBandWidth
{
public:
   int lower_val;
   int upper_val;
   MatrixBandWidth(const int l, const int u) : lower_val(l), upper_val(u) {}
   MatrixBandWidth(const int i) : lower_val(i), upper_val(i) {}
   MatrixBandWidth operator+(const MatrixBandWidth&) const;
   MatrixBandWidth operator*(const MatrixBandWidth&) const;
   MatrixBandWidth minimum(const MatrixBandWidth&) const;
   MatrixBandWidth t() const { return MatrixBandWidth(upper_val,lower_val); }
   bool operator==(const MatrixBandWidth& bw) const
      { return (lower_val == bw.lower_val) && (upper_val == bw.upper_val); }
   bool operator!=(const MatrixBandWidth& bw) const { return !operator==(bw); }
   int Upper() const { return upper_val; }
   int upper() const { return upper_val; }
   int Lower() const { return lower_val; }
   int lower() const { return lower_val; }
   FREE_CHECK(MatrixBandWidth)
};


// ********************* Array length specifier ************************/

// This class is introduced to avoid constructors such as
//   ColumnVector(int)
// being used for conversions

class ArrayLengthSpecifier
{
   int v;
public:
   int Value() const { return v; }
   int value() const { return v; }
   ArrayLengthSpecifier(int l) : v(l) {}
};

// ************************* Matrix routines ***************************/


class MatrixRowCol;                             // defined later
class MatrixRow;
class MatrixCol;
class MatrixColX;

class GeneralMatrix;                            // defined later
class AddedMatrix;
class MultipliedMatrix;
class SubtractedMatrix;
class SPMatrix;
class KPMatrix;
class ConcatenatedMatrix;
class StackedMatrix;
class SolvedMatrix;
class ShiftedMatrix;
class NegShiftedMatrix;
class ScaledMatrix;
class TransposedMatrix;
class ReversedMatrix;
class NegatedMatrix;
class InvertedMatrix;
class RowedMatrix;
class ColedMatrix;
class DiagedMatrix;
class MatedMatrix;
class GetSubMatrix;
class ReturnMatrix;
class Matrix;
class SquareMatrix;
class nricMatrix;
class RowVector;
class ColumnVector;
class SymmetricMatrix;
class UpperTriangularMatrix;
class LowerTriangularMatrix;
class DiagonalMatrix;
class CroutMatrix;
class BandMatrix;
class LowerBandMatrix;
class UpperBandMatrix;
class SymmetricBandMatrix;
class LinearEquationSolver;
class GenericMatrix;


#define MatrixTypeUnSp 0
//static MatrixType MatrixTypeUnSp(MatrixType::US);
//            // AT&T needs this

class BaseMatrix : public Janitor               // base of all matrix classes
{
protected:
   virtual int search(const BaseMatrix*) const = 0;
            // count number of times matrix
               // is referred to

public:
   virtual GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp) = 0;
            // evaluate temporary
   // for old version of G++
   //   virtual GeneralMatrix* Evaluate(MatrixType mt) = 0;
   //   GeneralMatrix* Evaluate() { return Evaluate(MatrixTypeUnSp); }
   AddedMatrix operator+(const BaseMatrix&) const;    // results of operations
   MultipliedMatrix operator*(const BaseMatrix&) const;
   SubtractedMatrix operator-(const BaseMatrix&) const;
   ConcatenatedMatrix operator|(const BaseMatrix&) const;
   StackedMatrix operator&(const BaseMatrix&) const;
   ShiftedMatrix operator+(Real) const;
   ScaledMatrix operator*(Real) const;
   ScaledMatrix operator/(Real) const;
   ShiftedMatrix operator-(Real) const;
   TransposedMatrix t() const;
//   TransposedMatrix t;
   NegatedMatrix operator-() const;                   // change sign of elements
   ReversedMatrix reverse() const;
   ReversedMatrix Reverse() const;
   InvertedMatrix i() const;
//   InvertedMatrix i;
   RowedMatrix as_row() const;
   RowedMatrix AsRow() const;
   ColedMatrix as_column() const;
   ColedMatrix AsColumn() const;
   DiagedMatrix as_diagonal() const;
   DiagedMatrix AsDiagonal() const;
   MatedMatrix as_matrix(int,int) const;
   MatedMatrix AsMatrix(int m, int n) const;
   GetSubMatrix submatrix(int,int,int,int) const;
   GetSubMatrix SubMatrix(int fr, int lr, int fc, int lc) const;
   GetSubMatrix sym_submatrix(int,int) const;
   GetSubMatrix SymSubMatrix(int f, int l) const;
   GetSubMatrix row(int) const;
   GetSubMatrix rows(int,int) const;
   GetSubMatrix column(int) const;
   GetSubMatrix columns(int,int) const;
   GetSubMatrix Row(int f) const;
   GetSubMatrix Rows(int f, int l) const;
   GetSubMatrix Column(int f) const;
   GetSubMatrix Columns(int f, int l) const;
   Real as_scalar() const;                      // conversion of 1 x 1 matrix
   Real AsScalar() const;
   virtual LogAndSign log_determinant() const;
   LogAndSign LogDeterminant() const { return log_determinant(); }
   Real determinant() const;
   Real Determinant() const { return determinant(); }
   virtual Real sum_square() const;
   Real SumSquare() const { return sum_square(); }
   Real norm_Frobenius() const;
   Real norm_frobenius() const { return norm_Frobenius(); }
   Real NormFrobenius() const { return norm_Frobenius(); }
   virtual Real sum_absolute_value() const;
   Real SumAbsoluteValue() const { return sum_absolute_value(); }
   virtual Real sum() const;
   virtual Real Sum() const { return sum(); }
   virtual Real maximum_absolute_value() const;
   Real MaximumAbsoluteValue() const { return maximum_absolute_value(); }
   virtual Real maximum_absolute_value1(int& i) const;
   Real MaximumAbsoluteValue1(int& i) const
      { return maximum_absolute_value1(i); }
   virtual Real maximum_absolute_value2(int& i, int& j) const;
   Real MaximumAbsoluteValue2(int& i, int& j) const
      { return maximum_absolute_value2(i,j); }
   virtual Real minimum_absolute_value() const;
   Real MinimumAbsoluteValue() const { return minimum_absolute_value(); }
   virtual Real minimum_absolute_value1(int& i) const;
   Real MinimumAbsoluteValue1(int& i) const
      { return minimum_absolute_value1(i); }
   virtual Real minimum_absolute_value2(int& i, int& j) const;
   Real MinimumAbsoluteValue2(int& i, int& j) const
      { return minimum_absolute_value2(i,j); }
   virtual Real maximum() const;
   Real Maximum() const { return maximum(); }
   virtual Real maximum1(int& i) const;
   Real Maximum1(int& i) const { return maximum1(i); }
   virtual Real maximum2(int& i, int& j) const;
   Real Maximum2(int& i, int& j) const { return maximum2(i,j); }
   virtual Real minimum() const;
   Real Minimum() const { return minimum(); }
   virtual Real minimum1(int& i) const;
   Real Minimum1(int& i) const { return minimum1(i); }
   virtual Real minimum2(int& i, int& j) const;
   Real Minimum2(int& i, int& j) const { return minimum2(i,j); }
   virtual Real trace() const;
   Real Trace() const { return trace(); }
   Real norm1() const;
   Real Norm1() const { return norm1(); }
   Real norm_infinity() const;
   Real NormInfinity() const { return norm_infinity(); }
   virtual MatrixBandWidth bandwidth() const;  // bandwidths of band matrix
   virtual MatrixBandWidth BandWidth() const { return bandwidth(); }
   void IEQND() const;                         // called by ineq. ops
   ReturnMatrix sum_square_columns() const;
   ReturnMatrix sum_square_rows() const;
   ReturnMatrix sum_columns() const;
   ReturnMatrix sum_rows() const;
   virtual void cleanup() {}
   void CleanUp() { cleanup(); }

//   virtual ReturnMatrix Reverse() const;       // reverse order of elements
//protected:
//   BaseMatrix() : t(this), i(this) {}

   friend class GeneralMatrix;
   friend class Matrix;
   friend class SquareMatrix;
   friend class nricMatrix;
   friend class RowVector;
   friend class ColumnVector;
   friend class SymmetricMatrix;
   friend class UpperTriangularMatrix;
   friend class LowerTriangularMatrix;
   friend class DiagonalMatrix;
   friend class CroutMatrix;
   friend class BandMatrix;
   friend class LowerBandMatrix;
   friend class UpperBandMatrix;
   friend class SymmetricBandMatrix;
   friend class AddedMatrix;
   friend class MultipliedMatrix;
   friend class SubtractedMatrix;
   friend class SPMatrix;
   friend class KPMatrix;
   friend class ConcatenatedMatrix;
   friend class StackedMatrix;
   friend class SolvedMatrix;
   friend class ShiftedMatrix;
   friend class NegShiftedMatrix;
   friend class ScaledMatrix;
   friend class TransposedMatrix;
   friend class ReversedMatrix;
   friend class NegatedMatrix;
   friend class InvertedMatrix;
   friend class RowedMatrix;
   friend class ColedMatrix;
   friend class DiagedMatrix;
   friend class MatedMatrix;
   friend class GetSubMatrix;
   friend class ReturnMatrix;
   friend class LinearEquationSolver;
   friend class GenericMatrix;
   NEW_DELETE(BaseMatrix)
};


// ***************************** working classes **************************/

class GeneralMatrix : public BaseMatrix         // declarable matrix types
{
   virtual GeneralMatrix* Image() const;        // copy of matrix
protected:
   int tag_val;                               // shows whether can reuse
   int nrows_val, ncols_val;                    // dimensions
   int storage;                                 // total store required
   Real* store;                                 // point to store (0=not set)
   GeneralMatrix();                             // initialise with no store
   GeneralMatrix(ArrayLengthSpecifier);         // constructor getting store
   void Add(GeneralMatrix*, Real);              // sum of GM and Real
   void Add(Real);                              // add Real to this
   void NegAdd(GeneralMatrix*, Real);           // Real - GM
   void NegAdd(Real);                           // this = this - Real
   void Multiply(GeneralMatrix*, Real);         // product of GM and Real
   void Multiply(Real);                         // multiply this by Real
   void Negate(GeneralMatrix*);                 // change sign
   void Negate();                               // change sign
   void ReverseElements();                      // internal reverse of elements
   void ReverseElements(GeneralMatrix*);        // reverse order of elements
   void operator=(Real);                        // set matrix to constant
   Real* GetStore();                            // get store or copy
   GeneralMatrix* BorrowStore(GeneralMatrix*, MatrixType);
                                                // temporarily access store
   void GetMatrix(const GeneralMatrix*);        // used by = and initialise
   void Eq(const BaseMatrix&, MatrixType);      // used by =
   void Eq(const GeneralMatrix&);               // version with no conversion
   void Eq(const BaseMatrix&, MatrixType, bool);// used by <<
   void Eq2(const BaseMatrix&, MatrixType);     // cut down version of Eq
   int search(const BaseMatrix*) const;
   virtual GeneralMatrix* Transpose(TransposedMatrix*, MatrixType);
   void CheckConversion(const BaseMatrix&);     // check conversion OK
   void resize(int, int, int);                  // change dimensions
   virtual short SimpleAddOK(const GeneralMatrix* gm) { return 0; }
             // see bandmat.cpp for explanation
   virtual void MiniCleanUp()
      { store = 0; storage = 0; nrows_val = 0; ncols_val = 0; tag_val = -1;}
             // CleanUp when the data array has already been deleted
   void PlusEqual(const GeneralMatrix& gm);
   void MinusEqual(const GeneralMatrix& gm);
   void PlusEqual(Real f);
   void MinusEqual(Real f);
   void swap(GeneralMatrix& gm);                // swap values
public:
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   virtual MatrixType type() const = 0;         // type of a matrix
   MatrixType Type() const { return type(); }
   int Nrows() const { return nrows_val; }      // get dimensions
   int Ncols() const { return ncols_val; }
   int Storage() const { return storage; }
   Real* Store() const { return store; }
   // updated names
   int nrows() const { return nrows_val; }      // get dimensions
   int ncols() const { return ncols_val; }
   int size() const { return storage; }
   Real* data() { return store; }
   const Real* data() const { return store; }
   const Real* const_data() const { return store; }
   virtual ~GeneralMatrix();                    // delete store if set
   void tDelete();                              // delete if tag_val permits
   bool reuse();                                // true if tag_val allows reuse
   void protect() { tag_val=-1; }               // cannot delete or reuse
   void Protect() { tag_val=-1; }               // cannot delete or reuse
   int tag() const { return tag_val; }
   int Tag() const { return tag_val; }
   bool is_zero() const;                        // test matrix has all zeros
   bool IsZero() const { return is_zero(); }    // test matrix has all zeros
   void Release() { tag_val=1; }                // del store after next use
   void Release(int t) { tag_val=t; }           // del store after t accesses
   void ReleaseAndDelete() { tag_val=0; }       // delete matrix after use
   void release() { tag_val=1; }                // del store after next use
   void release(int t) { tag_val=t; }           // del store after t accesses
   void release_and_delete() { tag_val=0; }     // delete matrix after use
   void operator<<(const double*);              // assignment from an array
   void operator<<(const float*);               // assignment from an array
   void operator<<(const int*);                 // assignment from an array
   void operator<<(const BaseMatrix& X)
      { Eq(X,this->type(),true); }              // = without checking type
   void inject(const GeneralMatrix&);           // copy stored els only
   void Inject(const GeneralMatrix& GM) { inject(GM); }
   void operator+=(const BaseMatrix&);
   void operator-=(const BaseMatrix&);
   void operator*=(const BaseMatrix&);
   void operator|=(const BaseMatrix&);
   void operator&=(const BaseMatrix&);
   void operator+=(Real);
   void operator-=(Real r) { operator+=(-r); }
   void operator*=(Real);
   void operator/=(Real r) { operator*=(1.0/r); }
   virtual GeneralMatrix* MakeSolver();         // for solving
   virtual void Solver(MatrixColX&, const MatrixColX&) {}
   virtual void GetRow(MatrixRowCol&) = 0;      // Get matrix row
   virtual void RestoreRow(MatrixRowCol&) {}    // Restore matrix row
   virtual void NextRow(MatrixRowCol&);         // Go to next row
   virtual void GetCol(MatrixRowCol&) = 0;      // Get matrix col
   virtual void GetCol(MatrixColX&) = 0;        // Get matrix col
   virtual void RestoreCol(MatrixRowCol&) {}    // Restore matrix col
   virtual void RestoreCol(MatrixColX&) {}      // Restore matrix col
   virtual void NextCol(MatrixRowCol&);         // Go to next col
   virtual void NextCol(MatrixColX&);           // Go to next col
   Real sum_square() const;
   Real sum_absolute_value() const;
   Real sum() const;
   Real maximum_absolute_value1(int& i) const;
   Real minimum_absolute_value1(int& i) const;
   Real maximum1(int& i) const;
   Real minimum1(int& i) const;
   Real maximum_absolute_value() const;
   Real maximum_absolute_value2(int& i, int& j) const;
   Real minimum_absolute_value() const;
   Real minimum_absolute_value2(int& i, int& j) const;
   Real maximum() const;
   Real maximum2(int& i, int& j) const;
   Real minimum() const;
   Real minimum2(int& i, int& j) const;
   LogAndSign log_determinant() const;
   virtual bool IsEqual(const GeneralMatrix&) const;
                                                // same type, same values
   void CheckStore() const;                     // check store is non-zero
   virtual void SetParameters(const GeneralMatrix*) {}
                                                // set parameters in GetMatrix
   operator ReturnMatrix() const;               // for building a ReturnMatrix
   ReturnMatrix for_return() const;
   ReturnMatrix ForReturn() const;
   //virtual bool SameStorageType(const GeneralMatrix& A) const;
   //virtual void ReSizeForAdd(const GeneralMatrix& A, const GeneralMatrix& B);
   //virtual void ReSizeForSP(const GeneralMatrix& A, const GeneralMatrix& B);
   virtual void resize(const GeneralMatrix& A);
   virtual void ReSize(const GeneralMatrix& A) { resize(A); }
   MatrixInput operator<<(double);                // for loading a list
   MatrixInput operator<<(float);                // for loading a list
   MatrixInput operator<<(int f);
//   ReturnMatrix Reverse() const;                // reverse order of elements
   void cleanup();                              // to clear store

   friend class Matrix;
   friend class SquareMatrix;
   friend class nricMatrix;
   friend class SymmetricMatrix;
   friend class UpperTriangularMatrix;
   friend class LowerTriangularMatrix;
   friend class DiagonalMatrix;
   friend class CroutMatrix;
   friend class RowVector;
   friend class ColumnVector;
   friend class BandMatrix;
   friend class LowerBandMatrix;
   friend class UpperBandMatrix;
   friend class SymmetricBandMatrix;
   friend class BaseMatrix;
   friend class AddedMatrix;
   friend class MultipliedMatrix;
   friend class SubtractedMatrix;
   friend class SPMatrix;
   friend class KPMatrix;
   friend class ConcatenatedMatrix;
   friend class StackedMatrix;
   friend class SolvedMatrix;
   friend class ShiftedMatrix;
   friend class NegShiftedMatrix;
   friend class ScaledMatrix;
   friend class TransposedMatrix;
   friend class ReversedMatrix;
   friend class NegatedMatrix;
   friend class InvertedMatrix;
   friend class RowedMatrix;
   friend class ColedMatrix;
   friend class DiagedMatrix;
   friend class MatedMatrix;
   friend class GetSubMatrix;
   friend class ReturnMatrix;
   friend class LinearEquationSolver;
   friend class GenericMatrix;
   NEW_DELETE(GeneralMatrix)
};



class Matrix : public GeneralMatrix             // usual rectangular matrix
{
   GeneralMatrix* Image() const;                // copy of matrix
public:
   Matrix() {}
   ~Matrix() {}
   Matrix(int, int);                            // standard declaration
   Matrix(const BaseMatrix&);                   // evaluate BaseMatrix
   void operator=(const BaseMatrix&);
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   void operator=(const Matrix& m) { Eq(m); }
   MatrixType type() const;
   Real& operator()(int, int);                  // access element
   Real& element(int, int);                     // access element
   Real operator()(int, int) const;            // access element
   Real element(int, int) const;               // access element
#ifdef SETUP_C_SUBSCRIPTS
   Real* operator[](int m) { return store+m*ncols_val; }
   const Real* operator[](int m) const { return store+m*ncols_val; }
   // following for Numerical Recipes in C++
   Matrix(Real, int, int);
   Matrix(const Real*, int, int);
#endif
   Matrix(const Matrix& gm) { GetMatrix(&gm); }
   GeneralMatrix* MakeSolver();
   Real trace() const;
   void GetRow(MatrixRowCol&);
   void GetCol(MatrixRowCol&);
   void GetCol(MatrixColX&);
   void RestoreCol(MatrixRowCol&);
   void RestoreCol(MatrixColX&);
   void NextRow(MatrixRowCol&);
   void NextCol(MatrixRowCol&);
   void NextCol(MatrixColX&);
   virtual void resize(int,int);           // change dimensions
      // virtual so we will catch it being used in a vector called as a matrix
   virtual void resize_keep(int,int);
   virtual void ReSize(int m, int n) { resize(m, n); }
   void resize(const GeneralMatrix& A);
   void ReSize(const GeneralMatrix& A) { resize(A); }
   Real maximum_absolute_value2(int& i, int& j) const;
   Real minimum_absolute_value2(int& i, int& j) const;
   Real maximum2(int& i, int& j) const;
   Real minimum2(int& i, int& j) const;
   void operator+=(const Matrix& M) { PlusEqual(M); }
   void operator-=(const Matrix& M) { MinusEqual(M); }
   void operator+=(Real f) { GeneralMatrix::Add(f); }
   void operator-=(Real f) { GeneralMatrix::Add(-f); }
   void swap(Matrix& gm) { GeneralMatrix::swap((GeneralMatrix&)gm); }
   friend Real dotproduct(const Matrix& A, const Matrix& B);
   NEW_DELETE(Matrix)
};

class SquareMatrix : public Matrix              // square matrix
{
   GeneralMatrix* Image() const;                // copy of matrix
public:
   SquareMatrix() {}
   ~SquareMatrix() {}
   SquareMatrix(ArrayLengthSpecifier);          // standard declaration
   SquareMatrix(const BaseMatrix&);             // evaluate BaseMatrix
   void operator=(const BaseMatrix&);
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   void operator=(const SquareMatrix& m) { Eq(m); }
   void operator=(const Matrix& m);
   MatrixType type() const;
   SquareMatrix(const SquareMatrix& gm) { GetMatrix(&gm); }
   SquareMatrix(const Matrix& gm);
   void resize(int);                            // change dimensions
   void ReSize(int m) { resize(m); }
   void resize_keep(int);
   void resize_keep(int,int);
   void resize(int,int);                        // change dimensions
   void ReSize(int m, int n) { resize(m, n); }
   void resize(const GeneralMatrix& A);
   void ReSize(const GeneralMatrix& A) { resize(A); }
   void operator+=(const Matrix& M) { PlusEqual(M); }
   void operator-=(const Matrix& M) { MinusEqual(M); }
   void operator+=(Real f) { GeneralMatrix::Add(f); }
   void operator-=(Real f) { GeneralMatrix::Add(-f); }
   void swap(SquareMatrix& gm) { GeneralMatrix::swap((GeneralMatrix&)gm); }
   NEW_DELETE(SquareMatrix)
};

class nricMatrix : public Matrix                // for use with Numerical
                                                // Recipes in C
{
   GeneralMatrix* Image() const;                // copy of matrix
   Real** row_pointer;                          // points to rows
   void MakeRowPointer();                       // build rowpointer
   void DeleteRowPointer();
public:
   nricMatrix() {}
   nricMatrix(int m, int n)                     // standard declaration
      :  Matrix(m,n) { MakeRowPointer(); }
   nricMatrix(const BaseMatrix& bm)             // evaluate BaseMatrix
      :  Matrix(bm) { MakeRowPointer(); }
   void operator=(const BaseMatrix& bm)
      { DeleteRowPointer(); Matrix::operator=(bm); MakeRowPointer(); }
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   void operator=(const nricMatrix& m)
      { DeleteRowPointer(); Eq(m); MakeRowPointer(); }
   void operator<<(const BaseMatrix& X)
      { DeleteRowPointer(); Eq(X,this->type(),true); MakeRowPointer(); }
   nricMatrix(const nricMatrix& gm) { GetMatrix(&gm); MakeRowPointer(); }
   void resize(int m, int n)               // change dimensions
      { DeleteRowPointer(); Matrix::resize(m,n); MakeRowPointer(); }
   void resize_keep(int m, int n)               // change dimensions
      { DeleteRowPointer(); Matrix::resize_keep(m,n); MakeRowPointer(); }
   void ReSize(int m, int n)               // change dimensions
      { DeleteRowPointer(); Matrix::resize(m,n); MakeRowPointer(); }
   void resize(const GeneralMatrix& A);
   void ReSize(const GeneralMatrix& A) { resize(A); }
   ~nricMatrix() { DeleteRowPointer(); }
   Real** nric() const { CheckStore(); return row_pointer-1; }
   void cleanup();                                // to clear store
   void MiniCleanUp();
   void operator+=(const Matrix& M) { PlusEqual(M); }
   void operator-=(const Matrix& M) { MinusEqual(M); }
   void operator+=(Real f) { GeneralMatrix::Add(f); }
   void operator-=(Real f) { GeneralMatrix::Add(-f); }
   void swap(nricMatrix& gm);
   NEW_DELETE(nricMatrix)
};

class SymmetricMatrix : public GeneralMatrix
{
   GeneralMatrix* Image() const;                // copy of matrix
public:
   SymmetricMatrix() {}
   ~SymmetricMatrix() {}
   SymmetricMatrix(ArrayLengthSpecifier);
   SymmetricMatrix(const BaseMatrix&);
   void operator=(const BaseMatrix&);
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   void operator=(const SymmetricMatrix& m) { Eq(m); }
   Real& operator()(int, int);                  // access element
   Real& element(int, int);                     // access element
   Real operator()(int, int) const;             // access element
   Real element(int, int) const;                // access element
#ifdef SETUP_C_SUBSCRIPTS
   Real* operator[](int m) { return store+(m*(m+1))/2; }
   const Real* operator[](int m) const { return store+(m*(m+1))/2; }
#endif
   MatrixType type() const;
   SymmetricMatrix(const SymmetricMatrix& gm) { GetMatrix(&gm); }
   Real sum_square() const;
   Real sum_absolute_value() const;
   Real sum() const;
   Real trace() const;
   void GetRow(MatrixRowCol&);
   void GetCol(MatrixRowCol&);
   void GetCol(MatrixColX&);
   void RestoreCol(MatrixRowCol&) {}
   void RestoreCol(MatrixColX&);
   GeneralMatrix* Transpose(TransposedMatrix*, MatrixType);
   void resize(int);                           // change dimensions
   void ReSize(int m) { resize(m); }
   void resize_keep(int);
   void resize(const GeneralMatrix& A);
   void ReSize(const GeneralMatrix& A) { resize(A); }
   void operator+=(const SymmetricMatrix& M) { PlusEqual(M); }
   void operator-=(const SymmetricMatrix& M) { MinusEqual(M); }
   void operator+=(Real f) { GeneralMatrix::Add(f); }
   void operator-=(Real f) { GeneralMatrix::Add(-f); }
   void swap(SymmetricMatrix& gm) { GeneralMatrix::swap((GeneralMatrix&)gm); }
   NEW_DELETE(SymmetricMatrix)
};

class UpperTriangularMatrix : public GeneralMatrix
{
   GeneralMatrix* Image() const;                // copy of matrix
public:
   UpperTriangularMatrix() {}
   ~UpperTriangularMatrix() {}
   UpperTriangularMatrix(ArrayLengthSpecifier);
   void operator=(const BaseMatrix&);
   void operator=(const UpperTriangularMatrix& m) { Eq(m); }
   UpperTriangularMatrix(const BaseMatrix&);
   UpperTriangularMatrix(const UpperTriangularMatrix& gm) { GetMatrix(&gm); }
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   Real& operator()(int, int);                  // access element
   Real& element(int, int);                     // access element
   Real operator()(int, int) const;             // access element
   Real element(int, int) const;                // access element
#ifdef SETUP_C_SUBSCRIPTS
   Real* operator[](int m) { return store+m*ncols_val-(m*(m+1))/2; }
   const Real* operator[](int m) const
      { return store+m*ncols_val-(m*(m+1))/2; }
#endif
   MatrixType type() const;
   GeneralMatrix* MakeSolver() { return this; } // for solving
   void Solver(MatrixColX&, const MatrixColX&);
   LogAndSign log_determinant() const;
   Real trace() const;
   void GetRow(MatrixRowCol&);
   void GetCol(MatrixRowCol&);
   void GetCol(MatrixColX&);
   void RestoreCol(MatrixRowCol&);
   void RestoreCol(MatrixColX& c) { RestoreCol((MatrixRowCol&)c); }
   void NextRow(MatrixRowCol&);
   void resize(int);                       // change dimensions
   void ReSize(int m) { resize(m); }
   void resize(const GeneralMatrix& A);
   void ReSize(const GeneralMatrix& A) { resize(A); }
   void resize_keep(int);
   MatrixBandWidth bandwidth() const;
   void operator+=(const UpperTriangularMatrix& M) { PlusEqual(M); }
   void operator-=(const UpperTriangularMatrix& M) { MinusEqual(M); }
   void operator+=(Real f) { GeneralMatrix::operator+=(f); }
   void operator-=(Real f) { GeneralMatrix::operator-=(f); }
   void swap(UpperTriangularMatrix& gm)
      { GeneralMatrix::swap((GeneralMatrix&)gm); }
   NEW_DELETE(UpperTriangularMatrix)
};

class LowerTriangularMatrix : public GeneralMatrix
{
   GeneralMatrix* Image() const;                // copy of matrix
public:
   LowerTriangularMatrix() {}
   ~LowerTriangularMatrix() {}
   LowerTriangularMatrix(ArrayLengthSpecifier);
   LowerTriangularMatrix(const LowerTriangularMatrix& gm) { GetMatrix(&gm); }
   LowerTriangularMatrix(const BaseMatrix& M);
   void operator=(const BaseMatrix&);
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   void operator=(const LowerTriangularMatrix& m) { Eq(m); }
   Real& operator()(int, int);                  // access element
   Real& element(int, int);                     // access element
   Real operator()(int, int) const;             // access element
   Real element(int, int) const;                // access element
#ifdef SETUP_C_SUBSCRIPTS
   Real* operator[](int m) { return store+(m*(m+1))/2; }
   const Real* operator[](int m) const { return store+(m*(m+1))/2; }
#endif
   MatrixType type() const;
   GeneralMatrix* MakeSolver() { return this; } // for solving
   void Solver(MatrixColX&, const MatrixColX&);
   LogAndSign log_determinant() const;
   Real trace() const;
   void GetRow(MatrixRowCol&);
   void GetCol(MatrixRowCol&);
   void GetCol(MatrixColX&);
   void RestoreCol(MatrixRowCol&);
   void RestoreCol(MatrixColX& c) { RestoreCol((MatrixRowCol&)c); }
   void NextRow(MatrixRowCol&);
   void resize(int);                       // change dimensions
   void ReSize(int m) { resize(m); }
   void resize_keep(int);
   void resize(const GeneralMatrix& A);
   void ReSize(const GeneralMatrix& A) { resize(A); }
   MatrixBandWidth bandwidth() const;
   void operator+=(const LowerTriangularMatrix& M) { PlusEqual(M); }
   void operator-=(const LowerTriangularMatrix& M) { MinusEqual(M); }
   void operator+=(Real f) { GeneralMatrix::operator+=(f); }
   void operator-=(Real f) { GeneralMatrix::operator-=(f); }
   void swap(LowerTriangularMatrix& gm)
      { GeneralMatrix::swap((GeneralMatrix&)gm); }
   NEW_DELETE(LowerTriangularMatrix)
};

class DiagonalMatrix : public GeneralMatrix
{
   GeneralMatrix* Image() const;                // copy of matrix
public:
   DiagonalMatrix() {}
   ~DiagonalMatrix() {}
   DiagonalMatrix(ArrayLengthSpecifier);
   DiagonalMatrix(const BaseMatrix&);
   DiagonalMatrix(const DiagonalMatrix& gm) { GetMatrix(&gm); }
   void operator=(const BaseMatrix&);
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   void operator=(const DiagonalMatrix& m) { Eq(m); }
   Real& operator()(int, int);                  // access element
   Real& operator()(int);                       // access element
   Real operator()(int, int) const;             // access element
   Real operator()(int) const;
   Real& element(int, int);                     // access element
   Real& element(int);                          // access element
   Real element(int, int) const;                // access element
   Real element(int) const;                     // access element
#ifdef SETUP_C_SUBSCRIPTS
   Real& operator[](int m) { return store[m]; }
   const Real& operator[](int m) const { return store[m]; }
#endif
   MatrixType type() const;

   LogAndSign log_determinant() const;
   Real trace() const;
   void GetRow(MatrixRowCol&);
   void GetCol(MatrixRowCol&);
   void GetCol(MatrixColX&);
   void NextRow(MatrixRowCol&);
   void NextCol(MatrixRowCol&);
   void NextCol(MatrixColX&);
   GeneralMatrix* MakeSolver() { return this; } // for solving
   void Solver(MatrixColX&, const MatrixColX&);
   GeneralMatrix* Transpose(TransposedMatrix*, MatrixType);
   void resize(int);                       // change dimensions
   void ReSize(int m) { resize(m); }
   void resize_keep(int);
   void resize(const GeneralMatrix& A);
   void ReSize(const GeneralMatrix& A) { resize(A); }
   Real* nric() const
      { CheckStore(); return store-1; }         // for use by NRIC
   MatrixBandWidth bandwidth() const;
//   ReturnMatrix Reverse() const;                // reverse order of elements
   void operator+=(const DiagonalMatrix& M) { PlusEqual(M); }
   void operator-=(const DiagonalMatrix& M) { MinusEqual(M); }
   void operator+=(Real f) { GeneralMatrix::operator+=(f); }
   void operator-=(Real f) { GeneralMatrix::operator-=(f); }
   void swap(DiagonalMatrix& gm)
      { GeneralMatrix::swap((GeneralMatrix&)gm); }
   NEW_DELETE(DiagonalMatrix)
};

class RowVector : public Matrix
{
   GeneralMatrix* Image() const;                // copy of matrix
public:
   RowVector() { nrows_val = 1; }
   ~RowVector() {}
   RowVector(ArrayLengthSpecifier n) : Matrix(1,n.Value()) {}
   RowVector(const BaseMatrix&);
   RowVector(const RowVector& gm) { GetMatrix(&gm); }
   void operator=(const BaseMatrix&);
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   void operator=(const RowVector& m) { Eq(m); }
   Real& operator()(int);                       // access element
   Real& element(int);                          // access element
   Real operator()(int) const;                  // access element
   Real element(int) const;                     // access element
#ifdef SETUP_C_SUBSCRIPTS
   Real& operator[](int m) { return store[m]; }
   const Real& operator[](int m) const { return store[m]; }
   // following for Numerical Recipes in C++
   RowVector(Real a, int n) : Matrix(a, 1, n) {}
   RowVector(const Real* a, int n) : Matrix(a, 1, n) {}
#endif
   MatrixType type() const;
   void GetCol(MatrixRowCol&);
   void GetCol(MatrixColX&);
   void NextCol(MatrixRowCol&);
   void NextCol(MatrixColX&);
   void RestoreCol(MatrixRowCol&) {}
   void RestoreCol(MatrixColX& c);
   GeneralMatrix* Transpose(TransposedMatrix*, MatrixType);
   void resize(int);                       // change dimensions
   void ReSize(int m) { resize(m); }
   void resize_keep(int);
   void resize_keep(int,int);
   void resize(int,int);                   // in case access is matrix
   void ReSize(int m,int n) { resize(m, n); }
   void resize(const GeneralMatrix& A);
   void ReSize(const GeneralMatrix& A) { resize(A); }
   Real* nric() const
      { CheckStore(); return store-1; }         // for use by NRIC
   void cleanup();                              // to clear store
   void MiniCleanUp()
      { store = 0; storage = 0; nrows_val = 1; ncols_val = 0; tag_val = -1; }
   // friend ReturnMatrix GetMatrixRow(Matrix& A, int row);
   void operator+=(const Matrix& M) { PlusEqual(M); }
   void operator-=(const Matrix& M) { MinusEqual(M); }
   void operator+=(Real f) { GeneralMatrix::Add(f); }
   void operator-=(Real f) { GeneralMatrix::Add(-f); }
   void swap(RowVector& gm)
      { GeneralMatrix::swap((GeneralMatrix&)gm); }
   NEW_DELETE(RowVector)
};

class ColumnVector : public Matrix
{
   GeneralMatrix* Image() const;                // copy of matrix
public:
   ColumnVector() { ncols_val = 1; }
   ~ColumnVector() {}
   ColumnVector(ArrayLengthSpecifier n) : Matrix(n.Value(),1) {}
   ColumnVector(const BaseMatrix&);
   ColumnVector(const ColumnVector& gm) { GetMatrix(&gm); }
   void operator=(const BaseMatrix&);
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   void operator=(const ColumnVector& m) { Eq(m); }
   Real& operator()(int);                       // access element
   Real& element(int);                          // access element
   Real operator()(int) const;                  // access element
   Real element(int) const;                     // access element
#ifdef SETUP_C_SUBSCRIPTS
   Real& operator[](int m) { return store[m]; }
   const Real& operator[](int m) const { return store[m]; }
   // following for Numerical Recipes in C++
   ColumnVector(Real a, int m) : Matrix(a, m, 1) {}
   ColumnVector(const Real* a, int m) : Matrix(a, m, 1) {}
#endif
   MatrixType type() const;
   GeneralMatrix* Transpose(TransposedMatrix*, MatrixType);
   void resize(int);                       // change dimensions
   void ReSize(int m) { resize(m); }
   void resize_keep(int);
   void resize_keep(int,int);
   void resize(int,int);                   // in case access is matrix
   void ReSize(int m,int n) { resize(m, n); }
   void resize(const GeneralMatrix& A);
   void ReSize(const GeneralMatrix& A) { resize(A); }
   Real* nric() const
      { CheckStore(); return store-1; }         // for use by NRIC
   void cleanup();                              // to clear store
   void MiniCleanUp()
      { store = 0; storage = 0; nrows_val = 0; ncols_val = 1; tag_val = -1; }
//   ReturnMatrix Reverse() const;                // reverse order of elements
   void operator+=(const Matrix& M) { PlusEqual(M); }
   void operator-=(const Matrix& M) { MinusEqual(M); }
   void operator+=(Real f) { GeneralMatrix::Add(f); }
   void operator-=(Real f) { GeneralMatrix::Add(-f); }
   void swap(ColumnVector& gm)
      { GeneralMatrix::swap((GeneralMatrix&)gm); }
   NEW_DELETE(ColumnVector)
};

class CroutMatrix : public GeneralMatrix        // for LU decomposition
{
   int* indx;
   bool d;                              // number of row swaps = even or odd
   bool sing;
   void ludcmp();
   void get_aux(CroutMatrix&);                  // for copying indx[] etc
   GeneralMatrix* Image() const;                // copy of matrix
public:
   CroutMatrix(const BaseMatrix&);
   CroutMatrix() : indx(0), d(true), sing(true) {}
   CroutMatrix(const CroutMatrix&);
   void operator=(const CroutMatrix&);
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixType type() const;
   void lubksb(Real*, int=0);
   ~CroutMatrix();
   GeneralMatrix* MakeSolver() { return this; } // for solving
   LogAndSign log_determinant() const;
   void Solver(MatrixColX&, const MatrixColX&);
   void GetRow(MatrixRowCol&);
   void GetCol(MatrixRowCol&);
   void GetCol(MatrixColX& c) { GetCol((MatrixRowCol&)c); }
   void cleanup();                                // to clear store
   void MiniCleanUp();
   bool IsEqual(const GeneralMatrix&) const;
   bool is_singular() const { return sing; }
   bool IsSingular() const { return sing; }
   const int* const_data_indx() const { return indx; }
   bool even_exchanges() const { return d; }
   void swap(CroutMatrix& gm);
   NEW_DELETE(CroutMatrix)
};

// ***************************** band matrices ***************************/

class BandMatrix : public GeneralMatrix         // band matrix
{
   GeneralMatrix* Image() const;                // copy of matrix
protected:
   void CornerClear() const;                    // set unused elements to zero
   short SimpleAddOK(const GeneralMatrix* gm);
public:
   int lower_val, upper_val;                            // band widths
   BandMatrix() { lower_val=0; upper_val=0; CornerClear(); }
   ~BandMatrix() {}
   BandMatrix(int n,int lb,int ub) { resize(n,lb,ub); CornerClear(); }
                                                // standard declaration
   BandMatrix(const BaseMatrix&);               // evaluate BaseMatrix
   void operator=(const BaseMatrix&);
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   void operator=(const BandMatrix& m) { Eq(m); }
   MatrixType type() const;
   Real& operator()(int, int);                  // access element
   Real& element(int, int);                     // access element
   Real operator()(int, int) const;             // access element
   Real element(int, int) const;                // access element
#ifdef SETUP_C_SUBSCRIPTS
   Real* operator[](int m) { return store+(upper_val+lower_val)*m+lower_val; }
   const Real* operator[](int m) const
      { return store+(upper_val+lower_val)*m+lower_val; }
#endif
   BandMatrix(const BandMatrix& gm) { GetMatrix(&gm); }
   LogAndSign log_determinant() const;
   GeneralMatrix* MakeSolver();
   Real trace() const;
   Real sum_square() const
      { CornerClear(); return GeneralMatrix::sum_square(); }
   Real sum_absolute_value() const
      { CornerClear(); return GeneralMatrix::sum_absolute_value(); }
   Real sum() const
      { CornerClear(); return GeneralMatrix::sum(); }
   Real maximum_absolute_value() const
      { CornerClear(); return GeneralMatrix::maximum_absolute_value(); }
   Real minimum_absolute_value() const
      { int i, j; return GeneralMatrix::minimum_absolute_value2(i, j); }
   Real maximum() const { int i, j; return GeneralMatrix::maximum2(i, j); }
   Real minimum() const { int i, j; return GeneralMatrix::minimum2(i, j); }
   void GetRow(MatrixRowCol&);
   void GetCol(MatrixRowCol&);
   void GetCol(MatrixColX&);
   void RestoreCol(MatrixRowCol&);
   void RestoreCol(MatrixColX& c) { RestoreCol((MatrixRowCol&)c); }
   void NextRow(MatrixRowCol&);
   virtual void resize(int, int, int);             // change dimensions
   virtual void ReSize(int m, int n, int b) { resize(m, n, b); }
   void resize(const GeneralMatrix& A);
   void ReSize(const GeneralMatrix& A) { resize(A); }
   //bool SameStorageType(const GeneralMatrix& A) const;
   //void ReSizeForAdd(const GeneralMatrix& A, const GeneralMatrix& B);
   //void ReSizeForSP(const GeneralMatrix& A, const GeneralMatrix& B);
   MatrixBandWidth bandwidth() const;
   void SetParameters(const GeneralMatrix*);
   MatrixInput operator<<(double);                // will give error
   MatrixInput operator<<(float);                // will give error
   MatrixInput operator<<(int f);
   void operator<<(const double* r);              // will give error
   void operator<<(const float* r);              // will give error
   void operator<<(const int* r);               // will give error
      // the next is included because Zortech and Borland
      // cannot find the copy in GeneralMatrix
   void operator<<(const BaseMatrix& X) { GeneralMatrix::operator<<(X); }
   void swap(BandMatrix& gm);
   NEW_DELETE(BandMatrix)
};

class UpperBandMatrix : public BandMatrix       // upper band matrix
{
   GeneralMatrix* Image() const;                // copy of matrix
public:
   UpperBandMatrix() {}
   ~UpperBandMatrix() {}
   UpperBandMatrix(int n, int ubw)              // standard declaration
      : BandMatrix(n, 0, ubw) {}
   UpperBandMatrix(const BaseMatrix&);          // evaluate BaseMatrix
   void operator=(const BaseMatrix&);
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   void operator=(const UpperBandMatrix& m) { Eq(m); }
   MatrixType type() const;
   UpperBandMatrix(const UpperBandMatrix& gm) { GetMatrix(&gm); }
   GeneralMatrix* MakeSolver() { return this; }
   void Solver(MatrixColX&, const MatrixColX&);
   LogAndSign log_determinant() const;
   void resize(int, int, int);             // change dimensions
   void ReSize(int m, int n, int b) { resize(m, n, b); }
   void resize(int n,int ubw)              // change dimensions
      { BandMatrix::resize(n,0,ubw); }
   void ReSize(int n,int ubw)              // change dimensions
      { BandMatrix::resize(n,0,ubw); }
   void resize(const GeneralMatrix& A) { BandMatrix::resize(A); }
   void ReSize(const GeneralMatrix& A) { BandMatrix::resize(A); }
   Real& operator()(int, int);
   Real operator()(int, int) const;
   Real& element(int, int);
   Real element(int, int) const;
#ifdef SETUP_C_SUBSCRIPTS
   Real* operator[](int m) { return store+upper_val*m; }
   const Real* operator[](int m) const { return store+upper_val*m; }
#endif
   void swap(UpperBandMatrix& gm)
      { BandMatrix::swap((BandMatrix&)gm); }
   NEW_DELETE(UpperBandMatrix)
};

class LowerBandMatrix : public BandMatrix       // upper band matrix
{
   GeneralMatrix* Image() const;                // copy of matrix
public:
   LowerBandMatrix() {}
   ~LowerBandMatrix() {}
   LowerBandMatrix(int n, int lbw)              // standard declaration
      : BandMatrix(n, lbw, 0) {}
   LowerBandMatrix(const BaseMatrix&);          // evaluate BaseMatrix
   void operator=(const BaseMatrix&);
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   void operator=(const LowerBandMatrix& m) { Eq(m); }
   MatrixType type() const;
   LowerBandMatrix(const LowerBandMatrix& gm) { GetMatrix(&gm); }
   GeneralMatrix* MakeSolver() { return this; }
   void Solver(MatrixColX&, const MatrixColX&);
   LogAndSign log_determinant() const;
   void resize(int, int, int);             // change dimensions
   void ReSize(int m, int n, int b) { resize(m, n, b); }
   void resize(int n,int lbw)             // change dimensions
      { BandMatrix::resize(n,lbw,0); }
   void ReSize(int n,int lbw)             // change dimensions
      { BandMatrix::resize(n,lbw,0); }
   void resize(const GeneralMatrix& A) { BandMatrix::resize(A); }
   void ReSize(const GeneralMatrix& A) { BandMatrix::resize(A); }
   Real& operator()(int, int);
   Real operator()(int, int) const;
   Real& element(int, int);
   Real element(int, int) const;
#ifdef SETUP_C_SUBSCRIPTS
   Real* operator[](int m) { return store+lower_val*(m+1); }
   const Real* operator[](int m) const { return store+lower_val*(m+1); }
#endif
   void swap(LowerBandMatrix& gm)
      { BandMatrix::swap((BandMatrix&)gm); }
   NEW_DELETE(LowerBandMatrix)
};

class SymmetricBandMatrix : public GeneralMatrix
{
   GeneralMatrix* Image() const;                // copy of matrix
   void CornerClear() const;                    // set unused elements to zero
   short SimpleAddOK(const GeneralMatrix* gm);
public:
   int lower_val;                                   // lower band width
   SymmetricBandMatrix() { lower_val=0; CornerClear(); }
   ~SymmetricBandMatrix() {}
   SymmetricBandMatrix(int n, int lb) { resize(n,lb); CornerClear(); }
   SymmetricBandMatrix(const BaseMatrix&);
   void operator=(const BaseMatrix&);
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   void operator=(const SymmetricBandMatrix& m) { Eq(m); }
   Real& operator()(int, int);                  // access element
   Real& element(int, int);                     // access element
   Real operator()(int, int) const;             // access element
   Real element(int, int) const;                // access element
#ifdef SETUP_C_SUBSCRIPTS
   Real* operator[](int m) { return store+lower_val*(m+1); }
   const Real* operator[](int m) const { return store+lower_val*(m+1); }
#endif
   MatrixType type() const;
   SymmetricBandMatrix(const SymmetricBandMatrix& gm) { GetMatrix(&gm); }
   GeneralMatrix* MakeSolver();
   Real sum_square() const;
   Real sum_absolute_value() const;
   Real sum() const;
   Real maximum_absolute_value() const
      { CornerClear(); return GeneralMatrix::maximum_absolute_value(); }
   Real minimum_absolute_value() const
      { int i, j; return GeneralMatrix::minimum_absolute_value2(i, j); }
   Real maximum() const { int i, j; return GeneralMatrix::maximum2(i, j); }
   Real minimum() const { int i, j; return GeneralMatrix::minimum2(i, j); }
   Real trace() const;
   LogAndSign log_determinant() const;
   void GetRow(MatrixRowCol&);
   void GetCol(MatrixRowCol&);
   void GetCol(MatrixColX&);
   void RestoreCol(MatrixRowCol&) {}
   void RestoreCol(MatrixColX&);
   GeneralMatrix* Transpose(TransposedMatrix*, MatrixType);
   void resize(int,int);                       // change dimensions
   void ReSize(int m,int b) { resize(m, b); }
   void resize(const GeneralMatrix& A);
   void ReSize(const GeneralMatrix& A) { resize(A); }
   //bool SameStorageType(const GeneralMatrix& A) const;
   //void ReSizeForAdd(const GeneralMatrix& A, const GeneralMatrix& B);
   //void ReSizeForSP(const GeneralMatrix& A, const GeneralMatrix& B);
   MatrixBandWidth bandwidth() const;
   void SetParameters(const GeneralMatrix*);
   void operator<<(const double* r);              // will give error
   void operator<<(const float* r);              // will give error
   void operator<<(const int* r);               // will give error
   void operator<<(const BaseMatrix& X) { GeneralMatrix::operator<<(X); }
   void swap(SymmetricBandMatrix& gm);
   NEW_DELETE(SymmetricBandMatrix)
};

class BandLUMatrix : public GeneralMatrix
// for LU decomposition of band matrix
{
   int* indx;
   bool d;
   bool sing;                                   // true if singular
   Real* store2;
   int storage2;
   int m1,m2;                                   // lower and upper
   void ludcmp();
   void get_aux(BandLUMatrix&);                 // for copying indx[] etc
   GeneralMatrix* Image() const;                // copy of matrix
public:
   BandLUMatrix(const BaseMatrix&);
   BandLUMatrix()
     : indx(0), d(true), sing(true), store2(0), storage2(0), m1(0), m2(0) {}
   BandLUMatrix(const BandLUMatrix&);
   void operator=(const BandLUMatrix&);
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixType type() const;
   void lubksb(Real*, int=0);
   ~BandLUMatrix();
   GeneralMatrix* MakeSolver() { return this; } // for solving
   LogAndSign log_determinant() const;
   void Solver(MatrixColX&, const MatrixColX&);
   void GetRow(MatrixRowCol&);
   void GetCol(MatrixRowCol&);
   void GetCol(MatrixColX& c) { GetCol((MatrixRowCol&)c); }
   void cleanup();                                // to clear store
   void MiniCleanUp();
   bool IsEqual(const GeneralMatrix&) const;
   bool is_singular() const { return sing; }
   bool IsSingular() const { return sing; }
   const Real* const_data2() const { return store2; }
   int size2() const { return storage2; }
   const int* const_data_indx() const { return indx; }
   bool even_exchanges() const { return d; }
   MatrixBandWidth bandwidth() const;
   void swap(BandLUMatrix& gm);
   NEW_DELETE(BandLUMatrix)
};

// ************************** special matrices ****************************

class IdentityMatrix : public GeneralMatrix
{
   GeneralMatrix* Image() const;          // copy of matrix
public:
   IdentityMatrix() {}
   ~IdentityMatrix() {}
   IdentityMatrix(ArrayLengthSpecifier n) : GeneralMatrix(1)
      { nrows_val = ncols_val = n.Value(); *store = 1; }
   IdentityMatrix(const IdentityMatrix& gm) { GetMatrix(&gm); }
   IdentityMatrix(const BaseMatrix&);
   void operator=(const BaseMatrix&);
   void operator=(const IdentityMatrix& m) { Eq(m); }
   void operator=(Real f) { GeneralMatrix::operator=(f); }
   MatrixType type() const;

   LogAndSign log_determinant() const;
   Real trace() const;
   Real sum_square() const;
   Real sum_absolute_value() const;
   Real sum() const { return trace(); }
   void GetRow(MatrixRowCol&);
   void GetCol(MatrixRowCol&);
   void GetCol(MatrixColX&);
   void NextRow(MatrixRowCol&);
   void NextCol(MatrixRowCol&);
   void NextCol(MatrixColX&);
   GeneralMatrix* MakeSolver() { return this; } // for solving
   void Solver(MatrixColX&, const MatrixColX&);
   GeneralMatrix* Transpose(TransposedMatrix*, MatrixType);
   void resize(int n);
   void ReSize(int n) { resize(n); }
   void resize(const GeneralMatrix& A);
   void ReSize(const GeneralMatrix& A) { resize(A); }
   MatrixBandWidth bandwidth() const;
//   ReturnMatrix Reverse() const;                // reverse order of elements
   void swap(IdentityMatrix& gm)
      { GeneralMatrix::swap((GeneralMatrix&)gm); }
   NEW_DELETE(IdentityMatrix)
};




// ************************** GenericMatrix class ************************/

class GenericMatrix : public BaseMatrix
{
   GeneralMatrix* gm;
   int search(const BaseMatrix* bm) const;
   friend class BaseMatrix;
public:
   GenericMatrix() : gm(0) {}
   GenericMatrix(const BaseMatrix& bm)
      { gm = ((BaseMatrix&)bm).Evaluate(); gm = gm->Image(); }
   GenericMatrix(const GenericMatrix& bm)
      { gm = bm.gm->Image(); }
   void operator=(const GenericMatrix&);
   void operator=(const BaseMatrix&);
   void operator+=(const BaseMatrix&);
   void operator-=(const BaseMatrix&);
   void operator*=(const BaseMatrix&);
   void operator|=(const BaseMatrix&);
   void operator&=(const BaseMatrix&);
   void operator+=(Real);
   void operator-=(Real r) { operator+=(-r); }
   void operator*=(Real);
   void operator/=(Real r) { operator*=(1.0/r); }
   ~GenericMatrix() { delete gm; }
   void cleanup() { delete gm; gm = 0; }
   void Release() { gm->Release(); }
   void release() { gm->release(); }
   GeneralMatrix* Evaluate(MatrixType = MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;
   void swap(GenericMatrix& x);
   NEW_DELETE(GenericMatrix)
};

// *************************** temporary classes *************************/

class MultipliedMatrix : public BaseMatrix
{
protected:
   // if these union statements cause problems, simply remove them
   // and declare the items individually
   union { const BaseMatrix* bm1; GeneralMatrix* gm1; };
              // pointers to summands
   union { const BaseMatrix* bm2; GeneralMatrix* gm2; };
   MultipliedMatrix(const BaseMatrix* bm1x, const BaseMatrix* bm2x)
      : bm1(bm1x),bm2(bm2x) {}
   int search(const BaseMatrix*) const;
   friend class BaseMatrix;
   friend class GeneralMatrix;
   friend class GenericMatrix;
public:
   ~MultipliedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;
   NEW_DELETE(MultipliedMatrix)
};

class AddedMatrix : public MultipliedMatrix
{
protected:
   AddedMatrix(const BaseMatrix* bm1x, const BaseMatrix* bm2x)
      : MultipliedMatrix(bm1x,bm2x) {}

   friend class BaseMatrix;
   friend class GeneralMatrix;
   friend class GenericMatrix;
public:
   ~AddedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;
   NEW_DELETE(AddedMatrix)
};

class SPMatrix : public AddedMatrix
{
protected:
   SPMatrix(const BaseMatrix* bm1x, const BaseMatrix* bm2x)
      : AddedMatrix(bm1x,bm2x) {}

   friend class BaseMatrix;
   friend class GeneralMatrix;
   friend class GenericMatrix;
public:
   ~SPMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;

   friend SPMatrix SP(const BaseMatrix&, const BaseMatrix&);

   NEW_DELETE(SPMatrix)
};

class KPMatrix : public MultipliedMatrix
{
protected:
   KPMatrix(const BaseMatrix* bm1x, const BaseMatrix* bm2x)
      : MultipliedMatrix(bm1x,bm2x) {}

   friend class BaseMatrix;
   friend class GeneralMatrix;
   friend class GenericMatrix;
public:
   ~KPMatrix() {}
   MatrixBandWidth bandwidth() const;
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   friend KPMatrix KP(const BaseMatrix&, const BaseMatrix&);
   NEW_DELETE(KPMatrix)
};

class ConcatenatedMatrix : public MultipliedMatrix
{
protected:
   ConcatenatedMatrix(const BaseMatrix* bm1x, const BaseMatrix* bm2x)
      : MultipliedMatrix(bm1x,bm2x) {}

   friend class BaseMatrix;
   friend class GeneralMatrix;
   friend class GenericMatrix;
public:
   ~ConcatenatedMatrix() {}
   MatrixBandWidth bandwidth() const;
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   NEW_DELETE(ConcatenatedMatrix)
};

class StackedMatrix : public ConcatenatedMatrix
{
protected:
   StackedMatrix(const BaseMatrix* bm1x, const BaseMatrix* bm2x)
      : ConcatenatedMatrix(bm1x,bm2x) {}

   friend class BaseMatrix;
   friend class GeneralMatrix;
   friend class GenericMatrix;
public:
   ~StackedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   NEW_DELETE(StackedMatrix)
};

class SolvedMatrix : public MultipliedMatrix
{
   SolvedMatrix(const BaseMatrix* bm1x, const BaseMatrix* bm2x)
      : MultipliedMatrix(bm1x,bm2x) {}
   friend class BaseMatrix;
   friend class InvertedMatrix;                        // for operator*
public:
   ~SolvedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;
   NEW_DELETE(SolvedMatrix)
};

class SubtractedMatrix : public AddedMatrix
{
   SubtractedMatrix(const BaseMatrix* bm1x, const BaseMatrix* bm2x)
      : AddedMatrix(bm1x,bm2x) {}
   friend class BaseMatrix;
   friend class GeneralMatrix;
   friend class GenericMatrix;
public:
   ~SubtractedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   NEW_DELETE(SubtractedMatrix)
};

class ShiftedMatrix : public BaseMatrix
{
protected:
   union { const BaseMatrix* bm; GeneralMatrix* gm; };
   Real f;
   ShiftedMatrix(const BaseMatrix* bmx, Real fx) : bm(bmx),f(fx) {}
   int search(const BaseMatrix*) const;
   friend class BaseMatrix;
   friend class GeneralMatrix;
   friend class GenericMatrix;
public:
   ~ShiftedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   friend ShiftedMatrix operator+(Real f, const BaseMatrix& BM);
   NEW_DELETE(ShiftedMatrix)
};

class NegShiftedMatrix : public ShiftedMatrix
{
protected:
   NegShiftedMatrix(Real fx, const BaseMatrix* bmx) : ShiftedMatrix(bmx,fx) {}
   friend class BaseMatrix;
   friend class GeneralMatrix;
   friend class GenericMatrix;
public:
   ~NegShiftedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   friend NegShiftedMatrix operator-(Real, const BaseMatrix&);
   NEW_DELETE(NegShiftedMatrix)
};

class ScaledMatrix : public ShiftedMatrix
{
   ScaledMatrix(const BaseMatrix* bmx, Real fx) : ShiftedMatrix(bmx,fx) {}
   friend class BaseMatrix;
   friend class GeneralMatrix;
   friend class GenericMatrix;
public:
   ~ScaledMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;
   friend ScaledMatrix operator*(Real f, const BaseMatrix& BM);
   NEW_DELETE(ScaledMatrix)
};

class NegatedMatrix : public BaseMatrix
{
protected:
   union { const BaseMatrix* bm; GeneralMatrix* gm; };
   NegatedMatrix(const BaseMatrix* bmx) : bm(bmx) {}
   int search(const BaseMatrix*) const;
private:
   friend class BaseMatrix;
public:
   ~NegatedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;
   NEW_DELETE(NegatedMatrix)
};

class TransposedMatrix : public NegatedMatrix
{
   TransposedMatrix(const BaseMatrix* bmx) : NegatedMatrix(bmx) {}
   friend class BaseMatrix;
public:
   ~TransposedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;
   NEW_DELETE(TransposedMatrix)
};

class ReversedMatrix : public NegatedMatrix
{
   ReversedMatrix(const BaseMatrix* bmx) : NegatedMatrix(bmx) {}
   friend class BaseMatrix;
public:
   ~ReversedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   NEW_DELETE(ReversedMatrix)
};

class InvertedMatrix : public NegatedMatrix
{
   InvertedMatrix(const BaseMatrix* bmx) : NegatedMatrix(bmx) {}
public:
   ~InvertedMatrix() {}
   SolvedMatrix operator*(const BaseMatrix&) const;       // inverse(A) * B
   ScaledMatrix operator*(Real t) const { return BaseMatrix::operator*(t); }
   friend class BaseMatrix;
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;
   NEW_DELETE(InvertedMatrix)
};

class RowedMatrix : public NegatedMatrix
{
   RowedMatrix(const BaseMatrix* bmx) : NegatedMatrix(bmx) {}
   friend class BaseMatrix;
public:
   ~RowedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;
   NEW_DELETE(RowedMatrix)
};

class ColedMatrix : public NegatedMatrix
{
   ColedMatrix(const BaseMatrix* bmx) : NegatedMatrix(bmx) {}
   friend class BaseMatrix;
public:
   ~ColedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;
   NEW_DELETE(ColedMatrix)
};

class DiagedMatrix : public NegatedMatrix
{
   DiagedMatrix(const BaseMatrix* bmx) : NegatedMatrix(bmx) {}
   friend class BaseMatrix;
public:
   ~DiagedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;
   NEW_DELETE(DiagedMatrix)
};

class MatedMatrix : public NegatedMatrix
{
   int nr, nc;
   MatedMatrix(const BaseMatrix* bmx, int nrx, int ncx)
      : NegatedMatrix(bmx), nr(nrx), nc(ncx) {}
   friend class BaseMatrix;
public:
   ~MatedMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   MatrixBandWidth bandwidth() const;
   NEW_DELETE(MatedMatrix)
};

class ReturnMatrix : public BaseMatrix    // for matrix return
{
   GeneralMatrix* gm;
   int search(const BaseMatrix*) const;
public:
   ~ReturnMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   friend class BaseMatrix;
   ReturnMatrix(const ReturnMatrix& tm) : gm(tm.gm) {}
   ReturnMatrix(const GeneralMatrix* gmx) : gm((GeneralMatrix*&)gmx) {}
//   ReturnMatrix(GeneralMatrix&);
   MatrixBandWidth bandwidth() const;
   NEW_DELETE(ReturnMatrix)
};


// ************************** submatrices ******************************/

class GetSubMatrix : public NegatedMatrix
{
   int row_skip;
   int row_number;
   int col_skip;
   int col_number;
   bool IsSym;

   GetSubMatrix
      (const BaseMatrix* bmx, int rs, int rn, int cs, int cn, bool is)
      : NegatedMatrix(bmx),
      row_skip(rs), row_number(rn), col_skip(cs), col_number(cn), IsSym(is) {}
   void SetUpLHS();
   friend class BaseMatrix;
public:
   GetSubMatrix(const GetSubMatrix& g)
      : NegatedMatrix(g.bm), row_skip(g.row_skip), row_number(g.row_number),
      col_skip(g.col_skip), col_number(g.col_number), IsSym(g.IsSym) {}
   ~GetSubMatrix() {}
   GeneralMatrix* Evaluate(MatrixType mt=MatrixTypeUnSp);
   void operator=(const BaseMatrix&);
   void operator+=(const BaseMatrix&);
   void operator-=(const BaseMatrix&);
   void operator=(const GetSubMatrix& m) { operator=((const BaseMatrix&)m); }
   void operator<<(const BaseMatrix&);
   void operator<<(const double*);                // copy from array
   void operator<<(const float*);                // copy from array
   void operator<<(const int*);                 // copy from array
   MatrixInput operator<<(double);                // for loading a list
   MatrixInput operator<<(float);                // for loading a list
   MatrixInput operator<<(int f);
   void operator=(Real);                        // copy from constant
   void operator+=(Real);                       // add constant
   void operator-=(Real r) { operator+=(-r); }  // subtract constant
   void operator*=(Real);                       // multiply by constant
   void operator/=(Real r) { operator*=(1.0/r); } // divide by constant
   void inject(const GeneralMatrix&);           // copy stored els only
   void Inject(const GeneralMatrix& GM) { inject(GM); }
   MatrixBandWidth bandwidth() const;
   NEW_DELETE(GetSubMatrix)
};

// ******************** linear equation solving ****************************/

class LinearEquationSolver : public BaseMatrix
{
   GeneralMatrix* gm;
   int search(const BaseMatrix*) const { return 0; }
   friend class BaseMatrix;
public:
   LinearEquationSolver(const BaseMatrix& bm);
   ~LinearEquationSolver() { delete gm; }
   void cleanup() { delete gm; } 
   GeneralMatrix* Evaluate(MatrixType) { return gm; }
   // probably should have an error message if MatrixType != UnSp
   NEW_DELETE(LinearEquationSolver)
};

// ************************** matrix input *******************************/

class MatrixInput          // for reading a list of values into a matrix
                           // the difficult part is detecting a mismatch
                           // in the number of elements
{
   int n;                  // number values still to be read
   Real* r;                // pointer to next location to be read to
public:
   MatrixInput(const MatrixInput& mi) : n(mi.n), r(mi.r) {}
   MatrixInput(int nx, Real* rx) : n(nx), r(rx) {}
   ~MatrixInput();
   MatrixInput operator<<(double);
   MatrixInput operator<<(float);
   MatrixInput operator<<(int f);
   friend class GeneralMatrix;
};



// **************** a very simple integer array class ********************/

// A minimal array class to imitate a C style array but giving dynamic storage
// mostly intended for internal use by newmat

class SimpleIntArray : public Janitor
{
protected:
   int* a;                    // pointer to the array
   int n;                     // length of the array
public:
   SimpleIntArray(int xn);    // build an array length xn
   SimpleIntArray() : a(0), n(0) {}  // build an array length 0
   ~SimpleIntArray();         // return the space to memory
   int& operator[](int i);    // access element of the array - start at 0
   int operator[](int i) const;
            // access element of constant array
   void operator=(int ai);    // set the array equal to a constant
   void operator=(const SimpleIntArray& b);
            // copy the elements of an array
   SimpleIntArray(const SimpleIntArray& b);
            // make a new array equal to an existing one
   int Size() const { return n; }
            // return the size of the array
   int size() const { return n; }
            // return the size of the array
   int* Data() { return a; }  // pointer to the data
   const int* Data() const { return a; }  // pointer to the data
   int* data() { return a; }  // pointer to the data
   const int* data() const { return a; }  // pointer to the data
   const int* const_data() const { return a; }  // pointer to the data
   void resize(int i, bool keep = false);
                              // change length, keep data if keep = true
   void ReSize(int i, bool keep = false) { resize(i, keep); }
                              // change length, keep data if keep = true
   void resize_keep(int i) { resize(i, true); }
                              // change length, keep data
   void cleanup() { resize(0); }
   void CleanUp() { resize(0); }
   NEW_DELETE(SimpleIntArray)
};

// ********************** C subscript classes ****************************

class RealStarStar
{
   Real** a;
public:
   RealStarStar(Matrix& A);
   ~RealStarStar() { delete [] a; }
   operator Real**() { return a; }
};

class ConstRealStarStar
{
   const Real** a;
public:
   ConstRealStarStar(const Matrix& A);
   ~ConstRealStarStar() { delete [] a; }
   operator const Real**() { return a; }
};

// *************************** exceptions ********************************/

class NPDException : public Runtime_error     // Not positive definite
{
public:
   static unsigned long Select;          // for identifying exception
   NPDException(const GeneralMatrix&);
};

class ConvergenceException : public Runtime_error
{
public:
   static unsigned long Select;          // for identifying exception
   ConvergenceException(const GeneralMatrix& A);
   ConvergenceException(const char* c);
};

class SingularException : public Runtime_error
{
public:
   static unsigned long Select;          // for identifying exception
   SingularException(const GeneralMatrix& A);
};

class OverflowException : public Runtime_error
{
public:
   static unsigned long Select;          // for identifying exception
   OverflowException(const char* c);
};

class ProgramException : public Logic_error
{
protected:
   ProgramException();
public:
   static unsigned long Select;          // for identifying exception
   ProgramException(const char* c);
   ProgramException(const char* c, const GeneralMatrix&);
   ProgramException(const char* c, const GeneralMatrix&, const GeneralMatrix&);
   ProgramException(const char* c, MatrixType, MatrixType);
};

class IndexException : public Logic_error
{
public:
   static unsigned long Select;          // for identifying exception
   IndexException(int i, const GeneralMatrix& A);
   IndexException(int i, int j, const GeneralMatrix& A);
   // next two are for access via element function
   IndexException(int i, const GeneralMatrix& A, bool);
   IndexException(int i, int j, const GeneralMatrix& A, bool);
};

class VectorException : public Logic_error    // cannot convert to vector
{
public:
   static unsigned long Select;          // for identifying exception
   VectorException();
   VectorException(const GeneralMatrix& A);
};

class NotSquareException : public Logic_error
{
public:
   static unsigned long Select;          // for identifying exception
   NotSquareException(const GeneralMatrix& A);
   NotSquareException();
};

class SubMatrixDimensionException : public Logic_error
{
public:
   static unsigned long Select;          // for identifying exception
   SubMatrixDimensionException();
};

class IncompatibleDimensionsException : public Logic_error
{
public:
   static unsigned long Select;          // for identifying exception
   IncompatibleDimensionsException();
   IncompatibleDimensionsException(const GeneralMatrix&);
   IncompatibleDimensionsException(const GeneralMatrix&, const GeneralMatrix&);
};

class NotDefinedException : public Logic_error
{
public:
   static unsigned long Select;          // for identifying exception
   NotDefinedException(const char* op, const char* matrix);
};

class CannotBuildException : public Logic_error
{
public:
   static unsigned long Select;          // for identifying exception
   CannotBuildException(const char* matrix);
};


class InternalException : public Logic_error
{
public:
   static unsigned long Select;          // for identifying exception
   InternalException(const char* c);
};

// ************************ functions ************************************ //

bool operator==(const GeneralMatrix& A, const GeneralMatrix& B);
bool operator==(const BaseMatrix& A, const BaseMatrix& B);
inline bool operator!=(const GeneralMatrix& A, const GeneralMatrix& B)
   { return ! (A==B); }
inline bool operator!=(const BaseMatrix& A, const BaseMatrix& B)
   { return ! (A==B); }

   // inequality operators are dummies included for compatibility
   // with STL. They throw an exception if actually called.
inline bool operator<=(const BaseMatrix& A, const BaseMatrix&)
   { A.IEQND(); return true; }
inline bool operator>=(const BaseMatrix& A, const BaseMatrix&)
   { A.IEQND(); return true; }
inline bool operator<(const BaseMatrix& A, const BaseMatrix&)
   { A.IEQND(); return true; }
inline bool operator>(const BaseMatrix& A, const BaseMatrix&)
   { A.IEQND(); return true; }

bool is_zero(const BaseMatrix& A);
inline bool IsZero(const BaseMatrix& A) { return is_zero(A); }

Real dotproduct(const Matrix& A, const Matrix& B);
Matrix crossproduct(const Matrix& A, const Matrix& B);
ReturnMatrix crossproduct_rows(const Matrix& A, const Matrix& B);
ReturnMatrix crossproduct_columns(const Matrix& A, const Matrix& B);

inline Real DotProduct(const Matrix& A, const Matrix& B)
   { return dotproduct(A, B); }
inline Matrix CrossProduct(const Matrix& A, const Matrix& B)
   { return crossproduct(A, B); }
inline ReturnMatrix CrossProductRows(const Matrix& A, const Matrix& B)
   { return crossproduct_rows(A, B); }
inline ReturnMatrix CrossProductColumns(const Matrix& A, const Matrix& B)
   { return crossproduct_columns(A, B); }
   
void newmat_block_copy(int n, Real* from, Real* to);



// ********************* inline functions ******************************** //


inline LogAndSign log_determinant(const BaseMatrix& B)
   { return B.log_determinant(); }
inline LogAndSign LogDeterminant(const BaseMatrix& B)
   { return B.log_determinant(); }
inline Real determinant(const BaseMatrix& B)
   { return B.determinant(); }
inline Real Determinant(const BaseMatrix& B)
   { return B.determinant(); }
inline Real sum_square(const BaseMatrix& B) { return B.sum_square(); }
inline Real SumSquare(const BaseMatrix& B) { return B.sum_square(); }
inline Real norm_Frobenius(const BaseMatrix& B) { return B.norm_Frobenius(); }
inline Real norm_frobenius(const BaseMatrix& B) { return B.norm_Frobenius(); }
inline Real NormFrobenius(const BaseMatrix& B) { return B.norm_Frobenius(); }
inline Real trace(const BaseMatrix& B) { return B.trace(); }
inline Real Trace(const BaseMatrix& B) { return B.trace(); }
inline Real sum_absolute_value(const BaseMatrix& B)
   { return B.sum_absolute_value(); }
inline Real SumAbsoluteValue(const BaseMatrix& B)
   { return B.sum_absolute_value(); }
inline Real sum(const BaseMatrix& B)
   { return B.sum(); }
inline Real Sum(const BaseMatrix& B)
   { return B.sum(); }
inline Real maximum_absolute_value(const BaseMatrix& B)
   { return B.maximum_absolute_value(); }
inline Real MaximumAbsoluteValue(const BaseMatrix& B)
   { return B.maximum_absolute_value(); }
inline Real minimum_absolute_value(const BaseMatrix& B)
   { return B.minimum_absolute_value(); }
inline Real MinimumAbsoluteValue(const BaseMatrix& B)
   { return B.minimum_absolute_value(); }
inline Real maximum(const BaseMatrix& B) { return B.maximum(); }
inline Real Maximum(const BaseMatrix& B) { return B.maximum(); }
inline Real minimum(const BaseMatrix& B) { return B.minimum(); }
inline Real Minimum(const BaseMatrix& B) { return B.minimum(); }
inline Real norm1(const BaseMatrix& B) { return B.norm1(); }
inline Real Norm1(const BaseMatrix& B) { return B.norm1(); }
inline Real norm1(RowVector& RV) { return RV.maximum_absolute_value(); }
inline Real Norm1(RowVector& RV) { return RV.maximum_absolute_value(); }
inline Real norm_infinity(const BaseMatrix& B) { return B.norm_infinity(); }
inline Real NormInfinity(const BaseMatrix& B) { return B.norm_infinity(); }
inline Real norm_infinity(ColumnVector& CV)
   { return CV.maximum_absolute_value(); }
inline Real NormInfinity(ColumnVector& CV)
   { return CV.maximum_absolute_value(); }
inline bool IsZero(const GeneralMatrix& A) { return A.IsZero(); }
inline bool is_zero(const GeneralMatrix& A) { return A.is_zero(); }


inline MatrixInput MatrixInput::operator<<(int f) { return *this << (Real)f; }
inline MatrixInput GeneralMatrix::operator<<(int f) { return *this << (Real)f; }
inline MatrixInput BandMatrix::operator<<(int f) { return *this << (Real)f; }
inline MatrixInput GetSubMatrix::operator<<(int f) { return *this << (Real)f; }

inline ReversedMatrix BaseMatrix::Reverse() const { return reverse(); }
inline RowedMatrix BaseMatrix::AsRow() const { return as_row(); }
inline ColedMatrix BaseMatrix::AsColumn() const { return as_column(); }
inline DiagedMatrix BaseMatrix::AsDiagonal() const { return as_diagonal(); }
inline MatedMatrix BaseMatrix::AsMatrix(int m, int n) const
   { return as_matrix(m, n); }
inline GetSubMatrix BaseMatrix::SubMatrix(int fr, int lr, int fc, int lc) const
   { return submatrix(fr, lr, fc, lc); }
inline GetSubMatrix BaseMatrix::SymSubMatrix(int f, int l) const
   { return sym_submatrix(f, l); }
inline GetSubMatrix BaseMatrix::Row(int f) const { return row(f); }
inline GetSubMatrix BaseMatrix::Rows(int f, int l) const { return rows(f, l); }
inline GetSubMatrix BaseMatrix::Column(int f) const { return column(f); }
inline GetSubMatrix BaseMatrix::Columns(int f, int l) const
   { return columns(f, l); }
inline Real BaseMatrix::AsScalar() const { return as_scalar(); }

inline ReturnMatrix GeneralMatrix::ForReturn() const { return for_return(); }

inline void swap(Matrix& A, Matrix& B) { A.swap(B); }
inline void swap(SquareMatrix& A, SquareMatrix& B) { A.swap(B); }
inline void swap(nricMatrix& A, nricMatrix& B) { A.swap(B); }
inline void swap(UpperTriangularMatrix& A, UpperTriangularMatrix& B)
   { A.swap(B); }
inline void swap(LowerTriangularMatrix& A, LowerTriangularMatrix& B)
   { A.swap(B); }
inline void swap(SymmetricMatrix& A, SymmetricMatrix& B) { A.swap(B); }
inline void swap(DiagonalMatrix& A, DiagonalMatrix& B) { A.swap(B); }
inline void swap(RowVector& A, RowVector& B) { A.swap(B); }
inline void swap(ColumnVector& A, ColumnVector& B) { A.swap(B); }
inline void swap(CroutMatrix& A, CroutMatrix& B) { A.swap(B); }
inline void swap(BandMatrix& A, BandMatrix& B) { A.swap(B); }
inline void swap(UpperBandMatrix& A, UpperBandMatrix& B) { A.swap(B); }
inline void swap(LowerBandMatrix& A, LowerBandMatrix& B) { A.swap(B); }
inline void swap(SymmetricBandMatrix& A, SymmetricBandMatrix& B) { A.swap(B); }
inline void swap(BandLUMatrix& A, BandLUMatrix& B) { A.swap(B); }
inline void swap(IdentityMatrix& A, IdentityMatrix& B) { A.swap(B); }
inline void swap(GenericMatrix& A, GenericMatrix& B) { A.swap(B); }

#ifdef OPT_COMPATIBLE                    // for compatibility with opt++

inline Real Norm2(const ColumnVector& CV) { return CV.norm_Frobenius(); }
inline Real Dot(ColumnVector& CV1, ColumnVector& CV2)
   { return dotproduct(CV1, CV2); }

#endif


#ifdef use_namespace
}
#endif


#endif

// body file: newmat1.cpp
// body file: newmat2.cpp
// body file: newmat3.cpp
// body file: newmat4.cpp
// body file: newmat5.cpp
// body file: newmat6.cpp
// body file: newmat7.cpp
// body file: newmat8.cpp
// body file: newmatex.cpp
// body file: bandmat.cpp
// body file: submat.cpp

//$$ newmatrc.h              definition file for row/column classes

// Copyright (C) 1991,2,3,4,7: R B Davies

#ifndef NEWMATRC_LIB
#define NEWMATRC_LIB 0

#ifdef use_namespace
namespace NEWMAT {
#endif

  //$$ controlw.h                Control word class

#ifndef CONTROL_WORD_LIB
#define CONTROL_WORD_LIB 0

  // for organising an int as a series of bits which indicate whether an
  // option is on or off.

  class ControlWord
  {
  protected:
    int cw;                                      // the control word
  public:
    ControlWord() : cw(0) {}                     // do nothing
    ControlWord(int i) : cw(i) {}                // load an integer

    // select specific bits (for testing at least one set)
    ControlWord operator*(ControlWord i) const
    { return ControlWord(cw & i.cw); }
    void operator*=(ControlWord i)  { cw &= i.cw; }

    // set bits
    ControlWord operator+(ControlWord i) const
    { return ControlWord(cw | i.cw); }
    void operator+=(ControlWord i)  { cw |= i.cw; }

    // reset bits
    ControlWord operator-(ControlWord i) const
    { return ControlWord(cw - (cw & i.cw)); }
    void operator-=(ControlWord i) { cw -= (cw & i.cw); }

    // check if all of selected bits set or reset
    bool operator>=(ControlWord i) const { return (cw & i.cw) == i.cw; }
    bool operator<=(ControlWord i) const { return (cw & i.cw) == cw; }

    // flip selected bits
    ControlWord operator^(ControlWord i) const
    { return ControlWord(cw ^ i.cw); }
    ControlWord operator~() const { return ControlWord(~cw); }

    // convert to integer
    int operator+() const { return cw; }
    int operator!() const { return cw==0; }
    FREE_CHECK(ControlWord)
  };


#endif


  /************** classes MatrixRowCol, MatrixRow, MatrixCol *****************/

  // Used for accessing the rows and columns of matrices
  // All matrix classes must provide routines for calculating matrix rows and
  // columns. Assume rows can be found very efficiently.

  enum LSF { LoadOnEntry=1,StoreOnExit=2,DirectPart=4,StoreHere=8,HaveStore=16 };


  class LoadAndStoreFlag : public ControlWord
  {
  public:
    LoadAndStoreFlag() {}
    LoadAndStoreFlag(int i) : ControlWord(i) {}
    LoadAndStoreFlag(LSF lsf) : ControlWord(lsf) {}
    LoadAndStoreFlag(const ControlWord& cwx) : ControlWord(cwx) {}
  };

  class MatrixRowCol
    // the row or column of a matrix
  {
  public:                                        // these are public to avoid
    // numerous friend statements
    int length;                                 // row or column length
    int skip;                                   // initial number of zeros
    int storage;                                // number of stored elements
    int rowcol;                                 // row or column number
    GeneralMatrix* gm;                          // pointer to parent matrix
    Real* data;                                 // pointer to local storage
    LoadAndStoreFlag cw;                        // Load? Store? Is a Copy?
    void IncrMat() { rowcol++; data += storage; }   // used by NextRow
    void IncrDiag() { rowcol++; skip++; data++; }
    void IncrId() { rowcol++; skip++; }
    void IncrUT() { rowcol++; data += storage; storage--; skip++; }
    void IncrLT() { rowcol++; data += storage; storage++; }

  public:
    void Zero();                                // set elements to zero
    void Add(const MatrixRowCol&);              // add a row/col
    void AddScaled(const MatrixRowCol&, Real);  // add a multiple of a row/col
    void Add(const MatrixRowCol&, const MatrixRowCol&);
    // add two rows/cols
    void Add(const MatrixRowCol&, Real);        // add a row/col
    void NegAdd(const MatrixRowCol&, Real);     // Real - a row/col
    void Sub(const MatrixRowCol&);              // subtract a row/col
    void Sub(const MatrixRowCol&, const MatrixRowCol&);
    // sub a row/col from another
    void RevSub(const MatrixRowCol&);           // subtract from a row/col
    void ConCat(const MatrixRowCol&, const MatrixRowCol&);
    // concatenate two row/cols
    void Multiply(const MatrixRowCol&);         // multiply a row/col
    void Multiply(const MatrixRowCol&, const MatrixRowCol&);
    // multiply two row/cols
    void KP(const MatrixRowCol&, const MatrixRowCol&);
    // Kronecker Product two row/cols
    void Copy(const MatrixRowCol&);             // copy a row/col
    void CopyCheck(const MatrixRowCol&);        // ... check for data loss
    void Check(const MatrixRowCol&);            // just check for data loss
    void Check();                               // check full row/col present
    void Copy(const double*&);                  // copy from an array
    void Copy(const float*&);                   // copy from an array
    void Copy(const int*&);                     // copy from an array
    void Copy(Real);                            // copy from constant
    void Add(Real);                             // add a constant
    void Multiply(Real);                        // multiply by constant
    Real SumAbsoluteValue();                    // sum of absolute values
    Real MaximumAbsoluteValue1(Real r, int& i); // maximum of absolute values
    Real MinimumAbsoluteValue1(Real r, int& i); // minimum of absolute values
    Real Maximum1(Real r, int& i);              // maximum
    Real Minimum1(Real r, int& i);              // minimum
    Real Sum();                                 // sum of values
    void Inject(const MatrixRowCol&);           // copy stored els of a row/col
    void Negate(const MatrixRowCol&);           // change sign of a row/col
    void Multiply(const MatrixRowCol&, Real);   // scale a row/col
    friend Real DotProd(const MatrixRowCol&, const MatrixRowCol&);
    // sum of pairwise product
    Real* Data() { return data; }
    int Skip() { return skip; }                 // number of elements skipped
    int Storage() { return storage; }           // number of elements stored
    int Length() { return length; }             // length of row or column
    void Skip(int i) { skip=i; }
    void Storage(int i) { storage=i; }
    void Length(int i) { length=i; }
    void SubRowCol(MatrixRowCol&, int, int) const;
    // get part of a row or column
    MatrixRowCol() {}                           // to stop warning messages
    ~MatrixRowCol();
    FREE_CHECK(MatrixRowCol)
  };

  class MatrixRow : public MatrixRowCol
  {
  public:
    // bodies for these are inline at the end of this .h file
    MatrixRow(GeneralMatrix*, LoadAndStoreFlag, int=0);
    // extract a row
    ~MatrixRow();
    void Next();                                // get next row
    FREE_CHECK(MatrixRow)
  };

  class MatrixCol : public MatrixRowCol
  {
  public:
    // bodies for these are inline at the end of this .h file
    MatrixCol(GeneralMatrix*, LoadAndStoreFlag, int=0);
    // extract a col
    MatrixCol(GeneralMatrix*, Real*, LoadAndStoreFlag, int=0);
    // store/retrieve a col
    ~MatrixCol();
    void Next();                                // get next row
    FREE_CHECK(MatrixCol)
  };

  // MatrixColX is an alternative to MatrixCol where the complete
  // column is stored externally

  class MatrixColX : public MatrixRowCol
  {
  public:
    // bodies for these are inline at the end of this .h file
    MatrixColX(GeneralMatrix*, Real*, LoadAndStoreFlag, int=0);
    // store/retrieve a col
    ~MatrixColX();
    void Next();                                // get next row
    Real* store;                                // pointer to local storage
    //    less skip
    FREE_CHECK(MatrixColX)
  };

  /**************************** inline bodies ****************************/

  inline MatrixRow::MatrixRow(GeneralMatrix* gmx, LoadAndStoreFlag cwx, int row)
  { gm=gmx; cw=cwx; rowcol=row; gm->GetRow(*this); }

  inline void MatrixRow::Next() { gm->NextRow(*this); }

  inline MatrixCol::MatrixCol(GeneralMatrix* gmx, LoadAndStoreFlag cwx, int col)
  { gm=gmx; cw=cwx; rowcol=col; gm->GetCol(*this); }

  inline MatrixCol::MatrixCol(GeneralMatrix* gmx, Real* r,
    LoadAndStoreFlag cwx, int col)
  { gm=gmx; data=r; cw=cwx+StoreHere; rowcol=col; gm->GetCol(*this); }

  inline MatrixColX::MatrixColX(GeneralMatrix* gmx, Real* r,
    LoadAndStoreFlag cwx, int col)
  { gm=gmx; store=data=r; cw=cwx+StoreHere; rowcol=col; gm->GetCol(*this); }


  inline void MatrixCol::Next() { gm->NextCol(*this); }

  inline void MatrixColX::Next() { gm->NextCol(*this); }

#ifdef use_namespace
}
#endif

#endif
//$$ newmatap.h           definition file for matrix package applications

// Copyright (C) 1991,2,3,4,8: R B Davies

#ifndef NEWMATAP_LIB
#define NEWMATAP_LIB 0

#include "newmat.h"

#ifdef use_namespace
namespace NEWMAT {
#endif


// ************************** applications *****************************/


void QRZT(Matrix&, LowerTriangularMatrix&);

void QRZT(const Matrix&, Matrix&, Matrix&);

void QRZ(Matrix&, UpperTriangularMatrix&);

void QRZ(const Matrix&, Matrix&, Matrix&);

inline void HHDecompose(Matrix& X, LowerTriangularMatrix& L)
{ QRZT(X,L); }

inline void HHDecompose(const Matrix& X, Matrix& Y, Matrix& M)
{ QRZT(X, Y, M); }

void updateQRZT(Matrix& X, LowerTriangularMatrix& L);

void updateQRZ(Matrix& X, UpperTriangularMatrix& U);

inline void UpdateQRZT(Matrix& X, LowerTriangularMatrix& L)
   { updateQRZT(X, L); }

inline void UpdateQRZ(Matrix& X, UpperTriangularMatrix& U)
   { updateQRZ(X, U); }

// Matrix A's first n columns are orthonormal
// so A.Columns(1,n).t() * A.Columns(1,n) is the identity matrix.
// Fill out the remaining columns of A to make them orthonormal
// so A.t() * A is the identity matrix 
void extend_orthonormal(Matrix& A, int n);


ReturnMatrix Cholesky(const SymmetricMatrix&);

ReturnMatrix Cholesky(const SymmetricBandMatrix&);


// produces the Cholesky decomposition of A + x.t() * x where A = chol.t() * chol
// and x is a RowVector
void update_Cholesky(UpperTriangularMatrix& chol, RowVector x);
inline void UpdateCholesky(UpperTriangularMatrix& chol, const RowVector& x)
   { update_Cholesky(chol, x); }

// produces the Cholesky decomposition of A - x.t() * x where A = chol.t() * chol
// and x is a RowVector
void downdate_Cholesky(UpperTriangularMatrix &chol, RowVector x);
inline void DowndateCholesky(UpperTriangularMatrix &chol, const RowVector& x)
   { downdate_Cholesky(chol, x); }

// a RIGHT circular shift of the rows and columns from
// 1,...,k-1,k,k+1,...l,l+1,...,p to
// 1,...,k-1,l,k,k+1,...l-1,l+1,...p
void right_circular_update_Cholesky(UpperTriangularMatrix &chol, int k, int l);
inline void RightCircularUpdateCholesky(UpperTriangularMatrix &chol,
  int k, int l) { right_circular_update_Cholesky(chol, k, l); }

// a LEFT circular shift of the rows and columns from
// 1,...,k-1,k,k+1,...l,l+1,...,p to
// 1,...,k-1,k+1,...l,k,l+1,...,p to
void left_circular_update_Cholesky(UpperTriangularMatrix &chol, int k, int l); 
inline void LeftCircularUpdateCholesky(UpperTriangularMatrix &chol,
   int k, int l) { left_circular_update_Cholesky(chol, k, l); } 


void SVD(const Matrix&, DiagonalMatrix&, Matrix&, Matrix&,
    bool=true, bool=true);

void SVD(const Matrix&, DiagonalMatrix&);

inline void SVD(const Matrix& A, DiagonalMatrix& D, Matrix& U,
   bool withU = true) { SVD(A, D, U, U, withU, false); }

void SortSV(DiagonalMatrix& D, Matrix& U, bool ascending = false);

void SortSV(DiagonalMatrix& D, Matrix& U, Matrix& V, bool ascending = false);

void Jacobi(const SymmetricMatrix&, DiagonalMatrix&);

void Jacobi(const SymmetricMatrix&, DiagonalMatrix&, SymmetricMatrix&);

void Jacobi(const SymmetricMatrix&, DiagonalMatrix&, Matrix&);

void Jacobi(const SymmetricMatrix&, DiagonalMatrix&, SymmetricMatrix&,
   Matrix&, bool=true);

void eigenvalues(const SymmetricMatrix&, DiagonalMatrix&);

void eigenvalues(const SymmetricMatrix&, DiagonalMatrix&, SymmetricMatrix&);

void eigenvalues(const SymmetricMatrix&, DiagonalMatrix&, Matrix&);

inline void EigenValues(const SymmetricMatrix& A, DiagonalMatrix& D)
   { eigenvalues(A, D); }

inline void EigenValues(const SymmetricMatrix& A, DiagonalMatrix& D,
   SymmetricMatrix& S) { eigenvalues(A, D, S); }

inline void EigenValues(const SymmetricMatrix& A, DiagonalMatrix& D, Matrix& V)
   { eigenvalues(A, D, V); }

class SymmetricEigenAnalysis
// not implemented yet
{
public:
   SymmetricEigenAnalysis(const SymmetricMatrix&);
private:
   DiagonalMatrix diag;
   DiagonalMatrix offdiag;
   SymmetricMatrix backtransform;
   FREE_CHECK(SymmetricEigenAnalysis)
};

void sort_ascending(GeneralMatrix&);

void sort_descending(GeneralMatrix&);

inline void SortAscending(GeneralMatrix& gm) { sort_ascending(gm); }

inline void SortDescending(GeneralMatrix& gm) { sort_descending(gm); }

// class for deciding which fft to use and containing new fft function
class FFT_Controller
{
public:
   static bool OnlyOldFFT;
   static bool ar_1d_ft (int PTS, Real* X, Real *Y);
   static bool CanFactor(int PTS);
};

void FFT(const ColumnVector&, const ColumnVector&,
   ColumnVector&, ColumnVector&);

void FFTI(const ColumnVector&, const ColumnVector&,
   ColumnVector&, ColumnVector&);

void RealFFT(const ColumnVector&, ColumnVector&, ColumnVector&);

void RealFFTI(const ColumnVector&, const ColumnVector&, ColumnVector&);

void DCT_II(const ColumnVector&, ColumnVector&);

void DCT_II_inverse(const ColumnVector&, ColumnVector&);

void DST_II(const ColumnVector&, ColumnVector&);

void DST_II_inverse(const ColumnVector&, ColumnVector&);

void DCT(const ColumnVector&, ColumnVector&);

void DCT_inverse(const ColumnVector&, ColumnVector&);

void DST(const ColumnVector&, ColumnVector&);

void DST_inverse(const ColumnVector&, ColumnVector&);

void FFT2(const Matrix& U, const Matrix& V, Matrix& X, Matrix& Y);

void FFT2I(const Matrix& U, const Matrix& V, Matrix& X, Matrix& Y);


// This class is used by the new FFT program

// Suppose an integer is expressed as a sequence of digits with each
// digit having a different radix.
// This class supposes we are counting with this multi-radix number
// but also keeps track of the number with the digits (and radices)
// reversed.
// The integer starts at zero
// operator++() increases it by 1
// Counter gives the number of increments
// Reverse() gives the value with the digits in reverse order
// Swap is true if reverse is less than counter
// Finish is true when we have done a complete cycle and are back at zero

class MultiRadixCounter
{
   const SimpleIntArray& Radix;
                              // radix of each digit
                              // n-1 highest order, 0 lowest order
   SimpleIntArray& Value;     // value of each digit
   const int n;               // number of digits
   int reverse;               // value when order of digits is reversed
   int product;               // product of radices
   int counter;               // counter
   bool finish;               // true when we have gone over whole range
public:
   MultiRadixCounter(int nx, const SimpleIntArray& rx,
      SimpleIntArray& vx);
   void operator++();         // increment the multi-radix counter
   bool Swap() const { return reverse < counter; }
   bool Finish() const { return finish; }
   int Reverse() const { return reverse; }
   int Counter() const { return counter; }
};

// multiplication by Helmert matrix
ReturnMatrix Helmert(int n, bool full=false);
ReturnMatrix Helmert(const ColumnVector& X, bool full=false);
ReturnMatrix Helmert(int n, int j, bool full=false);
ReturnMatrix Helmert_transpose(const ColumnVector& Y, bool full=false);
Real Helmert_transpose(const ColumnVector& Y, int j, bool full=false);
ReturnMatrix Helmert(const Matrix& X, bool full=false);
ReturnMatrix Helmert_transpose(const Matrix& Y, bool full=false);




#ifdef use_namespace
}
#endif



#endif

// body file: cholesky.cpp
// body file: evalue.cpp
// body file: fft.cpp
// body file: hholder.cpp
// body file: jacobi.cpp
// body file: newfft.cpp
// body file: sort.cpp
// body file: svd.cpp
// body file: nm_misc.cpp


//$$ myexcept.h                                  Exception handling classes


// A set of classes to simulate exceptions in C++
//
//   Partially copied from Carlos Vidal s article in the C users  journal
//   September 1992, pp 19-28
//
//   Operations defined
//      Try {     }
//      Throw ( exception object )
//      ReThrow
//      Catch ( exception class ) {      }
//      CatchAll {      }
//      CatchAndThrow
//
//   All catch lists must end with a CatchAll or CatchAndThrow statement
//   but not both.
//
//   When exceptions are finally implemented replace Try, Throw(E), Rethrow,
//   Catch, CatchAll, CatchAndThrow by try, throw E, throw, catch,
//   catch(...), and {}.
//
//   All exception classes must be derived from BaseException, have no
//   non-static variables and must include the statement
//
//      static unsigned long Select;
//
//   Any constructor in one of these exception classes must include
//
//      Select = BaseException::Select;
//
//   For each exceptions class, EX_1, some .cpp file must include
//
//      unsigned long EX_1::Select;
//


#ifndef EXCEPTION_LIB
#define EXCEPTION_LIB

#include "include.h"

#ifdef use_namespace
namespace RBD_COMMON {
#endif


void MatrixTerminate();


//********** classes for setting up exceptions and reporting ************//

class BaseException;

class Tracer                             // linked list showing how
{                                        // we got here
   const char* entry;
   Tracer* previous;
public:
   Tracer(const char*);
   ~Tracer();
   void ReName(const char*);
   static void PrintTrace();             // for printing trace
   static void AddTrace();               // insert trace in exception record
   static Tracer* last;                  // points to Tracer list
   friend class BaseException;
};


class BaseException                          // The base exception class
{
protected:
   static char* what_error;              // error message
   static int SoFar;                     // no. characters already entered
   static int LastOne;                   // last location in error buffer
public:
   static void AddMessage(const char* a_what);
                                         // messages about exception
   static void AddInt(int value);        // integer to error message
   static unsigned long Select;          // for identifying exception
   BaseException(const char* a_what = 0);
   static const char* what() { return what_error; }
                                         // for getting error message
};

#ifdef TypeDefException
typedef BaseException Exception;        // for compatibility with my older libraries
#endif

inline Tracer::Tracer(const char* e)
   : entry(e), previous(last) { last = this; }

inline Tracer::~Tracer() { last = previous; }

inline void Tracer::ReName(const char* e) { entry=e; }

#ifdef SimulateExceptions                // SimulateExceptions

#include <setjmp.h>


//************* the definitions of Try, Throw and Catch *****************//


class JumpItem;
class Janitor;

class JumpBase         // pointer to a linked list of jmp_buf s
{
public:
   static JumpItem *jl;
   static jmp_buf env;
};

class JumpItem         // an item in a linked list of jmp_buf s
{
public:
   JumpItem *ji;
   jmp_buf env;
   Tracer* trace;                     // to keep check on Tracer items
   Janitor* janitor;                  // list of items for cleanup
   JumpItem() : ji(JumpBase::jl), trace(0), janitor(0)
      { JumpBase::jl = this; }
   ~JumpItem() { JumpBase::jl = ji; }
};

void Throw();

inline void Throw(const BaseException&) { Throw(); }

#define Try                                             \
   if (!setjmp( JumpBase::jl->env )) {                  \
   JumpBase::jl->trace = Tracer::last;               \
   JumpItem JI387256156;

#define ReThrow Throw()

#define Catch(EXCEPTION)                                \
   } else if (BaseException::Select == EXCEPTION::Select) {

#define CatchAll } else

#define CatchAndThrow  } else Throw();


//****************** cleanup heap following Throw ***********************//

class Janitor
{
protected:
   static bool do_not_link;                  // set when new is called
   bool OnStack;                             // false if created by new
public:
   Janitor* NextJanitor;
   virtual void CleanUp() {}
   Janitor();
   virtual ~Janitor();
};


// The tiresome old trick for initializing the Janitor class
// this is needed for classes derived from Janitor which have objects
// declared globally

class JanitorInitializer
{
public:
   JanitorInitializer();
private:
   static int ref_count;
};

static JanitorInitializer JanInit;

#endif                                // end of SimulateExceptions

#ifdef UseExceptions

#define Try try
#define Throw(E) throw E
#define ReThrow throw
#define Catch catch
#define CatchAll catch(...)
#define CatchAndThrow {}

#endif                                // end of UseExceptions


#ifdef DisableExceptions              // Disable exceptions

#define Try {
#define ReThrow Throw()
#define Catch(EXCEPTION) } if (false) {
#define CatchAll } if (false)
#define CatchAndThrow }

inline void Throw() { MatrixTerminate(); }
inline void Throw(const BaseException&) { MatrixTerminate(); }


#endif                                // end of DisableExceptions

#ifndef SimulateExceptions            // ! SimulateExceptions

class Janitor                         // a dummy version
{
public:
   virtual void CleanUp() {}
   Janitor() {}
   virtual ~Janitor() {}
};

#endif                                // end of ! SimulateExceptions


//******************** FREE_CHECK and NEW_DELETE ***********************//

#ifdef DO_FREE_CHECK                          // DO_FREE_CHECK
// Routines for tracing whether new and delete calls are balanced

class FreeCheck;

class FreeCheckLink
{
protected:
   FreeCheckLink* next;
   void* ClassStore;
   FreeCheckLink();
   virtual void Report()=0;                   // print details of link
   friend class FreeCheck;
};

class FCLClass : public FreeCheckLink         // for registering objects
{
   char* ClassName;
   FCLClass(void* t, char* name);
   void Report();
   friend class FreeCheck;
};

class FCLRealArray : public FreeCheckLink     // for registering real arrays
{
   char* Operation;
   int size;
   FCLRealArray(void* t, char* o, int s);
   void Report();
   friend class FreeCheck;
};

class FCLIntArray : public FreeCheckLink     // for registering int arrays
{
   char* Operation;
   int size;
   FCLIntArray(void* t, char* o, int s);
   void Report();
   friend class FreeCheck;
};


class FreeCheck
{
   static FreeCheckLink* next;
   static int BadDelete;
public:
   static void Register(void*, char*);
   static void DeRegister(void*, char*);
   static void RegisterR(void*, char*, int);
   static void DeRegisterR(void*, char*, int);
   static void RegisterI(void*, char*, int);
   static void DeRegisterI(void*, char*, int);
   static void Status();
   friend class FreeCheckLink;
   friend class FCLClass;
   friend class FCLRealArray;
   friend class FCLIntArray;
};

#define FREE_CHECK(Class)                                                  \
public:                                                                    \
   void* operator new(size_t size)                                         \
   {                                                                       \
      void* t = ::operator new(size); FreeCheck::Register(t,#Class);       \
      return t;                                                            \
   }                                                                       \
   void operator delete(void* t)                                           \
   { FreeCheck::DeRegister(t,#Class); ::operator delete(t); }


#ifdef SimulateExceptions         // SimulateExceptions

#define NEW_DELETE(Class)                                                  \
public:                                                                    \
   void* operator new(size_t size)                                         \
   {                                                                       \
      do_not_link=true;                                                    \
      void* t = ::operator new(size); FreeCheck::Register(t,#Class);       \
      return t;                                                            \
   }                                                                       \
   void operator delete(void* t)                                           \
   { FreeCheck::DeRegister(t,#Class); ::operator delete(t); }


#endif                           // end of SimulateExceptions


#define MONITOR_REAL_NEW(Operation, Size, Pointer)                         \
  FreeCheck::RegisterR(Pointer, Operation, Size);
#define MONITOR_INT_NEW(Operation, Size, Pointer)                          \
  FreeCheck::RegisterI(Pointer, Operation, Size);
#define MONITOR_REAL_DELETE(Operation, Size, Pointer)                      \
  FreeCheck::DeRegisterR(Pointer, Operation, Size);
#define MONITOR_INT_DELETE(Operation, Size, Pointer)                       \
  FreeCheck::DeRegisterI(Pointer, Operation, Size);

#else                            // DO_FREE_CHECK not defined

#define FREE_CHECK(Class) public:
#define MONITOR_REAL_NEW(Operation, Size, Pointer) {}
#define MONITOR_INT_NEW(Operation, Size, Pointer) {}
#define MONITOR_REAL_DELETE(Operation, Size, Pointer) {}
#define MONITOR_INT_DELETE(Operation, Size, Pointer) {}


#ifdef SimulateExceptions         // SimulateExceptions


#define NEW_DELETE(Class)                                                  \
public:                                                                    \
  void* operator new(size_t size)                                    \
  { do_not_link=true; void* t = ::operator new(size); return t; }    \
  void operator delete(void* t) { ::operator delete(t); }

#endif                            // end of SimulateExceptions

#endif                            // end of ! DO_FREE_CHECK

#ifndef SimulateExceptions        // ! SimulateExceptions

#define NEW_DELETE(Class) FREE_CHECK(Class)

#endif                            // end of ! SimulateExceptions


//********************* derived exceptions ******************************//

class Logic_error : public BaseException
{
public:
   static unsigned long Select;
   Logic_error(const char* a_what = 0);
};

class Runtime_error : public BaseException
{
public:
   static unsigned long Select;
   Runtime_error(const char* a_what = 0);
};

class Domain_error : public Logic_error
{
public:
   static unsigned long Select;
   Domain_error(const char* a_what = 0);
};

class Invalid_argument : public Logic_error
{
public:
   static unsigned long Select;
   Invalid_argument(const char* a_what = 0);
};

class Length_error : public Logic_error
{
public:
   static unsigned long Select;
   Length_error(const char* a_what = 0);
};

class Out_of_range : public Logic_error
{
public:
   static unsigned long Select;
   Out_of_range(const char* a_what = 0);
};

//class Bad_cast : public Logic_error
//{
//public:
//   static unsigned long Select;
//   Bad_cast(const char* a_what = 0);
//};

//class Bad_typeid : public Logic_error
//{
//public:
//   static unsigned long Select;
//   Bad_typeid(const char* a_what = 0);
//};

class Range_error : public Runtime_error
{
public:
   static unsigned long Select;
   Range_error(const char* a_what = 0);
};

class Overflow_error : public Runtime_error
{
public:
   static unsigned long Select;
   Overflow_error(const char* a_what = 0);
};

class Bad_alloc : public BaseException
{
public:
   static unsigned long Select;
   Bad_alloc(const char* a_what = 0);
};

#ifdef use_namespace
}
#endif


#endif                            // end of EXCEPTION_LIB


// body file: myexcept.cpp












