// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use itertools::Itertools;
use std::cmp;
use tw_keypair::schnorr;
use tw_utxo::signature::BitcoinSchnorrSignature;

pub struct CovenantCommittee;

impl CovenantCommittee {
    /// Sort the keys in lexicographical order bytes.
    pub fn sort_public_keys(pks: &mut [schnorr::XOnlyPublicKey]) {
        pks.sort_by(Self::cmp_pk);
    }

    /// Sort the signatures by corresponding public keys in lexicographical order bytes.
    pub fn sort_signatures<'a, I>(sigs: I) -> Vec<BitcoinSchnorrSignature>
    where
        I: IntoIterator<Item = &'a (schnorr::XOnlyPublicKey, BitcoinSchnorrSignature)>,
    {
        sigs.into_iter()
            // Sort the elements by public keys.
            .sorted_by(|x, y| Self::cmp_pk(&x.0, &y.0))
            .map(|(_pk, sig)| sig.clone())
            .collect()
    }

    fn cmp_pk(pk1: &schnorr::XOnlyPublicKey, pk2: &schnorr::XOnlyPublicKey) -> cmp::Ordering {
        pk1.as_slice().cmp(pk2.as_slice())
    }
}
