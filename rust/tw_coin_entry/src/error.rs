// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#[derive(Debug)]
pub enum AddressError {
    MissingPrefix,
    FromHexError,
    PublicKeyTypeMismatch,
}
