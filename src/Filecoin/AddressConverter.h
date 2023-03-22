// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Address.h"

namespace TW::Filecoin {

class AddressConverter {
public:
    /// Converts a Filecoin address to Ethereum.
    static bool convertToEthereum(const std::string& filecoinAddress, std::string& ethereumAddress);

    /// Converts an Ethereum address to Filecoin.
    static std::string convertFromEthereum(const std::string& ethereumAddress);
};

}
