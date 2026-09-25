// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::{SubstrateAddress, SubstratePrefix};
use crate::modules::transaction_util::SubstrateTransactionUtil;
use crate::substrate_coin_entry::SubstrateCoinEntry;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::common::compile_input::SingleSignaturePubkey;
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::json_signer::NoJsonSigner;
use tw_coin_entry::modules::message_signer::NoMessageSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::modules::transaction_decoder::NoTransactionDecoder;
use tw_coin_entry::modules::wallet_connector::NoWalletConnector;
use tw_keypair::{ed25519, traits::KeyPairTrait, tw::PublicKey};
use tw_scale::RawOwned;
use tw_ss58_address::{NetworkId, SS58Address};

pub struct SubstrateEntry<T>(pub T);

impl<T: SubstrateCoinEntry> SubstrateEntry<T> {
    #[inline]
    fn sign_impl(
        &self,
        coin: &dyn CoinContext,
        input: T::SigningInput<'_>,
    ) -> SigningResult<RawOwned> {
        let keypair = self.0.get_keypair(coin, &input)?;
        let public_key = Some(keypair.public().clone());
        let unsigned_tx = self.0.build_transaction(coin, public_key, &input)?;
        let signed_tx = unsigned_tx.sign(&keypair)?;
        Ok(RawOwned::new(signed_tx))
    }

    #[inline]
    fn preimage_hashes_impl(
        &self,
        coin: &dyn CoinContext,
        input: T::SigningInput<'_>,
    ) -> SigningResult<RawOwned> {
        let keypair = self.0.get_keypair(coin, &input).ok();
        let public_key = keypair.map(|p| p.public().clone());
        let unsigned_tx = self.0.build_transaction(coin, public_key, &input)?;
        let pre_image = unsigned_tx.encode_payload()?;
        Ok(RawOwned(pre_image))
    }

    #[inline]
    fn compile_impl(
        &self,
        coin: &dyn CoinContext,
        input: T::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<RawOwned> {
        let SingleSignaturePubkey {
            signature,
            public_key,
        } = SingleSignaturePubkey::from_sign_pubkey_list(signatures, public_keys)?;
        let signature = ed25519::Signature::try_from(signature.as_slice())?;
        let public_key = ed25519::sha512::PublicKey::try_from(public_key.as_slice())?;

        let unsigned_tx = self.0.build_transaction(coin, Some(public_key), &input)?;
        let signed_tx = unsigned_tx.into_signed(signature)?;
        Ok(RawOwned::new(signed_tx))
    }
}

impl<T: SubstrateCoinEntry> CoinEntry for SubstrateEntry<T> {
    type AddressPrefix = SubstratePrefix;
    type Address = SubstrateAddress;
    type SigningInput<'a> = T::SigningInput<'a>;
    type SigningOutput = T::SigningOutput;
    type PreSigningOutput = T::PreSigningOutput;

    // Optional modules:
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = NoPlanBuilder;
    type MessageSigner = NoMessageSigner;
    type WalletConnector = NoWalletConnector;
    type TransactionDecoder = NoTransactionDecoder;
    type TransactionUtil = SubstrateTransactionUtil;

    #[inline]
    fn parse_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        let prefix = prefix.or_else(|| coin.ss58_prefix().map(SubstratePrefix::new_unchecked));
        SubstrateAddress::from_str(address)?.with_network_check(prefix)
    }

    #[inline]
    fn parse_address_unchecked(&self, address: &str) -> AddressResult<Self::Address> {
        SubstrateAddress::from_str(address)
    }

    #[inline]
    fn derive_address(
        &self,
        coin: &dyn CoinContext,
        public_key: PublicKey,
        _derivation: Derivation,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        let network = prefix
            .map(SubstratePrefix::network)
            .or_else(|| {
                coin.ss58_prefix()
                    .and_then(|prefix| NetworkId::from_u16(prefix).ok())
            })
            .unwrap_or_default();
        let public_key = public_key
            .to_ed25519()
            .ok_or(AddressError::PublicKeyTypeMismatch)?;

        SS58Address::from_public_key(public_key, network).map(SubstrateAddress)
    }

    #[inline]
    fn sign(&self, coin: &dyn CoinContext, input: Self::SigningInput<'_>) -> Self::SigningOutput {
        let res = self.sign_impl(coin, input);
        self.0.signing_output(coin, res)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        let res = self.preimage_hashes_impl(coin, input);
        self.0.presigning_output(coin, res)
    }

    #[inline]
    fn compile(
        &self,
        coin: &dyn CoinContext,
        input: Self::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        let res = self.compile_impl(coin, input, signatures, public_keys);
        self.0.signing_output(coin, res)
    }

    #[inline]
    fn transaction_util(&self) -> Option<Self::TransactionUtil> {
        Some(SubstrateTransactionUtil)
    }
}
