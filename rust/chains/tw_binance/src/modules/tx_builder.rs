// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::BinanceAddress;
use crate::transaction::message::Token;
use crate::transaction::message::{BinanceMessage, BinanceMessageBox};
use crate::transaction::UnsignedTransaction;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_proto::Binance::Proto;

pub struct TxBuilder;

impl TxBuilder {
    pub fn unsigned_tx_from_proto(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<UnsignedTransaction> {
        let msg = Self::msg_from_proto(coin, input)?;

        Ok(UnsignedTransaction {
            account_number: input.account_number,
            chain_id: input.chain_id.to_string(),
            data: None,
            memo: input.memo.to_string(),
            msgs: vec![msg],
            sequence: input.sequence,
            source: input.source,
        })
    }

    pub fn msg_from_proto(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use Proto::mod_SigningInput::OneOforder_oneof as OrderEnum;

        match input.order_oneof {
            OrderEnum::trade_order(ref new_order) => Self::trade_order_from_proto(coin, new_order),
            OrderEnum::cancel_trade_order(ref cancel_order) => {
                Self::cancel_order_from_proto(coin, cancel_order)
            },
            OrderEnum::send_order(ref send_order) => Self::send_order_from_proto(coin, send_order),
            OrderEnum::freeze_order(ref freeze_order) => {
                Self::freeze_order_from_proto(coin, freeze_order)
            },
            OrderEnum::unfreeze_order(ref unfreeze_order) => {
                Self::unfreeze_order_from_proto(coin, unfreeze_order)
            },
            OrderEnum::htlt_order(ref htlt_order) => Self::htlt_order_from_proto(coin, htlt_order),
            OrderEnum::depositHTLT_order(ref deposit_htlt) => {
                Self::deposit_htlt_order_from_proto(coin, deposit_htlt)
            },
            OrderEnum::claimHTLT_order(ref claim_htlt) => {
                Self::claim_htlt_order_from_proto(coin, claim_htlt)
            },
            OrderEnum::refundHTLT_order(ref refund_htlt) => {
                Self::refund_htlt_order_from_proto(coin, refund_htlt)
            },
            // TODO insert between
            OrderEnum::issue_order(ref issue_order) => {
                Self::issue_order_from_proto(coin, issue_order)
            },
            OrderEnum::mint_order(ref mint_order) => Self::mint_order_from_proto(coin, mint_order),
            OrderEnum::burn_order(ref burn_order) => Self::burn_order_from_proto(coin, burn_order),
            OrderEnum::None => Err(SigningError(SigningErrorType::Error_invalid_params)),
            _ => todo!(),
        }
    }

    pub fn trade_order_from_proto(
        coin: &dyn CoinContext,
        new_order: &Proto::TradeOrder<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use crate::transaction::message::trade_order::NewTradeOrder;
        use crate::transaction::message::trade_order::OrderType;

        let order_type = OrderType::from_repr(new_order.ordertype)
            .ok_or(SigningError(SigningErrorType::Error_invalid_params))?;
        let sender = BinanceAddress::from_key_hash_with_coin(coin, new_order.sender.to_vec())?;

        Ok(NewTradeOrder {
            id: new_order.id.to_string(),
            order_type,
            price: new_order.price,
            quantity: new_order.quantity,
            sender,
            side: new_order.side,
            symbol: new_order.symbol.to_string(),
            time_in_force: new_order.timeinforce,
        }
        .into_boxed())
    }

    pub fn cancel_order_from_proto(
        coin: &dyn CoinContext,
        cancel_order: &Proto::CancelTradeOrder<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use crate::transaction::message::trade_order::CancelTradeOrder;

        let sender = BinanceAddress::from_key_hash_with_coin(coin, cancel_order.sender.to_vec())?;
        Ok(CancelTradeOrder {
            sender,
            symbol: cancel_order.symbol.to_string(),
            refid: cancel_order.refid.to_string(),
        }
        .into_boxed())
    }

    pub fn send_order_from_proto(
        coin: &dyn CoinContext,
        send_order: &Proto::SendOrder<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use crate::transaction::message::send_order::{InOut, SendOrder};

        fn in_out_from_proto(
            coin: &dyn CoinContext,
            address_key_hash: &[u8],
            coins: &[Proto::mod_SendOrder::Token],
        ) -> SigningResult<InOut> {
            let address = BinanceAddress::from_key_hash_with_coin(coin, address_key_hash.to_vec())?;
            let coins = coins.iter().map(TxBuilder::token_from_proto).collect();

            Ok(InOut { address, coins })
        }

        let inputs = send_order
            .inputs
            .iter()
            .map(|input| in_out_from_proto(coin, &input.address, &input.coins))
            .collect::<SigningResult<Vec<_>>>()?;

        let outputs = send_order
            .outputs
            .iter()
            .map(|output| in_out_from_proto(coin, &output.address, &output.coins))
            .collect::<SigningResult<Vec<_>>>()?;

        Ok(SendOrder { inputs, outputs }.into_boxed())
    }

    pub fn freeze_order_from_proto(
        coin: &dyn CoinContext,
        freeze_order: &Proto::TokenFreezeOrder<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use crate::transaction::message::token_order::TokenFreezeOrder;

        let from = BinanceAddress::from_key_hash_with_coin(coin, freeze_order.from.to_vec())?;
        Ok(TokenFreezeOrder {
            from,
            symbol: freeze_order.symbol.to_string(),
            amount: freeze_order.amount,
        }
        .into_boxed())
    }

    pub fn unfreeze_order_from_proto(
        coin: &dyn CoinContext,
        unfreeze_order: &Proto::TokenUnfreezeOrder<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use crate::transaction::message::token_order::TokenUnfreezeOrder;

        let from = BinanceAddress::from_key_hash_with_coin(coin, unfreeze_order.from.to_vec())?;
        Ok(TokenUnfreezeOrder {
            from,
            symbol: unfreeze_order.symbol.to_string(),
            amount: unfreeze_order.amount,
        }
        .into_boxed())
    }

    pub fn htlt_order_from_proto(
        coin: &dyn CoinContext,
        htlt_order: &Proto::HTLTOrder<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use crate::transaction::message::htlt_order::HTLTOrder;

        let from = BinanceAddress::from_key_hash_with_coin(coin, htlt_order.from.to_vec())?;
        let to = BinanceAddress::from_key_hash_with_coin(coin, htlt_order.to.to_vec())?;

        let amount = htlt_order
            .amount
            .iter()
            .map(Self::token_from_proto)
            .collect();

        Ok(HTLTOrder {
            from,
            to,
            recipient_other_chain: htlt_order.recipient_other_chain.to_string(),
            sender_other_chain: htlt_order.sender_other_chain.to_string(),
            random_number_hash: htlt_order.random_number_hash.to_vec(),
            timestamp: htlt_order.timestamp,
            amount,
            expected_income: htlt_order.expected_income.to_string(),
            height_span: htlt_order.height_span,
            cross_chain: htlt_order.cross_chain,
        }
        .into_boxed())
    }

    pub fn deposit_htlt_order_from_proto(
        coin: &dyn CoinContext,
        deposit_htlt: &Proto::DepositHTLTOrder<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use crate::transaction::message::htlt_order::DepositHTLTOrder;

        let from = BinanceAddress::from_key_hash_with_coin(coin, deposit_htlt.from.to_vec())?;

        let amount = deposit_htlt
            .amount
            .iter()
            .map(Self::token_from_proto)
            .collect();

        Ok(DepositHTLTOrder {
            from,
            amount,
            swap_id: deposit_htlt.swap_id.to_vec(),
        }
        .into_boxed())
    }

    pub fn claim_htlt_order_from_proto(
        coin: &dyn CoinContext,
        claim_htlt: &Proto::ClaimHTLOrder<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use crate::transaction::message::htlt_order::ClaimHTLTOrder;

        let from = BinanceAddress::from_key_hash_with_coin(coin, claim_htlt.from.to_vec())?;

        Ok(ClaimHTLTOrder {
            from,
            swap_id: claim_htlt.swap_id.to_vec(),
            random_number: claim_htlt.random_number.to_vec(),
        }
        .into_boxed())
    }

    pub fn refund_htlt_order_from_proto(
        coin: &dyn CoinContext,
        refund_htlt: &Proto::RefundHTLTOrder<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use crate::transaction::message::htlt_order::RefundHTLTOrder;

        let from = BinanceAddress::from_key_hash_with_coin(coin, refund_htlt.from.to_vec())?;
        let swap_id = refund_htlt.swap_id.to_vec();

        Ok(RefundHTLTOrder { from, swap_id }.into_boxed())
    }

    pub fn issue_order_from_proto(
        coin: &dyn CoinContext,
        issue_order: &Proto::TokenIssueOrder<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use crate::transaction::message::token_order::TokenIssueOrder;

        let from = BinanceAddress::from_key_hash_with_coin(coin, issue_order.from.to_vec())?;
        Ok(TokenIssueOrder {
            from,
            name: issue_order.name.to_string(),
            symbol: issue_order.symbol.to_string(),
            total_supply: issue_order.total_supply,
            mintable: issue_order.mintable,
        }
        .into_boxed())
    }

    pub fn mint_order_from_proto(
        coin: &dyn CoinContext,
        mint_order: &Proto::TokenMintOrder<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use crate::transaction::message::token_order::TokenMintOrder;

        let from = BinanceAddress::from_key_hash_with_coin(coin, mint_order.from.to_vec())?;
        Ok(TokenMintOrder {
            from,
            symbol: mint_order.symbol.to_string(),
            amount: mint_order.amount,
        }
        .into_boxed())
    }

    pub fn burn_order_from_proto(
        coin: &dyn CoinContext,
        burn_order: &Proto::TokenBurnOrder<'_>,
    ) -> SigningResult<BinanceMessageBox> {
        use crate::transaction::message::token_order::TokenBurnOrder;

        let from = BinanceAddress::from_key_hash_with_coin(coin, burn_order.from.to_vec())?;
        Ok(TokenBurnOrder {
            from,
            symbol: burn_order.symbol.to_string(),
            amount: burn_order.amount,
        }
        .into_boxed())
    }

    fn token_from_proto(token: &Proto::mod_SendOrder::Token) -> Token {
        Token {
            denom: token.denom.to_string(),
            amount: token.amount,
        }
    }
}
