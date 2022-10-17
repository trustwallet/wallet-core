static WALLET_CORE_PROJECT_DIR: &str = "../../build";
    // libs to link with
    static LIBS: [&str; 4] = ["c++", "protobuf", "TrezorCrypto", "TrustWalletCore"];

fn main() {
    println!("cargo:rustc-link-search=native={}", WALLET_CORE_PROJECT_DIR);
    println!("cargo:rustc-link-search=native={}/trezor-crypto", WALLET_CORE_PROJECT_DIR);

    for i in 0..LIBS.len() {
        println!("cargo:rustc-link-lib={}", LIBS[i]);
    }
}
