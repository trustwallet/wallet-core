// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <string>

#include "proto/Tezos.pb.h"

std::string forgeOperationList(TW::Tezos::Proto::OperationList);
int checkDecodeAndDropPrefix(const std::string& input, size_t prefixLength, uint8_t *prefix, uint8_t *output);

// Exposed for testing.
std::string forgeBool(bool);
std::string forgeAddress(std::string);
std::string forgeZarith(std::string);
std::string forgeZarith(int input);
std::string forgePublicKey(std::string publicKey);
std::string forgePublicKeyHash(const std::string &publicKeyHash);
std::string forgeBranch(std::string);
std::string forgeOperation(TW::Tezos::Proto::Operation);
