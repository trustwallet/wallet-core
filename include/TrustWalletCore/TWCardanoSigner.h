// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWCardanoProto.h"

TW_EXTERN_C_BEGIN

/// Helper class to sign Cardano transactions.
TW_EXPORT_CLASS
struct TWCardanoSigner;

/// Plan a transaction: compute fee, decide input UTXOs
TW_EXPORT_STATIC_METHOD
TW_Cardano_Proto_TransactionPlan TWCardanoSignerPlanTransaction(TW_Cardano_Proto_SigningInput input);

/// Build and sign a Cardano transaction, using plan from planTransaction().
TW_EXPORT_STATIC_METHOD
TW_Cardano_Proto_SigningOutput TWCardanoSignerSign(TW_Cardano_Proto_SigningInput input, TW_Cardano_Proto_TransactionPlan plan);

TW_EXTERN_C_END
