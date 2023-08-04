// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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