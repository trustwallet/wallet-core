// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ABI.h"
#include "AddressChecksum.h"
#include "EIP1014.h"
#include "Hash.h"
#include "HexCoding.h"
#include "../proto/Barz.pb.h"
#include "AsnParser.h"
#include "Base64.h"

namespace TW::Barz {

using ParamBasePtr = std::shared_ptr<Ethereum::ABI::ParamBase>;
using ParamCollection = std::vector<ParamBasePtr>;

std::string getCounterfactualAddress(const Proto::ContractAddressInput input) {
    auto params = Ethereum::ABI::ParamTuple();
    params.addParam(std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(input.account_facet())));
    params.addParam(std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(input.verification_facet())));
    params.addParam(std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(input.entry_point())));
    params.addParam(std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(input.facet_registry())));
    params.addParam(std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(input.default_fallback())));
    params.addParam(std::make_shared<Ethereum::ABI::ParamByteArray>(parse_hex(input.public_key())));

    Data encoded;
    params.encode(encoded);

    Data initCode = parse_hex(input.bytecode());
    append(initCode, encoded);

    const Data initCodeHash = Hash::keccak256(initCode);
    Data salt = store(input.salt(), 32);
    return Ethereum::checksumed(Ethereum::Address(hexEncoded(Ethereum::create2Address(input.factory(), salt, initCodeHash))));
}

Data getInitCode(const std::string& factoryAddress, const PublicKey& publicKey, const std::string& verificationFacet, const uint32_t salt) {
    auto createAccountFunc = Ethereum::ABI::Function("createAccount", ParamCollection{
                                                                std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(verificationFacet)),
                                                                std::make_shared<Ethereum::ABI::ParamByteArray>(publicKey.bytes),
                                                                std::make_shared<Ethereum::ABI::ParamUInt256>(salt)});
    Data createAccountFuncEncoded;
    createAccountFunc.encode(createAccountFuncEncoded);

    Data envelope;
    append(envelope, parse_hex(factoryAddress));
    append(envelope, createAccountFuncEncoded);
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
        return Data();
    }
    const Data parsedSignature = parsedSignatureOptional.value();
    const Data rValue = subData(parsedSignature, 0, 32);
    const Data sValue = subData(parsedSignature, 32, 64);

    auto params = Ethereum::ABI::ParamTuple();
    params.addParam(std::make_shared<Ethereum::ABI::ParamUInt256>(uint256_t(hexEncoded(rValue))));
    params.addParam(std::make_shared<Ethereum::ABI::ParamUInt256>(uint256_t(hexEncoded(sValue))));
    params.addParam(std::make_shared<Ethereum::ABI::ParamByteArray>(authenticatorData));
    params.addParam(std::make_shared<Ethereum::ABI::ParamString>(clientDataJSONPre));
    params.addParam(std::make_shared<Ethereum::ABI::ParamString>(clientDataJSONPost));

    Data encoded;
    params.encode(encoded);
    return encoded;
}

} // namespace TW::Barz
