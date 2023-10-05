// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![no_main]

use libfuzzer_sys::{arbitrary, fuzz_target};
use tw_evm::evm_context::StandardEvmContext;
use tw_evm::modules::rlp_encoder::RlpEncoder;
use tw_proto::EthereumRlp::Proto;

#[derive(arbitrary::Arbitrary, Debug)]
enum RlpItem<'a> {
    String(&'a str),
    U64(u64),
    U256(&'a [u8]),
    Address(&'a str),
    Data(&'a [u8]),
    List(Vec<RlpItem<'a>>),
    RawEncoded(&'a [u8]),
    None,
}

impl<'a> From<RlpItem<'a>> for Proto::RlpItem<'a> {
    fn from(item: RlpItem<'a>) -> Self {
        use Proto::mod_RlpItem::OneOfitem as ProtoEnum;

        let one_of_item = match item {
            RlpItem::String(s) => ProtoEnum::string_item(s.into()),
            RlpItem::U64(u) => ProtoEnum::number_u64(u),
            RlpItem::U256(data) => ProtoEnum::number_u256(data.into()),
            RlpItem::Address(addr) => ProtoEnum::address(addr.into()),
            RlpItem::Data(data) => ProtoEnum::data(data.into()),
            RlpItem::List(items) => {
                let items = items.into_iter().map(Proto::RlpItem::from).collect();
                ProtoEnum::list(Proto::RlpList { items })
            },
            RlpItem::RawEncoded(data) => ProtoEnum::raw_encoded(data.into()),
            RlpItem::None => ProtoEnum::None,
        };
        Proto::RlpItem { item: one_of_item }
    }
}

fuzz_target!(|item: RlpItem<'_>| {
    let input = Proto::EncodingInput {
        item: Some(item.into()),
    };
    let _ = RlpEncoder::<StandardEvmContext>::encode_with_proto(input);
});
