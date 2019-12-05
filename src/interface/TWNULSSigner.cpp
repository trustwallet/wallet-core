// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNULSSigner.h>
#include <boost/exception/diagnostic_information.hpp>

#include "../NULS/Signer.h"
#include "../PrivateKey.h"
#include "../proto/NULS.pb.h"
#include "../HexCoding.h"

using namespace TW;
using namespace TW::NULS;

TW_NULS_Proto_SigningOutput TWNULSSignerSign(TW_NULS_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto output = Proto::SigningOutput();
    try {
        const auto signer = Signer(input);
        const auto data = signer.sign();
        output.set_encoded(data.data(), data.size());
    }
    catch(...) {
        std::clog << boost::current_exception_diagnostic_information() << std::endl;
    }
    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()), serialized.size());
}

TWData *_Nonnull TWNULSSignerMessage(TW_NULS_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    Data unsignedTxBytes;
    try {
        const auto signer = Signer(input);
        unsignedTxBytes = signer.buildUnsignedTx();
    }
    catch(...) {
        std::clog << boost::current_exception_diagnostic_information() << std::endl;
    }
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(unsignedTxBytes.data()), unsignedTxBytes.size());
}

TWData *_Nonnull TWNULSSignerTransaction(TW_NULS_Proto_SigningInput data, TWData *_Nonnull pubkey, TWData *_Nonnull sig) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    Data publicKey(TWDataBytes(pubkey), TWDataBytes(pubkey) + TWDataSize(pubkey));
    Data signature(TWDataBytes(sig), TWDataBytes(sig) + TWDataSize(sig));

    Data signedTxBytes;
    try {
        const auto signer = Signer(input);
        signedTxBytes = signer.buildSignedTx(publicKey, signature);
    }
    catch(...) {
        std::clog << boost::current_exception_diagnostic_information() << std::endl;
    }
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(signedTxBytes.data()), signedTxBytes.size());
}
