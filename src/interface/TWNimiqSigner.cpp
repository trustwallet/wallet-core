// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNimiqSigner.h>

#include "../Nimiq/Signer.h"
#include "../PrivateKey.h"
#include "../proto/Nimiq.pb.h"

using namespace TW;
using namespace TW::Nimiq;

TW_Nimiq_Proto_SigningOutput TWNimiqSignerSign(TW_Nimiq_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubkey = key.getPublicKey(TWPublicKeyTypeED25519);
    std::array<uint8_t, 32> pubkeyBytes;
    std::copy(pubkey.bytes.begin(), pubkey.bytes.end(), pubkeyBytes.data());
    auto transaction = Transaction(
        /* sender_pub_key */pubkeyBytes,
        /* destination */Address(input.destination()),
        /* amount */input.value(),
        /* fee */input.fee(),
        /* vsh */input.validity_start_height()
    );

    auto signer = Signer();
    signer.sign(key, transaction);

    auto protoOutput = Proto::SigningOutput();
    auto encoded = transaction.serialize();
    protoOutput.set_encoded(encoded.data(), encoded.size());

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
