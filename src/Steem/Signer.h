// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bravo/Signer.h"
#include <boost/multiprecision/cpp_int.hpp>

using namespace TW::Bravo;

namespace TW {
namespace Steem {
	static int is_canonical(uint8_t by, uint8_t sig[64]) {
		using boost::multiprecision::uint256_t;
		using namespace boost::multiprecision::literals;
		constexpr uint256_t n_2 =
			0x7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5D576E7357A4501DDFE92F46681B20A0_cppui256;

		// BIP-0062 states that sig must be in [1,n/2], however because a sig of value 0 is an invalid
		// signature under all circumstances, the lower bound does not need checking
		return memcmp(sig + 32, &n_2, sizeof(uint256_t)) <= 0;
	}
}}

struct TWSteemSigner {
	TW::Bravo::Signer impl;
};
