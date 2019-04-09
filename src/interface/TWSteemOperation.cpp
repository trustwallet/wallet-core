// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWSteemOperation.h>

#include "../Steem/Operation.h"
#include "../Bravo/Operation.h"
#include "../Bravo/Asset.h"

#include <string>

using namespace TW;
using namespace TW::Bravo;

struct TWSteemOperation *_Nullable TWSteemOperationNewTransferOperation(TWString *_Nonnull recipient, TWString *_Nonnull sender, uint64_t amount, bool isTestNet, TWString *_Nonnull memo) {
	if (amount > static_cast<uint64_t>(INT64_MAX) || amount == 0) {
		return nullptr;
	}

	auto to = reinterpret_cast<const std::string *>(recipient);
	auto from = reinterpret_cast<const std::string *>(sender);
	auto note = reinterpret_cast<const std::string *>(memo);
	try {
		Asset asset {static_cast<int64_t>(amount), Asset::decimals, "STEEM"};
		auto transOp = new TransferOperation(*to, *from, asset, *note);
		return new TWSteemOperation{ std::unique_ptr<Operation>(transOp) };
	}
	catch (...) {
		return nullptr;
	}
}

size_t TWSteemOperationMaxMemoSize() {
	return MaxMemoSize;
}

size_t TWSteemOperationMaxAccountNameSize() {
	return MaxAccountNameSize;
}

size_t TWSteemOperationAssetDecimals() {
	return Asset::decimals;
}

void TWSteemOperationDelete(struct TWSteemOperation *_Nonnull operation) {
	operation->implP.reset();
	delete operation;
}