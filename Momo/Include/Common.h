#ifndef MOMO_COMMON_INCLUDED
#define MOMO_COMMON_INCLUDED

// A macro to disallow the copy constructor and operator= functions
// This must be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#ifdef MOMO_DEBUG
# include <type_traits>
# define POD_CHECK(Type) static_assert(std::is_pod<Type>::value, #Type " is not POD!");
#else
# define POD_CHECK(Type)
#endif

#endif //MOMO_COMMON_INCLUDED
