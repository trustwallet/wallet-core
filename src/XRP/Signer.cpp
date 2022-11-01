// Copyright © 2017-2022 Trust Wallet.
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
            /* fee */input.fee(),
            /* flags */input.flags(),
            /* sequence */input.sequence(),
            /* last_ledger_sequence */input.last_ledger_sequence(),
            /* account */Address(input.account()));
    switch (input.operation_oneof_case()) {
        case Proto::SigningInput::kOpPayment: {
            const int64_t tag = input.op_payment().destination_tag();
            if (tag > std::numeric_limits<uint32_t>::max() || tag < 0) {
                output.set_error(Common::Proto::SigningError::Error_invalid_memo);
                return output;
            }

            switch (input.op_payment().amount_oneof_case()) {
                case Proto::OperationPayment::kAmount:
                    transaction.createXrpPayment(
                        /* amount */input.op_payment().amount(),
                        /* destination */input.op_payment().destination(),
                        /* destination_tag*/tag);
                    break;

                case Proto::OperationPayment::kCurrencyAmt:
                    transaction.createTokenPayment(
                        /* currency */input.op_payment().currency_amt().currency(),
                        /* value */input.op_payment().currency_amt().value(),
                        /* issuer */input.op_payment().currency_amt().issuer(),
                        /* destination */input.op_payment().destination(),
                        /* destination_tag*/tag);
                    break;

                default:
                    break;
            }

            break;
        }

        case Proto::SigningInput::kOpNftokenBurn:
            transaction.createNFTokenBurn(
                /* nftoken_id */input.op_nftoken_burn().nftoken_id());
            break;

        case Proto::SigningInput::kOpNftokenCreateOffer:
            transaction.createNFTokenCreateOffer(
                /* token_id */input.op_nftoken_create_offer().nftoken_id(),
                /* destination */input.op_nftoken_create_offer().destination());
            break;

        case Proto::SigningInput::kOpNftokenAcceptOffer:
            transaction.createNFTokenAcceptOffer(/* sell_offer */input.op_nftoken_accept_offer().sell_offer());
            break;

        case Proto::SigningInput::kOpNftokenCancelOffer: {
                std::vector<std::string> token_offers;
                for (int i = 0; i < input.op_nftoken_cancel_offer().token_offers_size(); i++) {
                    token_offers.push_back(input.op_nftoken_cancel_offer().token_offers(i));
                }

                transaction.createNFTokenCancelOffer(/* token_offers */token_offers);
            }

            break;

        case Proto::SigningInput::kOpTrustSet:
            transaction.createTrustSet(
                /* currency */input.op_trust_set().limit_amt().currency(),
                /* value */input.op_trust_set().limit_amt().value(),
                /* issuer */input.op_trust_set().limit_amt().issuer());

        default:
            break;
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
    //transaction.flags |= fullyCanonical; // deprecated
    transaction.pub_key = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1).bytes;

    auto unsignedTx = transaction.getPreImage();
    auto hash = Hash::sha512(unsignedTx);
    auto half = Data(hash.begin(), hash.begin() + 32);

    transaction.signature = privateKey.signAsDER(half);
}

} // namespace TW::Ripple
