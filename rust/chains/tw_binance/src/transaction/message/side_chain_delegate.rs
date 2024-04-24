// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::BinanceAddress;
use crate::amino::AminoEncoder;
use crate::transaction::message::{BinanceMessage, TWBinanceProto, Token};
use serde::{Deserialize, Serialize};
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_evm::address::Address as EthereumAddress;
use tw_memory::Data;
use tw_misc::serde::Typed;
use tw_proto::Binance::Proto;

pub type SideDelegateOrder = Typed<SideDelegateOrderValue>;

/// cosmos-sdk/MsgSideChainDelegate
#[derive(Deserialize, Serialize)]
pub struct SideDelegateOrderValue {
    #[serde(serialize_with = "Token::serialize_with_string_amount")]
    pub delegation: Token,
    pub delegator_addr: BinanceAddress,
    pub side_chain_id: String,
    pub validator_addr: BinanceAddress,
}

impl SideDelegateOrderValue {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xE3, 0xA0, 0x7F, 0xD2];
    /// cbindgen:ignore
    pub const MESSAGE_TYPE: &'static str = "cosmos-sdk/MsgSideChainDelegate";
}

impl BinanceMessage for SideDelegateOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&SideDelegateOrderValue::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for SideDelegateOrder {
    type Proto<'a> = Proto::SideChainDelegate<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let delegator_addr =
            BinanceAddress::from_key_hash_with_coin(coin, msg.delegator_addr.to_vec())?;
        let validator_addr = BinanceAddress::new_validator_addr(msg.validator_addr.to_vec())?;

        let delegation = msg
            .delegation
            .as_ref()
            .or_tw_err(SigningErrorType::Error_invalid_params)?;

        let value = SideDelegateOrderValue {
            delegator_addr,
            validator_addr,
            delegation: Token::from_tw_proto(delegation),
            side_chain_id: msg.chain_id.to_string(),
        };
        Ok(Typed {
            ty: SideDelegateOrderValue::MESSAGE_TYPE.to_string(),
            value,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::SideChainDelegate {
            delegator_addr: self.value.delegator_addr.data().into(),
            validator_addr: self.value.validator_addr.data().into(),
            delegation: Some(self.value.delegation.to_tw_proto()),
            chain_id: self.value.side_chain_id.clone().into(),
        }
    }
}

pub type SideRedelegateOrder = Typed<SideRedelegateOrderValue>;

/// cosmos-sdk/MsgSideChainRedelegate
#[derive(Deserialize, Serialize)]
pub struct SideRedelegateOrderValue {
    #[serde(serialize_with = "Token::serialize_with_string_amount")]
    pub amount: Token,
    pub delegator_addr: BinanceAddress,
    pub side_chain_id: String,
    pub validator_dst_addr: BinanceAddress,
    pub validator_src_addr: BinanceAddress,
}

impl SideRedelegateOrderValue {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0xE3, 0xCE, 0xD3, 0x64];
    /// cbindgen:ignore
    pub const MESSAGE_TYPE: &'static str = "cosmos-sdk/MsgSideChainRedelegate";
}

impl BinanceMessage for SideRedelegateOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&SideRedelegateOrderValue::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for SideRedelegateOrder {
    type Proto<'a> = Proto::SideChainRedelegate<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let delegator_addr =
            BinanceAddress::from_key_hash_with_coin(coin, msg.delegator_addr.to_vec())?;
        let validator_src_addr =
            BinanceAddress::new_validator_addr(msg.validator_src_addr.to_vec())?;
        let validator_dst_addr =
            BinanceAddress::new_validator_addr(msg.validator_dst_addr.to_vec())?;

        let amount = msg
            .amount
            .as_ref()
            .or_tw_err(SigningErrorType::Error_invalid_params)?;

