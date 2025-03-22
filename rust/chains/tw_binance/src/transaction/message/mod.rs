// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::{Deserialize, Serialize, Serializer};
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;
use tw_proto::Binance::Proto::{self, mod_SigningInput::OneOforder_oneof as BinanceMessageProto};

pub mod htlt_order;
pub mod send_order;
pub mod side_chain_delegate;
pub mod time_lock_order;
pub mod token_order;
pub mod trade_order;
pub mod transfer_out_order;

pub trait BinanceMessage {
    fn to_amino_protobuf(&self) -> SigningResult<Data>;
}

/// A Binance message represented as a Trust Wallet Core Protobuf message.
pub trait TWBinanceProto: Sized {
    type Proto<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self>;

    fn to_tw_proto(&self) -> Self::Proto<'static>;
}

/// Please note that some of the fields are typed such as `SideDelegateOrder`.
#[derive(Deserialize, Serialize)]
#[serde(untagged)]
pub enum BinanceMessageEnum {
    HTLTOrder(htlt_order::HTLTOrder),
    DepositHTLTOrder(htlt_order::DepositHTLTOrder),
    ClaimHTLTOrder(htlt_order::ClaimHTLTOrder),
    RefundHTLTOrder(htlt_order::RefundHTLTOrder),
    SendOrder(send_order::SendOrder),
    SideDelegateOrder(side_chain_delegate::SideDelegateOrder),
    SideRedelegateOrder(side_chain_delegate::SideRedelegateOrder),
    SideUndelegateOrder(side_chain_delegate::SideUndelegateOrder),
    StakeMigrationOrder(side_chain_delegate::StakeMigrationOrder),
    TimeLockOrder(time_lock_order::TimeLockOrder),
    TimeRelockOrder(time_lock_order::TimeRelockOrder),
    TimeUnlockOrder(time_lock_order::TimeUnlockOrder),
    TokenFreezeOrder(token_order::TokenFreezeOrder),
    TokenUnfreezeOrder(token_order::TokenUnfreezeOrder),
    TokenIssueOrder(token_order::TokenIssueOrder),
    TokenMintOrder(token_order::TokenMintOrder),
    TokenBurnOrder(token_order::TokenBurnOrder),
    NewTradeOrder(trade_order::NewTradeOrder),
    CancelTradeOrder(trade_order::CancelTradeOrder),
    TransferOutOrder(transfer_out_order::TransferOutOrder),
}

impl TWBinanceProto for BinanceMessageEnum {
    type Proto<'a> = BinanceMessageProto<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        match msg {
            BinanceMessageProto::trade_order(ref order) => {
                trade_order::NewTradeOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::NewTradeOrder)
            },
            BinanceMessageProto::cancel_trade_order(ref order) => {
                trade_order::CancelTradeOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::CancelTradeOrder)
            },
            BinanceMessageProto::send_order(ref order) => {
                send_order::SendOrder::from_tw_proto(coin, order).map(BinanceMessageEnum::SendOrder)
            },
            BinanceMessageProto::freeze_order(ref order) => {
                token_order::TokenFreezeOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::TokenFreezeOrder)
            },
            BinanceMessageProto::unfreeze_order(ref order) => {
                token_order::TokenUnfreezeOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::TokenUnfreezeOrder)
            },
            BinanceMessageProto::htlt_order(ref order) => {
                htlt_order::HTLTOrder::from_tw_proto(coin, order).map(BinanceMessageEnum::HTLTOrder)
            },
            BinanceMessageProto::depositHTLT_order(ref order) => {
                htlt_order::DepositHTLTOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::DepositHTLTOrder)
            },
            BinanceMessageProto::claimHTLT_order(ref order) => {
                htlt_order::ClaimHTLTOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::ClaimHTLTOrder)
            },
            BinanceMessageProto::refundHTLT_order(ref order) => {
                htlt_order::RefundHTLTOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::RefundHTLTOrder)
            },
            BinanceMessageProto::issue_order(ref order) => {
                token_order::TokenIssueOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::TokenIssueOrder)
            },
            BinanceMessageProto::mint_order(ref order) => {
                token_order::TokenMintOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::TokenMintOrder)
            },
            BinanceMessageProto::burn_order(ref order) => {
                token_order::TokenBurnOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::TokenBurnOrder)
            },
            BinanceMessageProto::transfer_out_order(ref order) => {
                transfer_out_order::TransferOutOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::TransferOutOrder)
            },
            BinanceMessageProto::side_delegate_order(ref order) => {
                side_chain_delegate::SideDelegateOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::SideDelegateOrder)
            },
            BinanceMessageProto::side_redelegate_order(ref order) => {
                side_chain_delegate::SideRedelegateOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::SideRedelegateOrder)
            },
            BinanceMessageProto::side_undelegate_order(ref order) => {
                side_chain_delegate::SideUndelegateOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::SideUndelegateOrder)
            },
            BinanceMessageProto::time_lock_order(ref order) => {
                time_lock_order::TimeLockOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::TimeLockOrder)
            },
            BinanceMessageProto::time_relock_order(ref order) => {
                time_lock_order::TimeRelockOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::TimeRelockOrder)
            },
            BinanceMessageProto::time_unlock_order(ref order) => {
                time_lock_order::TimeUnlockOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::TimeUnlockOrder)
            },
            BinanceMessageProto::side_stake_migration_order(ref order) => {
                side_chain_delegate::StakeMigrationOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::StakeMigrationOrder)
            },
            BinanceMessageProto::None => SigningError::err(SigningErrorType::Error_invalid_params),
        }
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        match self {
            BinanceMessageEnum::HTLTOrder(m) => BinanceMessageProto::htlt_order(m.to_tw_proto()),
            BinanceMessageEnum::DepositHTLTOrder(m) => {
                BinanceMessageProto::depositHTLT_order(m.to_tw_proto())
            },
            BinanceMessageEnum::ClaimHTLTOrder(m) => {
                BinanceMessageProto::claimHTLT_order(m.to_tw_proto())
            },
            BinanceMessageEnum::RefundHTLTOrder(m) => {
                BinanceMessageProto::refundHTLT_order(m.to_tw_proto())
            },
            BinanceMessageEnum::SendOrder(m) => BinanceMessageProto::send_order(m.to_tw_proto()),
            BinanceMessageEnum::SideDelegateOrder(m) => {
                BinanceMessageProto::side_delegate_order(m.to_tw_proto())
            },
            BinanceMessageEnum::SideRedelegateOrder(m) => {
                BinanceMessageProto::side_redelegate_order(m.to_tw_proto())
            },
            BinanceMessageEnum::SideUndelegateOrder(m) => {
                BinanceMessageProto::side_undelegate_order(m.to_tw_proto())
            },
            BinanceMessageEnum::StakeMigrationOrder(m) => {
                BinanceMessageProto::side_stake_migration_order(m.to_tw_proto())
            },
            BinanceMessageEnum::TimeLockOrder(m) => {
                BinanceMessageProto::time_lock_order(m.to_tw_proto())
            },
            BinanceMessageEnum::TimeRelockOrder(m) => {
                BinanceMessageProto::time_relock_order(m.to_tw_proto())
            },
            BinanceMessageEnum::TimeUnlockOrder(m) => {
                BinanceMessageProto::time_unlock_order(m.to_tw_proto())
            },
            BinanceMessageEnum::TokenFreezeOrder(m) => {
                BinanceMessageProto::freeze_order(m.to_tw_proto())
            },
            BinanceMessageEnum::TokenUnfreezeOrder(m) => {
                BinanceMessageProto::unfreeze_order(m.to_tw_proto())
            },
            BinanceMessageEnum::TokenIssueOrder(m) => {
                BinanceMessageProto::issue_order(m.to_tw_proto())
            },
            BinanceMessageEnum::TokenMintOrder(m) => {
                BinanceMessageProto::mint_order(m.to_tw_proto())
            },
            BinanceMessageEnum::TokenBurnOrder(m) => {
                BinanceMessageProto::burn_order(m.to_tw_proto())
            },
            BinanceMessageEnum::NewTradeOrder(m) => {
                BinanceMessageProto::trade_order(m.to_tw_proto())
            },
            BinanceMessageEnum::CancelTradeOrder(m) => {
                BinanceMessageProto::cancel_trade_order(m.to_tw_proto())
            },
            BinanceMessageEnum::TransferOutOrder(m) => {
                BinanceMessageProto::transfer_out_order(m.to_tw_proto())
            },
        }
    }
}

