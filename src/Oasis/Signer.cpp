// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <Oasis/Signer.h>

#include "Address.h"
#include "Signer.h"

#define TRANSFER_METHOD "staking.Transfer"
#define ESCROW_METHOD "staking.AddEscrow"
#define RECLAIM_ESCROW_METHOD "staking.ReclaimEscrow"

using namespace TW;

namespace TW::Oasis {

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto output = Proto::SigningOutput();
    auto signer = Signer(input);
    auto encoded = signer.build();
    output.set_encoded(encoded.data(), encoded.size());
    return output;
}

Data Signer::build() const {
    auto privateKey = PrivateKey(input.private_key());
    auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeED25519);

    if(input.has_transfer()) {
        auto tx = buildTransfer();
        auto signature = signTransaction<Transaction>(tx);
        auto encoded = tx.serialize(signature, publicKey);
        return encoded;
    }

    if(input.has_escrow()) {
        auto tx = buildEscrow();
        auto signature = signTransaction<Escrow>(tx);
        auto encoded = tx.serialize(signature, publicKey);
        return encoded;
    }

    if(input.has_reclaimescrow()) {
        auto tx = buildReclaimEscrow();
        auto signature = signTransaction<ReclaimEscrow>(tx);
        auto encoded = tx.serialize(signature, publicKey);
        return encoded;
    }

    throw std::invalid_argument("Invalid message");
}

template <typename T>
Data Signer::signTransaction(T& tx) const {
    auto privateKey = PrivateKey(input.private_key());

    // The use of this context thing is explained here -->
    // https://docs.oasis.dev/oasis-core/common-functionality/crypto#domain-separation
    auto encodedMessage = tx.encodeMessage().encoded();
    Data dataToHash(tx.context.begin(), tx.context.end());
    dataToHash.insert(dataToHash.end(), encodedMessage.begin(), encodedMessage.end());
    auto hash = Hash::sha512_256(dataToHash);

    auto signature = privateKey.sign(hash, TWCurveED25519);
    return Data(signature.begin(), signature.end());
}

Data Signer::signaturePreimage() const {
    if(input.has_transfer()) {
        auto tx = buildTransfer();
        return tx.signaturePreimage();
    }

    if(input.has_escrow()) {
        auto tx = buildEscrow();
        return tx.signaturePreimage();
    }

    if(input.has_reclaimescrow()) {
        auto tx = buildReclaimEscrow();
        return tx.signaturePreimage();
    }

    throw std::invalid_argument("Invalid message");
}

Proto::SigningOutput Signer::compile(const Data& signature, const PublicKey& publicKey) const {
    Proto::SigningOutput output;

    if(input.has_transfer()) {
        auto tx = buildTransfer();
        auto encoded = tx.serialize(signature, publicKey);
        output.set_encoded(encoded.data(), encoded.size());
        return output;
    }

    if(input.has_escrow()) {
        auto tx = buildEscrow();
        auto encoded = tx.serialize(signature, publicKey);
        output.set_encoded(encoded.data(), encoded.size());
        return output;
    }

    if(input.has_reclaimescrow()) {
        auto tx = buildReclaimEscrow();
        auto encoded = tx.serialize(signature, publicKey);
        output.set_encoded(encoded.data(), encoded.size());
        return output;
    }

    throw std::invalid_argument("Invalid message");
}


Transaction Signer::buildTransfer() const {
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
        /* to */ address,
        /* method */ TRANSFER_METHOD,
        /* gasPrice */ input.transfer().gas_price(),
        /* gasAmount */ gasAmount,
        /* amount */ amount,
        /* nonce */ input.transfer().nonce(),
        /* context */ input.transfer().context());
    return transaction;
}

Escrow Signer::buildEscrow() const {
    // Create empty address var and check if value we want to load is valid
    Address address(input.escrow().account());

    // Load value on that address var we create before
    Address::decode(input.escrow().account(), address);

    // Convert values from string to uint256
    std::istringstream amountStream(input.escrow().amount());
    uint256_t amount;
    amountStream >> amount;

    std::istringstream gasAmountStream(input.escrow().gas_amount());
    uint256_t gasAmount;
    gasAmountStream >> gasAmount;

    Escrow escrow(
        /* method */ ESCROW_METHOD,
        /* gasPrice */ input.escrow().gas_price(),
        /* gasAmount */ gasAmount,
        /* nonce */ input.escrow().nonce(),
        /* account */ address,
        /* amount */ amount,
        /* context */ input.escrow().context());
    return escrow;
}

ReclaimEscrow Signer::buildReclaimEscrow() const {
     // Create empty address var and check if value we want to load is valid
    Address address(input.reclaimescrow().account());

    // Load value on that address var we create before
    Address::decode(input.reclaimescrow().account(), address);

    // Convert values from string to uint256
    std::istringstream sharesStream(input.reclaimescrow().shares());
    uint256_t shares;
    sharesStream >> shares;

    std::istringstream gasAmountStream(input.reclaimescrow().gas_amount());
    uint256_t gasAmount;
    gasAmountStream >> gasAmount;

    ReclaimEscrow reclaimEscrow(
        /* method */ RECLAIM_ESCROW_METHOD,
        /* gasPrice */ input.reclaimescrow().gas_price(),
        /* gasAmount */ gasAmount,
        /* nonce */ input.reclaimescrow().nonce(),
        /* account */ address,
        /* shares */ shares,
        /* context */ input.reclaimescrow().context());
    return reclaimEscrow;
}

} // namespace TW::Oasis
