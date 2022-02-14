#ifndef SER_HPP
#define SER_HPP

#include <type_traits>
#include <tuple>

#include "extend_traits.hpp"
#include "typedefs.hpp"

namespace serdes {
template<class... TPack>
ByteArray& serialize_tuple(ByteArray& output, const std::tuple<TPack...>& tuple);

template<typename T>
ByteArray& _serialize();

template<typename T, typename std::enable_if<is_trivial_serializable<T>::value, bool>::type = true>
ByteArray _serialize(const T& value)
{
    using _Iterator = uint8_t*;   // typename ByteArray::pointer;

    _Iterator _begin = (_Iterator) & value;
    return ByteArray(_begin, _begin + sizeof(T));
}

template<typename T, typename std::enable_if<is_contiguous_container<T>::value, bool>::type = true>
ByteArray _serialize(const T& value)
{
    using _Element  = typename T::value_type;   // typename T::typename;
    using _Iterator = typename ByteArray::pointer;

    _Iterator _begin = (_Iterator)value.data();
    _Iterator _end   = _begin + (sizeof(_Element) * value.size());
    ByteArray retval = _serialize<uint32_t>(value.size());
    for (auto& x:value) {
        auto serialized_element= _serialize(x);
        std::copy(serialized_element.begin(), serialized_element.end(), std::back_inserter(retval));
    }
    return retval;
}

template<typename T, typename std::enable_if<is_trivial_serializable<T>::value, bool>::type = true>
ByteArray& _serialize(ByteArray& output, const T& value)
{
    using _Iterator = typename ByteArray::pointer;   // typename ByteArray::pointer;

    _Iterator _begin = (_Iterator) & value;
    std::copy(_begin, _begin + sizeof(T), std::back_inserter(output));

    return output;
}

template<typename T, typename std::enable_if<is_contiguous_container<T>::value, bool>::type = true>
ByteArray& _serialize(ByteArray& output, const T& value)
{
    using _Element  = typename T::value_type;   // typename T::typename;
    using _Iterator = typename ByteArray::pointer;

    _Iterator _begin = (_Iterator)value.data();
    _Iterator _end   = _begin + (sizeof(_Element) * value.size());

    _serialize<uint32_t>(output, value.size());
    std::copy(_begin, _end, std::back_inserter(output));

    return output;
}

template<typename T, typename std::enable_if<is_tuple<T>::value, bool>::type = true>
ByteArray& _serialize(ByteArray& output, const T& value)
{
    return serialize_tuple(output, value);
}

template<typename T, typename std::enable_if<is_tuple<T>::value, bool>::type = true>
ByteArray _serialize(const T& value)
{
    ByteArray retval{};
    serialize_tuple(retval, value);
    return retval;
}

template<typename T>
ByteArray& serialize(ByteArray& output, T& value)
{
    return _serialize<typename std::remove_reference<T>::type>(output, value);
}

template<typename T>
ByteArray& serialize(ByteArray& output, T&& value)
{
    return _serialize<typename std::remove_reference<T>::type>(output, value);
}

template<typename T>
ByteArray serialize(T& value)
{
    return _serialize<typename std::remove_reference<T>::type>(value);
}

template<typename T>
ByteArray serialize(T&& value)
{
    return _serialize<typename std::remove_reference<T>::type>(value);
}

template<typename ... TPack>
ByteArray& serialize(ByteArray& output, TPack&&... value)
{
    (serialize(output, std::forward<TPack>(value)), ...);
    return output;
}

namespace {
/**
     * @struct _serialize_each_ - std::tuple unfold helper class
     */
template<std::size_t Index, class ... TPack>
struct _serialize_each_
{
    static void _tupleSerialize_(serdes::ByteArray& output, const std::tuple<TPack...>& tuple)
    {
        const std::size_t idx = sizeof...(TPack) - Index;
        serdes::serialize(output, std::get<idx>(tuple));
        _serialize_each_<Index - 1, TPack...>::_tupleSerialize_(output, tuple);
    }
};

template<class ... TPack>
struct _serialize_each_<0, TPack...>
{
    static void _tupleSerialize_(serdes::ByteArray&, const std::tuple<TPack...>&) { }
};
}   // namespace

template<class... TPack>
ByteArray& serialize_tuple(ByteArray& output, const std::tuple<TPack...>& tuple)
{
    _serialize_each_<sizeof...(TPack), TPack...>::_tupleSerialize_(output, tuple);
    return output;
}
}   // namespace serdes

#endif   // SER_HPP
