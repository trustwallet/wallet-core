// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::str::FromStr;
use move_core_types::account_address::AccountAddress;
use tw_proto::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload;
use tw_proto::Aptos::Proto::{NftMessage, OfferNftMessage};

pub struct NftOffer {
    pub receiver: AccountAddress,
    pub creator: AccountAddress,
    pub collection: Vec<u8>,
    pub name: Vec<u8>,
    pub property_version: u64,
    pub amount: u64
}

pub enum NftOperation {
    Claim,
    Offer(NftOffer)
}

impl From<NftMessage<'_>> for NftOperation {
    fn from(value: NftMessage) -> Self {
        match value.nft_transaction_payload {
            OneOfnft_transaction_payload::offer_nft(msg) => { NftOperation::Offer(msg.into()) }
            OneOfnft_transaction_payload::cancel_offer_nft(_) => { todo!() }
            OneOfnft_transaction_payload::claim_nft(_) => { todo!() }
            OneOfnft_transaction_payload::None => { todo!() }
        }
    }
}

impl From<OfferNftMessage<'_>> for NftOffer {
    fn from(value: OfferNftMessage) -> Self {
        NftOffer {
            receiver: AccountAddress::from_str(&value.receiver).unwrap(),
            creator: AccountAddress::from_str(&value.creator).unwrap(),
            collection: value.collectionName.as_bytes().to_vec(),
            name: value.name.as_bytes().to_vec(),
            property_version: value.property_version,
            amount: value.amount,
        }
    }
}