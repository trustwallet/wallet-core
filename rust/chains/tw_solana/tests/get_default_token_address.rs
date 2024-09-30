// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;
use tw_solana::address::SolanaAddress;
use tw_solana::blockhash::Blockhash;
use tw_solana::defined_addresses::TOKEN_PROGRAM_ID_ADDRESS;
use tw_solana::program::stake_program::StakeProgram;

fn test_get_default_token_address_impl(
    main_address: &str,
    token_mint_address: &str,
    expected: &str,
) {
    let main_address = SolanaAddress::from_str(main_address).unwrap();
    let token_mint_address = SolanaAddress::from_str(token_mint_address).unwrap();
    let expected = SolanaAddress::from_str(expected).unwrap();

    let actual = StakeProgram::get_associated_token_address(
        main_address,
        *TOKEN_PROGRAM_ID_ADDRESS,
        token_mint_address,
    )
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

fn test_address_from_recent_blockhash_impl(main_address: &str, blockhash: &str, expected: &str) {
    let main_address = SolanaAddress::from_str(main_address).unwrap();
    let expected = SolanaAddress::from_str(expected).unwrap();
    let blockhash = Blockhash::from_str(blockhash).unwrap();

    let actual = StakeProgram::address_from_recent_blockhash(&main_address, &blockhash);
    assert_eq!(actual, expected);
}

#[test]
fn test_address_from_recent_blockhash() {
    test_address_from_recent_blockhash_impl(
        "zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu",
        "11111111111111111111111111111111",
        "GQDDc5EVGJZFC7AvpEJ8eoCQ75Yy4gr7eu17frCjvQRQ",
    );
    test_address_from_recent_blockhash_impl(
        "zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu",
        "9ipJh5xfyoyDaiq8trtrdqQeAhQbQkWy2eANizKvx75K",
        "2Kos1xJRBq3Ae1GnVNBx7HgJhq8KvdUe2bXE4QGdNaXb",
    );
    test_address_from_recent_blockhash_impl(
        "B1iGmDJdvmxyUiYM8UEo2Uw2D58EmUrw4KyLYMmrhf8V",
        "AfzzEC8NVXoxKoHdjXLDVzqwqvvZmgPuqyJqjuHiPY1D",
        "Fxhhm82PZVuXEwycT28vGqknUEnVeoHh4UWEnJNQUDbv",
    );
}
