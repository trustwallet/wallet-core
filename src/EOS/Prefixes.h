// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

namespace TW::EOS {

enum class Type {
    Legacy,
    ModernK1,
    ModernR1,
};

namespace Legacy {
static const std::string prefix = "EOS";
};

namespace Modern {
static const std::string pubBasePrefix = "PUB";
static const std::string sigBasePrefix = "SIG";

namespace K1 {
static const std::string prefix = "K1";
static const std::string fullPubPrefix = pubBasePrefix + "_" + Modern::K1::prefix + "_";
static const std::string fullSigPrefix = sigBasePrefix + "_" + Modern::K1::prefix + "_";
};

namespace R1 {
static const std::string prefix = "R1";
static const std::string fullPubPrefix = pubBasePrefix + "_" + Modern::R1::prefix + "_";
static const std::string fullSigPrefix = sigBasePrefix + "_" + Modern::R1::prefix + "_";
};
};

inline std::string pubPrefixForType(Type t) {
    switch (t) {
    case Type::Legacy:
        return Legacy::prefix;

    case Type::ModernK1:
        return Modern::K1::fullPubPrefix;

    case Type::ModernR1:
        return Modern::R1::fullPubPrefix;
    }
}

inline std::string sigPrefixForType(Type t) {
    switch (t) {
    case Type::Legacy:
        return Legacy::prefix;

    case Type::ModernK1:
        return Modern::K1::fullSigPrefix;

    case Type::ModernR1:
        return Modern::R1::fullSigPrefix;
    }
}
} // namespace TW::EOS
