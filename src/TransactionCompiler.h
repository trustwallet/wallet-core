// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <TrustWalletCore/TWCoinType.h>
#include "Data.h"
#include "CoinEntry.h"

#include <string>
#include <vector>

namespace TW {

/// Non-core transaction utility methods, like building a transaction using an external signature
class TransactionCompiler {
public:
    /// Build a coin-specific SigningInput protobuf transaction input, from simple transaction parameters
    static Data buildInput(TWCoinType coinType, const std::string& from, const std::string& to, const std::string& amount, const std::string& asset, const std::string& memo, const std::string& chainId);

    /// Obtain pre-signing hash of a transaction. 
    /// It will return a proto object named `PreSigningOutput` which will include hash.
    /// We provide a default `PreSigningOutput` in TransactionCompiler.proto.
    /// For some special coins, such as bitcoin, we will create a custom `PreSigningOutput` object in its proto file.
    static Data preImageHashes(TWCoinType coinType, const Data& txInputData);

    /// Compile a complete transation with an external signature, put together from transaction input and provided public key and signature
    static Data compileWithSignatures(TWCoinType coinType, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<Data>& publicKeys);
    
    static Data compileWithSignaturesAndPubKeyType(TWCoinType coinType, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<Data>& publicKeys, TWPublicKeyType pubKeyType);

};

} // namespace TW
