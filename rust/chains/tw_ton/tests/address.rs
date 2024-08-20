use tw_keypair::ed25519::sha512::KeyPair;
use tw_keypair::traits::KeyPairTrait;
use tw_ton::wallet::wallet_v5;

/// Tests for TON V5R1 address.
///
/// Note: These tests should move to rust/tw_any_coin/tests/chains/ton/ton_address.rs after we define
/// a new Derivation for the TON V5R1 address.
#[test]
fn test_ton_v5r1_address_derive() {
    let test_cases = [(
        "3570e35f54cfb843f2cfaf2b8cae7ceeb7b32225d7dbbd86f611056d74d9073e", // private key
        "UQAU3o5-Sp1MYRpw3U7b_wmARxqI49LxiFhEoVCxpUKjTYXk", // V5R1 address on the mainnet
    )];

    for test_case in test_cases.iter() {
        let private_key_hex = test_case.0;
        let expected_address = test_case.1;

        // Decode the private key from hex
        let private_key_bytes =
            tw_encoding::hex::decode(private_key_hex).expect("Invalid hex string");

        // Create a KeyPair from the private key bytes
        let key_pair =
            KeyPair::try_from(private_key_bytes.as_slice()).expect("Failed to create key pair");

        // Extract the public key from the KeyPair
        let public_key = key_pair.public().clone();

        // Create the VersionedTonWallet using the public key
        let wallet = wallet_v5::WalletV5R1::std_with_public_key(public_key)
            .expect("Failed to create wallet");

        let actual_address = wallet.address.to_string();

        assert_eq!(actual_address, expected_address);
    }
}
