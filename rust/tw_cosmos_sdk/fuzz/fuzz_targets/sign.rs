#![no_main]

use libfuzzer_sys::fuzz_target;
use tw_coin_entry::test_utils::test_context::TestCoinContext;
use tw_cosmos_sdk::context::StandardCosmosContext;
use tw_cosmos_sdk::modules::signer::tw_signer::TWSigner;
use tw_keypair::tw::PublicKeyType;
use tw_proto::Cosmos::Proto;

fuzz_target!(|input: Proto::SigningInput<'_>| {
    let coin = TestCoinContext::default()
        .with_public_key_type(PublicKeyType::Secp256k1)
        .with_hrp("cosmos");

    let _ = TWSigner::<StandardCosmosContext>::sign(&coin, input);
});
