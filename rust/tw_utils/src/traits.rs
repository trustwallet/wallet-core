// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub trait ToBytesVec {
    fn to_vec(&self) -> Vec<u8>;
}

impl<T> ToBytesVec for T
where
    T: AsRef<[u8]>,
{
    fn to_vec(&self) -> Vec<u8> {
        self.as_ref().to_vec()
    }
}
