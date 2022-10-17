use libc::c_char;
use std::ffi::CString;
use std::ffi::CStr;

// signatures
extern "C" {
    fn TWStringCreateWithUTF8Bytes(twstring: *const c_char) -> *const i8;
    fn TWStringUTF8Bytes(twstring: *const i8) -> *const c_char;

    fn TWHDWalletCreateWithMnemonic(mnemonic: *const i8, passphrase: *const i8) -> *const i8;
    fn TWHDWalletGetAddressForCoin(wallet: *const i8, coin: i32) -> *const i8;

    fn TWMnemonicIsValid(mnemonic: *const i8) -> bool;
}

// type utilities
struct TWString {
    wrapped: *const i8
    // TODO delete when destructing with TWStringDelete
}

trait FromString {
    fn from_str(s: &str) -> Self;
}

impl FromString for TWString {
    //fn from_str(s: &str) -> Result<Self, Self::Err> {
    fn from_str(s: &str) -> Self {
        let cstring = CString::new(s).unwrap();
        let ptr = unsafe { TWStringCreateWithUTF8Bytes(cstring.as_ptr()) };
        return TWString { wrapped: ptr };
    }
}

impl ToString for TWString {
    fn to_string(&self) -> String {
        let s1 = unsafe { TWStringUTF8Bytes(self.wrapped) };
        let c_str: &CStr = unsafe { CStr::from_ptr(s1) };
        let str_slice: &str = c_str.to_str().unwrap();
        let str_buf: String = str_slice.to_owned();
        return str_buf;
    }
}

struct HDWallet {
    wrapped: *const i8
}

// wrappers
fn hd_wallet_create_with_mnemonic(mnemonic: &TWString, passphrase: &TWString) -> HDWallet {
    let ptr = unsafe {
        TWHDWalletCreateWithMnemonic(mnemonic.wrapped, passphrase.wrapped)
    };
    return HDWallet {wrapped: ptr};
}

fn hd_wallet_get_address_for_coin(wallet: &HDWallet, coin: i32) -> TWString {
    let s: *const c_char = unsafe {
        TWHDWalletGetAddressForCoin(wallet.wrapped, coin)
    };
    return TWString { wrapped: s };
}

fn mnemonic_is_valid(mnemonic: &TWString) -> bool {
    unsafe {
        TWMnemonicIsValid(mnemonic.wrapped)
    }
}

fn test_address(coin: i32, coin_name: &str, wallet: &HDWallet) {
    let address = hd_wallet_get_address_for_coin(wallet, coin);
    println!("==> addr for {}: {}", coin_name, address.to_string());
}

fn main() {
	println!("==> Calling wallet-core from Rust");

    let mnemonic = "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal";
    println!("==> mnemonic is valid: {}", mnemonic_is_valid(&TWString::from_str(mnemonic)));

    let mnemonic_tw = TWString::from_str(mnemonic);
    if !mnemonic_is_valid(&mnemonic_tw) {
        println!("Mnemonic is invalid! {}", mnemonic);
        return;
    }
    println!("==> wallet created, mnemonic {}", mnemonic);
    let wallet = hd_wallet_create_with_mnemonic(&mnemonic_tw, &TWString::from_str(""));

    test_address(0, "bitcoin", &wallet);

    test_address(60, "ethereum", &wallet);
}
