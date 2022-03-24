/**
 * @file test_class.cpp
 * @author Dmitry Donskikh (deedonskihdev@gmail.com)
 * @brief
 *
 * @copyright Copyright (c) 2018-2022 Dmitriy Donskikh
 *            All rights reserved.
 */

#include "test_class.hpp"

#include "../serdes.hpp"

namespace serdes::test {
using namespace std::string_literals;
namespace internal {
using TestClassTuple = std::tuple<std::string, uint64_t, uint8_t>;
}

TestClass::TestClass(std::string name, uint64_t id, Count count)
    : _name(name)
    , _id(id)
    , _count(count)
{ }

std::string TestClass::toString()
{
    return "Test class: "s + _name + "\nID: "s + std::to_string(_id)
           + "\nCount: " + std::to_string(static_cast<uint8_t>(_count));
}

std::vector<uint8_t> TestClass::Serialize() const
{
    return serdes::serialize<internal::TestClassTuple>(
        std::make_tuple(_name, _id, static_cast<uint8_t>(_count)));
}

size_t TestClass::Deserialize(std::vector<uint8_t>& buffer, size_t offset)
{
    auto result_tpl = serdes::deserialize<internal::TestClassTuple>(buffer, offset);
    _name           = std::get<0>(result_tpl);
    _id             = std::get<1>(result_tpl);
    _count          = static_cast<Count>(std::get<2>(result_tpl));
    return offset;
}

bool TestClass::operator==(const TestClass& rhs) const
{
    return _name == rhs._name && _id == rhs._id && _count == rhs._count;
}

bool TestClass::operator!=(const TestClass& rhs) const
{
    return !(rhs == *this);
}
}   // namespace serdes::test
