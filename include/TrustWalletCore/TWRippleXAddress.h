// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWString.h"
#include "TWData.h"
#include "TWHRP.h"

TW_EXTERN_C_BEGIN

struct TWPublicKey;

/// Represents a Ripple X-address.
TW_EXPORT_CLASS
struct TWRippleXAddress;

/// Compares two addresses for equality.
///
/// \param lhs left non-null pointer to a Ripple Address
/// \param rhs right non-null pointer to a Ripple Address
/// \return true if both address are equal, false otherwise
TW_EXPORT_STATIC_METHOD
bool TWRippleXAddressEqual(struct TWRippleXAddress *_Nonnull lhs, struct TWRippleXAddress *_Nonnull rhs);

/// Determines if the string is a valid Ripple address.
///
/// \param string Non-null pointer to a string that represent the Ripple Address to be checked
/// \return true if the given address is a valid Ripple address, false otherwise
TW_EXPORT_STATIC_METHOD
bool TWRippleXAddressIsValidString(TWString *_Nonnull string);

/// Creates an address from a string representation.
///
/// \param string Non-null pointer to a string that should be a valid ripple address
/// \note Should be deleted with \TWRippleXAddressDelete
/// \return Null pointer if the given string is an invalid ripple address, pointer to a Ripple address otherwise
TW_EXPORT_STATIC_METHOD
struct TWRippleXAddress *_Nullable TWRippleXAddressCreateWithString(TWString *_Nonnull string);

/// Creates an address from a public key and destination tag.
///
/// \param publicKey Non-null pointer to a public key
/// \param tag valid ripple destination tag (1-10)
/// \note Should be deleted with \TWRippleXAddressDelete
/// \return Non-null pointer to a Ripple Address
TW_EXPORT_STATIC_METHOD
struct TWRippleXAddress *_Nonnull TWRippleXAddressCreateWithPublicKey(struct TWPublicKey *_Nonnull publicKey, uint32_t tag);

/// Delete the given ripple address
///
/// \param address Non-null pointer to a Ripple Address
TW_EXPORT_METHOD
void TWRippleXAddressDelete(struct TWRippleXAddress *_Nonnull address);

/// Returns the address string representation.
///
/// \param address Non-null pointer to a Ripple Address
/// \return Non-null pointer to the ripple address string representation
TW_EXPORT_PROPERTY
TWString *_Nonnull TWRippleXAddressDescription(struct TWRippleXAddress *_Nonnull address);

/// Returns the destination tag.
///
/// \param address Non-null pointer to a Ripple Address
/// \return The destination tag of the given Ripple Address (1-10)
TW_EXPORT_PROPERTY
uint32_t TWRippleXAddressTag(struct TWRippleXAddress *_Nonnull address);

TW_EXTERN_C_END
