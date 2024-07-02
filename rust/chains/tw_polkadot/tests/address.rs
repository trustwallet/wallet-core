use std::str::FromStr;

use tw_polkadot::address::PolkadotAddress;

#[test]
fn test_polkadot_address_valid() {
    // Polkadot ed25519
    PolkadotAddress::from_str("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu")
        .expect("error parsing address");

    // Polkadot sr25519
    PolkadotAddress::from_str("15AeCjMpcSt3Fwa47jJBd7JzQ395Kr2cuyF5Zp4UBf1g9ony")
        .expect("error parsing address");
}

#[test]
fn test_polkadot_address_invalid() {
    // Empty address
    PolkadotAddress::from_str("").expect_err("no error parsing invalid address");

    // Invalid address
    PolkadotAddress::from_str("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsT^^^")
        .expect_err("no error parsing invalid address");
}
