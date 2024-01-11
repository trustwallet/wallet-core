// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::coin_context::CoinContext;
use tw_proto::WalletConnect::Proto as WCProto;

pub trait WalletConnector {
    /// Parses a signing request received through Wallet Connect.
    fn parse_request(
        &self,
        coin: &dyn CoinContext,
        request: WCProto::ParseRequestInput<'_>,
    ) -> WCProto::ParseRequestOutput<'static>;
}

/// `NoWalletConnector` can't be created since there are no enum variants.
pub enum NoWalletConnector {}

impl WalletConnector for NoWalletConnector {
    fn parse_request(
        &self,
        _coin: &dyn CoinContext,
        _request: WCProto::ParseRequestInput<'_>,
    ) -> WCProto::ParseRequestOutput<'static> {
        panic!("`NoWalletConnector` should never be constructed and used")
    }
}
