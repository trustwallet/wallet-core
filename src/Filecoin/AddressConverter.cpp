// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "AddressConverter.h"

#include "BinaryCoding.h"
#include "Ethereum/Address.h"

namespace TW::Filecoin {

static constexpr std::size_t ACTOR_ID_ENCODED_LEN = sizeof(uint64_t);

/// https://github.com/filecoin-project/lotus/blob/61f29a84b5a61060c4ac8aabe9b9360a2cdf5e7e/chain/types/ethtypes/eth_types.go#L279
MaybeAddressString AddressConverter::convertToEthereumString(const std::string& filecoinAddress) {
    // This may throw an exception if the given address is invalid.
    Address addr(filecoinAddress);
    if (auto &&eth_opt = convertToEthereum(addr); eth_opt) {
        return eth_opt->string();
    }
    return std::nullopt;
}

MaybeEthAddress AddressConverter::convertToEthereum(const Address& filecoinAddress) {
    switch (filecoinAddress.type) {
        case Address::Type::ID: {
            Data payload(Ethereum::Address::size - ACTOR_ID_ENCODED_LEN, 0);
            payload[0] = 0xFF;

            Data encodedActorID;
            encodedActorID.reserve(ACTOR_ID_ENCODED_LEN);
            encode64BE(filecoinAddress.actorID, encodedActorID);

            append(payload, encodedActorID);

            Ethereum::Address ethAddr(payload);
            return ethAddr;
        }
        case Address::Type::DELEGATED: {
            if (filecoinAddress.actorID != Address::ETHEREUM_ADDRESS_MANAGER_ACTOR_ID) {
                return std::nullopt;
            }

            if (filecoinAddress.payload.size() != Ethereum::Address::size) {
                return std::nullopt;
            }

            Ethereum::Address ethAddr(filecoinAddress.payload);
            return ethAddr;
        }
        default:
            return std::nullopt;
    }
}

std::string AddressConverter::convertFromEthereumString(const std::string& ethereumAddress) {
    // This may throw an exception if the given address is invalid.
    Ethereum::Address addr(ethereumAddress);

    return convertFromEthereum(addr).string();
}

Address AddressConverter::convertFromEthereum(const Ethereum::Address& ethereumAddress) noexcept {
    Data addrPayload(ethereumAddress.bytes.begin(), ethereumAddress.bytes.end());
    return Address::delegatedAddress(Address::ETHEREUM_ADDRESS_MANAGER_ACTOR_ID, std::move(addrPayload));
}

}
