// Copyright © 2017-2019 Trust Wallet.
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
struct TWX509 {
    uint8_t unused; // C doesn't allow zero-sized struct
};

/// Encodes ed25519 public key into X.509 certificate.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWX509EncodeED25519PublicKey(TWData *_Nonnull publicKey);

/// Decodes a X.509 certificate into ed25519 public key.
TW_EXPORT_STATIC_METHOD
TWData *_Nullable TWX509DecodeED25519PublicKey(TWData *_Nonnull data);


TW_EXTERN_C_END
