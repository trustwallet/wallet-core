// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <OasisProtocol/Signer.h>

#include "Signer.h"
#include "Address.h"

using namespace TW;
using namespace TW::OasisProtocol;


Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto output = Proto::SigningOutput();
    auto signer = Signer(input);
    auto encoded = signer.build();
    output.set_encoded(encoded.data(), encoded.size());
    return output;
}

Data Signer::build() const {
    Transaction transaction(
        /* to */     Address(input.transfer().to()),
        /* method */ input.transfer().method(),
        /* gasPrice */ input.transfer().gas_price(),
        /* gasAmount */ load(input.transfer().gas_amount()),
        /* amount */ load(input.transfer().amount()),
        /* nonce */ input.transfer().nonce(),
        /* context */ input.transfer().context());

    auto signature = sign(transaction);
    auto encoded = transaction.serialize(signature);

    return encoded;
}

TW::Data Signer::sign(Transaction& tx) const {
    auto key = PrivateKey(input.private_key());
    auto hash = Hash::sha512_256(tx.getCBORmessage().encoded());
    auto signature = key.sign(hash, TWCurveED25519);
    return Data(signature.begin(), signature.end());
}
