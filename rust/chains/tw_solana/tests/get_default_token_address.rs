// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_solana::address::SolanaAddress;
use tw_solana::program::stake_program::StakeProgram;

fn test_get_default_token_address_impl(
    main_address: &str,
    token_mint_address: &str,
    expected: &str,
) {
    let main_address = SolanaAddress::from_str(main_address).unwrap();
    let token_mint_address = SolanaAddress::from_str(token_mint_address).unwrap();
    let expected = SolanaAddress::from_str(expected).unwrap();

    let actual = StakeProgram::get_associated_token_address(main_address, token_mint_address)
        .expect("!get_associated_token_address");
    assert_eq!(actual, expected);
}

#[test]
fn test_get_default_token_address() {
    test_get_default_token_address_impl(
        "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V",
        "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt",
        "EDNd1ycsydWYwVmrYZvqYazFqwk1QjBgAUKFjBoz1jKP",
    );
    test_get_default_token_address_impl(
        "FtsZbVDYGBUw4R5Rcy8p58RAdYRFUJcAiBRdQAona7t1",
        "FQYWEccPpAVR5Q16FKoTFUa6z8BWzdK5eh3D586fdQbM",
        "2XGUJePx3CJSYQvAQcSgLf7xpjCdMUgx8LNaSCUhE2LS",
    );
    test_get_default_token_address_impl(
        "74nsHsFivzUPLEygULuZLs193MnDNZfnKrSEkgA4qkY7",
        "FQYWEccPpAVR5Q16FKoTFUa6z8BWzdK5eh3D586fdQbM",
        "823YRj6GozU23rwSyd1f78vPopVoQPCeK79ppWCu5SYQ",
    );
    test_get_default_token_address_impl(
        "HBYC51YrGFAZ8rM7Sj8e9uqKggpSrDYrinQDZzvMtqQp",
        "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt",
        "6X4X1Ae24mkoWeCEpktevySVG9jzeCufut5vtUW3wFrD",
    );
    test_get_default_token_address_impl(
        "Eg5jqooyG6ySaXKbQUu4Lpvu2SqUPZrNkM4zXs9iUDLJ",
        "SRMuApVNdxXokk5GT7XD5cUUgXMBCoAz2LHeuAoKWRt",
        "ANVCrmRw7Ww7rTFfMbrjApSPXEEcZpBa6YEiBdf98pAf",
    );
}
