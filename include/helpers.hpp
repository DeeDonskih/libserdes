#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>

#include "extend_traits.hpp"
#include "typedefs.hpp"


namespace serdes {
template<typename ArgT, typename std::enable_if<is_trivial_serializable<ArgT>::value, bool>::type = true>
size_t argumentSize(const ArgT&)
{
    return sizeof(ArgT);
}

template<typename ArgT, typename std::enable_if<is_contiguous_container<ArgT>::value, bool>::type = true>
size_t argumentSize(const ArgT& arg)
{
    return arg.size();
}

template<typename... TPack>
size_t argumentSize(const std::tuple<TPack...>& arg);



namespace {
    template<std::size_t Index, class... TPack>
    struct _tuple_size_helper_
    {
        static void _tupleElementSizeAdd_(const std::tuple<TPack...>& in, size_t& t_size)
        {
            const std::size_t idx = sizeof...(TPack) - Index;
            t_size += argumentSize(std::get<idx>(in));
            _tuple_size_helper_<Index - 1, TPack...>::_tupleElementSizeAdd_(in, t_size);
        }
    };

    template<class... TPack>
    struct _tuple_size_helper_<0, TPack...>
    {
        static void _tupleElementSizeAdd_(const std::tuple<TPack...>&, size_t&) { }
    };
}   // namespace

template<typename... TPack>
size_t argumentSize(const std::tuple<TPack...>& arg)
{
    size_t retval = 0;
    _tuple_size_helper_<sizeof...(TPack), TPack...>::_tupleElementSizeAdd_(arg, retval);
    return retval;
}

}   // namespace serdes

#endif   // HELPERS_HPP
