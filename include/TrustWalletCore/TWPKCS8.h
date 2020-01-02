// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_STRUCT
struct TWPKCS8 {
    uint8_t unused; // C doesn't allow zero-sized struct
};

/// Encodes a ed25519 private key into PKCS#8 format.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWPKCS8EncodeED25519PrivateKey(TWData *_Nonnull PrivateKey);

/// Decodes a PKCS#8 ed25519 private key.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWPKCS8DecodeED25519PrivateKey(TWData *_Nonnull data);


TW_EXTERN_C_END
