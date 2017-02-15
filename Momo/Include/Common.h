#ifndef MOMO_COMMON_INCLUDED
#define MOMO_COMMON_INCLUDED

// A macro to disallow the copy constructor and operator= functions
// This must be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#endif //MOMO_COMMON_INCLUDED
