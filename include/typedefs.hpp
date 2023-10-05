/**
 * @file typedefs.hpp
 * @author Dmitrii Donskikh (deedonskihdev@gmail.com)
 * @brief type aliases
 *
 * @copyright Copyright (c) 2018-2022 Dmitrii Donskikh
 *            All rights reserved.
 */
#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

#include <array>
#include <cinttypes>
#include <memory>
#include <string>
#include <vector>

namespace serdes
{

using ByteArray       = std::vector<uint8_t>;
using SharedByteArray = std::shared_ptr<ByteArray>;
using ArraySize       = uint32_t;

}   // namespace serdes

#endif   // TYPEDEFS_HPP
