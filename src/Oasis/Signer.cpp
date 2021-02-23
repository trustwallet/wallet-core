// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <Oasis/Signer.h>

#include "Signer.h"
#include "Address.h"

#define TRANSFER_METHOD "staking.Transfer"

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

    // Convert values from string to uint256
    std::istringstream amountStream(input.transfer().amount());
    uint256_t amount;
    amountStream >> amount;

    std::istringstream gasAmountStream(input.transfer().gas_amount());
    uint256_t gasAmount;
    gasAmountStream >> gasAmount;

    Transaction transaction(
        /* to */     address,
        /* method */ TRANSFER_METHOD,
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

    // The use of this context thing is explained here --> https://docs.oasis.dev/oasis-core/common-functionality/crypto#domain-separation
    auto encodedMessage = tx.encodeMessage().encoded();
    Data dataToHash(tx.context.begin(), tx.context.end());
    dataToHash.insert(dataToHash.end(), encodedMessage.begin(), encodedMessage.end());
    auto hash = Hash::sha512_256(dataToHash);

    auto signature = privateKey.sign(hash, TWCurveED25519);
    return Data(signature.begin(), signature.end());
}
