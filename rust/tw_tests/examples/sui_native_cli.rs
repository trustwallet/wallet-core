// Test harness: signs / pre-images a serialized Sui `SigningInput` with the real wallet-core Rust code, the same
// code the Android and iOS libraries run. Used by the tsdk devnet test (`sui-address-balance.devnet.test.ts`) and
// for ad-hoc checks. Build: `cargo build --release -p tw_tests --example sui_native_cli`.
// usage: sui_native_cli <sign|preimage>   (stdin: hex SigningInput, stdout: hex SigningOutput)
use std::io::Read;
use tw_any_coin::ffi::tw_any_signer::tw_any_signer_sign;
use tw_any_coin::ffi::tw_transaction_compiler::tw_transaction_compiler_pre_image_hashes;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;

fn main() {
    let mode = std::env::args().nth(1).expect("mode");
    let mut stdin = String::new();
    std::io::stdin().read_to_string(&mut stdin).unwrap();
    let input = hex::decode(stdin.trim().trim_start_matches("0x")).unwrap();
    let input = TWDataHelper::create(input);
    let out = unsafe {
        match mode.as_str() {
            "sign" => tw_any_signer_sign(input.ptr(), CoinType::Sui as u32),
            "preimage" => {
                tw_transaction_compiler_pre_image_hashes(CoinType::Sui as u32, input.ptr())
            },
            m => panic!("unknown mode {m}"),
        }
    };
    let out = TWDataHelper::wrap(out).to_vec().expect("null output");
    println!("{}", hex::encode(out, false));
}
