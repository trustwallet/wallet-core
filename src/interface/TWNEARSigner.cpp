// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNEARSigner.h>

#include "../NEAR/Serialization.h"
#include "../NEAR/Signer.h"
#include "../proto/NEAR.pb.h"

using namespace TW;
using namespace TW::NEAR;

TW_NEAR_Proto_SigningOutput TWNEARSignerSign(TW_NEAR_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    Proto::SigningOutput output = Signer::sign(std::move(input));

    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                 serialized.size());
}

TWData* _Nonnull TWNEARSignerMessage(TW_NEAR_Proto_SigningInput data, TWData* _Nonnull publicKey) {
    try {
        Proto::SigningInput input;
        input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
        auto transaction = TW::NEAR::transactionDataWithPublicKey(
            std::move(input), TW::data(TWDataBytes(publicKey), TWDataSize(publicKey)));
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(transaction.data()),
                                     transaction.size());
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
        throw e;
    }
}

TWData* _Nonnull TWNEARSignerTransaction(TW_NEAR_Proto_SigningInput data,
                                         TWData* _Nonnull signature, TWData* _Nonnull publicKey) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto transaction = TW::NEAR::transactionDataWithPublicKey(
        std::move(input), TW::data(TWDataBytes(publicKey), TWDataSize(publicKey)));

    Data signatureData = TW::data(TWDataBytes(signature), TWDataSize(signature));
    auto signedTransaction = signedTransactionData(transaction, signatureData);
    auto output = Proto::SigningOutput();
    output.set_signed_transaction(signedTransaction.data(), signedTransaction.size());
    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                 serialized.size());
}