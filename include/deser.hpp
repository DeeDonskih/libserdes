/**
 * @file deser.hpp
 * @author Dmitrii Donskikh (deedonskihdev@gmail.com)
 * @brief Generic deserialization functions
 *
 * @copyright Copyright (c) 2018-2022 Dmitrii Donskikh
 *            All rights reserved.
 */
#pragma once

#include <cstddef>
#include <cstring>
#include <tuple>
#include <type_traits>

#include "extend_traits.hpp"
#include "typedefs.hpp"

namespace serdes {
using _Iterator = typename ByteArray::pointer;
using _Offset   = size_t;

    template<class ... TPack>
    void deserialize_tuple(std::tuple<TPack...>& output, ByteArray& in, _Offset& offset);

template<typename T>
T _deserialize_trivial(ByteArray& in, _Offset& offset)
{
    T retval;
    memcpy(&retval, &in[offset], sizeof(T));
    offset += sizeof(T);
    return retval;
}

template<typename T>
T _deserialize_object(ByteArray& in, _Offset& offset)
{
    T retval;
    offset = retval.Deserialize(in, offset);
    return retval;
}

template<typename T>
T _deserialize_contiguous(ByteArray& in, _Offset& offset)
{
    using _RetValueType = typename T::value_type;
    using _RIterator    = typename T::pointer;
    size_t _retsize     = _deserialize_trivial<uint32_t>(in, offset);
    if constexpr (is_trivial_serializable<_RetValueType>::value) {
        _RIterator _begin = (_RIterator) & in[offset];
        _RIterator _end   = _begin + _retsize;
        T retval(_begin, _end);
        offset += (_retsize * sizeof(_RetValueType));
        return retval;
    } else if constexpr (is_contiguous_container<_RetValueType>::value) {
        T retval;
        for (size_t i = 0; i < _retsize; ++i) {
            retval.emplace_back(_deserialize_contiguous<_RetValueType>(in, offset));
        }
        return retval;
    } else if constexpr (is_tuple<_RetValueType>::value) {
        T retval;
        for (size_t i = 0; i < _retsize; ++i) {
            _RetValueType tuple_element;
            retval.emplace_back(deserialize_tuple(tuple_element,in,offset));
        }
        return retval;
    } else if constexpr(is_serializable_object<_RetValueType>::value) {
        T retval;
        for (size_t i = 0; i < _retsize; ++i) {
            retval.emplace_back(_deserialize_object<_RetValueType>(in,offset));
        }
        return retval;
    }
    return T();
}

template<typename T, typename std::enable_if<is_trivial_serializable<T>::value, bool>::type = true>
T deserialize(ByteArray& in, _Offset& offset)
{
    return _deserialize_trivial<T>(in, offset);
}

template<typename T, typename std::enable_if<is_contiguous_container<T>::value, bool>::type = true>
T deserialize(ByteArray& in, _Offset& offset)
{
    return _deserialize_contiguous<T>(in, offset);
}

template<typename ... T, typename std::enable_if<(sizeof...(T) > 1)>::type = true>
std::tuple<T...> deserialize(ByteArray& in, _Offset& offset)
{
    return std::make_tuple(deserialize<T>(in, offset)...);
}



template<typename T, typename std::enable_if<is_tuple<T>::value, bool>::type = true>
T deserialize(ByteArray& in, _Offset& offset)
{
    T retval;
    deserialize_tuple(retval, in, offset);
    return retval;
}

template<typename T, typename std::enable_if<is_serializable_object<T>::value, bool>::type = true>
T deserialize(ByteArray& in, _Offset& offset)
{
    return _deserialize_object<T>(in,offset);
}

namespace {
/**
     * @struct _serialize_each_ - std::tuple unfold helper class
     */
template<std::size_t Index, class ... TPack>
struct _deserialize_each_
{
    static void _tupleDeserialize_(std::tuple<TPack...>& output, ByteArray& in, _Offset& offset)
    {
        const std::size_t idx = sizeof...(TPack) - Index;
        std::get<idx>(output) = deserialize<std::tuple_element_t<idx, std::tuple<TPack...> > >(in, offset);
        _deserialize_each_<Index - 1, TPack...>::_tupleDeserialize_(output, in, offset);
    }
};

template<class ... TPack>
struct _deserialize_each_<0, TPack...>
{
    static void _tupleDeserialize_(std::tuple<TPack...>&, ByteArray&, _Offset&) { }
};
}   // namespace

template<class ... TPack>
void deserialize_tuple(std::tuple<TPack...>& output, ByteArray& in, _Offset& offset)
{
    _deserialize_each_<sizeof...(TPack), TPack...>::_tupleDeserialize_(output, in, offset);
}
}   // namespace serdes
