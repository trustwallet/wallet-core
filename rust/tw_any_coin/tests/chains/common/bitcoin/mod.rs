// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

// This seems to be required, even if the tests in `tests/` actually use
// functions/constants.
#![allow(dead_code)]

pub mod data;
pub mod plan;
pub mod sign;

pub const ONE_BTC: i64 = 100_000_000;
pub const MINER_FEE: i64 = 1_000_000;
pub const DUST: i64 = 546;

pub const SIGHASH_ALL: u32 = 1;

pub use tw_proto::BitcoinV3::Proto::mod_Input::mod_InputBuilder::OneOfvariant as InputBuilderType;
pub use tw_proto::BitcoinV3::Proto::mod_Input::{
    InputBuilder, OneOfclaiming_script as ClaimingScriptType,
};
pub use tw_proto::BitcoinV3::Proto::mod_Output::mod_OutputBuilder::OneOfvariant as OutputBuilderType;
pub use tw_proto::BitcoinV3::Proto::mod_Output::{
    OneOfto_recipient as RecipientType, OutputBuilder,
};
pub use tw_proto::BitcoinV3::Proto::mod_PublicKeyOrHash::OneOfvariant as PublicKeyOrHashType;

use tw_proto::BitcoinV3::Proto;

pub fn btc_info() -> Option<Proto::ChainInfo> {
    Some(Proto::ChainInfo {
        p2pkh_prefix: 0,
        p2sh_prefix: 5,
    })
}

pub fn dust_threshold(threshold: i64) -> Proto::mod_SigningInput::OneOfdust_policy {
    Proto::mod_SigningInput::OneOfdust_policy::fixed_dust_threshold(threshold)
}

pub mod input {
    use super::*;
    use tw_encoding::hex::DecodeHex;
    use tw_memory::Data;

    pub fn reverse_txid(txid: &str) -> Data {
        txid.decode_hex().unwrap().into_iter().rev().collect()
    }

    pub fn out_point(txid: &str, vout: u32) -> Option<Proto::OutPoint<'static>> {
        Some(Proto::OutPoint {
            hash: reverse_txid(txid).into(),
            vout,
        })
    }

    pub fn claiming_script_builder(ty: InputBuilderType<'static>) -> ClaimingScriptType<'static> {
        ClaimingScriptType::script_builder(InputBuilder { variant: ty })
    }

    pub fn p2pkh(pubkey: Data) -> ClaimingScriptType<'static> {
        claiming_script_builder(InputBuilderType::p2pkh(Proto::PublicKeyOrHash {
            variant: PublicKeyOrHashType::pubkey(pubkey.into()),
        }))
    }

    pub fn p2wpkh(pubkey: Data) -> ClaimingScriptType<'static> {
        claiming_script_builder(InputBuilderType::p2wpkh(Proto::PublicKeyOrHash {
            variant: PublicKeyOrHashType::pubkey(pubkey.into()),
        }))
    }

    pub fn p2tr_key_path(pubkey: Data) -> ClaimingScriptType<'static> {
        claiming_script_builder(InputBuilderType::p2tr_key_path(pubkey.into()))
    }

    pub fn brc20_inscribe(
        inscribe_to: Data,
        ticker: &str,
        transfer_amount: &str,
    ) -> ClaimingScriptType<'static> {
        claiming_script_builder(InputBuilderType::brc20_inscribe(
            Proto::mod_Input::InputBrc20Inscription {
                inscribe_to: inscribe_to.into(),
                ticker: ticker.to_string().into(),
                transfer_amount: transfer_amount.to_string().into(),
            },
        ))
    }

    pub fn receiver_address(addr: &str) -> ClaimingScriptType<'static> {
        ClaimingScriptType::receiver_address(addr.to_string().into())
    }
}

pub mod output {
    use super::*;
    use tw_memory::Data;

    pub fn receiver_builder(ty: OutputBuilderType<'static>) -> RecipientType<'static> {
        RecipientType::builder(OutputBuilder { variant: ty })
    }

    pub fn p2pkh(pubkey: Data) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::p2pkh(Proto::PublicKeyOrHash {
            variant: PublicKeyOrHashType::pubkey(pubkey.into()),
        }))
    }

    pub fn p2wpkh(pubkey: Data) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::p2wpkh(Proto::PublicKeyOrHash {
            variant: PublicKeyOrHashType::pubkey(pubkey.into()),
        }))
    }

    pub fn p2tr_key_path(pubkey: Data) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::p2tr_key_path(pubkey.into()))
    }

    pub fn brc20_inscribe(
        inscribe_to: Data,
        ticker: &str,
        transfer_amount: &str,
    ) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::brc20_inscribe(
            Proto::mod_Output::OutputBrc20Inscription {
                inscribe_to: inscribe_to.into(),
                ticker: ticker.to_string().into(),
                transfer_amount: transfer_amount.to_string().into(),
            },
        ))
    }

    pub fn to_address(addr: &str) -> RecipientType<'static> {
        RecipientType::to_address(addr.to_string().into())
    }
}
