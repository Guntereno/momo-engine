#ifndef MOMO_COMMON_INCLUDED
#define MOMO_COMMON_INCLUDED

#include <type_traits>

// A macro to disallow the copy constructor and operator= functions
// This must be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// Macro to compile time asser that a type is POD
#ifdef MOMO_DEBUG
# define POD_CHECK(Type) static_assert(std::is_pod<Type>::value, #Type " is not POD!");
#else
# define POD_CHECK(Type)
#endif

// Use to surpress warnings about unused types
#define UNUSED(expr) do { (void)(expr); } while (0)

// Macro to define bitwise operators for enum class types
#define FLAG_OPS(enumType)                                      \
inline enumType operator|(enumType a, enumType b)               \
{                                                               \
	return static_cast<enumType>(                               \
		static_cast<std::underlying_type<enumType>::type>(a) |  \
		static_cast<std::underlying_type<enumType>::type>(b));  \
}                                                               \
inline enumType operator&(enumType a, enumType b)               \
{                                                               \
	return static_cast<enumType>(                               \
		static_cast<std::underlying_type<enumType>::type>(a) &  \
		static_cast<std::underlying_type<enumType>::type>(b));  \
}                                                               \
inline enumType& operator|=(enumType& a, enumType b)            \
{                                                               \
	return a = a | b;                                           \
}

#endif //MOMO_COMMON_INCLUDED
