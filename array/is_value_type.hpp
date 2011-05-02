#ifndef IS_VALUE_TYPE_HPP
#define IS_VALUE_TYPE_HPP

template<class T> bool is_value_type() { return false; }
template<> bool is_value_type<char>() { return true; }
template<> bool is_value_type<unsigned char>() { return true; }
template<> bool is_value_type<signed char>() { return true; }
template<> bool is_value_type<unsigned short>() { return true; }
template<> bool is_value_type<signed short>() { return true; }
template<> bool is_value_type<unsigned>() { return true; }
template<> bool is_value_type<signed>() { return true; }
template<> bool is_value_type<unsigned long>() { return true; }
template<> bool is_value_type<signed long>() { return true; }
template<> bool is_value_type<float>() { return true; }
template<> bool is_value_type<double>() { return true; }
template<> bool is_value_type<long double>() { return true; }

#endif //IS_VALUE_TYPE_HPP