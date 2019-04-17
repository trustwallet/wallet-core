#include <TrustWalletCore/TWBravoOperation.h>
#include <boost/lexical_cast.hpp>

#include "../Bravo/Operation.h"
#include "../Bravo/Asset.h"

#include <string>

using namespace TW;
using namespace TW::Bravo;

struct TWBravoOperation *_Nullable TWBravoOperationNewTransferOperation(TWString *_Nonnull recipient, TWString *_Nonnull sender, TWString *_Nonnull amount, bool isTestNet, TWString *_Nonnull memo) {
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
        auto transOp = new TransferOperation(*to, *from, amt, isTestNet, *note);
        return new TWBravoOperation { std::unique_ptr<Operation>(transOp) };
    } catch (...) {
        return nullptr;
    }
}

size_t TWBravoOperationMaxMemoSize() {
    return MaxMemoSize;
}

size_t TWBravoOperationMaxAccountNameSize() {
    return MaxAccountNameSize;
}

size_t TWBravoOperationAssetDecimals() {
    return Asset::decimals;
}

void TWBravoOperationDelete(struct TWBravoOperation *_Nonnull operation) {
	delete operation;
}