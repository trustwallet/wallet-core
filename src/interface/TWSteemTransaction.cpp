// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWSteemTransaction.h>

#include "../Steem/Transaction.h"
#include "../Steem/Operation.h"
#include "../Bravo/Transaction.h"

#include<string>

using namespace TW;
using namespace TW::Bravo;

struct TWSteemTransaction *_Nullable TWSteemTransactionCreate(TWString *_Nonnull referenceBlockId, uint32_t referenceBlockTime) {
	auto refBlockId = reinterpret_cast<const std::string *>(referenceBlockId);
	try {
		return new TWSteemTransaction{ Transaction(*refBlockId, referenceBlockTime) };
	}
	catch (...) {
		return nullptr;
	}
}

void TWSteemTransactionAddOperation(struct TWSteemTransaction *_Nonnull transaction, struct TWSteemOperation *_Nonnull operation) {
	transaction->impl.addOperation(operation->implP);
}

void TWSteemTransactionDelete(struct TWSteemTransaction *_Nonnull transaction) {
	delete transaction;
}

bool TWSteemTransactionIsValid(struct TWSteemTransaction *_Nonnull transaction) {
	return transaction->impl.isValid();
}