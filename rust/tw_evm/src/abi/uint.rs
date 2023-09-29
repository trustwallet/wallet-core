// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::{AbiError, AbiErrorKind, AbiResult};

// https://docs.soliditylang.org/en/latest/abi-spec.html#types
pub fn check_uint_bits(bits: usize) -> AbiResult<()> {
    if bits % 8 != 0 || bits == 0 || bits > 256 {
        return Err(AbiError(AbiErrorKind::Error_invalid_uint_value));
    }
    Ok(())
}
