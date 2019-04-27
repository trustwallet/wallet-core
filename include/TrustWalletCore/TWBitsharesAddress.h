#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"


TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a Bravo address.
TW_EXPORT_CLASS
struct TWBitsharesAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWBitsharesAddressEqual(struct TWBitsharesAddress *_Nonnull lhs, struct TWBitsharesAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWBitsharesAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWBitsharesAddress *_Nullable TWBitsharesAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWBitsharesAddress *_Nonnull TWBitsharesAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

/// Creates an address from a key hash.
TW_EXPORT_STATIC_METHOD
struct TWBitsharesAddress *_Nullable TWBitsharesAddressCreateWithKeyHash(TWData *_Nonnull keyHash);

TW_EXPORT_METHOD
void TWBitsharesAddressDelete(struct TWBitsharesAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWBitsharesAddressDescription(struct TWBitsharesAddress *_Nonnull address);

TW_EXTERN_C_END