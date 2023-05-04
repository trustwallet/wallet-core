#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};
use tw_encoding::{base32, base58, base64, ffi::Base58Alphabet};

#[derive(arbitrary::Arbitrary, Debug)]
struct BaseDecodeInput<'a> {
    data: &'a str,
    alphabet_base58: Base58Alphabet,
    padding: bool,
    is_url: bool,
}

fuzz_target!(|input: BaseDecodeInput<'_>| {
    base32::decode(input.data, None, input.padding).ok();
    base58::decode(input.data, input.alphabet_base58.into()).ok();
    base64::decode(input.data, input.is_url).ok();
});
