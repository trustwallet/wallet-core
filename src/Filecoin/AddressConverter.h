// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <optional>

#include "Address.h"
#include "Ethereum/Address.h"

namespace TW::Filecoin {

using MaybeAddressString = std::optional<std::string>;
using MaybeEthAddress = std::optional<Ethereum::Address>;

class AddressConverter {
public:
    /// Converts a Filecoin address to Ethereum.
    static MaybeAddressString convertToEthereumString(const std::string& filecoinAddress);

    /// Converts a Filecoin address to Ethereum.
    static MaybeEthAddress convertToEthereum(const Address& filecoinAddress);

    /// Converts an Ethereum address to Filecoin.
    static std::string convertFromEthereumString(const std::string& ethereumAddress);

    /// Converts an Ethereum address to Filecoin.
    static Address convertFromEthereum(const Ethereum::Address& ethereumAddress) noexcept;
};

}
