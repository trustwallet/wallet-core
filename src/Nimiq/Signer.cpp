// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Signer.h"
#include <TrezorCrypto/ed25519.h>

#include <algorithm>

namespace TW::Nimiq {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) {
    auto key = PrivateKey(input.private_key(), TWCurveED25519);
    auto pubkey = key.getPublicKey(TWPublicKeyTypeED25519);
    std::array<uint8_t, 32> pubkeyBytes;
    std::copy(pubkey.bytes.begin(), pubkey.bytes.end(), pubkeyBytes.data());
    auto transaction = Transaction(
        /* sender_pub_key */pubkeyBytes,
        /* destination */Address(input.destination()),
        /* amount */input.value(),
        /* fee */input.fee(),
        /* vsh */input.validity_start_height(),
        /* networkId */input.network_id()
    );

    auto signer = Signer();
    signer.sign(key, transaction);

    auto output = Proto::SigningOutput();
    auto encoded = transaction.serialize();
    output.set_encoded(encoded.data(), encoded.size());
    return output;
}

void Signer::sign(const PrivateKey& privateKey, Transaction& transaction) const {
    auto preImage = transaction.getPreImage();
    auto signature = privateKey.sign(preImage, TWCurveED25519);
    std::copy(signature.begin(), signature.end(), transaction.signature.begin());
}

} // namespace TW::Nimiq
