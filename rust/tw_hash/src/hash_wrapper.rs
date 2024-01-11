// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use digest::Digest;

pub fn hasher<D: Digest>(input: &[u8]) -> Vec<u8> {
    let mut hasher = D::new();
    hasher.update(input);
    let result = &hasher.finalize()[..];
    result.to_vec()
}
