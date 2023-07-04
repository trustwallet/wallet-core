// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::Address;
use std::str::FromStr;
use tw_coin_entry::{AddressError, CoinContext, CoinEntry, Derivation, NoPrefix};
use tw_keypair::tw::PublicKey;
use tw_proto::Common::Proto::SigningError;
use tw_proto::Ethereum::Proto;

pub struct EthereumEntry;

impl CoinEntry for EthereumEntry {
    type AddressPrefix = NoPrefix;
    type Address = Address;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;

    fn parse_address(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
        _prefix: Option<Self::AddressPrefix>,
    ) -> Result<Self::Address, AddressError> {
        Address::from_str(address)
    }

    fn derive_address(
        &self,
        _coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        _prefix: Option<Self::AddressPrefix>,
    ) -> Result<Self::Address, AddressError> {
        let public_key = public_key
            .to_secp256k1()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;
        Ok(Address::with_secp256k1_pubkey(public_key))
    }

    fn sign(&self, _coin: &dyn CoinContext, _input: Self::SigningInput<'_>) -> Self::SigningOutput {
        Proto::SigningOutput {
            error: SigningError::Error_internal,
            error_message: "Not supported yet".into(),
            ..Proto::SigningOutput::default()
        }
    }
}
