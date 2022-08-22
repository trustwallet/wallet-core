// Copyright © 2017-2020 Trust Wallet.
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

// https://github.com/ecadlabs/taquito/blob/master/packages/taquito-local-forging/src/codec.ts#L19
Data forgePrefix(std::array<TW::byte, 3> prefix, const std::string& val) {
    const auto decoded = Base58::bitcoin.decodeCheck(val);
    if (!std::equal(prefix.begin(), prefix.end(), decoded.begin())) {
        throw std::invalid_argument("prefix not match");
    }

    const auto prefixSize = 3;
    Data forged = Data();
    forged.insert(forged.end(), decoded.begin() + prefixSize, decoded.end());
    return forged;
}

// Forge the given public key into a hex encoded string.
Data forgePublicKey(PublicKey publicKey) {
    std::string tag;
    std::array<uint8_t, 4> prefix;
    if (publicKey.type == TWPublicKeyTypeED25519) {
        prefix = {13, 15, 37, 217};
        tag = "00";
    } else if (publicKey.type == TWPublicKeyTypeSECP256k1) {
        prefix = {3, 254, 226, 86};
        tag = "01";
    }
    auto data = Data(prefix.begin(), prefix.end());
    auto bytes = Data(publicKey.bytes.begin(), publicKey.bytes.end());
    append(data, bytes);

    auto pk = Base58::bitcoin.encodeCheck(data);
    auto decoded = tag + base58ToHex(pk, 4);
    return parse_hex(decoded);
}

// Forge the given zarith hash into a hex encoded string.
Data forgeZarith(uint64_t input) {
    Data forged = Data();
    while (input >= 0x80) {
        forged.push_back(static_cast<TW::byte>((input & 0xff) | 0x80));
        input >>= 7;
    }
    forged.push_back(static_cast<TW::byte>(input));
    return forged;
}

// Forge the given operation.
Data forgeOperation(const Operation& operation) {
    auto forged = Data();
    auto source = Address(operation.source());
    auto forgedSource = source.forgePKH(); //https://github.com/ecadlabs/taquito/blob/master/packages/taquito-local-forging/src/schema/operation.ts#L40
    auto forgedFee = forgeZarith(operation.fee());
    auto forgedCounter = forgeZarith(operation.counter());
    auto forgedGasLimit = forgeZarith(operation.gas_limit());
    auto forgedStorageLimit = forgeZarith(operation.storage_limit());

    if (operation.kind() == Operation_OperationKind_REVEAL) {
        enum TWPublicKeyType type;
        if (operation.reveal_operation_data().public_key().size() == 32) {
            type = TWPublicKeyTypeED25519;
        } else if (operation.reveal_operation_data().public_key().size() == 33) {
            type = TWPublicKeyTypeSECP256k1;
        } else {
            throw std::invalid_argument("unsupported public key type");
        }
        auto publicKey = PublicKey(data(operation.reveal_operation_data().public_key()), type);
        auto forgedPublicKey = forgePublicKey(publicKey);

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

        forged.push_back(Operation_OperationKind_DELEGATION);
        append(forged, forgedSource);
        append(forged, forgedFee);
        append(forged, forgedCounter);
        append(forged, forgedGasLimit);
        append(forged, forgedStorageLimit);
        if (!delegate.empty()) {
            auto forgedPublicKeyHash = forgePublicKeyHash(delegate);

            append(forged, forgeBool(true));
            append(forged, forgedPublicKeyHash);
        } else {
            append(forged, forgeBool(false));
        }
        return forged;
    }

    if (operation.kind() == Operation_OperationKind_TRANSACTION) {
        auto forgedAmount = forgeZarith(operation.transaction_operation_data().amount());
        auto forgedDestination = Address(operation.transaction_operation_data().destination()).forge();

        forged.push_back(Operation_OperationKind_TRANSACTION);
        append(forged, forgedSource);
        append(forged, forgedFee);
        append(forged, forgedCounter);
        append(forged, forgedGasLimit);
        append(forged, forgedStorageLimit);
        append(forged, forgedAmount);
//        append(forged, forgeBool(false)); // address type prefix, included in address.forge()
        append(forged, forgedDestination);
        if (operation.transaction_operation_data().encoded_parameter().empty()) {
            append(forged, forgeBool(false));
        } else {
            append(forged, TW::data(operation.transaction_operation_data().encoded_parameter()));
        }
        return forged;
    }

    throw std::invalid_argument("Invalid operation kind");
}
