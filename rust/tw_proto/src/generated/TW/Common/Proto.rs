// Automatically generated rust module for 'Common.proto' file

#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(unused_imports)]
#![allow(unknown_lints)]
#![allow(clippy::all)]
#![cfg_attr(rustfmt, rustfmt_skip)]


use quick_protobuf::{BytesReader, Result, MessageInfo, MessageRead, MessageWrite};
use super::super::super::*;

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum SigningError {
    OK = 0,
    Error_general = 1,
    Error_internal = 2,
    Error_low_balance = 3,
    Error_zero_amount_requested = 4,
    Error_missing_private_key = 5,
    Error_invalid_private_key = 15,
    Error_invalid_address = 16,
    Error_invalid_utxo = 17,
    Error_invalid_utxo_amount = 18,
    Error_wrong_fee = 6,
    Error_signing = 7,
    Error_tx_too_big = 8,
    Error_missing_input_utxos = 9,
    Error_not_enough_utxos = 10,
    Error_script_redeem = 11,
    Error_script_output = 12,
    Error_script_witness_program = 13,
    Error_invalid_memo = 14,
    Error_input_parse = 19,
    Error_no_support_n2n = 20,
    Error_signatures_count = 21,
    Error_invalid_params = 22,
    Error_invalid_requested_token_amount = 23,
}

impl Default for SigningError {
    fn default() -> Self {
        SigningError::OK
    }
}

impl From<i32> for SigningError {
    fn from(i: i32) -> Self {
        match i {
            0 => SigningError::OK,
            1 => SigningError::Error_general,
            2 => SigningError::Error_internal,
            3 => SigningError::Error_low_balance,
            4 => SigningError::Error_zero_amount_requested,
            5 => SigningError::Error_missing_private_key,
            15 => SigningError::Error_invalid_private_key,
            16 => SigningError::Error_invalid_address,
            17 => SigningError::Error_invalid_utxo,
            18 => SigningError::Error_invalid_utxo_amount,
            6 => SigningError::Error_wrong_fee,
            7 => SigningError::Error_signing,
            8 => SigningError::Error_tx_too_big,
            9 => SigningError::Error_missing_input_utxos,
            10 => SigningError::Error_not_enough_utxos,
            11 => SigningError::Error_script_redeem,
            12 => SigningError::Error_script_output,
            13 => SigningError::Error_script_witness_program,
            14 => SigningError::Error_invalid_memo,
            19 => SigningError::Error_input_parse,
            20 => SigningError::Error_no_support_n2n,
            21 => SigningError::Error_signatures_count,
            22 => SigningError::Error_invalid_params,
            23 => SigningError::Error_invalid_requested_token_amount,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for SigningError {
    fn from(s: &'a str) -> Self {
        match s {
            "OK" => SigningError::OK,
            "Error_general" => SigningError::Error_general,
            "Error_internal" => SigningError::Error_internal,
            "Error_low_balance" => SigningError::Error_low_balance,
            "Error_zero_amount_requested" => SigningError::Error_zero_amount_requested,
            "Error_missing_private_key" => SigningError::Error_missing_private_key,
            "Error_invalid_private_key" => SigningError::Error_invalid_private_key,
            "Error_invalid_address" => SigningError::Error_invalid_address,
            "Error_invalid_utxo" => SigningError::Error_invalid_utxo,
            "Error_invalid_utxo_amount" => SigningError::Error_invalid_utxo_amount,
            "Error_wrong_fee" => SigningError::Error_wrong_fee,
            "Error_signing" => SigningError::Error_signing,
            "Error_tx_too_big" => SigningError::Error_tx_too_big,
            "Error_missing_input_utxos" => SigningError::Error_missing_input_utxos,
            "Error_not_enough_utxos" => SigningError::Error_not_enough_utxos,
            "Error_script_redeem" => SigningError::Error_script_redeem,
            "Error_script_output" => SigningError::Error_script_output,
            "Error_script_witness_program" => SigningError::Error_script_witness_program,
            "Error_invalid_memo" => SigningError::Error_invalid_memo,
            "Error_input_parse" => SigningError::Error_input_parse,
            "Error_no_support_n2n" => SigningError::Error_no_support_n2n,
            "Error_signatures_count" => SigningError::Error_signatures_count,
            "Error_invalid_params" => SigningError::Error_invalid_params,
            "Error_invalid_requested_token_amount" => SigningError::Error_invalid_requested_token_amount,
            _ => Self::default(),
        }
    }
}

