#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};
use tw_encoding::{base32, base58, base64, ffi::Base58Alphabet};

#[derive(arbitrary::Arbitrary, Debug)]
struct BaseEncodeInput<'a> {
    data: &'a [u8],
    alphabet_base58: Base58Alphabet,
    padding: bool,
    is_url: bool,
}

fuzz_target!(|input: BaseEncodeInput<'_>| {
    base32::encode(input.data, None, input.padding).ok();
    base58::encode(input.data, input.alphabet_base58.into());
    base64::encode(input.data, input.is_url);
});
