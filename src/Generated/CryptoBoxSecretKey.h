// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "rust/Wrapper.h"

using CryptoBoxSecretKeyPtr = std::shared_ptr<TW::Rust::TWCryptoBoxSecretKey>;

struct CryptoBoxSecretKey {
	explicit CryptoBoxSecretKey(TW::Rust::TWCryptoBoxSecretKey* raw_ptr): ptr(raw_ptr, TW::Rust::tw_crypto_box_secret_key_delete) {}

	CryptoBoxSecretKeyPtr ptr;
};

struct TWCryptoBoxSecretKey {
	CryptoBoxSecretKey impl;
};

