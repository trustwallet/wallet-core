// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "rust/Wrapper.h"

using CryptoBoxPublicKeyPtr = std::shared_ptr<TW::Rust::TWCryptoBoxPublicKey>;

struct CryptoBoxPublicKey {
	explicit CryptoBoxPublicKey(TW::Rust::TWCryptoBoxPublicKey* raw_ptr): ptr(raw_ptr, TW::Rust::tw_crypto_box_public_key_delete) {}

	CryptoBoxPublicKeyPtr ptr;
};

struct TWCryptoBoxPublicKey {
	CryptoBoxPublicKey impl;
};

