// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::str::FromStr;
use move_core_types::account_address::AccountAddress;
use tw_proto::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload;
use tw_proto::Aptos::Proto::{CancelOfferNftMessage, ClaimNftMessage, NftMessage, OfferNftMessage};

pub struct Offer {
    pub receiver: AccountAddress,
    pub creator: AccountAddress,
    pub collection: Vec<u8>,
    pub name: Vec<u8>,
    pub property_version: u64,
    pub amount: u64
}

pub struct Claim {
    pub sender: AccountAddress,
    pub creator: AccountAddress,
    pub collection: Vec<u8>,
    pub name: Vec<u8>,
    pub property_version: u64,
}

pub enum NftOperation {
    Claim(Claim),
    Offer(Offer),
    Cancel(Offer)
}

impl From<NftMessage<'_>> for NftOperation {
    fn from(value: NftMessage) -> Self {
        match value.nft_transaction_payload {
            OneOfnft_transaction_payload::offer_nft(msg) => { NftOperation::Offer(msg.into()) }
            OneOfnft_transaction_payload::cancel_offer_nft(msg) => { NftOperation::Cancel(msg.into()) }
            OneOfnft_transaction_payload::claim_nft(msg) => { NftOperation::Claim(msg.into()) }
            OneOfnft_transaction_payload::None => { todo!() }
        }
    }
}

impl From<NftOperation> for NftMessage<'_> {
    fn from(value: NftOperation) -> Self {
        match value {
            NftOperation::Claim(claim) => { NftMessage { nft_transaction_payload: OneOfnft_transaction_payload::claim_nft(claim.into()) } }
            NftOperation::Offer(offer) => { NftMessage { nft_transaction_payload: OneOfnft_transaction_payload::offer_nft(offer.into()) } }
            NftOperation::Cancel(cancel) => { NftMessage { nft_transaction_payload: OneOfnft_transaction_payload::cancel_offer_nft(cancel.into()) } }
        }
    }
}
impl From<OfferNftMessage<'_>> for Offer {
    fn from(value: OfferNftMessage) -> Self {
        Offer {
            receiver: AccountAddress::from_str(&value.receiver).unwrap(),
            creator: AccountAddress::from_str(&value.creator).unwrap(),
            collection: value.collectionName.as_bytes().to_vec(),
            name: value.name.as_bytes().to_vec(),
            property_version: value.property_version,
            amount: value.amount,
        }
    }
}

impl From<Offer> for OfferNftMessage<'_> {
    fn from(value: Offer) -> Self {
        OfferNftMessage {
            receiver: value.receiver.to_hex_literal().into(),
            creator: value.creator.to_hex_literal().into(),
            collectionName: String::from_utf8_lossy(value.collection.as_slice()).to_string().into(),
            name: String::from_utf8_lossy(&value.name).to_string().into(),
            property_version: value.property_version,
            amount: value.amount,
        }
    }
}

impl From<CancelOfferNftMessage<'_>> for Offer {
    fn from(value: CancelOfferNftMessage) -> Self {
        Offer {
            receiver: AccountAddress::from_str(&value.receiver).unwrap(),
            creator: AccountAddress::from_str(&value.creator).unwrap(),
            collection: value.collectionName.as_bytes().to_vec(),
            name: value.name.as_bytes().to_vec(),
            property_version: value.property_version,
            amount: 0,
        }
    }
}

impl From<Offer> for CancelOfferNftMessage<'_> {
    fn from(value: Offer) -> Self {
        CancelOfferNftMessage {
            receiver: value.receiver.to_hex_literal().into(),
            creator: value.creator.to_hex_literal().into(),
            collectionName: String::from_utf8_lossy(value.collection.as_slice()).to_string().into(),
            name: String::from_utf8_lossy(value.name.as_slice()).to_string().into(),
            property_version: value.property_version,
        }
    }
}

impl From<ClaimNftMessage<'_>> for Claim {
    fn from(value: ClaimNftMessage) -> Self {
        Claim {
            sender: AccountAddress::from_str(&value.sender).unwrap(),
            creator: AccountAddress::from_str(&value.creator).unwrap(),
            collection: value.collectionName.as_bytes().to_vec(),
            name: value.name.as_bytes().to_vec(),
            property_version: value.property_version,
        }
    }
}

impl From<Claim> for ClaimNftMessage<'_> {
    fn from(value: Claim) -> Self {
        ClaimNftMessage {
            sender: value.sender.to_hex_literal().into(),
            creator: value.creator.to_hex_literal().into(),
            collectionName: String::from_utf8_lossy(value.collection.as_slice()).to_string().into(),
            name: String::from_utf8_lossy(value.name.as_slice()).to_string().into(),
            property_version: value.property_version,
        }
    }
}