#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"

#include "TWBravoAddressType.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a Steem address.
TW_EXPORT_CLASS
struct TWSteemAddress;

/// Compares two addresses for equality.
TW_EXPORT_STATIC_METHOD
bool TWSteemAddressEqual(struct TWSteemAddress *_Nonnull lhs, struct TWSteemAddress *_Nonnull rhs);

/// Determines if the string is a valid address.
TW_EXPORT_STATIC_METHOD
bool TWSteemAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representaion.
TW_EXPORT_STATIC_METHOD
struct TWSteemAddress *_Nullable TWSteemAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key.
TW_EXPORT_STATIC_METHOD
struct TWSteemAddress *_Nonnull TWSteemAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, enum TWBravoAddressType type);

/// Creates an address from a key hash.
TW_EXPORT_STATIC_METHOD
struct TWSteemAddress *_Nullable TWSteemAddressCreateWithKeyHash(TWData *_Nonnull keyHash, enum TWBravoAddressType type);

TW_EXPORT_METHOD
void TWSteemAddressDelete(struct TWSteemAddress *_Nonnull address);

/// Returns the address string representation.
TW_EXPORT_PROPERTY
TWString *_Nonnull TWSteemAddressDescription(struct TWSteemAddress *_Nonnull address);

TW_EXTERN_C_END