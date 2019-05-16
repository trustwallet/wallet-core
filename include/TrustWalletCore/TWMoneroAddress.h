// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a Monero address
TW_EXPORT_CLASS
struct TWMoneroAddress;

/// Compares two addresses for equality
TW_EXPORT_STATIC_METHOD
bool TWMoneroAddressEqual(struct TWMoneroAddress *_Nonnull lhs, struct TWSemuxAddress *_Nonnull rhs);


/// Determines if a string is a valid address
TW_EXPORT_STATIC_METHOD
bool TWMoneroAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representation
TW_EXPORT_STATIC_METHOD
struct TWMoneroAddress *_Nonnull TWMoneroAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publickey);

TW_EXPORT_METHOD
void TWMoneroAddressDelete(struct TWMoneroAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWMoneroAddressDescription(struct TWMoneroAddress *_Nonnull address);

TW_EXTERN_C_END