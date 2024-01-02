// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use serde::ser::Error as SerError;
use serde::{Deserialize, Serialize, Serializer};
use serde_json::Value as Json;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_memory::Data;
use tw_proto::Binance::Proto::{self, mod_SigningInput::OneOforder_oneof as BinanceMessageProto};

pub mod htlt_order;
pub mod send_order;
pub mod side_chain_delegate;
pub mod time_lock_order;
pub mod token_order;
pub mod trade_order;
pub mod tranfer_out_order;

pub trait BinanceMessage {
    fn to_amino_protobuf(&self) -> SigningResult<Data>;
}

/// A Binance message represented as a Trust Wallet Core Protobuf message.
pub trait TWBinanceProto: Sized {
    type Proto<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self>;

    fn to_tw_proto(&self) -> Self::Proto<'static>;
}

/// Please note that some of the fields are typped such as `SideDelegateOrder`.
#[derive(Deserialize, Serialize)]
#[serde(untagged)]
pub enum BinanceMessageEnum {
    // HTLTOrder(htlt_order::HTLTOrder),
    // DepositHTLTOrder(htlt_order::DepositHTLTOrder),
    // ClaimHTLTOrder(htlt_order::ClaimHTLTOrder),
    RefundHTLTOrder(htlt_order::RefundHTLTOrder),
    // SendOrder(send_order::SendOrder),
    // SideDelegateOrder(side_chain_delegate::SideDelegateOrder),
    // SideRedelegateOrder(side_chain_delegate::SideRedelegateOrder),
    // SideUndelegateOrder(side_chain_delegate::SideUndelegateOrder),
    // TimeLockOrder(time_lock_order::TimeLockOrder),
    // TimeRelockOrder(time_lock_order::TimeRelockOrder),
    // TimeUnlockOrder(time_lock_order::TimeUnlockOrder),
    // TokenFreezeOrder(token_order::TokenFreezeOrder),
    // TokenUnfreezeOrder(token_order::TokenUnfreezeOrder),
    // TokenIssueOrder(token_order::TokenIssueOrder),
    // TokenMintOrder(token_order::TokenMintOrder),
    // TokenBurnOrder(token_order::TokenBurnOrder),
    // NewTradeOrder(trade_order::NewTradeOrder),
    // CancelTradeOrder(trade_order::CancelTradeOrder),
    // TransferOutOrder(tranfer_out_order::TransferOutOrder),
}

impl TWBinanceProto for BinanceMessageEnum {
    type Proto<'a> = BinanceMessageProto<'a>;

    fn from_tw_proto(coin: &dyn CoinContext, msg: &Self::Proto<'_>) -> SigningResult<Self> {
        match msg {
            // BinanceMessageProto::trade_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::NewTradeOrder)
            // },
            // BinanceMessageProto::cancel_trade_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::CancelTradeOrder)
            // },
            // BinanceMessageProto::send_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::SendOrder)
            // },
            // BinanceMessageProto::freeze_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::TokenFreezeOrder)
            // },
            // BinanceMessageProto::unfreeze_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::TokenUnfreezeOrder)
            // },
            // BinanceMessageProto::htlt_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::HTLTOrder)
            // },
            // BinanceMessageProto::depositHTLT_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::DepositHTLTOrder)
            // },
            // BinanceMessageProto::claimHTLT_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::ClaimHTLTOrder)
            // },
            BinanceMessageProto::refundHTLT_order(ref order) => {
                htlt_order::RefundHTLTOrder::from_tw_proto(coin, order)
                    .map(BinanceMessageEnum::RefundHTLTOrder)
            },
            // BinanceMessageProto::issue_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::TokenIssueOrder)
            // },
            // BinanceMessageProto::mint_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::TokenMintOrder)
            // },
            // BinanceMessageProto::burn_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::TokenBurnOrder)
            // },
            // BinanceMessageProto::transfer_out_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::TransferOutOrder)
            // },
            // BinanceMessageProto::side_delegate_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::SideDelegateOrder)
            // },
            // BinanceMessageProto::side_redelegate_order(ref order) => order
            //     .try_into()
            //     .map(BinanceMessageEnum::SideRedelegateOrder),
            // BinanceMessageProto::side_undelegate_order(ref order) => order
            //     .try_into()
            //     .map(BinanceMessageEnum::SideUndelegateOrder),
            // BinanceMessageProto::time_lock_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::TimeLockOrder)
            // },
            // BinanceMessageProto::time_relock_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::TimeRelockOrder)
            // },
            // BinanceMessageProto::time_unlock_order(ref order) => {
            //     order.try_into().map(BinanceMessageEnum::TimeUnlockOrder)
            // },
            // BinanceMessageProto::None => Err(SigningError(SigningErrorType::Error_invalid_params)),
            _ => todo!(),
        }
    }

