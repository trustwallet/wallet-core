#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

#include "TWBravoAddressType.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a Bravo address.
TW_EXPORT_CLASS
struct TWBravoAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWBravoAddressEqual(struct TWBravoAddress *_Nonnull lhs, struct TWBravoAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWBravoAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWBravoAddress *_Nullable TWBravoAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWBravoAddress *_Nonnull TWBravoAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, enum TWBravoAddressType type);

/// Creates an address from a key hash.
TW_EXPORT_STATIC_METHOD
struct TWBravoAddress *_Nullable TWBravoAddressCreateWithKeyHash(TWData *_Nonnull keyHash, enum TWBravoAddressType type);

TW_EXPORT_METHOD
void TWBravoAddressDelete(struct TWBravoAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWBravoAddressDescription(struct TWBravoAddress *_Nonnull address);

TW_EXTERN_C_END