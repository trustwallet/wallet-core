// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "Base64.h"
#include "PublicKey.h"

namespace {
enum IntentScope: int {
    TransactionData = 0,
    TransactionEffects = 1,
    AuthorityBatch = 2,
    CheckpointSummary = 3,
    PersonalMessage = 4,
};

enum IntentVersion: int {
    V0 = 0,
};

enum IntentAppId {
    Sui = 0
};
}

namespace TW::Sui {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto protoOutput = Proto::SigningOutput();
    auto unsignedTxData = TW::Base64::decode(input.unsigned_tx(), false);
    Data toSign{TransactionData, V0, IntentAppId::Sui};
    append(toSign, unsignedTxData);
    auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    Data signatureScheme{0x00};
    append(signatureScheme, privateKey.sign(toSign, TWCurveED25519));
    append(signatureScheme, privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes);
    protoOutput.set_unsigned_tx(input.unsigned_tx());
    protoOutput.set_encoded(TW::Base64::encode(signatureScheme));
    return protoOutput;
}

} // namespace TW::Sui
