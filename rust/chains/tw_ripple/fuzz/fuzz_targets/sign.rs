#![no_main]

use libfuzzer_sys::fuzz_target;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_proto::Ripple::Proto;
use tw_ripple::signer::RippleSigner;

fuzz_target!(|input: Proto::SigningInput<'_>| {
    let coin = TestCoinContext::default();
    let _ = RippleSigner::sign(&coin, input);
});
