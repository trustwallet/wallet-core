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
#include <WebAuthn.h>
#include "../proto/Barz.pb.h"
#include "AsnParser.h"

namespace TW::Barz {

using ParamBasePtr = std::shared_ptr<Ethereum::ABI::ParamBase>;
using ParamCollection = std::vector<ParamBasePtr>;

std::string getCounterfactualAddress(const Proto::ContractAddressInput input) {
    auto params = Ethereum::ABI::ParamTuple();
    params.addParam(std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(input.diamond_cut_facet())));
    params.addParam(std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(input.account_facet())));
    params.addParam(std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(input.verification_facet())));
    params.addParam(std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(input.entry_point())));
    params.addParam(std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(input.diamond_loupe_facet())));
    params.addParam(std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(input.diamond_init())));
    params.addParam(std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(input.facet_registry())));

    Data publicKey;
    switch (input.owner().kind_case()) {
    case Proto::ContractOwner::KindCase::KIND_NOT_SET:
        return "";
    case Proto::ContractOwner::KindCase::kPublicKey:
        publicKey = parse_hex(input.owner().public_key());
        break;
    case Proto::ContractOwner::KindCase::kAttestationObject:
        const auto attestationObject = parse_hex(input.owner().attestation_object());
        publicKey = subData(WebAuthn::getPublicKey(attestationObject)->bytes, 1); // Drop the first byte which corresponds to the public key type
        break;
    }
    params.addParam(std::make_shared<Ethereum::ABI::ParamByteArray>(publicKey));

    Data encoded;
    params.encode(encoded);

    Data initCode = parse_hex(input.bytecode());
    append(initCode, encoded);

    const Data initCodeHash = Hash::keccak256(initCode);
    const Data salt(32, 0);
    return Ethereum::checksumed(Ethereum::Address(hexEncoded(Ethereum::create2Address(input.factory(), salt, initCodeHash))));
}

Data getInitCodeFromPublicKey(const std::string& factoryAddress, const std::string& publicKey, const std::string& verificationFacet) {
    auto createAccountFunc = Ethereum::ABI::Function("createAccount", ParamCollection{
                                                                std::make_shared<Ethereum::ABI::ParamAddress>(parse_hex(verificationFacet)),
                                                                std::make_shared<Ethereum::ABI::ParamByteArray>(parse_hex(publicKey)),
                                                                std::make_shared<Ethereum::ABI::ParamUInt256>(0)});
    Data createAccountFuncEncoded;
    createAccountFunc.encode(createAccountFuncEncoded);

    Data envelope;
    append(envelope, parse_hex(factoryAddress));
    append(envelope, createAccountFuncEncoded);
    return envelope;
}

Data getInitCodeFromAttestationObject(const std::string& factoryAddress, const std::string& attestationObject, const std::string& verificationFacet) {
    const auto publicKey = subData(WebAuthn::getPublicKey(parse_hex(attestationObject))->bytes, 1);
    return getInitCodeFromPublicKey(factoryAddress, hexEncoded(publicKey), verificationFacet);
}

Data getFormattedSignature(const Data& signature, const Data& authenticatorData, const std::string& origin) {
    const std::string clientDataJSONPre = "{\"type\":\"webauthn.get\",\"challenge\":\"";
    const std::string clientDataJSONPost = "\",\"origin\":\"" + origin + "\"}";

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
