// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::evm_context::EvmContext;
use crate::rlp::buffer::RlpBuffer;
use crate::rlp::list::RlpList;
use crate::rlp::RlpEncode;
use std::borrow::Cow;
use std::marker::PhantomData;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_memory::Data;
use tw_number::U256;
use tw_proto::EthereumRlp::Proto;

/// cbindgen:ignore
pub const RECURSION_LIMIT: usize = 10;

pub struct RlpEncoder<Context: EvmContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: EvmContext> RlpEncoder<Context> {
    pub fn encode<T>(val: &T) -> Data
    where
        T: RlpEncode + ?Sized,
    {
        let mut buf = RlpBuffer::new();
        val.rlp_append(&mut buf);
        buf.finish()
    }

    pub fn encode_with_proto(input: Proto::EncodingInput<'_>) -> Proto::EncodingOutput<'static> {
        Self::encode_with_proto_impl(input)
            .unwrap_or_else(|err| signing_output_error!(Proto::EncodingOutput, err))
    }

    fn encode_with_proto_impl(
        input: Proto::EncodingInput<'_>,
    ) -> SigningResult<Proto::EncodingOutput<'static>> {
        let Some(rlp_item) = input.item else {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No RLP item provided");
        };

        let initial_depth = 0;
        let encoded = Self::encode_proto_item(initial_depth, rlp_item)?;
        Ok(Proto::EncodingOutput {
            encoded: Cow::from(encoded.as_slice().to_vec()),
            ..Proto::EncodingOutput::default()
        })
    }

    fn encode_proto_item(depth: usize, rlp_item: Proto::RlpItem) -> SigningResult<Data> {
        use Proto::mod_RlpItem::OneOfitem as Item;

        if depth >= RECURSION_LIMIT {
            return SigningError::err(SigningErrorType::Error_invalid_params).with_context(|| {
                format!("Allowed complex types with the {RECURSION_LIMIT} maximum depth")
            });
        }

        let encoded_item = match rlp_item.item {
            Item::string_item(str) => RlpEncoder::<Context>::encode(str.as_ref()),
            Item::number_u64(num) => RlpEncoder::<Context>::encode(&U256::from(num)),
            Item::number_u256(num_be) => {
                let num = U256::from_big_endian_slice(num_be.as_ref())
                    .into_tw()
                    .context("Invalid U256 number")?;
                RlpEncoder::<Context>::encode(&num)
            },
            Item::address(addr_s) => {
                let addr = Context::Address::from_str(addr_s.as_ref())?;
                RlpEncoder::<Context>::encode(&addr.into())
            },
            Item::data(data) => RlpEncoder::<Context>::encode(data.as_ref()),
            Item::list(proto_nested_list) => {
                let mut rlp_nested_list = RlpList::new();
                let new_depth = depth + 1;

                for proto_nested_list in proto_nested_list.items {
                    let encoded_item = Self::encode_proto_item(new_depth, proto_nested_list)?;
                    rlp_nested_list.append_raw_encoded(encoded_item.as_slice());
                }
                rlp_nested_list.finish()
            },
            // Pass the `raw_encoded` item as it is.
            Item::raw_encoded(encoded) => encoded.to_vec(),
            Item::None => {
                return SigningError::err(SigningErrorType::Error_invalid_params)
                    .context("No RLP item specified")
            },
        };
        Ok(encoded_item)
    }
}
