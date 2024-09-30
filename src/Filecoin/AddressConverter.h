// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

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
