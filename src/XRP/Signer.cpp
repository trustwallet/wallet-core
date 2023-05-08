// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "../BinaryCoding.h"
#include <limits>

namespace TW::Ripple {


Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    auto output = Proto::SigningOutput();

    auto transaction =
        Transaction(
            input.fee(),
            input.flags(),
            input.sequence(),
            input.last_ledger_sequence(),
            Address(input.account()));
    switch (input.operation_oneof_case()) {
        case Proto::SigningInput::kOpPayment:
            signPayment(input, output, transaction);
            break;

        case Proto::SigningInput::kOpNftokenBurn:
            transaction.createNFTokenBurn(input.op_nftoken_burn().nftoken_id());
            break;

        case Proto::SigningInput::kOpNftokenCreateOffer:
            transaction.createNFTokenCreateOffer(
                input.op_nftoken_create_offer().nftoken_id(),
                input.op_nftoken_create_offer().destination());
            break;

        case Proto::SigningInput::kOpNftokenAcceptOffer:
            transaction.createNFTokenAcceptOffer(input.op_nftoken_accept_offer().sell_offer());
            break;

        case Proto::SigningInput::kOpNftokenCancelOffer:
            signNfTokenCancelOffer(input, transaction);
            break;

        case Proto::SigningInput::kOpTrustSet:
            transaction.createTrustSet(
                input.op_trust_set().limit_amount().currency(),
                input.op_trust_set().limit_amount().value(),
                input.op_trust_set().limit_amount().issuer());
            break;

        default:
            break;
    }

    if (output.error()) {
        return output;
    }

    auto signer = Signer();
    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    signer.sign(key, transaction);

    auto encoded = transaction.serialize();
    output.set_encoded(encoded.data(), encoded.size());
    return output;
}

void Signer::sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept {
    /// See https://github.com/trezor/trezor-core/blob/master/src/apps/ripple/sign_tx.py#L59
    transaction.pub_key = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1).bytes;

    auto unsignedTx = transaction.getPreImage();
    auto hash = Hash::sha512(unsignedTx);
    auto half = Data(hash.begin(), hash.begin() + 32);

    transaction.signature = privateKey.signAsDER(half);
}

TW::Data Signer::preImage() const {
    auto output = Proto::SigningOutput();

    auto transaction =
        Transaction(
            input.fee(),
            input.flags(),
            input.sequence(),
            input.last_ledger_sequence(),
            Address(input.account()));
    switch (input.operation_oneof_case()) {
        case Proto::SigningInput::kOpPayment:
            signPayment(input, output, transaction);
            break;

        case Proto::SigningInput::kOpNftokenBurn:
            transaction.createNFTokenBurn(input.op_nftoken_burn().nftoken_id());
            break;

        case Proto::SigningInput::kOpNftokenCreateOffer:
            transaction.createNFTokenCreateOffer(
                input.op_nftoken_create_offer().nftoken_id(),
                input.op_nftoken_create_offer().destination());
            break;

        case Proto::SigningInput::kOpNftokenAcceptOffer:
            transaction.createNFTokenAcceptOffer(input.op_nftoken_accept_offer().sell_offer());
            break;

        case Proto::SigningInput::kOpNftokenCancelOffer:
            signNfTokenCancelOffer(input, transaction);
            break;

        case Proto::SigningInput::kOpTrustSet:
            transaction.createTrustSet(
                input.op_trust_set().limit_amount().currency(),
                input.op_trust_set().limit_amount().value(),
                input.op_trust_set().limit_amount().issuer());
            break;

        default:
            break;
    }

    if (output.error()) {
        return {};
    }

    auto publicKey = Data(input.public_key().begin(), input.public_key().end());
    transaction.pub_key = publicKey;

    auto unsignedTx = transaction.getPreImage();
    return unsignedTx;
}

Proto::SigningOutput Signer::compile(const Data& signature, const PublicKey& publicKey) const {
    auto output = Proto::SigningOutput();

    auto transaction =
        Transaction(
            input.fee(),
            input.flags(),
            input.sequence(),
            input.last_ledger_sequence(),
            Address(input.account()));
    switch (input.operation_oneof_case()) {
        case Proto::SigningInput::kOpPayment:
            signPayment(input, output, transaction);
            break;

        case Proto::SigningInput::kOpNftokenBurn:
            transaction.createNFTokenBurn(input.op_nftoken_burn().nftoken_id());
            break;

        case Proto::SigningInput::kOpNftokenCreateOffer:
            transaction.createNFTokenCreateOffer(
                input.op_nftoken_create_offer().nftoken_id(),
                input.op_nftoken_create_offer().destination());
            break;

        case Proto::SigningInput::kOpNftokenAcceptOffer:
            transaction.createNFTokenAcceptOffer(input.op_nftoken_accept_offer().sell_offer());
            break;

        case Proto::SigningInput::kOpNftokenCancelOffer:
            signNfTokenCancelOffer(input, transaction);
            break;

        case Proto::SigningInput::kOpTrustSet:
            transaction.createTrustSet(
                input.op_trust_set().limit_amount().currency(),
                input.op_trust_set().limit_amount().value(),
                input.op_trust_set().limit_amount().issuer());
            break;

        default:
            break;
    }

    if (output.error()) {
        return output;
    }

    auto pubKey = Data(input.public_key().begin(), input.public_key().end());
    transaction.pub_key = pubKey;

    auto unsignedTx = transaction.getPreImage();
    auto hash = Hash::sha512(unsignedTx);
    auto half = Data(hash.begin(), hash.begin() + 32);
    if (!publicKey.verifyAsDER(signature, half)) {
        output.set_error(Common::Proto::SigningError::Error_signing);
        output.set_error_message("Signature verification failed");
        return output;
    }

    transaction.signature = signature;

    auto encoded = transaction.serialize();
    output.set_encoded(encoded.data(), encoded.size());
    return output;
}

void Signer::signPayment(const Proto::SigningInput& input,
                         Proto::SigningOutput& output,
                         Transaction& transaction) {
    const int64_t tag = input.op_payment().destination_tag();
    if (tag > std::numeric_limits<uint32_t>::max() || tag < 0) {
        output.set_error(Common::Proto::SigningError::Error_invalid_memo);
        return;
    }

    switch (input.op_payment().amount_oneof_case()) {
        case Proto::OperationPayment::kAmount:
            transaction.createXrpPayment(
                input.op_payment().amount(),
                input.op_payment().destination(),
                tag);
            break;

        case Proto::OperationPayment::kCurrencyAmount:
            transaction.createTokenPayment(
                input.op_payment().currency_amount().currency(),
                input.op_payment().currency_amount().value(),
                input.op_payment().currency_amount().issuer(),
                input.op_payment().destination(),
                tag);
            break;

        default:
            break;
    }
}

void Signer::signNfTokenCancelOffer(const Proto::SigningInput& input, Transaction& transaction) noexcept {
    std::vector<std::string> token_offers;
    for (int i = 0; i < input.op_nftoken_cancel_offer().token_offers_size(); i++) {
        token_offers.emplace_back(input.op_nftoken_cancel_offer().token_offers(i));
    }

    transaction.createNFTokenCancelOffer(token_offers);
}

} // namespace TW::Ripple
