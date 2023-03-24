// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AddressConverter.h"

#include "Ethereum/Address.h"

namespace TW::Filecoin {

static byte toByte(uint64_t num) {
  return num & 0xFF;
}

/// https://cs.opensource.google/go/go/+/refs/tags/go1.20.2:src/encoding/binary/binary.go;l=184
static void putBigEndian(Data& dest, std::size_t pos, uint64_t num) {
    assert(dest.size() >= pos + sizeof(num));

    dest[pos] = toByte(num >> 56);
    dest[pos + 1] = toByte(num >> 48);
    dest[pos + 2] = toByte(num >> 40);
    dest[pos + 3] = toByte(num >> 32);
    dest[pos + 4] = toByte(num >> 24);
    dest[pos + 5] = toByte(num >> 16);
    dest[pos + 6] = toByte(num >> 8);
    dest[pos + 7] = toByte(num);
}

/// https://github.com/filecoin-project/lotus/blob/61f29a84b5a61060c4ac8aabe9b9360a2cdf5e7e/chain/types/ethtypes/eth_types.go#L279
std::optional<std::string> AddressConverter::convertToEthereumString(const std::string& filecoinAddress) {
    // This may throw an exception if the given address is invalid.
    Address addr(filecoinAddress);
    if (auto eth_opt = convertToEthereum(addr); eth_opt) {
        return eth_opt->string();
    }
    return std::nullopt;
}

std::optional<Ethereum::Address> AddressConverter::convertToEthereum(const Address& filecoinAddress) {
    switch (filecoinAddress.type) {
        case Address::Type::ID: {
            Data payload(Ethereum::Address::size, 0);
            payload[0] = 0xFF;
            putBigEndian(payload, 12, filecoinAddress.actorID);

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

    Data addrPayload(addr.bytes.begin(), addr.bytes.end());
    Address filecoinAddr = Address::delegatedAddress(Address::ETHEREUM_ADDRESS_MANAGER_ACTOR_ID, addrPayload);

    return filecoinAddr.string();
}

Address AddressConverter::convertFromEthereum(const Ethereum::Address& ethereumAddress) noexcept {
    Data addrPayload(ethereumAddress.bytes.begin(), ethereumAddress.bytes.end());
    return Address::delegatedAddress(Address::ETHEREUM_ADDRESS_MANAGER_ACTOR_ID, addrPayload);
}

}