        let value = SideRedelegateOrderValue {
            delegator_addr,
            validator_src_addr,
            validator_dst_addr,
            amount: Token::from_tw_proto(amount),
            side_chain_id: msg.chain_id.to_string(),
        };
        Ok(Typed {
            ty: SideRedelegateOrderValue::MESSAGE_TYPE.to_string(),
            value,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::SideChainRedelegate {
            delegator_addr: self.value.delegator_addr.data().into(),
            validator_src_addr: self.value.validator_src_addr.data().into(),
            validator_dst_addr: self.value.validator_dst_addr.data().into(),
            amount: Some(self.value.amount.to_tw_proto()),
            chain_id: self.value.side_chain_id.clone().into(),
        }
    }
}

pub type SideUndelegateOrder = Typed<SideUndelegateOrderValue>;

/// cosmos-sdk/MsgSideChainUndelegate
#[derive(Deserialize, Serialize)]
pub struct SideUndelegateOrderValue {
    #[serde(serialize_with = "Token::serialize_with_string_amount")]
    pub amount: Token,
    pub delegator_addr: BinanceAddress,
    pub side_chain_id: String,
    pub validator_addr: BinanceAddress,
}

impl SideUndelegateOrderValue {
    /// cbindgen:ignore
    pub const PREFIX: [u8; 4] = [0x51, 0x4F, 0x7E, 0x0E];
    /// cbindgen:ignore
    pub const MESSAGE_TYPE: &'static str = "cosmos-sdk/MsgSideChainUndelegate";
}

impl BinanceMessage for SideUndelegateOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&SideUndelegateOrderValue::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for SideUndelegateOrder {
    type Proto<'a> = Proto::SideChainUndelegate<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let delegator_addr =
            BinanceAddress::from_key_hash_with_coin(coin, msg.delegator_addr.to_vec())?;
        let validator_addr = BinanceAddress::new_validator_addr(msg.validator_addr.to_vec())?;

        let amount = msg
            .amount
            .as_ref()
            .or_tw_err(SigningErrorType::Error_invalid_params)?;

        let value = SideUndelegateOrderValue {
            delegator_addr,
            validator_addr,
            amount: Token::from_tw_proto(amount),
            side_chain_id: msg.chain_id.to_string(),
        };
        Ok(Typed {
            ty: SideUndelegateOrderValue::MESSAGE_TYPE.to_string(),
            value,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::SideChainUndelegate {
            delegator_addr: self.value.delegator_addr.data().into(),
            validator_addr: self.value.validator_addr.data().into(),
            amount: Some(self.value.amount.to_tw_proto()),
            chain_id: self.value.side_chain_id.clone().into(),
        }
    }
}

pub type StakeMigrationOrder = Typed<StakeMigrationOrderValue>;

/// https://github.com/bnb-chain/bnc-cosmos-sdk/blob/cf3ab19af300ccd6a6381287c3fae6bf6ac12f5e/x/stake/types/stake_migration.go#L29-L35
#[derive(Deserialize, Serialize)]
pub struct StakeMigrationOrderValue {
    #[serde(serialize_with = "Token::serialize_with_string_amount")]
    pub amount: Token,
    pub delegator_addr: EthereumAddress,
    pub refund_addr: BinanceAddress,
    pub validator_dst_addr: EthereumAddress,
    pub validator_src_addr: BinanceAddress,
}

impl StakeMigrationOrderValue {
    /// cbindgen:ignore
    /// https://github.com/bnb-chain/javascript-sdk/blob/442286ac2923fdfd7cb4fb2299f722ec263c714c/src/types/tx/stdTx.ts#L68
    pub const PREFIX: [u8; 4] = [0x38, 0x58, 0x91, 0x96];
    /// cbindgen:ignore
    pub const MESSAGE_TYPE: &'static str = "cosmos-sdk/MsgSideChainStakeMigration";
}

impl BinanceMessage for StakeMigrationOrder {
    fn to_amino_protobuf(&self) -> SigningResult<Data> {
        Ok(AminoEncoder::new(&StakeMigrationOrderValue::PREFIX)
            .extend_with_msg(&self.to_tw_proto())?
            .encode())
    }
}

impl TWBinanceProto for StakeMigrationOrder {
    type Proto<'a> = Proto::SideChainStakeMigration<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        let delegator_addr = EthereumAddress::try_from(msg.delegator_addr.as_ref())?;
        let refund_addr = BinanceAddress::from_key_hash_with_coin(coin, msg.refund_addr.to_vec())?;
        let validator_dst_addr = EthereumAddress::try_from(msg.validator_dst_addr.as_ref())?;
        let validator_src_addr =
            BinanceAddress::new_validator_addr(msg.validator_src_addr.to_vec())?;

        let amount = msg
            .amount
            .as_ref()
            .or_tw_err(SigningErrorType::Error_invalid_params)?;

        let value = StakeMigrationOrderValue {
            amount: Token::from_tw_proto(amount),
            delegator_addr,
            refund_addr,
            validator_dst_addr,
            validator_src_addr,
        };
        Ok(Typed {
            ty: StakeMigrationOrderValue::MESSAGE_TYPE.to_string(),
            value,
        })
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        Proto::SideChainStakeMigration {
            delegator_addr: self.value.delegator_addr.data().into(),
            validator_src_addr: self.value.validator_src_addr.data().into(),
            validator_dst_addr: self.value.validator_dst_addr.data().into(),
            refund_addr: self.value.refund_addr.data().into(),
            amount: Some(self.value.amount.to_tw_proto()),
        }
    }
}
