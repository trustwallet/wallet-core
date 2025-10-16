// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::crypto_hd_node::error::{Error, Result};
use bip32::DerivationPath;
use std::str::FromStr;

const MAX_DEPTH: usize = 10;

pub fn derivation_path_from_str(s: &str) -> Result<DerivationPath> {
    if s.split('/').count() > MAX_DEPTH {
        return Err(Error::InvalidDerivationPath);
    }
    DerivationPath::from_str(s).map_err(|_| Error::InvalidDerivationPath)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_derivation_path_from_str() {
        assert_eq!(
            derivation_path_from_str("m/44'/60'/0'/0/0"),
            Ok(DerivationPath::from_str("m/44'/60'/0'/0/0").unwrap())
        );
    }

    #[test]
    fn test_derivation_path_from_str_invalid() {
        assert_eq!(
            derivation_path_from_str("m/44'/60'/0'/0/0/0/0/0/0/0/0/0/0/0/0/0"),
            Err(Error::InvalidDerivationPath)
        );
    }
}
