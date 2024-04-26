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
