// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWWavesSigner.h>

#include "../Base58.h"
#include "../Waves/Signer.h"
#include "../proto/Waves.pb.h"

using namespace TW;
using namespace TW::Waves;

TW_Waves_Proto_SigningOutput TWWavesSignerSign(TW_Waves_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeCURVE25519);
    auto transaction = Transaction(input, publicKey.bytes);

    Data signature = Signer::sign(privateKey, transaction);

    Proto::SigningOutput protoOutput = Proto::SigningOutput();
    protoOutput.set_signature(reinterpret_cast<const char *>(signature.data()), signature.size());
    protoOutput.set_json(transaction.buildJson(signature).dump());
    std::string serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()),
                                 serialized.size());
}
