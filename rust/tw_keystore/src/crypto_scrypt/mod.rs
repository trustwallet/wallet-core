// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::crypto_scrypt::params::{InvalidParams, Params};
use pbkdf2::pbkdf2_hmac;
use sha2::Sha256;

pub mod params;
mod romix;

/// The scrypt key derivation function.
/// Original: https://github.com/RustCrypto/password-hashes/blob/a737bef1f992368f165face097d621bb1e76eba4/scrypt/src/lib.rs#L89
///
/// The only reason we should have rewritten the function is that it does unnecessary `log_n >= r * 16` check:
/// https://github.com/RustCrypto/password-hashes/blob/a737bef1f992368f165face097d621bb1e76eba4/scrypt/src/params.rs#L67-L72
pub fn scrypt(password: &[u8], salt: &[u8], params: &Params) -> Result<Vec<u8>, InvalidParams> {
    params.check_params()?;

    // The checks in the `Params::check_params` guarantee
    // that the following is safe:
    let n = params.n as usize;
    let r128 = (params.r as usize) * 128;
    let pr128 = (params.p as usize) * r128;
    let nr128 = n * r128;

    let mut b = vec![0u8; pr128];
    pbkdf2_hmac::<Sha256>(password, salt, 1, &mut b);

    let mut v = vec![0u8; nr128];
    let mut t = vec![0u8; r128];

    for chunk in &mut b.chunks_mut(r128) {
        romix::scrypt_ro_mix(chunk, &mut v, &mut t, n);
    }

    let mut output = vec![0u8; params.desired_len];
    pbkdf2_hmac::<Sha256>(password, &b, 1, &mut output);
    Ok(output)
}
