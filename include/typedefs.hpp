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

}   // namespace serdes

#endif   // TYPEDEFS_HPP
