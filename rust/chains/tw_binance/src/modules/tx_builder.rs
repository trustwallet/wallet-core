// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::BinanceAddress;
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
            OrderEnum::send_order(ref send_order) => Self::send_order_from_proto(coin, send_order),
            OrderEnum::cancel_trade_order(ref cancel_order) => {
                Self::cancel_order_from_proto(coin, cancel_order)
            },
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
        use crate::transaction::message::send_order::{InOut, SendOrder, Token};

        fn in_out_from_proto(
            coin: &dyn CoinContext,
            address_key_hash: &[u8],
            coins: &[Proto::mod_SendOrder::Token],
        ) -> SigningResult<InOut> {
            let address = BinanceAddress::from_key_hash_with_coin(coin, address_key_hash.to_vec())?;
            let coins = coins
                .iter()
                .map(|token| Token {
                    denom: token.denom.to_string(),
                    amount: token.amount,
                })
                .collect();

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
}
