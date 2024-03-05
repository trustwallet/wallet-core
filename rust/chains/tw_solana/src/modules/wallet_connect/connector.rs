// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::proto_builder::ProtoBuilder;
use crate::modules::wallet_connect::request::SignTransactionRequest;
use crate::transaction::versioned::VersionedTransaction;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::modules::wallet_connector::WalletConnector;
use tw_coin_entry::signing_output_error;
use tw_proto::Solana::Proto;
use tw_proto::WalletConnect::Proto::{
    self as WCProto, mod_ParseRequestOutput::OneOfsigning_input_oneof as SigningInputEnum,
};

pub struct SolanaWalletConnector;

impl WalletConnector for SolanaWalletConnector {
    fn parse_request(
        &self,
        coin: &dyn CoinContext,
        request: WCProto::ParseRequestInput<'_>,
    ) -> WCProto::ParseRequestOutput<'static> {
        Self::parse_request_impl(coin, request)
            .unwrap_or_else(|e| signing_output_error!(WCProto::ParseRequestOutput, e))
    }
}

impl SolanaWalletConnector {
    fn parse_request_impl(
        coin: &dyn CoinContext,
        request: WCProto::ParseRequestInput<'_>,
    ) -> SigningResult<WCProto::ParseRequestOutput<'static>> {
        match request.method {
            WCProto::Method::SolanaSignTransaction => {
                Self::parse_sign_transaction_request(coin, request)
            },
            _ => Err(SigningError(SigningErrorType::Error_not_supported)),
        }
    }

    pub fn parse_sign_transaction_request(
        _coin: &dyn CoinContext,
        request: WCProto::ParseRequestInput<'_>,
    ) -> SigningResult<WCProto::ParseRequestOutput<'static>> {
        let sign_req: SignTransactionRequest = serde_json::from_str(&request.payload)
            .map_err(|_| SigningError(SigningErrorType::Error_input_parse))?;
        let transaction: VersionedTransaction = bincode::deserialize(&sign_req.transaction.0)
            .map_err(|_| SigningError(SigningErrorType::Error_input_parse))?;

        let signing_input = Proto::SigningInput {
            raw_message: Some(ProtoBuilder::build_from_tx(&transaction)?),
            ..Proto::SigningInput::default()
        };

        Ok(WCProto::ParseRequestOutput {
            signing_input_oneof: SigningInputEnum::solana(signing_input),
            ..WCProto::ParseRequestOutput::default()
        })
    }
}
