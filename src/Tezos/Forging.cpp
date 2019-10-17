// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "BinaryCoding.h"
#include "../Base58.h"
#include "../Data.h"
#include "../HexCoding.h"
#include "../proto/Tezos.pb.h"

#include <sstream>

using namespace TW;
using namespace TW::Tezos;
using namespace TW::Tezos::Proto;

// Forge the given boolean into a hex encoded string.
Data forgeBool(bool input) {
    unsigned char result = input ? 0xff : 0x00;
    return Data{result};
}

// Forge the given public key hash into a hex encoded string.
// Note: This function supports tz1, tz2 and tz3 addresses.
Data forgePublicKeyHash(const std::string& publicKeyHash) {
    Data forged = Data();
    // Adjust prefix based on tz1, tz2 or tz3.
    switch ((char)publicKeyHash[2]) {
    case '1':
        forged.push_back(0x00);
        break;
    case '2':
        forged.push_back(0x01);
        break;
    case '3':
        forged.push_back(0x02);
        break;
    default:
        throw std::invalid_argument("Invalid Prefix");
    }
    const auto decoded = Base58::bitcoin.decodeCheck(publicKeyHash);
    const auto prefixSize = 3;
    forged.insert(forged.end(), decoded.begin() + prefixSize, decoded.end());
    return forged;
}

// Forge the given public key into a hex encoded string.
Data forgePublicKey(PublicKey publicKey) {
    std::array<uint8_t, 4> prefix = {13, 15, 37, 217};
    auto data = Data(prefix.begin(), prefix.end());
    auto bytes = Data(publicKey.bytes.begin(), publicKey.bytes.end());
    append(data, bytes);

    auto pk = Base58::bitcoin.encodeCheck(data);
    auto decoded = "00" + base58ToHex(pk, 4, prefix.data());
    return parse_hex(decoded);
}

// Forge the given zarith hash into a hex encoded string.
Data forgeZarith(uint64_t input) {
    Data forged = Data();
    while (input > 0x80) {
        forged.push_back(static_cast<byte>((input & 0xff) | 0x80));
        input >>= 7;
    }
    forged.push_back(static_cast<byte>(input));
    return forged;
}

// Forge the given operation.
Data forgeOperation(const Operation& operation) {
    auto forged = Data();
    auto source = Address(operation.source());
    auto forgedSource = source.forge();
    auto forgedFee = forgeZarith(operation.fee());
    auto forgedCounter = forgeZarith(operation.counter());
    auto forgedGasLimit = forgeZarith(operation.gas_limit());
    auto forgedStorageLimit = forgeZarith(operation.storage_limit());

    if (operation.kind() == Operation_OperationKind_REVEAL) {
        auto publicKey = PublicKey(operation.reveal_operation_data().public_key(), TWPublicKeyTypeED25519);
        auto forgedPublicKey = forgePublicKey(publicKey);

        /* Uncomment for debugging */
        auto forgedSourceHex = hex(forgedSource.begin(), forgedSource.end());
        auto forgedFeeHex = hex(forgedFee.begin(), forgedFee.end());
        auto forgedCounterHex = hex(forgedCounter.begin(), forgedCounter.end());
        auto forgedGasLimitHex = hex(forgedGasLimit.begin(), forgedGasLimit.end());
        auto forgedStorageLimitHex = hex(forgedStorageLimit.begin(), forgedStorageLimit.end());
        auto forgedPublicKeyHex = hex(forgedPublicKey.begin(), forgedPublicKey.end());


        forged.push_back(Operation_OperationKind_REVEAL);
        append(forged, forgedSource);
        append(forged, forgedFee);
        append(forged, forgedCounter);
        append(forged, forgedGasLimit);
        append(forged, forgedStorageLimit);
        append(forged, forgedPublicKey);
        return forged;
    }

    if (operation.kind() == Operation_OperationKind_DELEGATION) {
        auto delegate = operation.delegation_operation_data().delegate();
        auto forgedPublicKey = forgePublicKeyHash(delegate);

        /* Uncomment for debugging
        auto forgedSourceHex = hex(forgedSource.begin(), forgedSource.end());
        auto forgedFeeHex = hex(forgedFee.begin(), forgedFee.end());
        auto forgedCounterHex = hex(forgedCounter.begin(), forgedCounter.end());
        auto forgedGasLimitHex = hex(forgedGasLimit.begin(), forgedGasLimit.end());
        auto forgedStorageLimitHex = hex(forgedStorageLimit.begin(), forgedStorageLimit.end());
        auto forgedPublicKeyHex = hex(forgedPublicKey.begin(), forgedPublicKey.end());
        */

        forged.push_back(Operation_OperationKind_DELEGATION);
        append(forged, forgedSource);
        append(forged, forgedFee);
        append(forged, forgedCounter);
        append(forged, forgedGasLimit);
        append(forged, forgedStorageLimit);
        if (!delegate.empty()) {
            append(forged, forgeBool(true));
            append(forged, forgedPublicKey);
        } else {
            append(forged, forgeBool(false));
        }
        return forged;
    }

    if (operation.kind() == Operation_OperationKind_TRANSACTION) {
        auto forgedAmount = forgeZarith(operation.transaction_operation_data().amount());
        auto forgedDestination = Address(operation.transaction_operation_data().destination()).forge();

        /* Uncomment for debug
        auto forgedSourceHex = hex(forgedSource.begin(), forgedSource.end());
        auto forgedFeeHex = hex(forgedFee.begin(), forgedFee.end());
        auto forgedCounterHex = hex(forgedCounter.begin(), forgedCounter.end());
        auto forgedGasLimitHex = hex(forgedGasLimit.begin(), forgedGasLimit.end());
        auto forgedStorageLimitHex = hex(forgedStorageLimit.begin(), forgedStorageLimit.end());
        auto forgedAmountHex = hex(forgedAmount.begin(), forgedAmount.end());
        auto forgedDestinationHex = hex(forgedDestination.begin(), forgedDestination.end());
        */

        forged.push_back(Operation_OperationKind_TRANSACTION);
        append(forged, forgedSource);
        append(forged, forgedFee);
        append(forged, forgedCounter);
        append(forged, forgedGasLimit);
        append(forged, forgedStorageLimit);
        append(forged, forgedAmount);
        append(forged, forgeBool(false));
        append(forged, forgedDestination);
        append(forged, forgeBool(false));
        return forged;
    }

    throw std::invalid_argument("Invalid operation kind");
}