impl<'a> AsRef<dyn BinanceMessage + 'a> for BinanceMessageEnum {
    fn as_ref(&self) -> &(dyn BinanceMessage + 'a) {
        match self {
            BinanceMessageEnum::HTLTOrder(m) => m,
            BinanceMessageEnum::DepositHTLTOrder(m) => m,
            BinanceMessageEnum::ClaimHTLTOrder(m) => m,
            BinanceMessageEnum::RefundHTLTOrder(m) => m,
            BinanceMessageEnum::SendOrder(m) => m,
            BinanceMessageEnum::SideDelegateOrder(m) => m,
            BinanceMessageEnum::SideRedelegateOrder(m) => m,
            BinanceMessageEnum::SideUndelegateOrder(m) => m,
            BinanceMessageEnum::StakeMigrationOrder(m) => m,
            BinanceMessageEnum::TimeLockOrder(m) => m,
            BinanceMessageEnum::TimeRelockOrder(m) => m,
            BinanceMessageEnum::TimeUnlockOrder(m) => m,
            BinanceMessageEnum::TokenFreezeOrder(m) => m,
            BinanceMessageEnum::TokenUnfreezeOrder(m) => m,
            BinanceMessageEnum::TokenIssueOrder(m) => m,
            BinanceMessageEnum::TokenMintOrder(m) => m,
            BinanceMessageEnum::TokenBurnOrder(m) => m,
            BinanceMessageEnum::NewTradeOrder(m) => m,
            BinanceMessageEnum::CancelTradeOrder(m) => m,
            BinanceMessageEnum::TransferOutOrder(m) => m,
        }
    }
}

#[derive(Deserialize, Serialize)]
pub struct Token {
    /// Amount.
    pub amount: i64,
    /// Token ID.
    pub denom: String,
}

impl Token {
    pub fn serialize_with_string_amount<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        #[derive(Serialize)]
        struct TokenWithStringAmount<'a> {
            amount: String,
            denom: &'a str,
        }

        TokenWithStringAmount {
            amount: self.amount.to_string(),
            denom: &self.denom,
        }
        .serialize(serializer)
    }

    fn from_tw_proto(msg: &Proto::mod_SendOrder::Token<'_>) -> Self {
        Token {
            denom: msg.denom.to_string(),
            amount: msg.amount,
        }
    }

    fn to_tw_proto(&self) -> Proto::mod_SendOrder::Token<'static> {
        Proto::mod_SendOrder::Token {
            denom: self.denom.clone().into(),
            amount: self.amount,
        }
    }
}
