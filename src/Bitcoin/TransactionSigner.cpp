// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TransactionSigner.h"

#include "SignatureBuilder.h"

#include "../BitcoinDiamond/Transaction.h"
#include "../BitcoinDiamond/TransactionBuilder.h"
#include "../Groestlcoin/Transaction.h"
#include "../Verge/Transaction.h"
#include "../Verge/TransactionBuilder.h"
#include "../Zcash/Transaction.h"
#include "../Zcash/TransactionBuilder.h"
#include "../Zen/TransactionBuilder.h"

namespace TW::Bitcoin {

template <typename Transaction, typename TransactionBuilder>
TransactionPlan TransactionSigner<Transaction, TransactionBuilder>::plan(const SigningInput& input) {
    return TransactionBuilder::plan(input);
}

template <typename Transaction, typename TransactionBuilder>
Result<Transaction, Common::Proto::SigningError> TransactionSigner<Transaction, TransactionBuilder>::sign(const SigningInput& input, bool estimationMode, std::optional<SignaturePubkeyList> optionalExternalSigs) {
    TransactionPlan plan;
    if (input.plan.has_value()) {
        plan = input.plan.value();
    } else {
        plan = TransactionBuilder::plan(input);
    }
    auto tx_result = TransactionBuilder::template build<Transaction>(plan, input);
    if (!tx_result) {
        return Result<Transaction, Common::Proto::SigningError>::failure(tx_result.error());
    }
    Transaction transaction = tx_result.payload();
    SigningMode signingMode =
        estimationMode ? SigningMode_SizeEstimationOnly : optionalExternalSigs.has_value() ? SigningMode_External
                                                                                           : SigningMode_Normal;
    SignatureBuilder<Transaction> signer(std::move(input), plan, transaction, signingMode, optionalExternalSigs);
    return signer.sign();
}

template <typename Transaction, typename TransactionBuilder>
Result<HashPubkeyList, Common::Proto::SigningError> TransactionSigner<Transaction, TransactionBuilder>::preImageHashes(const SigningInput& input) {
    TransactionPlan plan;
    if (input.plan.has_value()) {
        plan = input.plan.value();
    } else {
        plan = TransactionBuilder::plan(input);
    }
    auto tx_result = TransactionBuilder::template build<Transaction>(plan, input);
    if (!tx_result) {
        return Result<HashPubkeyList, Common::Proto::SigningError>::failure(tx_result.error());
    }
    Transaction transaction = tx_result.payload();
    SignatureBuilder<Transaction> signer(std::move(input), plan, transaction, SigningMode_HashOnly);
    auto signResult = signer.sign();
    if (!signResult) {
        return Result<HashPubkeyList, Common::Proto::SigningError>::failure(signResult.error());
    }
    return Result<HashPubkeyList, Common::Proto::SigningError>::success(signer.getHashesForSigning());
}

// Explicitly instantiate a Signers for compatible transactions.
template class Bitcoin::TransactionSigner<Bitcoin::Transaction, TransactionBuilder>;
template class Bitcoin::TransactionSigner<Zcash::Transaction, Zcash::TransactionBuilder>;
template class Bitcoin::TransactionSigner<Bitcoin::Transaction, Zen::TransactionBuilder>;
template class Bitcoin::TransactionSigner<Groestlcoin::Transaction, TransactionBuilder>;
template class Bitcoin::TransactionSigner<Verge::Transaction, Verge::TransactionBuilder>;
template class Bitcoin::TransactionSigner<BitcoinDiamond::Transaction, BitcoinDiamond::TransactionBuilder>;

} // namespace TW::Bitcoin
