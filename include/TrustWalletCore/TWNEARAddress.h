#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a NEAR address.
TW_EXPORT_CLASS
struct TWNEARAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWNEARAddressEqual(struct TWNEARAddress *_Nonnull lhs, struct TWNEARAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWNEARAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWNEARAddress *_Nullable TWNEARAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWNEARAddress *_Nonnull TWNEARAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWNEARAddressDelete(struct TWNEARAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWNEARAddressDescription(struct TWNEARAddress *_Nonnull address);

TW_EXTERN_C_END
