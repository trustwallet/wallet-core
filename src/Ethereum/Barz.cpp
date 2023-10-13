// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ABI/Function.h"
#include "AddressChecksum.h"
#include "EIP1014.h"
#include "Hash.h"
#include "HexCoding.h"
#include "../proto/Barz.pb.h"
#include "AsnParser.h"
#include "Base64.h"

namespace TW::Barz {

std::string getCounterfactualAddress(const Proto::ContractAddressInput input) {
    auto encodedData = Ethereum::ABI::Function::encodeParams(Ethereum::ABI::BaseParams {
        std::make_shared<Ethereum::ABI::ProtoAddress>(input.account_facet()),
        std::make_shared<Ethereum::ABI::ProtoAddress>(input.verification_facet()),
        std::make_shared<Ethereum::ABI::ProtoAddress>(input.entry_point()),
        std::make_shared<Ethereum::ABI::ProtoAddress>(input.facet_registry()),
        std::make_shared<Ethereum::ABI::ProtoAddress>(input.default_fallback()),
        std::make_shared<Ethereum::ABI::ProtoByteArray>(parse_hex(input.public_key())),
    });
    if (!encodedData.has_value()) {
        return {};
    }

    Data initCode = parse_hex(input.bytecode());
    append(initCode, encodedData.value());

    const Data initCodeHash = Hash::keccak256(initCode);
    Data salt = store(input.salt(), 32);
    return Ethereum::checksumed(Ethereum::Address(hexEncoded(Ethereum::create2Address(input.factory(), salt, initCodeHash))));
}

Data getInitCode(const std::string& factoryAddress, const PublicKey& publicKey, const std::string& verificationFacet, const uint32_t salt) {
    auto createAccountFuncEncoded = Ethereum::ABI::Function::encodeFunctionCall("createAccount", Ethereum::ABI::BaseParams {
        std::make_shared<Ethereum::ABI::ProtoAddress>(verificationFacet),
        std::make_shared<Ethereum::ABI::ProtoByteArray>(publicKey.bytes),
        std::make_shared<Ethereum::ABI::ProtoUInt256>(salt),
    });
    if (!createAccountFuncEncoded.has_value()) {
        return {};
    }

    Data envelope;
    append(envelope, parse_hex(factoryAddress));
    append(envelope, createAccountFuncEncoded.value());
    return envelope;
}

Data getFormattedSignature(const Data& signature, const Data challenge, const Data& authenticatorData, const std::string& clientDataJSON) {
    std::string challengeBase64 = TW::Base64::encodeBase64Url(challenge);
    while (challengeBase64.back() == '=') {
        challengeBase64.pop_back();
    }
    size_t challengePos = clientDataJSON.find(challengeBase64);
    if (challengePos == std::string::npos) {
        return Data();
    }

    const std::string clientDataJSONPre = clientDataJSON.substr(0, challengePos);
    const std::string clientDataJSONPost = clientDataJSON.substr(challengePos + challengeBase64.size());

    const auto parsedSignatureOptional = ASN::AsnParser::ecdsa_signature_from_der(signature);
    if (!parsedSignatureOptional.has_value()) {
        return {};
    }
    const Data parsedSignature = parsedSignatureOptional.value();
    const Data rValue = subData(parsedSignature, 0, 32);
    const Data sValue = subData(parsedSignature, 32, 64);

    auto encoded = Ethereum::ABI::Function::encodeParams(Ethereum::ABI::BaseParams {
        std::make_shared<Ethereum::ABI::ProtoUInt256>(rValue),
        std::make_shared<Ethereum::ABI::ProtoUInt256>(sValue),
        std::make_shared<Ethereum::ABI::ProtoByteArray>(authenticatorData),
        std::make_shared<Ethereum::ABI::ProtoString>(clientDataJSONPre),
        std::make_shared<Ethereum::ABI::ProtoString>(clientDataJSONPost),
    });

    if (encoded.has_value()) {
        return encoded.value();
    }
    return {};
}

} // namespace TW::Barz
