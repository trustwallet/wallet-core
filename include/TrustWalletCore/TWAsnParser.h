// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWData.h"

TW_EXTERN_C_BEGIN

/// Represents an ASN.1 DER parser.
TW_EXPORT_STRUCT
struct TWAsnParser;

/// Parses the given ECDSA signature from ASN.1 DER encoded bytes.
///
/// \param encoded The ASN.1 DER encoded signature.
/// \return The ECDSA signature standard binary representation: RS, where R - 32 byte array, S - 32 byte array.
TW_EXPORT_STATIC_METHOD
TWData* _Nullable TWAsnParserEcdsaSignatureFromDer(TWData* _Nonnull encoded);

TW_EXTERN_C_END
