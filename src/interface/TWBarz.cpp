// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWBarz.h>
#include <TrustWalletCore/TWData.h>
#include <string>
#include "Ethereum/Barz.h"

TWString *_Nonnull TWBarzGetCounterfactualAddress(TWData *_Nonnull input) {
    TW::Barz::Proto::ContractAddressInput inputProto;

    const auto bytes = TWDataBytes(input);
    const auto size = static_cast<int>(TWDataSize(input));
    if (!inputProto.ParseFromArray(bytes, size)) {
        return "";
    }

    return TWStringCreateWithUTF8Bytes(TW::Barz::getCounterfactualAddress(inputProto).c_str());
}

TWData *_Nonnull TWBarzGetInitCode(TWString* _Nonnull factory, struct TWPublicKey* _Nonnull publicKey, TWString* _Nonnull verificationFacet, uint32_t salt) {
    const auto& factoryStr = *reinterpret_cast<const std::string*>(factory);
    const auto& publicKeyConverted = *reinterpret_cast<const TW::PublicKey*>(publicKey);
    const auto& verificationFacetStr = *reinterpret_cast<const std::string*>(verificationFacet);

    const auto initCode = TW::Barz::getInitCode(factoryStr, publicKeyConverted, verificationFacetStr, salt);
    return TWDataCreateWithData(&initCode);
}

TWData *_Nonnull TWBarzGetFormattedSignature(TWData* _Nonnull signature, TWData* _Nonnull challenge, TWData* _Nonnull authenticatorData, TWString* _Nonnull clientDataJSON) {
    const auto& signatureData = *reinterpret_cast<const TW::Data*>(signature);
    const auto& challengeData = *reinterpret_cast<const TW::Data*>(challenge);
    const auto& authenticatorDataConverted = *reinterpret_cast<const TW::Data*>(authenticatorData);
    const auto& clientDataJSONStr = *reinterpret_cast<const std::string*>(clientDataJSON);

    const auto initCode = TW::Barz::getFormattedSignature(signatureData, challengeData, authenticatorDataConverted, clientDataJSONStr);
    return TWDataCreateWithData(&initCode);
}

TWData *_Nonnull TWBarzGetPrefixedMsgHash(TWData* _Nonnull msgHash, TWString* _Nonnull barzAddress, uint32_t chainId) {
    const auto& msgHashData = *reinterpret_cast<const TW::Data*>(msgHash);
    const auto& barzAddressData = *reinterpret_cast<const std::string*>(barzAddress);

    const auto prefixedMsgHash = TW::Barz::getPrefixedMsgHash(msgHashData, barzAddressData, chainId);
    return TWDataCreateWithData(&prefixedMsgHash);
}

TWData *_Nonnull TWBarzGetDiamondCutCode(TWData *_Nonnull input) {
    TW::Barz::Proto::DiamondCutInput inputProto;

    const auto bytes = TWDataBytes(input);
    const auto size = static_cast<int>(TWDataSize(input));
    if (!inputProto.ParseFromArray(bytes, size)) {
        return "";
    }

    const auto diamondCutCode = TW::Barz::getDiamondCutCode(inputProto);
    return TWDataCreateWithData(&diamondCutCode);
}

TWData *_Nonnull TWBarzGetAuthorizationHash(TWData* _Nonnull chainId, TWString* _Nonnull contractAddress, TWString* _Nonnull nonce) {
    const auto& chainIdData = *reinterpret_cast<const TW::Data*>(chainId);
    const auto& contractAddressStr = *reinterpret_cast<const std::string*>(contractAddress);
    const auto& nonceData = *reinterpret_cast<const TW::Data*>(nonce);
    const auto authorizationHash = TW::Barz::getAuthorizationHash(chainIdData, contractAddressStr, nonceData);
    return TWDataCreateWithData(&authorizationHash);
}

TWString *_Nonnull TWBarzSignAuthorization(TWData* _Nonnull chainId, TWString* _Nonnull contractAddress, TWData* _Nonnull nonce, TWString* _Nonnull privateKey) {
    const auto& chainIdData = *reinterpret_cast<const TW::Data*>(chainId);
    const auto& contractAddressStr = *reinterpret_cast<const std::string*>(contractAddress);
    const auto& nonceData = *reinterpret_cast<const TW::Data*>(nonce);
    const auto& privateKeyStr = *reinterpret_cast<const std::string*>(privateKey);
    const auto signedAuthorization = TW::Barz::signAuthorization(chainIdData, contractAddressStr, nonceData, privateKeyStr);
    return TWStringCreateWithUTF8Bytes(signedAuthorization.c_str());
}

TWData *_Nonnull TWBarzGetEncodedHash(
    TWData* _Nonnull chainId,
    TWString* _Nonnull codeAddress,
    TWString* _Nonnull codeName,
    TWString* _Nonnull codeVersion,
    TWString* _Nonnull typeHash,
    TWString* _Nonnull domainSeparatorHash,
    TWString* _Nonnull sender,
    TWString* _Nonnull userOpHash) {
    const auto& chainIdData = *reinterpret_cast<const TW::Data*>(chainId);
    const auto& codeAddressStr = *reinterpret_cast<const std::string*>(codeAddress);
    const auto& codeNameStr = *reinterpret_cast<const std::string*>(codeName);
    const auto& codeVersionStr = *reinterpret_cast<const std::string*>(codeVersion);
    const auto& typeHashStr = *reinterpret_cast<const std::string*>(typeHash);
    const auto& domainSeparatorHashStr = *reinterpret_cast<const std::string*>(domainSeparatorHash);
    const auto& senderStr = *reinterpret_cast<const std::string*>(sender);
    const auto& userOpHashStr = *reinterpret_cast<const std::string*>(userOpHash);
    const auto encodedHash = TW::Barz::getEncodedHash(
        chainIdData,
        codeAddressStr,
        codeNameStr,
        codeVersionStr, typeHashStr,
        domainSeparatorHashStr,
        senderStr,
        userOpHashStr);
    return TWDataCreateWithData(&encodedHash);
}

TWData *_Nonnull TWBarzGetSignedHash(TWString* _Nonnull hash, TWString* _Nonnull privateKey) {
    const auto& hashStr = *reinterpret_cast<const std::string*>(hash);
    const auto& privateKeyStr = *reinterpret_cast<const std::string*>(privateKey);
    const auto signedHash = TW::Barz::getSignedHash(hashStr, privateKeyStr);
    return TWDataCreateWithData(&signedHash);
}
