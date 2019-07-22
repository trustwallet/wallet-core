#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a FIO address.
TW_EXPORT_CLASS
struct TWFIOAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWFIOAddressEqual(struct TWFIOAddress *_Nonnull lhs, struct TWFIOAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWFIOAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWFIOAddress *_Nullable TWFIOAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWFIOAddress *_Nonnull TWFIOAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey);

TW_EXPORT_METHOD
void TWFIOAddressDelete(struct TWFIOAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWFIOAddressDescription(struct TWFIOAddress *_Nonnull address);

TW_EXTERN_C_END
