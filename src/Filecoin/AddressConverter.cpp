// Copyright © 2017-2023 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AddressConverter.h"

#include "Ethereum/Address.h"

namespace TW::Filecoin {

/// The actor ID of the Ethereum Address Manager singleton.
static constexpr uint64_t ETHEREUM_ADDRESS_MANAGER_ACTOR_ID = 10;

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
bool AddressConverter::convertToEthereum(const std::string& filecoinAddress, std::string& ethereumAddress) {
    // This may throw an exception if the given address is invalid.
    Address addr(filecoinAddress);

    switch (addr.type) {
        case Address::Type::ID: {
            Data payload(Ethereum::Address::size, 0);
            payload[0] = 0xFF;
            putBigEndian(payload, 12, addr.actorID);

            Ethereum::Address ethAddr(payload);
            ethereumAddress = ethAddr.string();
            return true;
        }
        case Address::Type::DELEGATED: {
            if (addr.actorID != ETHEREUM_ADDRESS_MANAGER_ACTOR_ID) {
                return false;
            }

            if (addr.payload.size() != Ethereum::Address::size) {
                return false;
            }

            Ethereum::Address ethAddr(addr.payload);
            ethereumAddress = ethAddr.string();
            return true;
        }
        default:
            return false;
    }
}

std::string AddressConverter::convertFromEthereum(const std::string& ethereumAddress) {
    // This may throw an exception if the given address is invalid.
    Ethereum::Address addr(ethereumAddress);

    Data addrPayload(addr.bytes.begin(), addr.bytes.end());
    Address filecoinAddr(Address::Type::DELEGATED, ETHEREUM_ADDRESS_MANAGER_ACTOR_ID, addrPayload);

    return filecoinAddr.string();
}

}
