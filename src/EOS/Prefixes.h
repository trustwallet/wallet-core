// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <stdexcept>
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
    throw std::invalid_argument("Unknown EOS key type");
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
    throw std::invalid_argument("Unknown EOS key type");
}
} // namespace TW::EOS
