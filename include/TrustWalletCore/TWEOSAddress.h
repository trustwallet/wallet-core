#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

TW_EXPORT_ENUM(uint32_t)
enum TWEOSAddressType {
    TWEOSAddressTypeLegacy = 0,
    TWEOSAddressTypeModernK1 = 1,
};

/// Represents a EOS address.
TW_EXPORT_CLASS
struct TWEOSAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWEOSAddressEqual(struct TWEOSAddress *_Nonnull lhs, struct TWEOSAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWEOSAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWEOSAddress *_Nullable TWEOSAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWEOSAddress *_Nonnull TWEOSAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, TWEOSAddressType type);

/// Creates an address from a key hash.
TW_EXPORT_STATIC_METHOD
struct TWEOSAddress *_Nullable TWEOSAddressCreateWithKeyHash(TWData *_Nonnull keyHash, TWEOSAddressType type);

TW_EXPORT_METHOD
void TWEOSAddressDelete(struct TWEOSAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWEOSAddressDescription(struct TWEOSAddress *_Nonnull address);

TW_EXTERN_C_END