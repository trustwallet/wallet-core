// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Oasis/Signer.h>

#include "Signer.h"
#include "Address.h"

using namespace TW;
using namespace TW::Oasis;


Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto output = Proto::SigningOutput();
    auto signer = Signer(input);
    auto encoded = signer.build();
    output.set_encoded(encoded.data(), encoded.size());
    return output;
}

Data Signer::build() const {
    // Create empty address var and check if value we want to load is valid
    Address address(input.transfer().to());

    // Load value on that address var we create before
    Address::decode(input.transfer().to(), address);

    // Convert values from string to unsigned long
    auto amount = strtoul(input.transfer().amount().c_str(), nullptr,10);
    auto gasAmount = strtoul(input.transfer().gas_amount().c_str(), nullptr,10);

    Transaction transaction(
        /* to */     address,
        /* method */ input.transfer().method(),
        /* gasPrice */ input.transfer().gas_price(),
        /* gasAmount */ gasAmount,
        /* amount */ amount,
        /* nonce */ input.transfer().nonce(),
        /* context */ input.transfer().context());


    auto privateKey = PrivateKey(input.private_key());
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);

    auto signature = sign(transaction);
    auto encoded = transaction.serialize(signature, publicKey);

    return encoded;
}

Data Signer::sign(Transaction& tx) const {
    auto privateKey = PrivateKey(input.private_key());

    // https://docs.oasis.dev/oasis-core/common-functionality/crypto#domain-separation
    auto encodedMessage = tx.encodeMessage().encoded();
    Data dataToHash(tx.context.begin(), tx.context.end());
    dataToHash.insert(dataToHash.end(), encodedMessage.begin(), encodedMessage.end());
    auto hash = Hash::sha512_256(dataToHash);

    auto signature = privateKey.sign(hash, TWCurveED25519);
    return Data(signature.begin(), signature.end());
}
