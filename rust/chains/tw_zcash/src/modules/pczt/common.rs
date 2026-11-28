// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use serde::{Deserialize, Serialize};
use std::collections::BTreeMap;

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct Global {
    //
    // Transaction effecting data.
    //
    // These are required fields that are part of the final transaction, and are filled in
    // by the Creator when initializing the PCZT.
    //
    pub tx_version: u32,

    pub version_group_id: u32,

    /// The consensus branch ID for the chain in which this transaction will be mined.
    ///
    /// Non-optional because this commits to the set of consensus rules that will apply to
    /// the transaction; differences therein can affect every role.
    pub consensus_branch_id: u32,

    /// The transaction locktime to use if no inputs specify a required locktime.
    ///
    /// - This is set by the Creator.
    /// - If omitted, the fallback locktime is assumed to be 0.
    pub fallback_lock_time: Option<u32>,

    pub expiry_height: u32,

    /// The [SLIP 44] coin type, indicating the network for which this transaction is
    /// being constructed.
    ///
    /// This is technically information that could be determined indirectly from the
    /// `consensus_branch_id` but is included explicitly to enable easy identification.
    /// Note that this field is not included in the transaction and has no consensus
    /// effect (`consensus_branch_id` fills that role).
    ///
    /// - This is set by the Creator.
    /// - Roles that encode network-specific information (for example, derivation paths
    ///   for key identification) should check against this field for correctness.
    ///
    /// [SLIP 44]: https://github.com/satoshilabs/slips/blob/master/slip-0044.md
    pub coin_type: u32,

    /// A bitfield for various transaction modification flags.
    ///
    /// - Bit 0 is the Transparent Inputs Modifiable Flag and indicates whether
    ///   transparent inputs can be modified.
    ///   - This is set to `true` by the Creator.
    ///   - This is checked by the Constructor before adding transparent inputs, and may
    ///     be set to `false` by the Constructor.
    ///   - This is set to `false` by the IO Finalizer if there are shielded spends or
    ///     outputs.
    ///   - This is set to `false` by a Signer that adds a signature that does not use
    ///     `SIGHASH_ANYONECANPAY` (which includes all shielded signatures).
    ///   - The Combiner merges this bit towards `false`.
    /// - Bit 1 is the Transparent Outputs Modifiable Flag and indicates whether
    ///   transparent outputs can be modified.
    ///   - This is set to `true` by the Creator.
    ///   - This is checked by the Constructor before adding transparent outputs, and may
    ///     be set to `false` by the Constructor.
    ///   - This is set to `false` by the IO Finalizer if there are shielded spends or
    ///     outputs.
    ///   - This is set to `false` by a Signer that adds a signature that does not use
    ///     `SIGHASH_NONE` (which includes all shielded signatures).
    ///   - The Combiner merges this bit towards `false`.
    /// - Bit 2 is the Has `SIGHASH_SINGLE` Flag and indicates whether the transaction has
    ///   a `SIGHASH_SINGLE` transparent signature who's input and output pairing must be
    ///   preserved.
    ///   - This is set to `false` by the Creator.
    ///   - This is updated by a Constructor.
    ///   - This is set to `true` by a Signer that adds a signature that uses
    ///     `SIGHASH_SINGLE`.
    ///   - This essentially indicates that the Constructor must iterate the transparent
    ///     inputs to determine whether and how to add a transparent input.
    ///   - The Combiner merges this bit towards `true`.
    /// - Bits 3-6 must be 0.
    /// - Bit 7 is the Shielded Modifiable Flag and indicates whether shielded spends or
    ///   outputs can be modified.
    ///   - This is set to `true` by the Creator.
    ///   - This is checked by the Constructor before adding shielded spends or outputs,
    ///     and may be set to `false` by the Constructor.
    ///   - This is set to `false` by the IO Finalizer if there are shielded spends or
    ///     outputs.
    ///   - This is set to `false` by every Signer (as all signatures commit to all
    ///     shielded spends and outputs).
    ///   - The Combiner merges this bit towards `false`.
    pub tx_modifiable: u8,

    /// Proprietary fields related to the overall transaction.
    pub proprietary: BTreeMap<String, Vec<u8>>,
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct Zip32Derivation {
    /// The [ZIP 32 seed fingerprint](https://zips.z.cash/zip-0032#seed-fingerprints).
    pub seed_fingerprint: [u8; 32],

    /// The sequence of indices corresponding to the shielded HD path.
    ///
    /// Indices can be hardened or non-hardened (i.e. the hardened flag bit may be set).
    /// When used with a Sapling or Orchard spend, the derivation path will generally be
    /// entirely hardened; when used with a transparent spend, the derivation path will
    /// generally include a non-hardened section matching either the [BIP 44] path, or the
    /// path at which ephemeral addresses are derived for [ZIP 320] transactions.
    ///
    /// [BIP 44]: https://github.com/bitcoin/bips/blob/master/bip-0044.mediawiki
    /// [ZIP 320]: https://zips.z.cash/zip-0320
    pub derivation_path: Vec<u32>,
}

/// Determines the lock time for the transaction.
///
/// Based on `pczt::common::determine_lock_time`, following [BIP 370]:
/// inputs that specify both lock time types support either of them.
/// Returns `None` if the inputs require incompatible lock time types.
///
/// [BIP 370]: https://github.com/bitcoin/bips/blob/master/bip-0370.mediawiki#determining-lock-time
pub fn determine_lock_time(
    global: &Global,
    inputs: &[crate::modules::pczt::transparent::Input],
) -> Option<u32> {
    let have_required_lock_time = inputs.iter().any(|input| {
        input.required_time_lock_time.is_some() || input.required_height_lock_time.is_some()
    });
    // An input that specifies only one lock time type doesn't support the other one.
    let time_lock_time_unsupported = inputs.iter().any(|input| {
        input.required_height_lock_time.is_some() && input.required_time_lock_time.is_none()
    });
    let height_lock_time_unsupported = inputs.iter().any(|input| {
        input.required_time_lock_time.is_some() && input.required_height_lock_time.is_none()
    });

    match (
        have_required_lock_time,
        time_lock_time_unsupported,
        height_lock_time_unsupported,
    ) {
        (true, true, true) => None,
        (true, false, true) => inputs
            .iter()
            .filter_map(|input| input.required_time_lock_time)
            .max(),
        // If both lock time types are possible, the height lock time must be chosen.
        (true, _, false) => inputs
            .iter()
            .filter_map(|input| input.required_height_lock_time)
            .max(),
        // Fallback lock time is assumed to be 0 if not provided.
        (false, _, _) => Some(global.fallback_lock_time.unwrap_or(0)),
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::modules::pczt::transparent::Input;

    fn global(fallback_lock_time: Option<u32>) -> Global {
        Global {
            tx_version: 5,
            version_group_id: 0x26A7270A,
            consensus_branch_id: 0xC8E71055,
            fallback_lock_time,
            expiry_height: 0,
            coin_type: 133,
            tx_modifiable: 0,
            proprietary: BTreeMap::default(),
        }
    }

    fn input(time: Option<u32>, height: Option<u32>) -> Input {
        Input {
            prevout_txid: [0; 32],
            prevout_index: 0,
            sequence: None,
            required_time_lock_time: time,
            required_height_lock_time: height,
            script_sig: None,
            value: 0,
            script_pubkey: Vec::default(),
            redeem_script: None,
            partial_signatures: BTreeMap::default(),
            sighash_type: 1,
            bip32_derivation: BTreeMap::default(),
            ripemd160_preimages: BTreeMap::default(),
            sha256_preimages: BTreeMap::default(),
            hash160_preimages: BTreeMap::default(),
            hash256_preimages: BTreeMap::default(),
            proprietary: BTreeMap::default(),
        }
    }

    #[test]
    fn test_determine_lock_time() {
        // Fallback lock time is used if no input requires a lock time.
        assert_eq!(
            determine_lock_time(&global(None), &[input(None, None)]),
            Some(0)
        );
        assert_eq!(
            determine_lock_time(&global(Some(100)), &[input(None, None)]),
            Some(100)
        );
        // The maximum required lock time of the chosen type is used.
        let inputs = [
            input(None, Some(10)),
            input(None, Some(20)),
            input(None, None),
        ];
        assert_eq!(determine_lock_time(&global(Some(100)), &inputs), Some(20));
        let inputs = [
            input(Some(600_000_000), None),
            input(Some(500_000_001), None),
        ];
        assert_eq!(
            determine_lock_time(&global(None), &inputs),
            Some(600_000_000)
        );
        // Incompatible lock time types.
        let inputs = [input(Some(600_000_000), None), input(None, Some(10))];
        assert_eq!(determine_lock_time(&global(None), &inputs), None);
        // An input with both lock time types supports either of them, height is preferred.
        let inputs = [input(Some(600_000_000), Some(10))];
        assert_eq!(determine_lock_time(&global(None), &inputs), Some(10));
        let inputs = [input(Some(600_000_000), Some(10)), input(None, Some(20))];
        assert_eq!(determine_lock_time(&global(None), &inputs), Some(20));
        let inputs = [
            input(Some(600_000_000), Some(10)),
            input(Some(700_000_000), None),
        ];
        assert_eq!(
            determine_lock_time(&global(None), &inputs),
            Some(700_000_000)
        );
    }
}
