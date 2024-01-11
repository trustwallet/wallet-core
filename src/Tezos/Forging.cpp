// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Forging.h"
#include "Address.h"
#include "BinaryCoding.h"
#include "../HexCoding.h"
#include "../proto/Tezos.pb.h"
#include <sstream>

namespace TW::Tezos {

namespace {

constexpr const char* gTezosContractAddressPrefix{"KT1"};

void encodePrefix(const std::string& address, Data& forged) {
    const auto decoded = Base58::decodeCheck(address);
    constexpr auto prefixSize{3};
    forged.insert(forged.end(), decoded.begin() + prefixSize, decoded.end());
}

} // namespace

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
    encodePrefix(publicKeyHash, forged);
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
    }

    if (prefix == gTezosContractAddressPrefix) {
        Data forged{0x01};
        encodePrefix(address, forged);
        forged.emplace_back(0x00);
        return forged;
    }
    throw std::invalid_argument("Invalid Prefix");
}

// https://github.com/ecadlabs/taquito/blob/master/packages/taquito-local-forging/src/codec.ts#L19
Data forgePrefix(std::array<TW::byte, 3> prefix, const std::string& val) {
    const auto decoded = Base58::decodeCheck(val);
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

    auto pk = Base58::encodeCheck(data);
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
Data forgeOperation(const Proto::Operation& operation) {
    using namespace Proto;
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
        auto forgedDestination = forgeAddress(operation.transaction_operation_data().destination());

        forged.emplace_back(Operation_OperationKind_TRANSACTION);
        append(forged, forgedSource);
        append(forged, forgedFee);
        append(forged, forgedCounter);
        append(forged, forgedGasLimit);
        append(forged, forgedStorageLimit);
        append(forged, forgedAmount);
        append(forged, forgedDestination);
        if (!operation.transaction_operation_data().has_parameters() && operation.transaction_operation_data().encoded_parameter().empty()) {
            append(forged, forgeBool(false));
        } else if (operation.transaction_operation_data().has_parameters()) {
            append(forged, forgeBool(true));
            auto& parameters = operation.transaction_operation_data().parameters();
            switch (parameters.parameters_case()) {
            case OperationParameters::kFa12Parameters:
                append(forged, forgeEntrypoint(parameters.fa12_parameters().entrypoint()));
                append(forged, forgeArray(forgeMichelson(FA12ParameterToMichelson(parameters.fa12_parameters()))));
                break;
            case OperationParameters::kFa2Parameters:
                append(forged, forgeEntrypoint(parameters.fa2_parameters().entrypoint()));
                append(forged, forgeArray(forgeMichelson(FA2ParameterToMichelson(parameters.fa2_parameters()))));
                break;
            case OperationParameters::PARAMETERS_NOT_SET:
                break;
            }
        } else {
            append(forged, TW::data(operation.transaction_operation_data().encoded_parameter()));
        }
        return forged;
    }

    throw std::invalid_argument("Invalid operation kind");
}

Data forgePrim(const PrimValue& value) {
    Data forged;
    if (value.prim == "Pair") {
        // https://tezos.gitlab.io/developer/encodings.html?highlight=pair#pairs
        forged.reserve(2);
        constexpr uint8_t nbArgs = 2;
        // https://github.com/ecadlabs/taquito/blob/fd84d627171d24ce7ba81dd7b18763a95f16a99c/packages/taquito-local-forging/src/michelson/codec.ts#L195
        // https://github.com/baking-bad/netezos/blob/0bfd6db4e85ab1c99fb55503e476fe67cebd2dc5/Netezos/Forging/Local/LocalForge.Forgers.cs#L199
        const uint8_t preamble = static_cast<uint8_t>(std::min(2 * nbArgs + static_cast<uint8_t>(value.anots.size()) + 0x03, 9));
        forged.emplace_back(preamble);
        forged.emplace_back(PrimType::Pair);
        Data subForged;
        for (auto&& cur : value.args) {
            append(subForged, forgeMichelson(cur.value));
        }
        append(forged, subForged);
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
            append(forged, forgeMichelInt(res));
            return forged;
        } else if (std::holds_alternative<BytesValue>(value)) {
            return {};
        } else if (std::holds_alternative<MichelsonValue::MichelsonArray>(value)) {
            // array
            Data forged{2};
            Data subForged;
            auto array = std::get<MichelsonValue::MichelsonArray>(value);
            for (auto&& cur : array) {
                std::visit([&subForged](auto&& arg) { append(subForged, forgeMichelson(arg)); }, cur);
            }
            append(forged, forgeArray(subForged));
            return forged;
        } else {
            throw std::invalid_argument("Invalid variant");
        }
    };

    return std::visit(visit_functor, value);
}

Data forgeArray(const Data& data) {
    auto forged = forgeInt32(static_cast<int>(data.size()));
    append(forged, data);
    return forged;
}

Data forgeMichelInt(const TW::int256_t& value) {
    Data forged;
    auto abs = boost::multiprecision::abs(value);
    forged.emplace_back(static_cast<uint8_t>(value.sign() < 0 ? (abs & 0x3f - 0x40) : (abs & 0x3f)));
    abs >>= 6;
    while (abs > 0) {
        forged[forged.size() - 1] |= 0x80;
        forged.emplace_back(static_cast<uint8_t>(abs & 0x7F));
        abs >>= 7;
    }
    return forged;
}

} // namespace TW::Tezos
