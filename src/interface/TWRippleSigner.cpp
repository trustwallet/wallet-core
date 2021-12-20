// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWRippleSigner.h>

#include "../Ripple/Signer.h"
#include "../proto/Ripple.pb.h"
#include <TrezorCrypto/ecdsa.h>

using namespace TW;
using namespace TW::Ripple;

TW_Ripple_Proto_SigningOutput TWRippleSignerSign(TW_Ripple_Proto_SigningInput data) {
    try {
        Proto::SigningInput input;
        input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

        auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
        auto transaction = Transaction(
            /* amount */ input.amount(),
            /* fee */ input.fee(),
            /* flags */ input.flags(),
            /* sequence */ input.sequence(),
            /* last_ledger_sequence */ input.last_ledger_sequence(),
            /* account */ Address(input.account()),
            /* destination */ input.destination(),
            /* destination_tag*/ input.destination_tag());

        auto signer = Signer();
        signer.sign(key, transaction);

        auto protoOutput = Proto::SigningOutput();
        auto encoded = transaction.serialize();
        protoOutput.set_encoded(encoded.data(), encoded.size());

        auto serialized = protoOutput.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                     serialized.size());
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return nullptr;
}

TWData *_Nonnull TWRippleSignerMessage(TW_Ripple_Proto_SigningInput data) {
    try {
        Proto::SigningInput input;
        input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

        auto transaction = Transaction(
            /* amount */ input.amount(),
            /* fee */ input.fee(),
            /* flags */ input.flags(),
            /* sequence */ input.sequence(),
            /* last_ledger_sequence */ input.last_ledger_sequence(),
            /* account */ Address(input.account()),
            /* destination */ input.destination(),
            /* destination_tag*/ input.destination_tag());

        auto publicKey = PublicKey(Data(input.public_key().begin(), input.public_key().end()),
                                   TWPublicKeyTypeSECP256k1);
        transaction.pub_key = publicKey.bytes;

        auto encoded = transaction.getPreImage();

        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(encoded.data()),
                                     encoded.size());
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return nullptr;
}

TWData *_Nonnull TWRippleSignerTransaction(TW_Ripple_Proto_SigningInput data, TWData *_Nonnull signature) {
    try {
        Proto::SigningInput input;
        input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

        auto transaction = Transaction(
            /* amount */ input.amount(),
            /* fee */ input.fee(),
            /* flags */ input.flags(),
            /* sequence */ input.sequence(),
            /* last_ledger_sequence */ input.last_ledger_sequence(),
            /* account */ Address(input.account()),
            /* destination */ input.destination(),
            /* destination_tag*/ input.destination_tag());

        std::vector<uint8_t> signVec;
        auto rawSign = TWDataBytes(signature);
        signVec.assign(rawSign, rawSign + static_cast<int>(TWDataSize(signature)));
        std::array<uint8_t, 72> sigBytes;
        size_t size = ecdsa_sig_to_der(signVec.data(), sigBytes.data());
        auto sig = Data{};
        std::copy(sigBytes.begin(), sigBytes.begin() + size, std::back_inserter(sig));

        auto publicKey = PublicKey(Data(input.public_key().begin(), input.public_key().end()),
                                   TWPublicKeyTypeSECP256k1);

        transaction.pub_key = publicKey.bytes;
        transaction.signature = sig;

        auto serialized = transaction.serialize();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                     serialized.size());
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return nullptr;
}
