// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"
#include "TWAnyAddress.h"
#include "TWElrondNetworkConfig.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWElrondTransactionFactory;

TW_EXPORT_STATIC_METHOD
struct TWElrondTransactionFactory *_Nonnull TWElrondTransactionFactoryCreate(struct TWElrondNetworkConfig *_Nonnull networkConfig);

TW_EXPORT_METHOD
void TWElrondTransactionFactoryDelete(struct TWElrondTransactionFactory *_Nonnull self);

TW_EXPORT_METHOD
TWData *_Nonnull TWElrondTransactionFactoryCreateEGLDTransfer(
    struct TWElrondTransactionFactory *_Nonnull self, 
    TWString *_Nonnull sender,
    TWString *_Nonnull receiver,
    TWString *_Nonnull amount);

TW_EXPORT_METHOD
TWData *_Nonnull TWElrondTransactionFactoryCreateESDTTransfer(
    struct TWElrondTransactionFactory *_Nonnull self, 
    TWString *_Nonnull sender,
    TWString *_Nonnull receiver,
    TWString *_Nonnull tokenIdentifier,
    TWString *_Nonnull amount);

TW_EXPORT_METHOD
TWData *_Nonnull TWElrondTransactionFactoryCreateESDTNFTTransfer(
    struct TWElrondTransactionFactory *_Nonnull self, 
    TWString *_Nonnull sender,
    TWString *_Nonnull receiver,
    TWString *_Nonnull collection,
    uint64_t nonce,
    TWString *_Nonnull quantity);

TW_EXTERN_C_END
