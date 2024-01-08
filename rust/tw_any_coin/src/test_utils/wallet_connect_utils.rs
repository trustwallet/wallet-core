// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::ffi::tw_wallet_connect_request::tw_wallet_connect_request_parse;
use std::marker::PhantomData;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::test_utils::tw_data_helper::TWDataHelper;
use tw_memory::Data;
use tw_proto::{
    deserialize, serialize, Common::Proto as CommonProto, MessageRead,
    WalletConnect::Proto as WCProto,
};

#[derive(Default)]
pub struct WalletConnectRequestHelper<'a, SigningInput: MessageRead<'a>> {
    signing_input_data: Data,
    _signing_input_type: PhantomData<&'a SigningInput>,
}

impl<'a, SigningInput: MessageRead<'a>> WalletConnectRequestHelper<'a, SigningInput> {
    pub fn parse(
        &'a mut self,
        coin_type: CoinType,
        input: &WCProto::ParseRequestInput,
    ) -> Result<SigningInput, CommonProto::SigningError> {
        let input_data = TWDataHelper::create(serialize(input).unwrap());

        let output_data = TWDataHelper::wrap(unsafe {
            tw_wallet_connect_request_parse(coin_type as u32, input_data.ptr())
        })
        .to_vec()
        .expect("!tw_wallet_connect_request_parse returned nullptr");

        let output: WCProto::ParseRequestOutput = deserialize(&output_data).unwrap();

        if output.error != CommonProto::SigningError::OK {
            return Err(output.error);
        }

        self.signing_input_data = output.signing_input.to_vec();
        let signing_input: SigningInput = deserialize(&self.signing_input_data).unwrap();
        Ok(signing_input)
    }
}
