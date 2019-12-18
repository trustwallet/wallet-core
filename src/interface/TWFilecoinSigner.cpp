// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWFilecoinSigner.h>

#include "../Filecoin/Signer.h"
#include "../Filecoin/Transaction.h"
#include "../PrivateKey.h"
#include "../proto/Filecoin.pb.h"

using namespace TW;
using namespace TW::Filecoin;

TW_Filecoin_Proto_SigningOutput TWFilecoinSignerSign(TW_Filecoin_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubkey = key.getPublicKey(TWPublicKeyTypeSECP256k1Extended);
    Address from(pubkey);
    std::array<uint8_t, 32> pubkeyBytes = {};
    std::copy(pubkey.bytes.begin(), pubkey.bytes.end(), pubkeyBytes.data());
    Transaction transaction(
        /* from */ from,
        /* to */ Address(input.to_address()),
        /* nonce */ input.nonce(),
        /* value */ input.value(),
        /* gas_price */ input.gas_price(),
        /* gas_limit */ input.gas_limit()
    );

    Signer signer;
    signer.sign(key, transaction);

    auto protoOutput = Proto::SigningOutput();
    auto encoded = transaction.serialize();
    protoOutput.set_encoded(encoded.data(), encoded.size());

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
