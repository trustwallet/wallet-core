// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::chains::ton::cell_example::{
    comment_cell, contract_address_from_state_init, doge_chatbot_state_init,
};
use crate::chains::ton::ton_sign::assert_eq_boc;
use tw_any_coin::test_utils::sign_utils::AnySignerHelper;
use tw_coin_registry::coin_type::CoinType;
use tw_encoding::hex::{DecodeHex, ToHex};
use tw_number::U256;
use tw_proto::Common::Proto::SigningError;
use tw_proto::TheOpenNetwork::Proto;
use tw_proto::TheOpenNetwork::Proto::mod_Transfer::OneOfpayload as PayloadType;

#[test]
fn test_ton_sign_wallet_v5r1_transfer_and_deploy() {
    let private_key = "3570e35f54cfb843f2cfaf2b8cae7ceeb7b32225d7dbbd86f611056d74d9073e";

    let transfer = Proto::Transfer {
        dest: "EQBe6DtCpJZe8M4t-crMXe93JlEYgSl30S5OUuMSLOfeQfBu".into(),
        amount: U256::encode_be_compact(10),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        expire_at: 0xffffffff,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/Q32uRBqVprzNzc6iVgwxPeJPE92Fx21dfsqrHnCh5Ss=
    assert_eq_boc(&output.encoded, "te6cckECGwEAA2sAAkWIACm9HPyVOpjCNOG6nbf+EwCONRHHpeMQsIlCoWNKhUaaHgECAgE0AwQBoXNpZ25///8R/////wAAAACACOfqY7L3l3aKc58eNxuJTaeH/fgBw2aG0coM+hjDpjWhJKbYKmsAD8v054HYSuO6vN3bQnV5U19BhsGfe1MDoAUBFP8A9KQT9LzyyAsGAFGAAAAAP///iKrcG+d35KaMMtuxik4jqNofFL51Mu1f8Qf19onqsDlyIAIKDsPIbQMWBwIBIAgJAWJiAC90HaFSSy94Zxb85WYu97uTKIxAlLvolycpcYkWc+8giFAAAAAAAAAAAAAAAAABFgIBSAoLAQLyDALc0CDXScEgkVuPYyDXCx8gghBleHRuvSGCEHNpbnS9sJJfA+CCEGV4dG66jrSAINchAdB01yH6QDD6RPgo+kQwWL2RW+DtRNCBAUHXIfQFgwf0Dm+hMZEw4YBA1yFwf9s84DEg10mBAoC5kTDgcOIXDQIBIA4PAR4g1wsfghBzaWduuvLgin8NAeaO8O2i7fshgwjXIgKDCNcjIIAg1yHTH9Mf0x/tRNDSANMfINMf0//XCgAK+QFAzPkQmiiUXwrbMeHywIffArNQB7Dy0IRRJbry4IVQNrry4Ib4I7vy0IgikvgA3gGkf8jKAMsfAc8Wye1UIJL4D95w2zzYFwIBIBARABm+Xw9qJoQICg65D6AsAgFuEhMCAUgUFQAZrc52omhAIOuQ64X/wAAZrx32omhAEOuQ64WPwAAXsyX7UTQcdch1wsfgABGyYvtRNDXCgCAAAAP27aLt+wL0BCFukmwhjkwCIdc5MHCUIccAs44tAdcoIHYeQ2wg10nACPLgkyDXSsAC8uCTINcdBscSwgBSMLDy0InXTNc5MAGk6GwShAe78uCT10rAAPLgk+1V4tIAAcAAkVvg69csCBQgkXCWAdcsCBwS4lIQseMPINdKGBkaAJYB+kAB+kT4KPpEMFi68uCR7UTQgQFB1xj0BQSdf8jKAEAEgwf0U/Lgi44UA4MH9Fvy4Iwi1woAIW4Bs7Dy0JDiyFADzxYS9ADJ7VQAcjDXLAgkji0h8uCS0gDtRNDSAFETuvLQj1RQMJExnAGBAUDXIdcKAPLgjuLIygBYzxbJ7VST8sCN4gAQk1vbMeHXTNB8Ui06");
    assert_eq!(
        output.hash.to_hex(),
        "437dae441a95a6bccdcdcea2560c313de24f13dd85c76d5d7ecaab1e70a1e52b"
    );
}

#[test]
fn test_ton_sign_wallet_v5r1_transfer_ordinary() {
    let private_key = "3570e35f54cfb843f2cfaf2b8cae7ceeb7b32225d7dbbd86f611056d74d9073e";

    let transfer = Proto::Transfer {
        dest: "EQBe6DtCpJZe8M4t-crMXe93JlEYgSl30S5OUuMSLOfeQfBu".into(),
        amount: U256::encode_be_compact(10),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 1,
        expire_at: 1723469663,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/UhZFShj8QG6M4nUPELVROdY_IAxSSkeZojh-t2XC2IU=
    assert_eq_boc(&output.encoded, "te6cckEBBQEAuQABRYgAKb0c/JU6mMI04bqdt/4TAI41Ecel4xCwiUKhY0qFRpoMAQGhc2lnbn///xFmug9fAAAAAbsTmIIbTn/5n8piniYykIpNGhw0mstvB4PfGuTjdh7eIY3HAokbPMSdaAoOXq8mTWzT2p3v6XSoqTh7XKacOIDgAgIKDsPIbQMEAwFiYgAvdB2hUksveGcW/OVmLve7kyiMQJS76JcnKXGJFnPvIIhQAAAAAAAAAAAAAAAAAQQAAN6nHJg=");

    assert_eq!(
        output.hash.to_hex(),
        "5216454a18fc406e8ce2750f10b55139d63f200c524a4799a2387eb765c2d885"
    );
}

#[test]
fn test_ton_sign_wallet_v5r1_transfer_all_balance() {
    let private_key = "502d60b0f3327382e7d0585b789f1db9aa04907fe5cddc5c28818ec163ebf4ba";

    let transfer = Proto::Transfer {
        dest: "UQAU3o5-Sp1MYRpw3U7b_wmARxqI49LxiFhEoVCxpUKjTYXk".into(),
        amount: U256::encode_be_compact(0),
        mode: Proto::SendMode::ATTACH_ALL_CONTRACT_BALANCE as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 4,
        expire_at: 1723482783,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/q7MJULnzKZ1DY4itqKfHg_cj48ueFRIlT6I2O8rSLt0=
    assert_eq_boc(&output.encoded, "te6cckEBBQEAuAABRYgBQ8awIH6gcEaSzgT9ZAPxWhgEMUrjdKCkL1PJZ6pq4mgMAQGhc2lnbn///xFmukKfAAAABKg9bRJRcUN/+LR7Bny8XjKzf3+GzopWEf+n7ibzPpr9ZOZs5CRon4i0XFBiFMVVwfcnDI4RVWht6Jf5eSzOjwEgAgIKDsPIbYIEAwFgYgAKb0c/JU6mMI04bqdt/4TAI41Ecel4xCwiUKhY0qFRpoAAAAAAAAAAAAAAAAABBAAANXkwpA==");
    assert_eq!(
        output.hash.to_hex(),
        "abb30950b9f3299d436388ada8a7c783f723e3cb9e1512254fa2363bcad22edd"
    );
}

#[test]
fn test_ton_sign_wallet_v5r1_transfer_all_balance_non_bounceable() {
    let private_key = "3570e35f54cfb843f2cfaf2b8cae7ceeb7b32225d7dbbd86f611056d74d9073e";

    let transfer = Proto::Transfer {
        dest: "UQBe6DtCpJZe8M4t-crMXe93JlEYgSl30S5OUuMSLOfeQa2r".into(),
        amount: U256::encode_be_compact(0),
        mode: Proto::SendMode::ATTACH_ALL_CONTRACT_BALANCE as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: false,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 2,
        expire_at: 1723470825,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/f8meQabVMmfvUOJ0D_NTyji1qr4LJ58tTkPI6dlYSPE=
    assert_eq_boc(&output.encoded, "te6cckEBBQEAuAABRYgAKb0c/JU6mMI04bqdt/4TAI41Ecel4xCwiUKhY0qFRpoMAQGhc2lnbn///xFmuhPpAAAAAq9jgguGOlFmpN566XBRCth1GfF8kVEc0DJkvLPKOa7MQMGzr0L1xpLt5OLovDpy4KwPqNxALo/b9NluZDLcukPgAgIKDsPIbYIEAwFgQgAvdB2hUksveGcW/OVmLve7kyiMQJS76JcnKXGJFnPvIIAAAAAAAAAAAAAAAAABBAAAI3vzjA==");
    assert_eq!(
        output.hash.to_hex(),
        "7fc99e41a6d53267ef50e2740ff353ca38b5aabe0b279f2d4e43c8e9d95848f1"
    );
}

#[test]
fn test_ton_sign_wallet_v5r1_transfer_with_ascii_comment() {
    let private_key = "3570e35f54cfb843f2cfaf2b8cae7ceeb7b32225d7dbbd86f611056d74d9073e";

    let transfer = Proto::Transfer {
        dest: "EQBe6DtCpJZe8M4t-crMXe93JlEYgSl30S5OUuMSLOfeQfBu".into(),
        amount: U256::encode_be_compact(10),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        comment: "test comment".into(),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 3,
        expire_at: 1723472681,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/uc9sMl9Ey3yQ6sX48EKM0nFSeJoq2JcEi0xMzTLYV6M=
    assert_eq_boc(&output.encoded, "te6cckEBBgEAywABRYgAKb0c/JU6mMI04bqdt/4TAI41Ecel4xCwiUKhY0qFRpoMAQGhc2lnbn///xFmuhspAAAAA6oLkiOh1mWEhej/y/dCZAZZT1hFo6aX9ROjIOX5HvwBz99SLLlCmTJd4QV1Vp9D9eaTXL7aT28rswEjyBSYpwPgAgIKDsPIbQMDBAAAAWJiAC90HaFSSy94Zxb85WYu97uTKIxAlLvolycpcYkWc+8giFAAAAAAAAAAAAAAAAABBQAgAAAAAHRlc3QgY29tbWVudDHkkjo=");
    assert_eq!(
        output.hash.to_hex(),
        "b9cf6c325f44cb7c90eac5f8f0428cd27152789a2ad897048b4c4ccd32d857a3"
    );
}

#[test]
fn test_ton_sign_wallet_v5r1_transfer_with_utf8_comment() {
    let private_key = "3570e35f54cfb843f2cfaf2b8cae7ceeb7b32225d7dbbd86f611056d74d9073e";

    let transfer = Proto::Transfer {
        dest: "EQBe6DtCpJZe8M4t-crMXe93JlEYgSl30S5OUuMSLOfeQfBu".into(),
        amount: U256::encode_be_compact(10),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        comment: "тестовый комментарий".into(),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 4,
        expire_at: 1723472948,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/tijrXAMehLvfESLWWgGf1R5jhW4CMoio11ZFD_musFg=
    assert_eq_boc(&output.encoded, "te6cckEBBgEA5gABRYgAKb0c/JU6mMI04bqdt/4TAI41Ecel4xCwiUKhY0qFRpoMAQGhc2lnbn///xFmuhw0AAAABIDSMWXCM7WX+aQyurxO4nsYaqKhHm4ndzkjAS00AhtXe5fpKHUvVAFDvyqCi5T/PI/fwyZdralzzpXTrQaRP0CgAgIKDsPIbQMDBAAAAWJiAC90HaFSSy94Zxb85WYu97uTKIxAlLvolycpcYkWc+8giFAAAAAAAAAAAAAAAAABBQBWAAAAANGC0LXRgdGC0L7QstGL0Lkg0LrQvtC80LzQtdC90YLQsNGA0LjQuV6Inwc=");
    assert_eq!(
        output.hash.to_hex(),
        "b628eb5c031e84bbdf1122d65a019fd51e63856e023288a8d756450ff9aeb058"
    );
}

#[test]
fn test_ton_sign_wallet_v5r1_transfer_jettons() {
    let private_key = "502d60b0f3327382e7d0585b789f1db9aa04907fe5cddc5c28818ec163ebf4ba";

    let jetton_transfer = Proto::JettonTransfer {
        query_id: 69,
        // Transfer 0.12 USDT (decimal precision is 6).
        jetton_amount: U256::encode_be_compact(120000),
        to_owner: "UQAU3o5-Sp1MYRpw3U7b_wmARxqI49LxiFhEoVCxpUKjTYXk".into(),
        // Send unused toncoins back to sender.
        response_address: "UQCh41gQP1A4I0lnAn6yAfitDAIYpXG6UFIXqeSz1TVxNOJ_".into(),
        forward_amount: U256::encode_be_compact(1),
        ..Default::default()
    };

    let transfer = Proto::Transfer {
        dest: "EQDg4AjfaxQBVsUFueenkKlHLhhYWrcBvCEzbEgfrT0nxuGC".into(),
        amount: U256::encode_be_compact(100 * 1000 * 1000),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        payload: PayloadType::jetton_transfer(jetton_transfer),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 0,
        expire_at: 1723479990,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/CapFFJlwYNa_p8-fImgUPyKLxNuq4mR5K7U-G3zbFNg=
    assert_eq_boc(&output.encoded, "te6cckECHAEAA8QAAkWIAUPGsCB+oHBGks4E/WQD8VoYBDFK43SgpC9TyWeqauJoHgECAgE0AwQBoXNpZ25///8R/////wAAAACK9TWZkcw/aJwl28F2cStjO8o6owv7/VKsx3RW0XtPaiwwFgi+CkmtzzJLGGp24JEwbwslQ1jNHyRqqUEM41AD4AUBFP8A9KQT9LzyyAsGAFGAAAAAP///iOEJZED3G4tDJvijkvHmVvq03ZLJ8Db8Bmagv84HH0SSIAIKDsPIbQMHCAIBIAkKAAABaGIAcHAEb7WKAKtigtzz08hUo5cMLC1bgN4QmbYkD9aek+MgL68IAAAAAAAAAAAAAAAAAAELAgFIDA0BAvIOAKgPin6lAAAAAAAAAEUwHUwIACm9HPyVOpjCNOG6nbf+EwCONRHHpeMQsIlCoWNKhUabACh41gQP1A4I0lnAn6yAfitDAIYpXG6UFIXqeSz1TVxNAgIC3NAg10nBIJFbj2Mg1wsfIIIQZXh0br0hghBzaW50vbCSXwPgghBleHRuuo60gCDXIQHQdNch+kAw+kT4KPpEMFi9kVvg7UTQgQFB1yH0BYMH9A5voTGRMOGAQNchcH/bPOAxINdJgQKAuZEw4HDiGA8CASAQEQEeINcLH4IQc2lnbrry4Ip/DwHmjvDtou37IYMI1yICgwjXIyCAINch0x/TH9Mf7UTQ0gDTHyDTH9P/1woACvkBQMz5EJoolF8K2zHh8sCH3wKzUAew8tCEUSW68uCFUDa68uCG+CO78tCIIpL4AN4BpH/IygDLHwHPFsntVCCS+A/ecNs82BgCASASEwAZvl8PaiaECAoOuQ+gLAIBbhQVAgFIFhcAGa3OdqJoQCDrkOuF/8AAGa8d9qJoQBDrkOuFj8AAF7Ml+1E0HHXIdcLH4AARsmL7UTQ1woAgA/btou37AvQEIW6SbCGOTAIh1zkwcJQhxwCzji0B1yggdh5DbCDXScAI8uCTINdKwALy4JMg1x0GxxLCAFIwsPLQiddM1zkwAaTobBKEB7vy4JPXSsAA8uCT7VXi0gABwACRW+Dr1ywIFCCRcJYB1ywIHBLiUhCx4w8g10oZGhsAlgH6QAH6RPgo+kQwWLry4JHtRNCBAUHXGPQFBJ1/yMoAQASDB/RT8uCLjhQDgwf0W/LgjCLXCgAhbgGzsPLQkOLIUAPPFhL0AMntVAByMNcsCCSOLSHy4JLSAO1E0NIAURO68tCPVFAwkTGcAYEBQNch1woA8uCO4sjKAFjPFsntVJPywI3iABCTW9sx4ddM0JlgUSM=");
    assert_eq!(
        output.hash.to_hex(),
        "09aa4514997060d6bfa7cf9f2268143f228bc4dbaae264792bb53e1b7cdb14d8"
    );
}

#[test]
fn test_ton_sign_wallet_v5r1_transfer_jettons_with_comment() {
    let private_key = "502d60b0f3327382e7d0585b789f1db9aa04907fe5cddc5c28818ec163ebf4ba";

    let jetton_transfer = Proto::JettonTransfer {
        query_id: 0,
        // Transfer 0.11 USDT (decimal precision is 6).
        jetton_amount: U256::encode_be_compact(110000),
        to_owner: "UQAU3o5-Sp1MYRpw3U7b_wmARxqI49LxiFhEoVCxpUKjTYXk".into(),
        // Send unused toncoins back to sender.
        response_address: "UQCh41gQP1A4I0lnAn6yAfitDAIYpXG6UFIXqeSz1TVxNOJ_".into(),
        forward_amount: U256::encode_be_compact(1),
        ..Default::default()
    };

    let transfer = Proto::Transfer {
        dest: "EQDg4AjfaxQBVsUFueenkKlHLhhYWrcBvCEzbEgfrT0nxuGC".into(),
        amount: U256::encode_be_compact(100 * 1000 * 1000),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        comment: "test comment".into(),
        payload: PayloadType::jetton_transfer(jetton_transfer),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 1,
        expire_at: 1723480954,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/gkyDtRqJa_5-GGut6BZcVS5JmRLbObGiKrpmaIBPoxc=
    assert_eq_boc(&output.encoded, "te6cckECBgEAASIAAUWIAUPGsCB+oHBGks4E/WQD8VoYBDFK43SgpC9TyWeqauJoDAEBoXNpZ25///8RZro7egAAAAGYUu8+ZHZK/wWb9ojB++h5tz2ie7e4GktWZfr475zJcnpRLZADd9wbyuuV7GXr8QzKeqvQbSDVJlgv4bcdnjwDYAICCg7DyG0DAwQAAAFoYgBwcARvtYoAq2KC3PPTyFSjlwwsLVuA3hCZtiQP1p6T4yAvrwgAAAAAAAAAAAAAAAAAAQUAyA+KfqUAAAAAAAAAADAa2wgAKb0c/JU6mMI04bqdt/4TAI41Ecel4xCwiUKhY0qFRpsAKHjWBA/UDgjSWcCfrIB+K0MAhilcbpQUhep5LPVNXE0CAgAAAAB0ZXN0IGNvbW1lbnQIl4du");
    assert_eq!(
        output.hash.to_hex(),
        "824c83b51a896bfe7e186bade8165c552e499912db39b1a22aba6668804fa317"
    );
}

#[test]
fn test_ton_sign_wallet_v5r1_transfer_custom_payload() {
    // UQCh41gQP1A4I0lnAn6yAfitDAIYpXG6UFIXqeSz1TVxNOJ_
    let private_key = "502d60b0f3327382e7d0585b789f1db9aa04907fe5cddc5c28818ec163ebf4ba";

    let transfer = Proto::Transfer {
        dest: "UQAU3o5-Sp1MYRpw3U7b_wmARxqI49LxiFhEoVCxpUKjTYXk".into(),
        // 0.00025 TON
        amount: U256::encode_be_compact(250_000),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        payload: PayloadType::custom_payload(comment_cell("Hi there sir").into()),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        expire_at: 1723481664,
        messages: vec![transfer],
        sequence_number: 2,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/_Pr9kkthRh5qQtALT4HPpWJKRF3qK7L7DUa_hmu3VAI=
    assert_eq_boc(&output.encoded, "te6cckEBBgEAzQABRYgBQ8awIH6gcEaSzgT9ZAPxWhgEMUrjdKCkL1PJZ6pq4mgMAQGhc2lnbn///xFmuj5AAAAAAqWnjYMI5ukKZ/38yUBJq3dj7fHGvxzmY4WgFTqo6utiQ1rAW1gZTm8tErHXXXEZykBfG9qDSKaTB/XEvbvTK4CgAgIKDsPIbQMDBAAAAWZiAApvRz8lTqYwjThup23/hMAjjURx6XjELCJQqFjSoVGmmB6EgAAAAAAAAAAAAAAAAAEFACAAAAAASGkgdGhlcmUgc2lyZT4q+g==");
    assert_eq!(
        output.hash.to_hex(),
        "fcfafd924b61461e6a42d00b4f81cfa5624a445dea2bb2fb0d46bf866bb75402"
    );
}

#[test]
fn test_ton_sign_wallet_v5r1_transfer_custom_payload_with_state_init() {
    // UQCh41gQP1A4I0lnAn6yAfitDAIYpXG6UFIXqeSz1TVxNOJ_
    let private_key = "502d60b0f3327382e7d0585b789f1db9aa04907fe5cddc5c28818ec163ebf4ba";

    let current_timestamp = 1723481400;
    let expire_at = current_timestamp + 60 * 10;

    let doge_state_init = doge_chatbot_state_init(current_timestamp);
    let doge_contract_address = contract_address_from_state_init(&doge_state_init);
    assert_eq!(
        doge_state_init,
        "te6cckEBBAEAUwACATQBAgEU/wD0pBP0vPLICwMAEAAAAZFHfyLAAGrTMAGCCGlJILmRMODQ0wMx+kAwi0ZG9nZYcCCAGMjLBVAEzxaARfoCE8tqEssfAc8WyXP7AOj1qKs="
    );
    assert_eq!(
        doge_contract_address,
        "0:6af6c37f5e1704fc46174f0cbf5b558cbbf1dbc750c59c3c851bb98c7a94a145",
    );

    let transfer = Proto::Transfer {
        dest: doge_contract_address.into(),
        // 0.0069 TON
        amount: U256::encode_be_compact(6_900_000),
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: false,
        state_init: doge_state_init.into(),
        payload: PayloadType::custom_payload(
            comment_cell("This transaction deploys Doge Chatbot contract").into(),
        ),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        expire_at,
        messages: vec![transfer],
        sequence_number: 3,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/Y35nc-J86gbrMC5CljEMqzCUG5vFLilLjNdHgb193eA=
    assert_eq_boc(&output.encoded, "te6cckECCgEAAUQAAUWIAUPGsCB+oHBGks4E/WQD8VoYBDFK43SgpC9TyWeqauJoDAEBoXNpZ25///8RZro/kAAAAAOAA5mkphyRfc8/OtuqrYWeR9u1Eib135FXAntzMzjdjj+1xH96YruRHinS9gB3bfHxtK3QUvy+zrcqKliVvbtA4AICCg7DyG0DAwQAAAJnQgA1e2G/rwuCfiMLp4ZfrarGXfjt46hizh5CjdzGPUpQoptKSQAAAAAAAAAAAAAAAAADwAUGAgE0BwgAZAAAAABUaGlzIHRyYW5zYWN0aW9uIGRlcGxveXMgRG9nZSBDaGF0Ym90IGNvbnRyYWN0ART/APSkE/S88sgLCQAQAAABkUd/IsAAatMwAYIIaUkguZEw4NDTAzH6QDCLRkb2dlhwIIAYyMsFUATPFoBF+gITy2oSyx8BzxbJc/sArbH+dw==");
    assert_eq!(
        output.hash.to_hex(),
        "637e6773e27cea06eb302e4296310cab30941b9bc52e294b8cd74781bd7ddde0"
    );
}

#[test]
fn test_ton_sign_wallet_v5r1_missing_required_send_mode() {
    let private_key = "3570e35f54cfb843f2cfaf2b8cae7ceeb7b32225d7dbbd86f611056d74d9073e";

    let transfer = Proto::Transfer {
        dest: "EQBe6DtCpJZe8M4t-crMXe93JlEYgSl30S5OUuMSLOfeQfBu".into(),
        amount: U256::encode_be_compact(10),
        // Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS is required for wallet v5r1 external messages.
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32,
        bounceable: true,
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 1,
        expire_at: 1723469663,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::Error_internal);
}

#[test]
fn test_ton_sign_wallet_v5r1_mintless_jetton() {
    let private_key = "502d60b0f3327382e7d0585b789f1db9aa04907fe5cddc5c28818ec163ebf4ba";

    let jetton_transfer = Proto::JettonTransfer {
        query_id: 1,
        // Transfer 0 mintless jetton to self.
        jetton_amount: U256::encode_be_compact(0),
        to_owner: "UQCh41gQP1A4I0lnAn6yAfitDAIYpXG6UFIXqeSz1TVxNOJ_".into(),
        // Send unused toncoins back to sender.
        response_address: "UQCh41gQP1A4I0lnAn6yAfitDAIYpXG6UFIXqeSz1TVxNOJ_".into(),
        forward_amount: U256::encode_be_compact(1),
        custom_payload: "te6ccgECNQEABJMAAQgN9gLWAQlGA6+1FWXC4ss/wvDOFwMk2bVM97AUEWqaUhh63uWfQ26nAB4CIgWBcAIDBChIAQEZG2ZqtEYGAq27TvzHdGuGrhhKoICBU+Zg9Xq/qRMHGAAdIgEgBQYiASAHCChIAQEV0tdPcZG01smq0thhsmqf9ZzE0QqpP3c+ERvuHF1JDgAbKEgBAf3dO8qdKoPys7AWvavs1wMNWCOq5XashXaRopmksx/LABsiASAJCiIBIAsMKEgBAWP0xUs9JBrfQRl1FkF2tIfIDYpwLdf3fXqMi6BqxNtmABoiASANDihIAQFOErI5E7ld/nTAgHXdGI74UH8kxIaFyAkH42P54tEC9QAYIgEgDxAoSAEBrF16Czdlg18FB467CrR6Ucwxb8H+Z1e4qDeFWbkz1WEAFyIBIBESKEgBAXeWzg9xTFO6z0FP+axi8Njuxxp0zPrAUs4vnmt/dE3xABYoSAEBEZ7KazNpaWJoInmqO4II/AfncyhMNWxh6BE2qFU7/9wAFCIBIBMUKEgBAZleZTNXbgCF+8G08kiQeDPanQtNCVakzEU3g9GKB+K2ABQiASAVFihIAQFeCM83J7sm36g24qFeEDvStahHWn6SsEk+Wii49rzBiAASIgEgFxgoSAEBfV9jrgSeiAKVqeeLliXdoLrxFWe2HK0f4SG5h4kfb8YAESIBIBkaIgEgGxwoSAEBImHhXIbOHuOnOgE5f0KLqoXDB7/ZLQQGiHysuulUq2IAECIBIB0eKEgBAXT+qb5w1+qtvbJ1Fbn8y6IhO85YfxKIgKBga6ROO/yQAA8iASAfIChIAQGoJHXWXWRQGZdP9xIUrMowhvgnf+CwKTIIOBxlDiKgcAANKEgBAZ6tCuDr89HFRz3WwwK+wW4XmkE+O7Hf+NgUDI+uqnAJAAwiASAhIihIAQHtasTLBAw7MZHpRTsKyC47E1PZ/LAtF3n2Y2b5ThX0VgALIgEgIyQiASAlJihIAQGumGRf7UXrpK12Cuvj06565IC0Kbd4i2XoG6dnqC+uQAAJKEgBAXM19HUUkz6ns7o/2x45kQ2iLj8gl3zYhrAhISEUg0O1AAgiASAnKCIBICkqKEgBAa7kNA+lev+Z5T/xqKBbO648BvnLL6/hAp1auOiZTWRhAAcoSAEBxn19AKZGAUPYWs8pTpNQrCB4Ap0KfzyjOgB1Mc9PbIUABSIBICssKEgBAWarrCPqSS6+lq6NRcrWZ2/v6bN4b6Zd3GWAtN6j8a6BAAQiASAtLiIBIC8wKEgBAXYYqhLZ1tHg+HdKd8vLmTBsojkj61ZiafXB7pOt+hEFAAMiASAxMihIAQHt8p6qBiXtz+kKcgo13Udyh7Uo8irrdKlSSY2dOdALogAAIgFIMzQoSAEByacrlqsAKiFOlv4Rp4V1gNg2i4aVPkcHJq8Vug/89k4AAABduZA/UDgjSWcCfrIB+K0MAhilcbpQUhep5LPVNXE0Q7msoAAABm4N2AAABm9VrQgoSAEByIAktH0CNxT//QZ8Vgj68CApZON9XBKDfE0D2rY8Fx4AAA==".into()
    };

    let transfer = Proto::Transfer {
        dest: "UQCn2lssMz09Gn60mBnv544DgiqIX3mK5cqGEPEPKxCNbE0E".into(), // jetton wallet address
        amount: U256::encode_be_compact(90 * 1000 * 1000), // 0.09 TON as fee
        mode: Proto::SendMode::PAY_FEES_SEPARATELY as u32
            | Proto::SendMode::IGNORE_ACTION_PHASE_ERRORS as u32,
        bounceable: true,
        state_init: "te6ccgEBAwEAjwACATQBAghCAg7xnhv0Dyukkvyqw4buylm/aCejhQcI2fzZrbaDq8M2AMoAgBQ8awIH6gcEaSzgT9ZAPxWhgEMUrjdKCkL1PJZ6pq4mkAPpn0MdzkzH7w8jwjgGMZfR3Y2FqlpEArXYKCy3B42gyr7UVZcLiyz/C8M4XAyTZtUz3sBQRappSGHre5Z9DbqcAg==".into(),
        payload: PayloadType::jetton_transfer(jetton_transfer),
        ..Proto::Transfer::default()
    };

    let input = Proto::SigningInput {
        private_key: private_key.decode_hex().unwrap().into(),
        messages: vec![transfer],
        sequence_number: 7,
        expire_at: 1727346541,
        wallet_version: Proto::WalletVersion::WALLET_V5_R1,
        ..Proto::SigningInput::default()
    };

    let mut signer = AnySignerHelper::<Proto::SigningOutput>::default();
    let output = signer.sign(CoinType::TON, input);

    assert_eq!(output.error, SigningError::OK, "{}", output.error_message);
    // Successfully broadcasted: https://tonviewer.com/transaction/70f347338b3e0d33feb285a0cc7300d216b6011462ed7e76c6395bcca7abc649
    assert_eq_boc(&output.encoded, "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");
    assert_eq!(
        output.hash.to_hex(),
        "70f347338b3e0d33feb285a0cc7300d216b6011462ed7e76c6395bcca7abc649"
    );
}
