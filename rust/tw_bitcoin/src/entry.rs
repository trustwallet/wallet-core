use crate::modules::compiler::BitcoinCompiler;
use crate::modules::planner::BitcoinPlanner;
use crate::modules::signer::BitcoinSigner;
use bitcoin::address::NetworkChecked;
use std::fmt::Display;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::json_signer::NoJsonSigner;
use tw_coin_entry::modules::message_signer::NoMessageSigner;
use tw_coin_entry::modules::transaction_decoder::NoTransactionDecoder;
use tw_coin_entry::modules::wallet_connector::NoWalletConnector;
use tw_keypair::tw::PublicKey;
use tw_proto::BitcoinV3::Proto;
use tw_utxo::address::standard_bitcoin::{StandardBitcoinAddress, StandardBitcoinPrefix};

pub struct Address(pub bitcoin::address::Address<NetworkChecked>);

impl Display for Address {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{}", self.0)
    }
}

impl CoinAddress for Address {
    fn data(&self) -> tw_memory::Data {
        self.0.to_string().into_bytes()
    }
}

pub struct BitcoinEntry;

impl CoinEntry for BitcoinEntry {
    type AddressPrefix = StandardBitcoinPrefix;
    type Address = StandardBitcoinAddress;
    type SigningInput<'a> = Proto::SigningInput<'a>;
    type SigningOutput = Proto::SigningOutput<'static>;
    type PreSigningOutput = Proto::PreSigningOutput<'static>;

    // Optional modules:
    type JsonSigner = NoJsonSigner;
    type PlanBuilder = BitcoinPlanner;
    type MessageSigner = NoMessageSigner;
    type WalletConnector = NoWalletConnector;
    type TransactionDecoder = NoTransactionDecoder;

    #[inline]
    fn parse_address(
        &self,
        coin: &dyn CoinContext,
        address: &str,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        StandardBitcoinAddress::from_str_with_coin_and_prefix(coin, address, prefix)
    }

    #[inline]
    fn parse_address_unchecked(
        &self,
        _coin: &dyn CoinContext,
        address: &str,
    ) -> AddressResult<Self::Address> {
        StandardBitcoinAddress::from_str(address)
    }

    #[inline]
    fn derive_address(
        &self,
        coin: &dyn CoinContext,
        public_key: PublicKey,
        derivation: Derivation,
        prefix: Option<Self::AddressPrefix>,
    ) -> AddressResult<Self::Address> {
        StandardBitcoinAddress::derive_as_tw(coin, &public_key, derivation, prefix)
    }

    #[inline]
    fn sign(&self, coin: &dyn CoinContext, proto: Self::SigningInput<'_>) -> Self::SigningOutput {
        BitcoinSigner::sign(coin, &proto)
    }

    #[inline]
    fn preimage_hashes(
        &self,
        coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
    ) -> Self::PreSigningOutput {
        BitcoinCompiler::preimage_hashes(coin, proto)
    }

    #[inline]
    fn compile(
        &self,
        coin: &dyn CoinContext,
        proto: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Self::SigningOutput {
        BitcoinCompiler::compile(coin, proto, signatures, public_keys)
    }

    #[inline]
    fn plan_builder(&self) -> Option<Self::PlanBuilder> {
        Some(BitcoinPlanner)
    }
}
