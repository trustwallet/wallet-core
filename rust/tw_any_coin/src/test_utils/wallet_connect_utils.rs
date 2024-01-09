// Copyright © 2017-2024 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ffi::tw_wallet_connect_request::tw_wallet_connect_request_parse;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::Data;
use tw_proto::{deserialize, serialize, WalletConnect::Proto as WCProto};

#[derive(Default)]
pub struct WalletConnectRequestHelper {
    output_data: Data,
}

impl WalletConnectRequestHelper {
    pub fn parse<'a>(
        &'a mut self,
        coin_type: CoinType,
        input: &WCProto::ParseRequestInput,
    ) -> WCProto::ParseRequestOutput<'a> {
        let input_data = TWDataHelper::create(serialize(input).unwrap());

        self.output_data = TWDataHelper::wrap(unsafe {
            tw_wallet_connect_request_parse(coin_type as u32, input_data.ptr())
        })
        .to_vec()
        .expect("!tw_wallet_connect_request_parse returned nullptr");

        deserialize(&self.output_data).unwrap()
    }
}
