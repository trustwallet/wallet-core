// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
