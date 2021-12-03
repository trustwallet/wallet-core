// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../PrivateKey.h"
#include "../proto/Stellar.pb.h"
#include "../Stellar/Address.h"
#include "../Stellar/Signer.h"

#include <TrustWalletCore/TWStellarSigner.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <BinaryCoding.h>
#include <Base64.h>

using namespace TW;
using namespace TW::Stellar;

TW_Stellar_Proto_SigningOutput TWStellarSignerSign(TW_Stellar_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    const auto signer = Signer(input);
    const auto output = signer.sign();

    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_signature(output);

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TWData *_Nonnull TWStellarSignerMessage(TW_Stellar_Proto_SigningInput inputData) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(inputData), static_cast<int>(TWDataSize(inputData)));

    const auto signer = Signer(input);

    try {
        auto account = Address(input.account());
        auto encoded = signer.encode(input);

        auto encodedWithHeaders = Data();
        auto publicNetwork = input.passphrase(); // Header
        auto passphrase = Hash::sha256(publicNetwork);
        encodedWithHeaders.insert(encodedWithHeaders.end(), passphrase.begin(), passphrase.end());
        auto transactionType = Data{0, 0, 0, 2}; // Header
        encodedWithHeaders.insert(encodedWithHeaders.end(), transactionType.begin(),
                                  transactionType.end());
        encodedWithHeaders.insert(encodedWithHeaders.end(), encoded.begin(), encoded.end());

        auto hash = Hash::sha256(encodedWithHeaders);
        auto data = Data(hash.begin(), hash.end());

        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(data.data()), data.size());
    } catch (const std::exception& e) {
        // Here only address class will throw exception,
        // so if it happens, we print the account.
        std::cerr << "Account:" << input.account() << ", Err:" << e.what() << '\n';
        return nullptr;
    }
}

TWString *_Nonnull TWStellarSignerTransaction(TW_Stellar_Proto_SigningInput inputData, TWData *_Nonnull signature) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(inputData), static_cast<int>(TWDataSize(inputData)));

    Data sig(TWDataBytes(signature), TWDataBytes(signature) + TWDataSize(signature));

    const auto signer = Signer(input);

    auto account = Address(input.account());
    auto encoded = signer.encode(input);

    auto encodedWithHeaders = Data();
    auto publicNetwork = input.passphrase(); // Header
    auto passphrase = Hash::sha256(publicNetwork);
    encodedWithHeaders.insert(encodedWithHeaders.end(), passphrase.begin(), passphrase.end());
    auto transactionType = Data{0, 0, 0, 2}; // Header
    encodedWithHeaders.insert(encodedWithHeaders.end(), transactionType.begin(),
                              transactionType.end());
    encodedWithHeaders.insert(encodedWithHeaders.end(), encoded.begin(), encoded.end());

    auto encodedTx = Data();
    encodedTx.insert(encodedTx.end(), encoded.begin(), encoded.end());
    encode32BE(1, encodedTx);
    encodedTx.insert(encodedTx.end(), account.bytes.end() - 4, account.bytes.end());
    encode32BE(static_cast<uint32_t>(sig.size()), encodedTx);
    encodedTx.insert(encodedTx.end(), sig.begin(), sig.end());

    return TWStringCreateWithUTF8Bytes(Base64::encode(encodedTx).c_str());
}