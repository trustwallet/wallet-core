// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWSteemSigner.h>

#include "../Bravo/Signer.h"
#include "../Steem/Signer.h"
#include "../Steem/Transaction.h"
#include "../Data.h"

#include <string>

using namespace TW;
using namespace TW::Bravo;

struct TWSteemSigner *_Nullable TWSteemSignerCreate(TWData *_Nonnull chainID) {
	auto id = reinterpret_cast<const Data *>(chainID);
	return new TWSteemSigner{ Signer(*id) };
}

void TWSteemSignerSign(struct TWSteemSigner *_Nonnull signer, struct TWPrivateKey *_Nonnull privateKey, struct TWSteemTransaction *_Nonnull transaction) {
	signer->impl.sign(privateKey->impl, transaction->impl, TW::Steem::is_canonical);
}

void TWSteemSignerDelete(TWSteemSigner *_Nonnull signer) {
	delete signer;
}