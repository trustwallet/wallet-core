// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

/// TODO extend during adding new blockchains.
pub trait Prefix {
    // fn with_hrp(hrp: String) -> Option<Self> { None }
}

pub struct NoPrefix;

impl Prefix for NoPrefix {}
