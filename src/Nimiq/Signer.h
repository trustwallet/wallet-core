// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"
#include "../Hash.h"
#include "EdPrivateKey.h"
#include "Transaction.h"

namespace TW {
namespace Nimiq {

/// Helper class that performs Nimiq transaction signing.
class Signer {
public:
    /// Signs the given transaction.
    void sign(const EdPrivateKey& privateKey, Transaction& transaction) const noexcept;
};

}} // namespace

/// Wrapper for C interface
struct TWNimiqSigner {
    TW::Nimiq::Signer impl;
};
