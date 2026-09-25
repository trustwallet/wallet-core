// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::from_account_error;
use move_core_types::account_address::AccountAddress;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_proto::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload;
use tw_proto::Aptos::Proto::{CancelOfferNftMessage, ClaimNftMessage, NftMessage, OfferNftMessage};

pub struct Offer {
    pub receiver: AccountAddress,
    pub creator: AccountAddress,
    pub collection: Vec<u8>,
    pub name: Vec<u8>,
    pub property_version: u64,
    pub amount: u64,
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
    Cancel(Offer),
}

impl TryFrom<NftMessage<'_>> for NftOperation {
    type Error = SigningError;

    fn try_from(value: NftMessage) -> SigningResult<Self> {
        match value.nft_transaction_payload {
            OneOfnft_transaction_payload::offer_nft(msg) => {
                Ok(NftOperation::Offer(Offer::try_from(msg)?))
            },
            OneOfnft_transaction_payload::cancel_offer_nft(msg) => {
                Ok(NftOperation::Cancel(Offer::try_from(msg)?))
            },
            OneOfnft_transaction_payload::claim_nft(msg) => {
                Ok(NftOperation::Claim(Claim::try_from(msg)?))
            },
            OneOfnft_transaction_payload::None => {
                SigningError::err(SigningErrorType::Error_invalid_params)
                    .context("No transaction payload provided")
            },
        }
    }
}

impl From<NftOperation> for NftMessage<'_> {
    fn from(value: NftOperation) -> Self {
        match value {
            NftOperation::Claim(claim) => NftMessage {
                nft_transaction_payload: OneOfnft_transaction_payload::claim_nft(claim.into()),
            },
            NftOperation::Offer(offer) => NftMessage {
                nft_transaction_payload: OneOfnft_transaction_payload::offer_nft(offer.into()),
            },
            NftOperation::Cancel(cancel) => NftMessage {
                nft_transaction_payload: OneOfnft_transaction_payload::cancel_offer_nft(
                    cancel.into(),
                ),
            },
        }
    }
}

impl TryFrom<OfferNftMessage<'_>> for Offer {
    type Error = SigningError;

    fn try_from(value: OfferNftMessage) -> SigningResult<Self> {
        Ok(Offer {
            receiver: AccountAddress::from_str(&value.receiver).map_err(from_account_error)?,
            creator: AccountAddress::from_str(&value.creator).map_err(from_account_error)?,
            collection: value.collectionName.as_bytes().to_vec(),
            name: value.name.as_bytes().to_vec(),
            property_version: value.property_version,
            amount: value.amount,
        })
    }
}

impl From<Offer> for OfferNftMessage<'_> {
    fn from(value: Offer) -> Self {
        OfferNftMessage {
            receiver: value.receiver.to_hex_literal().into(),
            creator: value.creator.to_hex_literal().into(),
            collectionName: String::from_utf8_lossy(value.collection.as_slice())
                .to_string()
                .into(),
            name: String::from_utf8_lossy(&value.name).to_string().into(),
            property_version: value.property_version,
            amount: value.amount,
        }
    }
}

impl TryFrom<CancelOfferNftMessage<'_>> for Offer {
    type Error = SigningError;

    fn try_from(value: CancelOfferNftMessage) -> SigningResult<Self> {
        Ok(Offer {
            receiver: AccountAddress::from_str(&value.receiver)
                .map_err(from_account_error)
                .into_tw()
                .context("Invalid receiver address")?,
            creator: AccountAddress::from_str(&value.creator)
                .map_err(from_account_error)
                .into_tw()
                .context("Invalid creator address")?,
            collection: value.collectionName.as_bytes().to_vec(),
            name: value.name.as_bytes().to_vec(),
            property_version: value.property_version,
            amount: 0,
        })
    }
}

impl From<Offer> for CancelOfferNftMessage<'_> {
    fn from(value: Offer) -> Self {
        CancelOfferNftMessage {
            receiver: value.receiver.to_hex_literal().into(),
            creator: value.creator.to_hex_literal().into(),
            collectionName: String::from_utf8_lossy(value.collection.as_slice())
                .to_string()
                .into(),
            name: String::from_utf8_lossy(value.name.as_slice())
                .to_string()
                .into(),
            property_version: value.property_version,
        }
    }
}

impl TryFrom<ClaimNftMessage<'_>> for Claim {
    type Error = SigningError;

    fn try_from(value: ClaimNftMessage) -> SigningResult<Self> {
        Ok(Claim {
            sender: AccountAddress::from_str(&value.sender)
                .map_err(from_account_error)
                .into_tw()
                .context("Invalid sender address")?,
            creator: AccountAddress::from_str(&value.creator)
                .map_err(from_account_error)
                .into_tw()
                .context("Invalid creator address")?,
            collection: value.collectionName.as_bytes().to_vec(),
            name: value.name.as_bytes().to_vec(),
            property_version: value.property_version,
        })
    }
}

impl From<Claim> for ClaimNftMessage<'_> {
    fn from(value: Claim) -> Self {
        ClaimNftMessage {
            sender: value.sender.to_hex_literal().into(),
            creator: value.creator.to_hex_literal().into(),
            collectionName: String::from_utf8_lossy(value.collection.as_slice())
                .to_string()
                .into(),
            name: String::from_utf8_lossy(value.name.as_slice())
                .to_string()
                .into(),
            property_version: value.property_version,
        }
    }
}
