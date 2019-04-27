#pragma once

#include "../BinaryCoding.h"
#include "../Data.h"

using Data = TW::Data;

namespace TW::EOS {
class Name {
public:
    uint64_t value = 0;

    Name() { }
    Name(const std::string& str);
    uint64_t toSymbol(char c) const noexcept;
    std::string string() const noexcept;

    void serialize(Data& o) const noexcept;
};
} // namespace TW::EOS