// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TrustWalletCore/TWSolanaTransaction.h"
#include "DataVector.h"
#include "rust/Wrapper.h"

using namespace TW;

TWData *_Nonnull TWSolanaTransactionUpdateBlockhashAndSign(TWString *_Nonnull encodedTx,
                                                           TWString *_Nonnull recentBlockhash,
                                                           const struct TWDataVector *_Nonnull privateKeys) {
    auto& encodedTxRef = *reinterpret_cast<const std::string*>(encodedTx);
    auto& recentBlockhashRef = *reinterpret_cast<const std::string*>(recentBlockhash);

    Rust::TWStringWrapper encodedTxStr = encodedTxRef;
    Rust::TWStringWrapper recentBlockhashStr = recentBlockhashRef;
    Rust::TWDataVectorWrapper privateKeysVec = createFromTWDataVector(privateKeys);

    Rust::TWDataWrapper output = Rust::tw_solana_transaction_update_blockhash_and_sign(encodedTxStr.get(),
                                                                                       recentBlockhashStr.get(),
                                                                                       privateKeysVec.get());

    auto outputData = output.toDataOrDefault();
    return TWDataCreateWithBytes(outputData.data(), outputData.size());
}

TWString *_Nullable TWSolanaTransactionGetComputeUnitPrice(TWString *_Nonnull encodedTx) {
    auto& encodedTxRef = *reinterpret_cast<const std::string*>(encodedTx);

    Rust::TWStringWrapper encodedTxStr = encodedTxRef;
    Rust::TWStringWrapper maybePrice = Rust::tw_solana_transaction_get_compute_unit_price(encodedTxStr.get());

    if (!maybePrice) {
        return nullptr;
    }

    auto priceStr = maybePrice.toStringOrDefault();
    return TWStringCreateWithUTF8Bytes(priceStr.c_str());
}

TWString *_Nullable TWSolanaTransactionGetComputeUnitLimit(TWString *_Nonnull encodedTx) {
    auto& encodedTxRef = *reinterpret_cast<const std::string*>(encodedTx);

    Rust::TWStringWrapper encodedTxStr = encodedTxRef;
    Rust::TWStringWrapper maybeLimit = Rust::tw_solana_transaction_get_compute_unit_limit(encodedTxStr.get());

    if (!maybeLimit) {
        return nullptr;
    }

    auto limitStr = maybeLimit.toStringOrDefault();
    return TWStringCreateWithUTF8Bytes(limitStr.c_str());
}

TWString *_Nullable TWSolanaTransactionSetComputeUnitPrice(TWString *_Nonnull encodedTx, TWString *_Nonnull price) {
    auto& encodedTxRef = *reinterpret_cast<const std::string*>(encodedTx);
    auto& priceRef = *reinterpret_cast<const std::string*>(price);

    Rust::TWStringWrapper encodedTxStr = encodedTxRef;
    Rust::TWStringWrapper priceStr = priceRef;

    Rust::TWStringWrapper updatedTxStr = Rust::tw_solana_transaction_set_compute_unit_price(encodedTxStr.get(), priceStr.get());

    if (!updatedTxStr) {
        return nullptr;
    }

    auto updatedTx = updatedTxStr.toStringOrDefault();
    return TWStringCreateWithUTF8Bytes(updatedTx.c_str());
}

TWString *_Nullable TWSolanaTransactionSetComputeUnitLimit(TWString *_Nonnull encodedTx, TWString *_Nonnull limit) {
    auto& encodedTxRef = *reinterpret_cast<const std::string*>(encodedTx);
    auto& limitRef = *reinterpret_cast<const std::string*>(limit);

    Rust::TWStringWrapper encodedTxStr = encodedTxRef;
    Rust::TWStringWrapper limitStr = limitRef;

    Rust::TWStringWrapper updatedTxStr = Rust::tw_solana_transaction_set_compute_unit_limit(encodedTxStr.get(), limitStr.get());

    if (!updatedTxStr) {
        return nullptr;
    }

    auto updatedTx = updatedTxStr.toStringOrDefault();
    return TWStringCreateWithUTF8Bytes(updatedTx.c_str());
}

TWString *_Nullable TWSolanaTransactionSetFeePayer(TWString *_Nonnull encodedTx, TWString *_Nonnull feePayer) {
    auto& encodedTxRef = *reinterpret_cast<const std::string*>(encodedTx);
    auto& feePayerRef = *reinterpret_cast<const std::string*>(feePayer);

    const Rust::TWStringWrapper encodedTxStr = encodedTxRef;
    const Rust::TWStringWrapper feePayerStr = feePayerRef;

    Rust::TWStringWrapper updatedTxStr = Rust::tw_solana_transaction_set_fee_payer(encodedTxStr.get(), feePayerStr.get());

    if (!updatedTxStr) {
        return nullptr;
    }

    const auto updatedTx = updatedTxStr.toStringOrDefault();
    return TWStringCreateWithUTF8Bytes(updatedTx.c_str());
}
