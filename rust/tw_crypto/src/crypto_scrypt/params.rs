// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::mem::size_of;

#[derive(Clone, Copy, Debug)]
pub struct InvalidParams;

pub struct Params {
    /// Scrypt parameter `N`: CPU/memory cost.
    /// Must be a power of 2.
    pub n: u32,
    /// Scrypt parameter `r`: block size.
    pub r: u32,
    /// Scrypt parameter `p`: parallelism.
    pub p: u32,
    /// Scrypt parameter `Key length`.
    pub desired_len: usize,
}

impl Params {
    /// Create a new instance of [`Params`].
    ///
    /// # Conditions
    /// - `log_n` must be less than `64`
    /// - `r` must be greater than `0` and less than or equal to `4294967295`
    /// - `p` must be greater than `0` and less than `4294967295`
    /// - `desired_len` must be greater than `9` and less than or equal to `64`
    ///
    /// Original: https://github.com/RustCrypto/password-hashes/blob/a737bef1f992368f165face097d621bb1e76eba4/scrypt/src/params.rs#L44
    ///
    /// The only reason we should have rewritten the function is that it does unnecessary `log_n >= r * 16` check:
    /// https://github.com/RustCrypto/password-hashes/blob/a737bef1f992368f165face097d621bb1e76eba4/scrypt/src/params.rs#L67-L72
    pub fn check_params(&self) -> Result<(), InvalidParams> {
        let log_n = self.try_log_n()?;

        let cond1 = (log_n as usize) < usize::BITS as usize;
        let cond2 = size_of::<usize>() >= size_of::<u32>();
        let cond3 = self.r <= usize::MAX as u32 && self.p < usize::MAX as u32;
        let cond4 = (10..=64).contains(&self.desired_len);
        if !(self.r > 0 && self.p > 0 && cond1 && (cond2 || cond3) && cond4) {
            return Err(InvalidParams);
        }

        let r = self.r as usize;
        let p = self.p as usize;
        let n = self.n as usize;

        // check that r * 128 doesn't overflow
        let r128 = r.checked_mul(128).ok_or(InvalidParams)?;

        // check that n * r * 128 doesn't overflow
        r128.checked_mul(n).ok_or(InvalidParams)?;

        // check that p * r * 128 doesn't overflow
        r128.checked_mul(p).ok_or(InvalidParams)?;

        // This check required by Scrypt:
        // check: p <= ((2^32-1) * 32) / (128 * r)
        // It takes a bit of re-arranging to get the check above into this form,
        // but it is indeed the same.
        if r * p >= 0x4000_0000 {
            return Err(InvalidParams);
        }

        // The following checks are copied from C++:
        // https://github.com/trustwallet/wallet-core/blob/b530432921d7a9709428b0162673e0ab72de1c3d/src/Keystore/ScryptParameters.cpp#L27-L42

        if (n & (n - 1)) != 0 || n < 2 {
            // Invalid cost factor.
            return Err(InvalidParams);
        }

        let max_r = u32::MAX as usize / 128_usize / p;
        let max_n = u32::MAX as usize / 128 / r;
        if r > max_r || n > max_n {
            return Err(InvalidParams);
        }

        Ok(())
    }

    fn try_log_n(&self) -> Result<u8, InvalidParams> {
        let log_n = self.n.checked_ilog2().ok_or(InvalidParams)?;
        // `Params::n` must be equal to 2^log_n.
        if 1 << log_n != self.n {
            return Err(InvalidParams);
        }
        log_n.try_into().map_err(|_| InvalidParams)
    }
}
