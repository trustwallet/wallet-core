// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include <cstdint> // << std::uint8_t

struct MessageHeader {
    // The number of signatures required for this message to be considered
    // valid. The signatures must match the first `numRequiredSignatures` of
    // `accountKeys`.
    uint8_t numRequiredSignatures = 0;
    // The last numRequiredSignatures of the signed keys are
    // read-only accounts.
    uint8_t numReadOnlySignedAccounts = 0;
    // The last numReadOnlyUnsignedAccounts of the unsigned keys are
    // read-only accounts.
    uint8_t numReadOnlyUnsignedAccounts = 0;
};
