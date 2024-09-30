// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

// This seems to be required, even if the tests in `tests/` actually use
// functions/constants.
#![allow(dead_code)]

use tw_encoding::hex::DecodeHex;

pub mod compile;
pub mod data;
pub mod plan;
pub mod preimage;
pub mod psbt_plan;
pub mod psbt_sign;
pub mod sign;

pub const ONE_BTC: i64 = 100_000_000;
pub const MINER_FEE: i64 = 1_000_000;
pub const DUST: i64 = 546;

pub const SIGHASH_ALL: u32 = 0x01;
pub const SIGHASH_SINGLE: u32 = 0x03;
pub const SIGHASH_ANYONE_CAN_PAY: u32 = 0x80;

pub const BITCOIN_P2PKH_PREFIX: u8 = 0;
pub const BITCOIN_P2SH_PREFIX: u8 = 5;
pub const BITCOIN_HRP: &str = "bc";

pub type Amount = i64;

pub use tw_proto::BitcoinV2::Proto::mod_Input::mod_InputBuilder::OneOfvariant as InputBuilderType;
pub use tw_proto::BitcoinV2::Proto::mod_Input::{
    InputBuilder, OneOfclaiming_script as ClaimingScriptType,
};
pub use tw_proto::BitcoinV2::Proto::mod_Output::mod_OutputBuilder::OneOfvariant as OutputBuilderType;
pub use tw_proto::BitcoinV2::Proto::mod_Output::{
    OneOfto_recipient as RecipientType, OutputBuilder,
};
pub use tw_proto::BitcoinV2::Proto::mod_PublicKeyOrHash::OneOfvariant as PublicKeyOrHashType;
pub use tw_proto::BitcoinV2::Proto::mod_SigningInput::OneOftransaction as TransactionOneof;

use tw_proto::BitcoinV2::Proto;

pub fn btc_info() -> Option<Proto::ChainInfo<'static>> {
    Some(Proto::ChainInfo {
        p2pkh_prefix: BITCOIN_P2PKH_PREFIX as u32,
        p2sh_prefix: BITCOIN_P2SH_PREFIX as u32,
        hrp: "bc".into(),
    })
}

pub fn dust_threshold(threshold: i64) -> Proto::mod_TransactionBuilder::OneOfdust_policy {
    Proto::mod_TransactionBuilder::OneOfdust_policy::fixed_dust_threshold(threshold)
}

pub fn transaction_psbt(hex: &str) -> TransactionOneof {
    TransactionOneof::psbt(Proto::Psbt {
        psbt: hex.decode_hex().unwrap().into(),
    })
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

    pub fn sequence(sequence: u32) -> Option<Proto::mod_Input::Sequence> {
        Some(Proto::mod_Input::Sequence { sequence })
    }

    pub fn claiming_script_builder(ty: InputBuilderType<'static>) -> ClaimingScriptType<'static> {
        ClaimingScriptType::script_builder(InputBuilder { variant: ty })
    }

    pub fn p2pk(pubkey: Data) -> ClaimingScriptType<'static> {
        claiming_script_builder(InputBuilderType::p2pk(pubkey.into()))
    }

    pub fn p2pkh(pubkey: Data) -> ClaimingScriptType<'static> {
        claiming_script_builder(InputBuilderType::p2pkh(Proto::PublicKeyOrHash {
            variant: PublicKeyOrHashType::pubkey(pubkey.into()),
        }))
    }

    pub fn p2pkh_with_hash(pubkey_hash: Data) -> ClaimingScriptType<'static> {
        claiming_script_builder(InputBuilderType::p2pkh(Proto::PublicKeyOrHash {
            variant: PublicKeyOrHashType::hash(pubkey_hash.into()),
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

    pub fn custom_script(script: Data) -> ClaimingScriptType<'static> {
        ClaimingScriptType::script_data(script.into())
    }
}

pub mod output {
    use super::*;
    use tw_memory::Data;
    use tw_proto::BitcoinV2::Proto::mod_Output::mod_RedeemScriptOrHash::OneOfvariant as RedeemScriptOrHashType;
    use tw_proto::BitcoinV2::Proto::mod_Output::OutputTaprootScriptPath as TaprootScriptPath;

    pub fn receiver_builder(ty: OutputBuilderType<'static>) -> RecipientType<'static> {
        RecipientType::builder(OutputBuilder { variant: ty })
    }

    pub fn p2sh_with_script(redeem_script: Data) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::p2sh(
            Proto::mod_Output::RedeemScriptOrHash {
                variant: RedeemScriptOrHashType::redeem_script(redeem_script.into()),
            },
        ))
    }

    pub fn p2sh_with_hash(redeem_script_hash: Data) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::p2sh(
            Proto::mod_Output::RedeemScriptOrHash {
                variant: RedeemScriptOrHashType::hash(redeem_script_hash.into()),
            },
        ))
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

    pub fn p2wsh_with_script(redeem_script: Data) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::p2wsh(
            Proto::mod_Output::RedeemScriptOrHash {
                variant: RedeemScriptOrHashType::redeem_script(redeem_script.into()),
            },
        ))
    }

    pub fn p2wsh_with_hash(redeem_script_hash: Data) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::p2wsh(
            Proto::mod_Output::RedeemScriptOrHash {
                variant: RedeemScriptOrHashType::hash(redeem_script_hash.into()),
            },
        ))
    }

    pub fn p2tr_key_path(pubkey: Data) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::p2tr_key_path(pubkey.into()))
    }

    pub fn p2tr_assume_tweaked(tweaked_pubkey: Data) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::p2tr_dangerous_assume_tweaked(
            tweaked_pubkey.into(),
        ))
    }

    pub fn p2tr_script_path(internal_pubkey: Data, merkle_root: Data) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::p2tr_script_path(TaprootScriptPath {
            internal_key: internal_pubkey.into(),
            merkle_root: merkle_root.into(),
        }))
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

    pub fn op_return(data: Data) -> RecipientType<'static> {
        receiver_builder(OutputBuilderType::op_return(data.into()))
    }
}
