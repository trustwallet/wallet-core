// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWDerivationPath.h>
#include <TrustWalletCore/TWPurpose.h>
#include <TrustWalletCore/TWString.h>

#include "../DerivationPath.h"

using namespace TW;

struct TWDerivationPath* _Nonnull TWDerivationPathCreate(enum TWPurpose purpose, uint32_t coin, uint32_t account, uint32_t change, uint32_t address) {
    return new TWDerivationPath{DerivationPath(purpose, coin, account, change, address)};
}

struct TWDerivationPath* _Nullable TWDerivationPathCreateWithString(TWString* _Nonnull string) {
    auto& str = *reinterpret_cast<const std::string*>(string);
    try {
        return new TWDerivationPath{DerivationPath(str)};
    } catch (...) {
        return nullptr;
    }
}

void TWDerivationPathDelete(struct TWDerivationPath* _Nonnull path) {
    delete path;
}

uint32_t TWDerivationPathIndicesCount(struct TWDerivationPath* _Nonnull path) {
    return static_cast<uint32_t>(path->impl.indices.size());
}

struct TWDerivationPathIndex* _Nullable TWDerivationPathIndexAt(struct TWDerivationPath* _Nonnull path, uint32_t index) {
    if (index >= path->impl.indices.size()) {
        return nullptr;
    }
    return new TWDerivationPathIndex{path->impl.indices[index]};
}

enum TWPurpose TWDerivationPathPurpose(struct TWDerivationPath* _Nonnull path) {
    return path->impl.purpose();
}

uint32_t TWDerivationPathCoin(struct TWDerivationPath* _Nonnull path) {
    return path->impl.coin();
}

uint32_t TWDerivationPathAccount(struct TWDerivationPath* _Nonnull path) {
    return path->impl.account();
}

uint32_t TWDerivationPathChange(struct TWDerivationPath* _Nonnull path) {
    return path->impl.change();
}

uint32_t TWDerivationPathAddress(struct TWDerivationPath* _Nonnull path) {
    return path->impl.address();
}

TWString* _Nonnull TWDerivationPathDescription(struct TWDerivationPath* _Nonnull path) {
    return TWStringCreateWithUTF8Bytes(path->impl.string().c_str());
}
