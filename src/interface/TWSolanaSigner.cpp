// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../PrivateKey.h"
#include "../Solana/Address.h"
#include "../Solana/Signer.h"
#include "../proto/Solana.pb.h"

#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWSolanaSigner.h>

using namespace TW;
using namespace TW::Solana;

TW_Solana_Proto_SigningOutput TWSolanaSignerSign(TW_Solana_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto key = PrivateKey(input.private_key());
    auto blockhash = Solana::Hash(input.recent_blockhash());
    auto transaction = Transaction(
        /* from */ Address(key.getPublicKey(TWPublicKeyTypeED25519)),
        /* to */ Address(input.recipient()),
        /* lamports */ input.lamports(),
        /* recent_blockhash */ blockhash);

    auto signer = Signer();
    signer.sign({key}, transaction);

    auto protoOutput = Proto::SigningOutput();
    auto encoded = transaction.serialize();
    protoOutput.set_encoded(encoded.data(), encoded.size());

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()),
                                 serialized.size());
}
