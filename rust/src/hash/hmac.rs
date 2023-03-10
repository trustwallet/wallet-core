// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use sha2::Sha256;
use hmac::{Hmac, Mac};
use crate::memory::CByteArray;

type HmacSha256 = Hmac<Sha256>;

#[no_mangle]
pub extern "C" fn hmac__sha256(key: *const u8, key_len: usize, input: *const u8, input_len: usize) -> CByteArray {
    let key = unsafe { std::slice::from_raw_parts(key, key_len) };
    let input = unsafe { std::slice::from_raw_parts(input, input_len) };
    let mut mac = HmacSha256::new_from_slice(key).unwrap();
    mac.update(input);
    let res = mac.finalize();
    let code_bytes = res.into_bytes();
    code_bytes.to_vec().into()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_hmac_sha256() {
        let key = hex::decode("531cbfcf12a168faff61af28bf437377397b4bf435ee732cf4ac95761a651f14").unwrap();
        let key = key.as_slice();
        let data = hex::decode("f300888ca4f512cebdc0020ff0f7224c7f896315e90e172bed65d005138f224d").unwrap();
        let data = data.as_slice();
        let result = hmac__sha256(key.as_ptr(), key.len(), data.as_ptr(), data.len());
        let result = unsafe { std::slice::from_raw_parts(result.data, result.size) };
        let expected = "a7301d5563614e3955750e4480aabf7753f44b4975308aeb8e23c31e114962ab".to_string();
        assert_eq!(hex::encode(result), expected);
    }
}
