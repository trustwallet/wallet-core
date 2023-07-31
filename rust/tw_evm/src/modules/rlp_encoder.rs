// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::evm_context::EvmContext;
use crate::rlp::buffer::RlpBuffer;
use crate::rlp::list::RlpList;
use crate::rlp::RlpEncode;
use std::borrow::Cow;
use std::marker::PhantomData;
use std::str::FromStr;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::signing_output_error;
use tw_memory::Data;
use tw_number::{NumberError, U256};
use tw_proto::EthereumRLP::Proto;

pub type RlpResult<T> = Result<T, RlpError>;

pub enum RlpError {
    ErrorDecodingNumber,
}

impl From<NumberError> for RlpError {
    fn from(_: NumberError) -> Self {
        RlpError::ErrorDecodingNumber
    }
}

pub struct RlpEncoder<Context: EvmContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: EvmContext> RlpEncoder<Context> {
    pub fn encode<T>(val: T) -> Data
    where
        T: RlpEncode,
    {
        let mut buf = RlpBuffer::new();
        val.rlp_append(&mut buf);
        buf.finish()
    }

    pub fn encode_with_proto(input: Proto::EncodingInput) -> Proto::EncodingOutput {
        Self::encode_with_proto_impl(input)
            .unwrap_or_else(|err| signing_output_error!(Proto::EncodingOutput, err))
    }

    pub fn encode_with_proto_impl(
        input: Proto::EncodingInput,
    ) -> SigningResult<Proto::EncodingOutput> {
        let Some(rlp_item) = input.item else {
            return Err(SigningError(SigningErrorType::Error_invalid_params));
        };

        let encoded = RlpRecursion::<Context>::default().encode_proto_item(rlp_item)?;
        Ok(Proto::EncodingOutput {
            encoded: Cow::from(encoded.as_slice().to_vec()),
            ..Proto::EncodingOutput::default()
        })
    }
}

struct RlpRecursion<Context: EvmContext> {
    recursion_counter: usize,
    _phantom: PhantomData<Context>,
}

impl<Context: EvmContext> Default for RlpRecursion<Context> {
    fn default() -> Self {
        RlpRecursion {
            recursion_counter: 0,
            _phantom: PhantomData,
        }
    }
}

impl<Context: EvmContext> RlpRecursion<Context> {
    const RECURSION_LIMIT: usize = 5;

    // fn encode_proto_list(&mut self, proto_list: Proto::RlpList) -> SigningResult<Data> {
    //     use Proto::mod_RlpItem::OneOfitem as Item;
    //
    //     if self.recursion_counter >= Self::RECURSION_LIMIT {
    //         return Err(SigningError(SigningErrorType::Error_invalid_params));
    //     }
    //
    //     let mut rlp_list = RlpList::new();
    //     for proto_item in proto_list.items {
    //         match proto_item.item {
    //             Item::string_item(str) => Ok(RlpEncoder::encode(str.as_bytes())),
    //             Item::number_u64(num) => Ok(RlpEncoder::encode(U256::from(num))),
    //             Item::number_u256(num_be) => {
    //                 let num = U256::from_big_endian_slice(num_be.as_slice())?;
    //                 Ok(RlpEncoder::encode(num))
    //             },
    //             Item::address(addr) => {
    //                 let num = Context::Address::from_str(addr.as_ref())?;
    //                 Ok(RlpEncoder::encode(num))
    //             },
    //             Item::data(data) => Ok(RlpEncoder::encode(data.as_ref())),
    //             Item::list(list) => {
    //
    //             },
    //         }
    //     }
    // }

    // fn encode_proto_item(
    //     &mut self,
    //     buffer: &mut RlpBuffer,
    //     rlp_item: Proto::RlpItem,
    // ) -> SigningResult<()> {
    //     use Proto::mod_RlpItem::OneOfitem as Item;
    //
    //     if self.recursion_counter >= Self::RECURSION_LIMIT {
    //         return Err(SigningError(SigningErrorType::Error_invalid_params));
    //     }
    //     match rlp_item.item {
    //         Item::string_item(str) => buffer.append(str.as_ref()),
    //         Item::number_u64(num) => buffer.append(U256::from(num)),
    //         Item::number_u256(num_be) => {
    //             let num = U256::from_big_endian_slice(num_be.as_slice())?;
    //             buffer.append(num)
    //         },
    //         Item::address(addr_s) => {
    //             let addr = Context::Address::from_str(addr_s.as_ref())?;
    //             buffer.append(addr)
    //         },
    //         Item::data(data) => buffer.append_data(data.as_ref()),
    //         Item::list(proto_sublist) => {
    //             let mut rlp_sublist = RlpList::new();
    //             for proto_subitem in proto_sublist.items {
    //                 self.encode_proto_item(rlp_sublist.)
    //             }
    //         },
    //     };
    //
    //     Ok(())
    // }

    fn encode_proto_item(&mut self, rlp_item: Proto::RlpItem) -> SigningResult<Data> {
        use Proto::mod_RlpItem::OneOfitem as Item;

        if self.recursion_counter >= Self::RECURSION_LIMIT {
            return Err(SigningError(SigningErrorType::Error_invalid_params));
        }
        let encoded_item = match rlp_item.item {
            Item::string_item(str) => RlpEncoder::<Context>::encode(str.as_ref()),
            Item::number_u64(num) => RlpEncoder::<Context>::encode(U256::from(num)),
            Item::number_u256(num_be) => {
                let num = U256::from_big_endian_slice(num_be.as_ref())?;
                RlpEncoder::<Context>::encode(num)
            },
            Item::address(addr_s) => {
                let addr = Context::Address::from_str(addr_s.as_ref())?;
                RlpEncoder::<Context>::encode(addr.into())
            },
            Item::data(data) => RlpEncoder::<Context>::encode(data.as_ref()),
            Item::list(proto_sublist) => {
                let mut rlp_sublist = RlpList::new();
                for proto_subitem in proto_sublist.items {
                    let encoded_item = self.encode_proto_item(proto_subitem)?;
                    rlp_sublist.append_raw_encoded(encoded_item.as_slice());
                }
                rlp_sublist.finish()
            },
            Item::None => return Err(SigningError(SigningErrorType::Error_invalid_params)),
        };
        Ok(encoded_item)
    }

    // fn encode_proto_item(&mut self, rlp_item: Proto::RlpItem) -> SigningResult<Data> {
    //     use Proto::mod_RlpItem::OneOfitem as Item;
    //
    //     if self.recursion_counter >= Self::RECURSION_LIMIT {
    //         return Err(SigningError(SigningErrorType::Error_invalid_params));
    //     }
    //     match rlp_item.item {
    //         Item::string_item(str) => Ok(RlpEncoder::encode(str.as_bytes())),
    //         Item::number_u64(num) => Ok(RlpEncoder::encode(U256::from(num))),
    //         Item::number_u256(num_be) => {
    //             let num = U256::from_big_endian_slice(num_be.as_slice())?;
    //             Ok(RlpEncoder::encode(num))
    //         },
    //         Item::address(addr) => {
    //             let num = Context::Address::from_str(addr.as_ref())?;
    //             Ok(RlpEncoder::encode(num))
    //         },
    //         Item::data(data) => Ok(RlpEncoder::encode(data.as_ref())),
    //         Item::list(list) => {
    //
    //         },
    //     }
    // }
}
