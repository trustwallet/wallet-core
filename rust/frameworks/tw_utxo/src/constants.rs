// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

/// A standard transaction is limited to 400k weight units (WU).
/// https://bitcoin.stackexchange.com/questions/35570/what-is-the-maximum-number-of-inputs-outputs-a-transaction-can-have
pub const MAX_TRANSACTION_WEIGHT: usize = 400_000;
