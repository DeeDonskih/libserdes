/**
 * @file test_class.hpp
 * @author Dmitrii Donskikh (deedonskihdev@gmail.com)
 * @brief
 *
 * @copyright Copyright (c) 2018-2022 Dmitrii Donskikh
 *            All rights reserved.
 */

#ifndef LIBSERDES_TEST_CLASS_HPP
#define LIBSERDES_TEST_CLASS_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace serdes::test {
enum class Count
{
    NONE   = uint8_t(0),
    FIRST  = uint8_t(1),
    SECOND = uint8_t(2),
    THIRD,
    WRONG
};

class TestClass
{
    std::string _name{ "Default name" };
    uint64_t _id{ 0xABCD1234 };
    Count _count{ Count::NONE };

public:
    TestClass() = default;
    TestClass(std::string name, uint64_t id, Count count);

    std::string toString();

    /**
     * @brief serialize object to array of bytes
     * @return
     */
    std::vector<uint8_t> Serialize() const;
    /**
     * @brief deserialize object from raw buffer(array of bytes)
     * @param buffer - raw buffer with serialized object
     * @param offset - offset to serialized object
     * @return offset to position after serialized object
     */
    size_t Deserialize(std::vector<uint8_t>& buffer, size_t offset = 0);

    bool operator==(const TestClass& rhs) const;

    bool operator!=(const TestClass& rhs) const;
};
}   // namespace serdes::test

#endif   // LIBSERDES_TEST_CLASS_HPP
