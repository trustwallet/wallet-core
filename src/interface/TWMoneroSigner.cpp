// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWMoneroSigner.h>

#include "HexCoding.h"
#include "../Monero/Signer.h"
#include "../PrivateKey.h"
#include "../proto/Monero.pb.h"

using namespace TW;
using namespace TW::Monero;

TW_Monero_Proto_SigningOutput TWMoneroSignerSign(TW_Monero_Proto_SigningInput data) {
//    Proto::SigningInput input;
//    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
//
//    auto key = PrivateKey(input.private_key());
//    auto transaction = Transaction(
//            /* to */Address(input.destination()),
//            /* value */input.value(),
//            /* fee */input.fee(),
//            /* nonce */input.nonce(),
//            /* timestamp */input.timestamp()
//        );
//
//    auto signer = Signer();
//    signer.sign(key, transaction);
//
//    auto protoOutput = Proto::SigningOutput();
//    auto encoded = transaction.serialize();
//    protoOutput.set_encoded(encoded.data(), encoded.size());
//
//    auto serialized = protoOutput.SerializeAsString();
//    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}