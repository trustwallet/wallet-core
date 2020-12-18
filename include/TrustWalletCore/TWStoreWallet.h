// Copyright © 2019-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWString.h"
#include "TWCoinType.h"
#include "TWStoredKey.h"
#include "TWAnyAddress.h"
#include "TWPrivateKey.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWStoreWallet;

/// Creates a Wallet; path and encryption key is specified
TW_EXPORT_STATIC_METHOD
struct TWStoreWallet* _Nonnull TWStoreWalletCreate(TWString* _Nonnull path, const struct TWStoredKey* _Nonnull key);

TW_EXPORT_METHOD
void TWStoreWalletDelete(struct TWStoreWallet* _Nonnull wallet);

TW_EXPORT_PROPERTY
TWString* _Nonnull TWStoreWalletIndentifier(struct TWStoreWallet* _Nonnull wallet);

/// Returns the account for a specific coin.
TW_EXPORT_STATIC_METHOD
struct TWAnyAddress* _Nonnull TWStoreWalletGetAccount(struct TWStoreWallet* _Nonnull wallet, TWString* _Nonnull password, enum TWCoinType coin);

/// Returns the accounts for a specific coins.
//const std::vector<Account> getAccounts(const std::string& password, const std::vector<TWCoinType>& coins);

/// Returns the private key for a specific coin.
struct TWPrivateKey* _Nonnull TWStoreWalletPrivateKey(struct TWStoreWallet* _Nonnull wallet, const TWString* _Nonnull password, enum TWCoinType coin);

TW_EXTERN_C_END
