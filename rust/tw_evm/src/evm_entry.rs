// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_memory::Data;
use tw_proto::{deserialize, serialize, MessageRead, MessageWrite, ProtoResult};

/// An EVM-compatible chain entry.
pub trait EvmEntry {
    type RlpEncodingInput<'a>: MessageRead<'a> + MessageWrite;
    type RlpEncodingOutput: MessageWrite;

    /// Encodes an item or a list of items as Eth RLP binary format.
    fn encode_rlp(input: Self::RlpEncodingInput<'_>) -> Self::RlpEncodingOutput;
}

/// The [`EvmEntry`] trait extension.
pub trait EvmEntryExt {
    /// Encodes an item or a list of items as Eth RLP binary format.
    fn encode_rlp(&self, input: &[u8]) -> ProtoResult<Data>;
}

impl<T> EvmEntryExt for T
where
    T: EvmEntry,
{
    fn encode_rlp(&self, input: &[u8]) -> ProtoResult<Data> {
        let input = deserialize(input)?;
        let output = <Self as EvmEntry>::encode_rlp(input);
        serialize(&output)
    }
}
