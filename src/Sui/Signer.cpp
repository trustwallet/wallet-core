// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Base64.h"
#include "PublicKey.h"

namespace {

enum IntentScope : int {
    TransactionData = 0,
};

enum IntentVersion : int {
    V0 = 0,
};

enum IntentAppId {
    Sui = 0
};

} // namespace

namespace TW::Sui {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto protoOutput = Proto::SigningOutput();
    auto unsignedTx = input.sign_direct_message().unsigned_tx_msg();
    auto unsignedTxData = TW::Base64::decode(unsignedTx);
    Data toSign{TransactionData, V0, IntentAppId::Sui};
    append(toSign, unsignedTxData);
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    Data signatureScheme{0x00};
    append(signatureScheme, privateKey.sign(TW::Hash::blake2b(toSign, 32), TWCurveED25519));
    append(signatureScheme, privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes);
    protoOutput.set_unsigned_tx(unsignedTx);
    protoOutput.set_signature(TW::Base64::encode(signatureScheme));
    return protoOutput;
}

} // namespace TW::Sui
