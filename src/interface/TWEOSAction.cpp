#include <TrustWalletCore/TWEOSAction.h>

#include "../EOS/Action.h"

#include <string>

using namespace TW;
using namespace TW::EOS;

struct TWEOSAction *_Nullable TWEOSActionNewTransferAction(TWString *_Nonnull currency, TWString *_Nonnull recipient, TWString *_Nonnull sender, TWString *_Nonnull assetString, TWString *_Nonnull memo) {
    auto cur = reinterpret_cast<const std::string *>(currency);
    auto to = reinterpret_cast<const std::string *>(recipient);
    auto from = reinterpret_cast<const std::string *>(sender);
    auto asset = reinterpret_cast<const std::string *>(assetString);
    auto note = reinterpret_cast<const std::string *>(memo);
    try {
        return new TWEOSAction { TransferAction(*cur, *from, *to, *asset, *note) };
    } catch (...) {
        return nullptr;
    }
}

void TWEOSActionDelete(struct TWEOSAction *_Nonnull operation) {
    delete operation;
}