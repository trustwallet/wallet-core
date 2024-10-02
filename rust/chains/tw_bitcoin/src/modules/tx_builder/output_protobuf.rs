// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::tx_builder::BitcoinChainInfo;
use std::marker::PhantomData;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_hash::hasher::sha256_ripemd;
use tw_hash::sha2::sha256;
use tw_hash::{Hash, H256};
use tw_keypair::{ecdsa, schnorr};
use tw_memory::Data;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::context::UtxoContext;
use tw_utxo::script::Script;
use tw_utxo::transaction::standard_transaction::builder::OutputBuilder;
use tw_utxo::transaction::standard_transaction::TransactionOutput;

pub struct OutputProtobuf<'a, Context: UtxoContext> {
    chain_info: &'a BitcoinChainInfo,
    output: &'a Proto::Output<'a>,
    _phantom: PhantomData<Context>,
}

impl<'a, Context: UtxoContext> OutputProtobuf<'a, Context> {
    pub fn new(chain_info: &'a BitcoinChainInfo, output: &'a Proto::Output<'a>) -> Self {
        OutputProtobuf {
            chain_info,
            output,
            _phantom: PhantomData,
        }
    }

    pub fn output_from_proto(self) -> SigningResult<TransactionOutput> {
        use Proto::mod_Output::mod_OutputBuilder::OneOfvariant as BuilderType;
        use Proto::mod_Output::OneOfto_recipient as RecipientType;

        match self.output.to_recipient {
            RecipientType::builder(ref builder) => match builder.variant {
                BuilderType::p2sh(ref redeem) => self.p2sh(redeem),
                BuilderType::p2pk(ref pubkey) => self.p2pk(pubkey),
                BuilderType::p2pkh(ref pubkey_or_hash) => self.p2pkh(pubkey_or_hash),
                BuilderType::p2wsh(ref redeem) => self.p2wsh(redeem),
                BuilderType::p2wpkh(ref pubkey_or_hash) => self.p2wpkh(pubkey_or_hash),
                BuilderType::p2tr_key_path(ref key_path) => self.p2tr_key_path(key_path),
                BuilderType::p2tr_script_path(ref script) => self.p2tr_script_path(script),
                BuilderType::p2tr_dangerous_assume_tweaked(ref pubkey) => {
                    self.p2tr_dangerous_assume_tweaked(pubkey)
                },
                BuilderType::brc20_inscribe(ref inscription) => self.brc20_inscribe(inscription),
                BuilderType::op_return(ref data) => self.op_return(data),
                BuilderType::None => SigningError::err(SigningErrorType::Error_invalid_params)
                    .context("No Output Builder type provided"),
            },
            RecipientType::custom_script_pubkey(ref script) => self.custom_script(script.to_vec()),
            RecipientType::to_address(ref address) => self.recipient_address(address),
            RecipientType::None => SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No Output recipient type provided"),
        }
    }

    pub fn p2sh(
        &self,
        redeem: &Proto::mod_Output::RedeemScriptOrHash,
    ) -> SigningResult<TransactionOutput> {
        let redeem_hash =
            Self::redeem_hash_from_proto(redeem, sha256_ripemd).context("P2SH builder")?;
        Ok(self.prepare_builder()?.p2sh_from_hash(&redeem_hash))
    }

    pub fn p2pk(&self, pubkey: &[u8]) -> SigningResult<TransactionOutput> {
        let pubkey = ecdsa::secp256k1::PublicKey::try_from(pubkey)
            .into_tw()
            .context("Invalid P2PK public key")?;
        Ok(self.prepare_builder()?.p2pk(&pubkey))
    }

    pub fn p2pkh(
        &self,
        pubkey_or_hash: &Proto::PublicKeyOrHash,
    ) -> SigningResult<TransactionOutput> {
        let pubkey_hash = Self::pubkey_hash_from_proto(pubkey_or_hash).context("P2PKH builder")?;
        Ok(self.prepare_builder()?.p2pkh_from_hash(&pubkey_hash))
    }

    pub fn p2wsh(
        &self,
        redeem: &Proto::mod_Output::RedeemScriptOrHash,
    ) -> SigningResult<TransactionOutput> {
        let redeem_hash = Self::redeem_hash_from_proto(redeem, sha256).context("P2WSH builder")?;
        Ok(self.prepare_builder()?.p2wsh_from_hash(&redeem_hash))
    }

    pub fn p2wpkh(
        &self,
        pubkey_or_hash: &Proto::PublicKeyOrHash,
    ) -> SigningResult<TransactionOutput> {
        let pubkey_hash = Self::pubkey_hash_from_proto(pubkey_or_hash).context("P2WPKH builder")?;
        Ok(self.prepare_builder()?.p2wpkh_from_hash(&pubkey_hash))
    }

    pub fn p2tr_key_path(&self, taproot_pubkey: &[u8]) -> SigningResult<TransactionOutput> {
        let public_key = schnorr::PublicKey::try_from(taproot_pubkey)
            .into_tw()
            .context("Invalid P2TR key path. Must be a schnorr public key")?;
        Ok(self.prepare_builder()?.p2tr_key_path(&public_key))
    }

