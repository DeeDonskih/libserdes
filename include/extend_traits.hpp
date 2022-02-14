/**
 * @file extend_traits.hpp
 * @author Dmitry Donskikh (deedonskihdev@gmail.com)
 * @brief 
 * @version 1.3
 * @date 2018-03-15
 * 
 * @copyright Copyright (c) 2018-2021 Dmitriy Donskikh
 *            All rights reserved.
 */

#ifndef EXTEND_TYPE_TRAITS_HPP
#define EXTEND_TYPE_TRAITS_HPP

#include <iterator>
#include <type_traits>
#include <utility>
#include <tuple>

#include <iostream>

#define EXTEND_TRAITS_DEFINE_HAS_TYPE(MemType)                                      \
    template<typename, typename = std::void_t<>>                                    \
    struct HasTypeT_##MemType : std::false_type                                     \
    {                                                                               \
    };                                                                              \
    template<typename T>                                                            \
    struct HasTypeT_##MemType<T, std::void_t<typename T::MemType>> : std::true_type \
    {                                                                               \
    }   // ; intentionally skipped

#define EXTEND_TRAITS_DEFINE_HAS_MEMBER(Member)                                       \
    template<typename, typename = std::void_t<>>                                      \
    struct HasMemberT_##Member : std::false_type                                      \
    {                                                                                 \
    };                                                                                \
    template<typename T>                                                              \
    struct HasMemberT_##Member<T, std::void_t<decltype(&T::Member)>> : std::true_type \
    {                                                                                 \
    }


namespace serdes
{

template<typename, typename = std::void_t<>>
struct is_pod_new: std::false_type
{
};
template<typename T>
struct is_pod_new<
    T,
    typename std::enable_if<((std::is_standard_layout<T>::value) && (std::is_trivial<T>::value))>::type>
    : std::true_type
{
};

template<typename, typename = std::void_t<>>
struct is_trivial_serializable : std::false_type
{
};
template<typename T>
struct is_trivial_serializable<
    T,
    typename std::enable_if<((is_pod_new<T>::value) || (std::is_arithmetic<T>::value))>::type>
    : std::true_type
{
};

template<typename T>
inline constexpr bool is_trivial_serializable_V = (is_pod_new<T>::value) || (std::is_arithmetic<T>::value);


EXTEND_TRAITS_DEFINE_HAS_TYPE(value_type);
EXTEND_TRAITS_DEFINE_HAS_TYPE(iterator);
EXTEND_TRAITS_DEFINE_HAS_TYPE(difference_type);
EXTEND_TRAITS_DEFINE_HAS_TYPE(size_type);

template<typename, typename = std::void_t<>>
struct is_std_container : std::false_type
{
    using container_type = std::void_t<>;
};
template<typename T>
struct is_std_container<
    T,
    typename std::enable_if<(
        (HasTypeT_value_type<T>::value) && (HasTypeT_iterator<T>::value) &&
        (HasTypeT_difference_type<T>::value) && (HasTypeT_size_type<T>::value))>::type> : std::true_type
{
    using container_type = T;
};

EXTEND_TRAITS_DEFINE_HAS_TYPE(iterator_category);

template<typename, typename = std::void_t<>>
struct is_random_access_iteratior : std::false_type
{
};
template<typename T>
struct is_random_access_iteratior<
    T,
    typename std::enable_if<std::is_same<T, std::random_access_iterator_tag>::value>::type> : std::true_type
{
};

template<typename, typename = std::void_t<>>
struct is_forward_iteratior : std::false_type
{
};
template<typename Iterator>
struct is_forward_iteratior<
    Iterator,
    typename std::enable_if<std::is_same<Iterator, std::forward_iterator_tag>::value>::type> : std::true_type
{
};

template<typename Container, bool = is_std_container<Container>::value, typename = std::void_t<>>
struct has_random_access_iteratior : std::false_type
{
};
template<typename Container>
struct has_random_access_iteratior<
    Container,
    true,
    typename std::enable_if<
        is_random_access_iteratior<typename Container::iterator::iterator_category>::value>::type>
    : std::true_type
{
};

template<typename Container, bool = is_std_container<Container>::value, typename = std::void_t<>>
struct has_forward_iteratior : std::false_type
{
};
template<typename Container>
struct has_forward_iteratior<
    Container,
    true,
    typename std::enable_if<
        is_forward_iteratior<typename Container::iterator::iterator_category>::value>::type> : std::true_type
{
};

template<typename, typename = std::void_t<>>
struct is_contiguous_container : std::false_type
{
};
template<typename T>
struct is_contiguous_container<T, typename std::enable_if<has_random_access_iteratior<T>::value>::type>
    : std::true_type
{
};

template<typename, typename = std::void_t<>>
struct is_fw_iterable_container : std::false_type
{
};
template<typename T>
struct is_fw_iterable_container<T, typename std::enable_if<has_forward_iteratior<T>::value>::type>
    : std::true_type
{
};


template<typename T>
struct is_tuple_impl : std::false_type {};

template<typename... Ts>
struct is_tuple_impl<std::tuple<Ts...>> :std::true_type {};

template<typename T>
struct is_tuple : is_tuple_impl<std::decay_t<T>> {};


template<typename T>
void print_traits()
{
  if constexpr(std::is_const_v<T>)
  {
    std::cout<<"Constant ";
  }
  if constexpr(std::is_volatile_v<T>)
  {
    std::cout<<"Volatile ";
  }
  if constexpr(std::is_reference_v<T>)
  {
    std::cout<<"Reference ";
  }
  if constexpr(std::is_rvalue_reference_v<T>)
  {
    std::cout<<"RV_Reference ";
  }
  if constexpr(std::is_lvalue_reference_v<T>)
  {
    std::cout<<"LV_Reference ";
  }
  if constexpr(std::is_arithmetic_v<T>)
  {
    std::cout<<"Arithmetic ";
  }
  if constexpr(std::is_integral_v<T>)
  {
    std::cout<<"Integral ";
  }
  if constexpr(std::is_floating_point_v<T>)
  {
    std::cout<<"Float ";
  }
  if constexpr(is_pod_new<T>::value)
  {
    std::cout<<"POD ";
  }
  if constexpr(is_std_container<T>::value)
  {
    std::cout<<"Container ";
  }
  if constexpr(is_contiguous_container<T>::value)
  {
    std::cout<<"VectorLike ";
  }
  if constexpr(is_fw_iterable_container<T>::value)
  {
    std::cout<<"ListLike ";
  }

  std::cout<<std::endl;
}

template<typename T>
void print_traits([[maybe_unused]]T val)
{print_traits<T>();}

}   // namespace serdes


#endif   // EXTEND_TYPE_TRAITS_HPP
