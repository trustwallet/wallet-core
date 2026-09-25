// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use itertools::Itertools;
use std::collections::BTreeMap;
use tw_coin_entry::error::prelude::*;
use tw_keypair::schnorr;
use tw_utxo::signature::BitcoinSchnorrSignature;

type OptionalSignature = Option<BitcoinSchnorrSignature>;
type PubkeySigMap = BTreeMap<schnorr::XOnlyPublicKey, OptionalSignature>;

pub struct MultiSigOrderedKeys {
    pks: Vec<schnorr::XOnlyPublicKey>,
    quorum: u32,
}

impl MultiSigOrderedKeys {
    /// Sort the keys in lexicographical order.
    pub fn new(mut pks: Vec<schnorr::XOnlyPublicKey>, quorum: u32) -> SigningResult<Self> {
        if pks.is_empty() {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No public keys provided");
        }

        if pks.len() < quorum as usize {
            return SigningError::err(SigningErrorType::Error_invalid_params).context(
                "Required number of valid signers is greater than number of provided keys",
            );
        }

        // TODO it's not optimal to use a `HashSet` because the keys are sorted already.
        if !pks.iter().all_unique() {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Public keys must be unique");
        }

        pks.sort();
        Ok(MultiSigOrderedKeys { pks, quorum })
    }

    pub fn public_keys_ordered(&self) -> &[schnorr::XOnlyPublicKey] {
        &self.pks
    }

    pub fn quorum(&self) -> u32 {
        self.quorum
    }

    pub fn with_partial_signatures<'a, I>(self, sigs: I) -> SigningResult<MultiSigOrdered>
    where
        I: IntoIterator<Item = &'a (schnorr::XOnlyPublicKey, BitcoinSchnorrSignature)>,
    {
        let mut pk_sig_map = MultiSigOrdered::checked(self.pks, self.quorum);
        pk_sig_map.set_partial_signatures(sigs)?;
        pk_sig_map.check_quorum()?;
        Ok(pk_sig_map)
    }
}

#[derive(Clone, Debug)]
pub struct MultiSigOrdered {
    pk_sig_map: PubkeySigMap,
    quorum: u32,
}

impl MultiSigOrdered {
    /// `pk_sig_map` and `quorum` must be checked at [`MultiSigOrderedKeys::new`] already.
    fn checked(pks: Vec<schnorr::XOnlyPublicKey>, quorum: u32) -> Self {
        let mut pk_sig_map = PubkeySigMap::new();

        // Initialize the map with public keys and null signatures first.
        for pk in pks {
            pk_sig_map.insert(pk, None);
        }

        MultiSigOrdered { pk_sig_map, quorum }
    }

    fn set_partial_signatures<'a, I>(&mut self, sigs: I) -> SigningResult<()>
    where
        I: IntoIterator<Item = &'a (schnorr::XOnlyPublicKey, BitcoinSchnorrSignature)>,
    {
        // Set the signatures for the specific public keys.
        // There can be less signatures than public keys, but not less than `quorum`.
        for (pk, sig) in sigs {
            // Find the signature of the corresponding public key.
            let pk_sig = self
                .pk_sig_map
                .get_mut(pk)
                .or_tw_err(SigningErrorType::Error_invalid_params)
                .context("Signature provided for an unknown public key")?;

            // Only one signature per public key allowed.
            if pk_sig.is_some() {
                return SigningError::err(SigningErrorType::Error_invalid_params)
                    .context("Duplicate public key");
            }
            *pk_sig = Some(sig.clone());
        }
        Ok(())
    }

    fn check_quorum(&self) -> SigningResult<()> {
        let sig_num = self.pk_sig_map.values().filter(|sig| sig.is_some()).count();
        if sig_num < self.quorum as usize {
            return SigningError::err(SigningErrorType::Error_invalid_params).context(format!(
                "Number of signatures '{sig_num}' is less than quorum '{}'",
                self.quorum
            ));
        }
        Ok(())
    }

    /// Get signatures sorted by corresponding public keys in reverse lexicographical order
    /// because the script interpreter will pop the left-most byte-array as the first stack element,
    /// the second-left-most byte array as the second stack element, and so on.
    ///
    /// In other words,
    /// `[SigN] [SigN-1] ... [Sig0]`
    /// where the list `Sig0 ... SigN` are the Schnorr signatures corresponding to the public keys `Pk0 ... PkN`.
    ///
    /// https://gnusha.org/pi/bitcoindev/20220820134850.ofvz7225zwcyffit@artanis (=== Spending K-of-N Multisig outputs ===)
    pub fn get_signatures_reverse_order(&self) -> Vec<OptionalSignature> {
        self.pk_sig_map
            .iter()
            .rev()
            .map(|(_pk, sig)| sig.clone())
            .collect()
    }
}
