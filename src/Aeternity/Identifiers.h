// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <string>

/// refers to https://github.com/aeternity/aepp-sdk-go/blob/07aa8a77e5/aeternity/identifiers.go
namespace TW::Aeternity::Identifiers {

/// default network id
static const std::string networkId = "ae_mainnet";

/// Base58 prefixes
static const std::string prefixAccountPubkey = "ak_";
static const std::string prefixSignature = "sg_";

/// Base 64 encoded transactions
static const std::string prefixTransaction = "tx_";

/// version used in the rlp message
static const uint8_t rlpMessageVersion = 1;

/// Object tags
/// \see https://github.com/aeternity/protocol/blob/master/serializations.md#binary-serialization
static const uint64_t objectTagSignedTransaction = 11;
static const uint64_t objectTagSpendTransaction = 12;

/// Tag constant for ids
/// \see https://github.com/aeternity/protocol/blob/master/serializations.md#the-id-type
static const uint8_t iDTagAccount = 1;

} // namespace TW::Aeternity::Identifiers
