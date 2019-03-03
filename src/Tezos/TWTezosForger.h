// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

#include "proto/Tezos.pb.h"

std::string forgeBranch(std::string);
std::string forgeOperation(TW::Tezos::Proto::Operation);

// Exposed for testing.
std::string forgeBool(bool);
std::string forgeAddress(std::string);
std::string forgeZarith(std::string);
