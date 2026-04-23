// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "Tron/Protobuf/TronInternal.pb.h"
#include "proto/TransactionCompiler.pb.h"
#include "../PrivateKey.h"
#include "../Result.h"
#include "../proto/Tron.pb.h"

namespace TW::Tron {

/// Helper class that performs Tron transaction signing.
class Signer {
  public:
    Proto::SigningInput input;
    Signer() = delete;
    /// Initializes a transaction signer.
    explicit Signer(const Proto::SigningInput& input) : input(input) {}
    /// Signs the given transaction.
    static Proto::SigningOutput sign(const Proto::SigningInput& input);
    static Proto::SigningOutput signRawJson(const Proto::SigningInput& input);
    /// Compiles a raw JSON transaction with an externally provided signature.
    /// Uses deserializeAndValidateRawJson to validate the transaction before attaching the signature.
    static Proto::SigningOutput compileRawJson(const std::string& rawJson, const Data& signature);
    /// Returns the serialized raw transaction bytes for an externally validated raw JSON transaction.
    /// Uses deserializeAndValidateRawJson to validate before returning serializeTxRawData(tx).
    /// Returns a failure Result with a pre-filled SigningOutput on validation error.
    static Result<Data, Proto::SigningOutput> signaturePreimageRawJson(const std::string& rawJson);
    /// Deserializes and validates a raw JSON transaction string.
    /// Checks that txID == sha256(raw_data_hex) and txID == sha256(serialize(raw_data)).
    /// Returns the deserialized transaction on success, or a pre-filled SigningOutput with
    /// the appropriate error code and message on failure.
    static Result<protocol::Transaction, Proto::SigningOutput> deserializeAndValidateRawJson(const std::string& rawJson);
    Proto::SigningOutput compile(const Data& signature) const;
    TxCompiler::Proto::PreSigningOutput signaturePreimage() const;
};

} // namespace TW::Tron