    pub fn p2tr_dangerous_assume_tweaked(
        &self,
        tweaked_pubkey: &[u8],
    ) -> SigningResult<TransactionOutput> {
        let tweaked_x_only = H256::try_from(tweaked_pubkey)
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .context("Invalid P2TR tweaked public key. Expected 32 bytes x-only public key")?;
        Ok(self
            .prepare_builder()?
            .p2tr_dangerous_assume_tweaked(&tweaked_x_only))
    }

    pub fn p2tr_script_path(
        &self,
        taproot_script_path: &Proto::mod_Output::OutputTaprootScriptPath,
    ) -> SigningResult<TransactionOutput> {
        let public_key = schnorr::PublicKey::try_from(taproot_script_path.internal_key.as_ref())
            .into_tw()
            .context(
                "Invalid OutputTaprootScriptPath.internal_key. Must be a schnorr public key",
            )?;

        let merkle_root = H256::try_from(taproot_script_path.merkle_root.as_ref())
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .context("Invalid OutputTaprootScriptPath.merkle_root. Must be a 32 byte array")?;

        Ok(self
            .prepare_builder()?
            .p2tr_script_path(&public_key, merkle_root))
    }

    pub fn brc20_inscribe(
        &self,
        inscription: &Proto::mod_Output::OutputBrc20Inscription,
    ) -> SigningResult<TransactionOutput> {
        let public_key = schnorr::PublicKey::try_from(inscription.inscribe_to.as_ref())?;
        self.prepare_builder()?.brc20_transfer(
            &public_key,
            inscription.ticker.to_string(),
            inscription.transfer_amount.to_string(),
        )
    }

    pub fn custom_script(&self, script_data: Data) -> SigningResult<TransactionOutput> {
        let script = Script::from(script_data);
        Ok(self.prepare_builder()?.custom_script_pubkey(script))
    }

    pub fn recipient_address(&self, addr_str: &str) -> SigningResult<TransactionOutput> {
        let addr = Context::Address::from_str(addr_str)
            .into_tw()
            .context("Invalid recipient address")?;
        let claiming_script_pubkey =
            Context::addr_to_script_pubkey(&addr, self.chain_info.to_address_prefixes())?;
        self.custom_script(claiming_script_pubkey.into())
            .with_context(|| format!("Error handling {addr_str} output address"))
    }

    pub fn op_return(&self, op_return_data: &[u8]) -> SigningResult<TransactionOutput> {
        self.prepare_builder()?.op_return(op_return_data)
    }

    /// Tries to convert [`Proto::RedeemScriptOrHash`] to [`Hash<N>`] using a specific `hasher` function.
    /// Please note `P2SH` and `P2WSH` use different hashing functions.
    pub fn redeem_hash_from_proto<const N: usize, F>(
        input: &Proto::mod_Output::RedeemScriptOrHash,
        hasher: F,
    ) -> SigningResult<Hash<N>>
    where
        F: FnOnce(&[u8]) -> Data,
    {
        use Proto::mod_Output::mod_RedeemScriptOrHash::OneOfvariant as RedeemOrHashType;

        let hash_data = match input.variant {
            RedeemOrHashType::redeem_script(ref redeem) => hasher(redeem.as_ref()),
            RedeemOrHashType::hash(ref hash) => hash.to_vec(),
            RedeemOrHashType::None => {
                return SigningError::err(SigningErrorType::Error_invalid_params)
                    .context("Expected a redeem script or its hash")
            },
        };
        Hash::<N>::try_from(hash_data.as_slice())
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .with_context(|| format!("Expected exactly {N} bytes redeem script hash"))
    }

    pub fn prepare_builder(&self) -> SigningResult<OutputBuilder> {
        if self.output.value < 0 {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Transaction Output amount cannot be negative");
        }
        Ok(OutputBuilder::new(self.output.value))
    }

    /// Tries to convert [`Proto::PublicKeyOrHash`] to [`Hash<N>`].
    /// Please note `P2PKH` and `P2WPKH` use the same `ripemd(sha256(x))` hash function.
    pub fn pubkey_hash_from_proto<const N: usize>(
        input: &Proto::PublicKeyOrHash,
    ) -> SigningResult<Hash<N>> {
        use Proto::mod_PublicKeyOrHash::OneOfvariant as PublicKeyOrHashType;

        let hash_data = match input.variant {
            PublicKeyOrHashType::pubkey(ref pubkey) => sha256_ripemd(pubkey.as_ref()),
            PublicKeyOrHashType::hash(ref hash) => hash.to_vec(),
            PublicKeyOrHashType::None => {
                return SigningError::err(SigningErrorType::Error_invalid_params)
                    .context("Expected a public key or its hash")
            },
        };
        Hash::<N>::try_from(hash_data.as_slice())
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .with_context(|| format!("Expected exactly {N} bytes public key hash"))
    }
}
