// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Bravo/Operation.h"

using namespace TW::Bravo;

// Steem transfer operation is similar to Bravo. 
// Just creating TWSteemOperation struct for C interface 

struct TWSteemOperation {
	std::unique_ptr<TW::Bravo::Operation> implP;
};