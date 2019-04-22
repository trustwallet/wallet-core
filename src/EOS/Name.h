#pragma once

#include "../Bravo/Serialization.h"
#include "../BinaryCoding.h"

namespace TW::EOS {
    class Name: Bravo::Serializable {
    public:
        uint64_t value = 0;

        Name() { }
        Name(const std::string& str);
        uint64_t toSymbol(char c) const;
        std::string string() const;

        void serialize(Data& o) const;
    };
} // namespace TW::EOS