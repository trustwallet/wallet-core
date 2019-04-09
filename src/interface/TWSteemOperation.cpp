// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWSteemOperation.h>
#include <boost/lexical_cast.hpp>

#include "../Steem/Operation.h"
#include "../Bravo/Operation.h"
#include "../Bravo/Asset.h"

#include <string>

using namespace TW;
using namespace TW::Bravo;

struct TWSteemOperation *_Nullable TWSteemOperationNewTransferOperation(TWString *_Nonnull recipient, TWString *_Nonnull sender, TWString *_Nonnull amount, bool isTestNet, TWString *_Nonnull memo) {
    int64_t amt;
    try {
        // ensure the amount is in range of Int64
        double dblAmt = boost::lexical_cast<double>(amount);
        if (dblAmt > static_cast<double>(INT64_MAX) || dblAmt < static_cast<double>(INT64_MIN) ) {
            return nullptr;
        }

        amt = static_cast<int64_t>(dblAmt * Asset::precision);
    } catch (...) {
        return nullptr;
    }

	auto to = reinterpret_cast<const std::string *>(recipient);
	auto from = reinterpret_cast<const std::string *>(sender);
	auto note = reinterpret_cast<const std::string *>(memo);
	try {
		Asset asset {static_cast<int64_t>(amt), Asset::decimals, isTestNet ? Steem::TestNetAssetSymbol : Steem::MainNetAssetSymbol};
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