//
// Created by Roman Sztergbaum on 27/09/2022.
//

#include "BCS.h"

namespace TW::BCS {

Serializer& operator<<(Serializer& stream, std::byte b) noexcept {
    stream.add_byte(b);
    return stream;
}

Serializer& operator<<(Serializer& stream, uleb128 t) noexcept {
    std::integral auto value = t.value;

    while (value >= 0x80) {
        // Add the 7 lowest bits of data and set highest bit to 1
        stream << static_cast<uint8_t>((value & 0x7f) | 0x80);
        value >>= 7;
    }

    // Add the remaining bits of data (highest bit is already 0 at this point)
    stream << static_cast<uint8_t>(value);
    return stream;
}

Serializer& operator<<(Serializer& stream, std::string_view sv) noexcept {
    stream << uleb128{static_cast<uint32_t>(sv.size())};
    stream.add_bytes(sv.begin(), sv.end());
    return stream;
}

Serializer& operator<<(Serializer& stream, std::nullopt_t) noexcept {
    stream << false;
    return stream;
}

} // namespace TW::BCS
