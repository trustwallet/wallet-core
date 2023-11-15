#![no_main]

use libfuzzer_sys::fuzz_target;
use tw_aptos::signer::Signer;
use tw_proto::Aptos::Proto;

fuzz_target!(|input: Proto::SigningInput<'_>| {
    let _ = Signer::sign_proto(input);
});