    fn to_tw_proto(&self) -> Self::Proto<'static> {
        match self {
            // BinanceMessageEnum::HTLTOrder(m) => BinanceMessageProto::htlt_order(m.into()),
            // BinanceMessageEnum::DepositHTLTOrder(m) => {
            //     BinanceMessageProto::depositHTLT_order(m.into())
            // },
            // BinanceMessageEnum::ClaimHTLTOrder(m) => BinanceMessageProto::claimHTLT_order(m.into()),
            BinanceMessageEnum::RefundHTLTOrder(m) => {
                BinanceMessageProto::refundHTLT_order(m.to_tw_proto())
            },
            // BinanceMessageEnum::SendOrder(m) => BinanceMessageProto::send_order(m.into()),
            // BinanceMessageEnum::SideDelegateOrder(m) => {
            //     BinanceMessageProto::side_delegate_order(m.into())
            // },
            // BinanceMessageEnum::SideRedelegateOrder(m) => {
            //     BinanceMessageProto::side_redelegate_order(m.into())
            // },
            // BinanceMessageEnum::SideUndelegateOrder(m) => {
            //     BinanceMessageProto::side_undelegate_order(m.into())
            // },
            // BinanceMessageEnum::TimeLockOrder(m) => BinanceMessageProto::time_lock_order(m.into()),
            // BinanceMessageEnum::TimeRelockOrder(m) => {
            //     BinanceMessageProto::time_relock_order(m.into())
            // },
            // BinanceMessageEnum::TimeUnlockOrder(m) => {
            //     BinanceMessageProto::time_unlock_order(m.into())
            // },
            // BinanceMessageEnum::TokenFreezeOrder(m) => BinanceMessageProto::freeze_order(m.into()),
            // BinanceMessageEnum::TokenUnfreezeOrder(m) => {
            //     BinanceMessageProto::unfreeze_order(m.into())
            // },
            // BinanceMessageEnum::TokenIssueOrder(m) => BinanceMessageProto::issue_order(m.into()),
            // BinanceMessageEnum::TokenMintOrder(m) => BinanceMessageProto::mint_order(m.into()),
            // BinanceMessageEnum::TokenBurnOrder(m) => BinanceMessageProto::burn_order(m.into()),
            // BinanceMessageEnum::NewTradeOrder(m) => BinanceMessageProto::trade_order(m.into()),
            // BinanceMessageEnum::CancelTradeOrder(m) => {
            //     BinanceMessageProto::cancel_trade_order(m.into())
            // },
            // BinanceMessageEnum::TransferOutOrder(m) => {
            //     BinanceMessageProto::transfer_out_order(m.into())
            // },
        }
    }
}

impl<'a> AsRef<dyn BinanceMessage + 'a> for BinanceMessageEnum {
    fn as_ref(&self) -> &(dyn BinanceMessage + 'a) {
        match self {
            // BinanceMessageEnum::HTLTOrder(m) => m,
            // BinanceMessageEnum::DepositHTLTOrder(m) => m,
            // BinanceMessageEnum::ClaimHTLTOrder(m) => m,
            BinanceMessageEnum::RefundHTLTOrder(m) => m,
            // BinanceMessageEnum::SendOrder(m) => m,
            // BinanceMessageEnum::SideDelegateOrder(m) => m,
            // BinanceMessageEnum::SideRedelegateOrder(m) => m,
            // BinanceMessageEnum::SideUndelegateOrder(m) => m,
            // BinanceMessageEnum::TimeLockOrder(m) => m,
            // BinanceMessageEnum::TimeRelockOrder(m) => m,
            // BinanceMessageEnum::TimeUnlockOrder(m) => m,
            // BinanceMessageEnum::TokenFreezeOrder(m) => m,
            // BinanceMessageEnum::TokenUnfreezeOrder(m) => m,
            // BinanceMessageEnum::TokenIssueOrder(m) => m,
            // BinanceMessageEnum::TokenMintOrder(m) => m,
            // BinanceMessageEnum::TokenBurnOrder(m) => m,
            // BinanceMessageEnum::NewTradeOrder(m) => m,
            // BinanceMessageEnum::CancelTradeOrder(m) => m,
            // BinanceMessageEnum::TransferOutOrder(m) => m,
        }
    }
}

#[derive(Serialize)]
pub struct Token {
    /// Token ID.
    pub denom: String,
    /// Amount.
    pub amount: i64,
}

impl Token {
    pub fn to_proto(&self) -> Proto::mod_SendOrder::Token {
        Proto::mod_SendOrder::Token {
            denom: self.denom.clone().into(),
            amount: self.amount,
        }
    }

    pub fn serialize_with_string_amount<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        #[derive(Serialize)]
        struct TokenWithStringAmount<'a> {
            denom: &'a str,
            amount: String,
        }

        TokenWithStringAmount {
            denom: &self.denom,
            amount: self.amount.to_string(),
        }
        .serialize(serializer)
    }
}
