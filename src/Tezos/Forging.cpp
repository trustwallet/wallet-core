// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Forging.h"
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

Data forgeInt32(int value, int len) {
    Data out(len);
    for (int i = len - 1; i >= 0; i--, value >>= 8) {
        out[i] = (value & 0xFF);
    }
    return out;
}

Data forgeString(const std::string& value, std::size_t len) {
    auto bytes = data(value);
    auto result = forgeInt32(static_cast<int>(bytes.size()), static_cast<int>(len));
    append(result, bytes);
    return result;
}

Data forgeEntrypoint(const std::string& value) {
    if (value == "default")
        return Data{0x00};
    else if (value == "root")
        return Data{0x01};
    else if (value == "do")
        return Data{0x02};
    else if (value == "set_delegate")
        return Data{0x03};
    else if (value == "remove_delegate")
        return Data{0x04};
    else {
        Data forged{0xff};
        append(forged, forgeString(value, 1));
        return forged;
    }
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

Data forgeAddress(const std::string& address) {
    if (address.size() < 3) {
        throw std::invalid_argument("Invalid address size");
    }
    auto prefix = address.substr(0, 3);
    if (prefix == "tz1" || prefix == "tz2" || prefix == "tz3") {
        Data forged{0x00};
        append(forged, forgePublicKeyHash(address));
        return forged;
    } else if (prefix == "KT1") {
        Data forged{0x01};
        const auto decoded = Base58::bitcoin.decodeCheck(address);
        const auto prefixSize = 3;
        forged.insert(forged.end(), decoded.begin() + prefixSize, decoded.end());
        forged.push_back(0x00);
        return forged;
    }
    throw std::invalid_argument("Invalid Prefix");
}

// Forge the given public key into a hex encoded string.
Data forgePublicKey(PublicKey publicKey) {
    std::array<uint8_t, 4> prefix = {13, 15, 37, 217};
    auto data = Data(prefix.begin(), prefix.end());
    auto bytes = Data(publicKey.bytes.begin(), publicKey.bytes.end());
    append(data, bytes);

    auto pk = Base58::bitcoin.encodeCheck(data);
    auto decoded = "00" + base58ToHex(pk, 4);
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
    auto forgedSource = source.forge();
    auto forgedFee = forgeZarith(operation.fee());
    auto forgedCounter = forgeZarith(operation.counter());
    auto forgedGasLimit = forgeZarith(operation.gas_limit());
    auto forgedStorageLimit = forgeZarith(operation.storage_limit());

    if (operation.kind() == Operation_OperationKind_REVEAL) {
        auto publicKey = PublicKey(data(operation.reveal_operation_data().public_key()), TWPublicKeyTypeED25519);
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
        if (!operation.transaction_operation_data().has_parameters()) {
            append(forged, forgeBool(false));
            append(forged, forgedDestination);
        }
        if (operation.transaction_operation_data().has_parameters()) {
            append(forged, forgeAddress(operation.transaction_operation_data().destination()));
            append(forged, forgeBool(true));
            auto& parameters = operation.transaction_operation_data().parameters();
            switch (parameters.parameters_case()) {
            case TransactionParametersOperationData::kFa12Parameters:
                append(forged, forgeEntrypoint(parameters.fa12_parameters().entrypoint()));
                append(forged, forgeArray(forgeMichelson(FA12ParameterToMichelson(parameters.fa12_parameters()))));
                break;
            case TransactionParametersOperationData::kFa2Parameters:
                append(forged, forgeEntrypoint(parameters.fa2_parameters().entrypoint()));
                append(forged, forgeArray(forgeMichelson(FA2ParameterToMichelson(parameters.fa2_parameters()))));
                break;
            case TransactionParametersOperationData::PARAMETERS_NOT_SET:
                break;
            }
        } else {
            append(forged, forgeBool(false));
        }
        return forged;
    }

    throw std::invalid_argument("Invalid operation kind");
}

Data forgePrim(const PrimValue& value) {
    Data forged;
    if (value.prim == "Pair") {
        constexpr uint8_t nbArgs = 2;
        const uint8_t preamble = static_cast<uint8_t>(std::min(2 * nbArgs + static_cast<uint8_t>(value.anots.size()) + 0x03, 9));
        forged.emplace_back(preamble);
        forged.emplace_back(PrimType::Pair);
        Data tmpForged;
        for (auto&& cur : value.args) {
            append(tmpForged, forgeMichelson(cur.value));
        }
        append(forged, tmpForged);
    }
    return forged;
}

Data forgeMichelson(const MichelsonValue::MichelsonVariant& value) {
    auto visit_functor = [](const MichelsonValue::MichelsonVariant& value) -> Data {
        if (std::holds_alternative<PrimValue>(value)) {
            return forgePrim(std::get<PrimValue>(value));
        } else if (std::holds_alternative<StringValue>(value)) {
            Data forged{1};
            append(forged, forgeString(std::get<StringValue>(value).string));
            return forged;
        } else if (std::holds_alternative<IntValue>(value)) {
            Data forged{0};
            auto res = int256_t(std::get<IntValue>(value)._int);
            append(forged, forgeMicheInt(res));
            return forged;
        } else if (std::holds_alternative<BytesValue>(value)) {
            return {};
        } else if (std::holds_alternative<MichelsonValue::MichelsonArray>(value)) {
            // array
            Data forged{2};
            Data tmpForged;
            auto array = std::get<MichelsonValue::MichelsonArray>(value);
            for (auto&& cur : array) {
                std::visit([&tmpForged](auto&& arg) {
                    MichelsonValue::MichelsonVariant v = arg;
                    append(tmpForged, forgeMichelson(v));
                },
                           cur);
            }
            append(forged, forgeArray(tmpForged));
            return forged;
        } else {
            throw std::invalid_argument("Invalid variant");
        }
    };

    return std::visit(visit_functor, value);
}

MichelsonValue::MichelsonVariant FA12ParameterToMichelson(const FA12TransactionOperationData& data) {
    MichelsonValue::MichelsonVariant address = StringValue{.string = data.from()};
    MichelsonValue::MichelsonVariant to = StringValue{.string = data.to()};
    MichelsonValue::MichelsonVariant amount = IntValue{._int = data.value()};
    auto i = PrimValue{.prim = "Pair", .args{{to}, {amount}}};
    auto v = PrimValue{.prim = "Pair", .args{{address}, {i}}};
    MichelsonValue::MichelsonVariant variant = v;
    return v;
}

MichelsonValue::MichelsonVariant FA2ParameterToMichelson(const FA2TransactionOperationData& data) {
    auto& txObj = *data.txs_object().begin();
    MichelsonValue::MichelsonVariant from = StringValue{.string = txObj.from()};
    auto& tx = txObj.txs(0);
    MichelsonValue::MichelsonVariant tokenId = IntValue{._int = tx.token_id()};
    MichelsonValue::MichelsonVariant amount = IntValue{._int = tx.amount()};
    auto i = PrimValue{.prim = "Pair", .args{{tokenId}, {amount}}};
    MichelsonValue::MichelsonVariant to = StringValue{.string = tx.to()};
    MichelsonValue::MichelsonVariant txs = MichelsonValue::MichelsonArray{PrimValue{.prim = "Pair", .args{{to}, {i}}}};
    auto v = PrimValue{.prim = "Pair", .args{{from}, {txs}}};
    MichelsonValue::MichelsonVariant value = MichelsonValue::MichelsonArray{v};
    return value;
}

Data forgeArray(const Data& data, int len) {
    auto forged = forgeInt32(static_cast<int>(data.size()), len);
    append(forged, data);
    return forged;
}

Data forgeMicheInt(const TW::int256_t& value) {
    Data forged;
    auto abs = boost::multiprecision::abs(value);
    forged.push_back(static_cast<uint8_t>(value.sign() < 0 ? (abs & 0x3f - 0x40) : (abs & 0x3f)));
    abs >>= 6;
    while (abs > 0) {
        forged[forged.size() - 1] |= 0x80;
        forged.push_back(static_cast<uint8_t>(abs & 0x7F));
        abs >>= 7;
    }
    return forged;
}
