// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

//! ZIP-244 transaction identifier and signature digests for v5 transparent-only transactions.
//!
//! Ported from `zcash_primitives::transaction::{txid, sighash_v5}`.
//! Sapling and Orchard bundles are always empty, so their digests are constant.
//!
//! https://zips.z.cash/zip-0244

use crate::transaction::ZcashTransaction;
use tw_coin_entry::error::prelude::*;
use tw_hash::blake2::blake2_b_personal;
use tw_hash::{H128, H256, H32};
use tw_utxo::encode::stream::Stream;
use tw_utxo::sighash::SighashBase;
use tw_utxo::transaction::standard_transaction::TransactionOutput;
use tw_utxo::transaction::UtxoPreimageArgs;

const TX_PERSONALIZATION_PREFIX: &[u8; 12] = b"ZcashTxHash_";
const HEADERS_HASH_PERSONALIZATION: &[u8; 16] = b"ZTxIdHeadersHash";
const TRANSPARENT_HASH_PERSONALIZATION: &[u8; 16] = b"ZTxIdTranspaHash";
const SAPLING_HASH_PERSONALIZATION: &[u8; 16] = b"ZTxIdSaplingHash";
const ORCHARD_HASH_PERSONALIZATION: &[u8; 16] = b"ZTxIdOrchardHash";
const PREVOUTS_HASH_PERSONALIZATION: &[u8; 16] = b"ZTxIdPrevoutHash";
const SEQUENCE_HASH_PERSONALIZATION: &[u8; 16] = b"ZTxIdSequencHash";
const OUTPUTS_HASH_PERSONALIZATION: &[u8; 16] = b"ZTxIdOutputsHash";
const TRANSPARENT_INPUT_HASH_PERSONALIZATION: &[u8; 16] = b"Zcash___TxInHash";
const TRANSPARENT_AMOUNTS_HASH_PERSONALIZATION: &[u8; 16] = b"ZTxTrAmountsHash";
const TRANSPARENT_SCRIPTS_HASH_PERSONALIZATION: &[u8; 16] = b"ZTxTrScriptsHash";

/// Raw sighash types allowed by ZIP-244 (other values are consensus-invalid in v5).
const VALID_SIGHASH_TYPES: [u32; 6] = [0x01, 0x02, 0x03, 0x81, 0x82, 0x83];

/// ZIP-244 digests shared by all transparent inputs of a transaction.
/// Computed once per transaction so that signing `N` inputs costs `O(N)` hashing.
pub struct ZcashSighashV5 {
    header: H256,
    prevouts: H256,
    sequence: H256,
    outputs: H256,
    amounts: H256,
    scripts: H256,
    sapling: H256,
    orchard: H256,
}

impl ZcashSighashV5 {
    /// ZIP-244 transaction identifier in the internal byte order.
    pub fn txid_digest(tx: &ZcashTransaction) -> H256 {
        let transparent_digest =
            if tx.transparent_inputs.is_empty() && tx.transparent_outputs.is_empty() {
                hash(TRANSPARENT_HASH_PERSONALIZATION, &[])
            } else {
                let mut stream = Stream::default();
                stream
                    .append(&prevouts_digest(tx))
                    .append(&sequence_digest(tx))
                    .append(&outputs_digest(&tx.transparent_outputs));
                hash(TRANSPARENT_HASH_PERSONALIZATION, &stream.out())
            };
        root_hash(
            &tx.branch_id,
            &header_digest(tx),
            &transparent_digest,
            &hash(SAPLING_HASH_PERSONALIZATION, &[]),
            &hash(ORCHARD_HASH_PERSONALIZATION, &[]),
        )
    }

    /// ZIP-244 signature digest for the transparent input `args.input_index`.
    pub fn sighash_tx(tx: &ZcashTransaction, args: &UtxoPreimageArgs) -> SigningResult<H256> {
        Self::new(tx, args)?.sighash_input(tx, args)
    }

    /// ZIP-244 signature digests for all transparent inputs, in the same order as `args`.
    ///
    /// All `args` must share the same `spent_amounts` and `spent_script_pubkeys`,
    /// which holds for transparent Zcash inputs (there are no Taproot reveal scripts).
    pub fn sighash_all(
        tx: &ZcashTransaction,
        args: &[UtxoPreimageArgs],
    ) -> SigningResult<Vec<H256>> {
        let Some(first) = args.first() else {
            return Ok(Vec::new());
        };
        let digests = Self::new(tx, first)?;
        args.iter()
            .map(|args| digests.sighash_input(tx, args))
            .collect()
    }

    fn new(tx: &ZcashTransaction, args: &UtxoPreimageArgs) -> SigningResult<Self> {
        check_spent_outputs(tx, args)?;

        let mut amounts_stream = Stream::default();
        for amount in &args.spent_amounts {
            amounts_stream.append(amount);
        }
        let mut scripts_stream = Stream::default();
        for script in &args.spent_script_pubkeys {
            scripts_stream.append(script);
        }

        Ok(ZcashSighashV5 {
            header: header_digest(tx),
            prevouts: prevouts_digest(tx),
            sequence: sequence_digest(tx),
            outputs: outputs_digest(&tx.transparent_outputs),
            amounts: hash(
                TRANSPARENT_AMOUNTS_HASH_PERSONALIZATION,
                &amounts_stream.out(),
            ),
            scripts: hash(
                TRANSPARENT_SCRIPTS_HASH_PERSONALIZATION,
                &scripts_stream.out(),
            ),
            sapling: hash(SAPLING_HASH_PERSONALIZATION, &[]),
            orchard: hash(ORCHARD_HASH_PERSONALIZATION, &[]),
        })
    }

    fn sighash_input(&self, tx: &ZcashTransaction, args: &UtxoPreimageArgs) -> SigningResult<H256> {
        let sighash_ty = args.sighash_ty;
        let raw_sighash = sighash_ty.raw_sighash();
        if !VALID_SIGHASH_TYPES.contains(&raw_sighash) {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context(format!("Invalid ZIP-244 sighash type: {raw_sighash:#x}"));
        }

        let input = tx
            .transparent_inputs
            .get(args.input_index)
            .or_tw_err(SigningErrorType::Error_internal)
            .context("Zcash sighash error: input_index is out of bounds")?;
        check_spent_outputs(tx, args)?;

        // S.2b - S.2e
        let (prevouts, amounts, scripts, sequence) = if sighash_ty.anyone_can_pay() {
            (
                hash(PREVOUTS_HASH_PERSONALIZATION, &[]),
                hash(TRANSPARENT_AMOUNTS_HASH_PERSONALIZATION, &[]),
                hash(TRANSPARENT_SCRIPTS_HASH_PERSONALIZATION, &[]),
                hash(SEQUENCE_HASH_PERSONALIZATION, &[]),
            )
        } else {
            (self.prevouts, self.amounts, self.scripts, self.sequence)
        };

        // S.2f
        let outputs = match sighash_ty.base_type() {
            SighashBase::All => self.outputs,
            SighashBase::None => outputs_digest(&[]),
            SighashBase::Single => {
                // Refuse to sign SIGHASH_SINGLE without a corresponding output,
                // as such a signature doesn't commit to any output.
                //
                // This is a deliberate divergence from ZIP-244 (S.2f), which uses the hash
                // of an empty string as `outputs_digest` in that case, so consensus accepts
                // such signatures. We never produce them; verifying them is out of scope.
                let output = tx
                    .transparent_outputs
                    .get(args.input_index)
                    .or_tw_err(SigningErrorType::Error_invalid_params)
                    .context(
                        "SIGHASH_SINGLE requires an output with the same index as the input",
                    )?;
                outputs_digest(std::slice::from_ref(output))
            },
        };

        // S.2g: prevout, value, scriptPubKey and nSequence of the input being signed.
        let mut txin_stream = Stream::default();
        txin_stream
            .append(&input.previous_output)
            .append(&args.spent_amounts[args.input_index])
            .append(&args.spent_script_pubkeys[args.input_index])
            .append(&input.sequence);
        let txin_sig_digest = hash(TRANSPARENT_INPUT_HASH_PERSONALIZATION, &txin_stream.out());

        let mut stream = Stream::default();
        stream
            .append_raw_slice(&[raw_sighash as u8])
            .append(&prevouts)
            .append(&amounts)
            .append(&scripts)
            .append(&sequence)
            .append(&outputs)
            .append(&txin_sig_digest);
        let transparent_sig_digest = hash(TRANSPARENT_HASH_PERSONALIZATION, &stream.out());

        Ok(root_hash(
            &tx.branch_id,
            &self.header,
            &transparent_sig_digest,
            &self.sapling,
            &self.orchard,
        ))
    }
}

fn check_spent_outputs(tx: &ZcashTransaction, args: &UtxoPreimageArgs) -> SigningResult<()> {
    let inputs_len = tx.transparent_inputs.len();
    if args.spent_amounts.len() != inputs_len || args.spent_script_pubkeys.len() != inputs_len {
        return SigningError::err(SigningErrorType::Error_internal)
            .context("ZIP-244 requires amounts and scriptPubKeys of all inputs");
    }
    Ok(())
}

/// T.1: header digest.
fn header_digest(tx: &ZcashTransaction) -> H256 {
    let mut stream = Stream::default();
    stream
        .append(&tx.version)
        .append(&tx.version_group_id)
        .append_raw_slice(tx.branch_id.as_slice())
        .append(&tx.locktime)
        .append(&tx.expiry_height);
    hash(HEADERS_HASH_PERSONALIZATION, &stream.out())
}

/// T.2a: prevouts digest.
fn prevouts_digest(tx: &ZcashTransaction) -> H256 {
    let mut stream = Stream::default();
    for input in tx.transparent_inputs.iter() {
        stream.append(&input.previous_output);
    }
    hash(PREVOUTS_HASH_PERSONALIZATION, &stream.out())
}

/// T.2b: sequence digest.
fn sequence_digest(tx: &ZcashTransaction) -> H256 {
    let mut stream = Stream::default();
    for input in tx.transparent_inputs.iter() {
        stream.append(&input.sequence);
    }
    hash(SEQUENCE_HASH_PERSONALIZATION, &stream.out())
}

/// T.2c / S.2f: digest of the given outputs.
fn outputs_digest(outputs: &[TransactionOutput]) -> H256 {
    let mut stream = Stream::default();
    for output in outputs {
        stream.append(output);
    }
    hash(OUTPUTS_HASH_PERSONALIZATION, &stream.out())
}

/// Combines the header, transparent and (empty) shielded digests into the root hash.
fn root_hash(
    branch_id: &H32,
    header_digest: &H256,
    transparent_digest: &H256,
    sapling_digest: &H256,
    orchard_digest: &H256,
) -> H256 {
    let mut personalisation = H128::new();
    personalisation[..TX_PERSONALIZATION_PREFIX.len()].copy_from_slice(TX_PERSONALIZATION_PREFIX);
    personalisation[TX_PERSONALIZATION_PREFIX.len()..].copy_from_slice(branch_id.as_slice());

    let mut stream = Stream::default();
    stream
        .append(header_digest)
        .append(transparent_digest)
        .append(sapling_digest)
        .append(orchard_digest);
    hash(personalisation.as_slice(), &stream.out())
}

fn hash(personalisation: &[u8], data: &[u8]) -> H256 {
    let digest = blake2_b_personal(data, H256::LEN, personalisation)
        .expect("Hash length and personalisation length are valid");
    H256::try_from(digest.as_slice()).expect("Blake2b output is 32 bytes")
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex::{DecodeHex, ToHex};
    use tw_hash::hasher::Hasher;
    use tw_hash::H32;
    use tw_utxo::encode::Encodable;
    use tw_utxo::script::{Script, Witness};
    use tw_utxo::sighash::SighashType;
    use tw_utxo::signing_mode::SigningMethod;
    use tw_utxo::transaction::standard_transaction::{TransactionInput, TransactionOutput};
    use tw_utxo::transaction::transaction_interface::TransactionInterface;
    use tw_utxo::transaction::transaction_parts::OutPoint;
    use tw_utxo::transaction::UtxoTaprootPreimageArgs;

    /// Transparent-only test vectors from
    /// https://github.com/zcash/zcash-test-vectors/blob/master/zcash_test_vectors/zip_0244.py
    struct TestVector {
        tx: &'static str,
        txid: &'static str,
        amounts: &'static [i64],
        script_pubkeys: &'static [&'static str],
        transparent_input: usize,
        /// `(raw sighash type, expected sighash)`.
        sighashes: &'static [(u32, &'static str)],
    }

    const VECTORS: &[TestVector] = &[
        TestVector {
            tx: "050000800a27a726b4d0d6c2ff6acc0ffc2e490d03146b9d49dd8c7835f43a37dca0787e3ec9f6605223d5ba7ae0ab9025b73bc03f7fac36c009636363635100635365bca7e54cc1a12d127b57c8138976e791013b015f06a624f521b6ee04ec980893c7e5e01a3362035904ac000053d7445fe2d09130f63511da54832de9136b39f4599f5aa5dfbb45da60cdceab7eefde89be63f3f7c00452006aace1405def0244fd7f99b67d040004630063ac12f6465073e1020009636a5351520065ac65000000",
            txid: "bc34e5ca581c5c6544aafb3e5865348f71b8aa2a782df8b6bfa1791bf5a73758",
            amounts: &[1848924248978091, 447389782351145, 620151782842275],
            script_pubkeys: &["ac0000", "6565", ""],
            transparent_input: 0,
            sighashes: &[
                (0x01, "8f607656c52d9ad2231120c24faa7b6855ae571bee46e61d76a25cffb2bb4fcb"),
                (0x02, "fdcf8a04690dc2769d934337fe8b47242a6ed7c9d86d3e01332484d38d71e785"),
                (0x03, "b3a28ebd7cf37a443a40909f513f081fbe0fd78c67f7c4f01b5fb1159789861a"),
                (0x81, "62eada92bbc5af09ab7b95834ae6ba413b17cdf196a6f99c541b710ddacfa545"),
                (0x82, "dff0eb1e03b9c2fe301db9b0183583618893c8474ef565921d5f2832ede3bf49"),
                (0x83, "1de607f188a40b5c152226d94e44411b6902947d42fbb25fb86c9a4e77a4f35b"),
            ],
        },
        TestVector {
            tx: "050000800a27a726b4d0d6c223e119f635ef1d05024b216b7023fadc2d25949c90037e71e3e550726d210a2c688342e52440635e9cc14afe100665515151ac53782e9e4a5fa87f0a956f5b85509960285c22627c59483a5a4c28cce4b156e551406a7ee8355656a20043e38ce103bd9a274e288d020000aafe033252c7030005516a63656338eb8b41ca5104000653516365acac000000",
            txid: "90d2886cb628813371c7d1bd02031b6ca66b42d1db4e118d65f31b2dccb63235",
            amounts: &[1561051182746413, 1535468271734483],
            script_pubkeys: &["656a516aac516a6552", "52"],
            transparent_input: 1,
            sighashes: &[
                (0x01, "2bea7c00cf77fa59a63ab0bd3eb7b10659f8fc9e4c4894bafc37640e8655f562"),
                (0x02, "e63603eade7dde98ef0ed68de640707df6cc9c337837bcbdcde05a07ff7d873b"),
                (0x03, "e4b2f98ad7b55fb256a12923303abcaeee38351090e976b0195dd75bdcf94ad9"),
                (0x81, "61c705d5505f0d5cfb3a28519d791f1457704f7b38392eff885a8207fa683c57"),
                (0x82, "f35ea9beb0b7ed682d27ef78dde1178d6a55f307c85121cd830258d89d33cdd1"),
                (0x83, "aece3f45769876d87b50478b90cd5e3b37b7f4c55b38f756a57486a31061dfa8"),
            ],
        },
        TestVector {
            tx: "050000800a27a726b4d0d6c281836c3be99a081703a460e968aa7109870bbed17df5f888c8ca1467ae17dbbcde31c1105cb5bda88ac6c627000452ac52ac0ffe81ec58bf1e6d1bb7aaada41fba0bb588778a7f65202ad811ea73d26c74550395aff75325107c096a5251acac00650051a2e7424719a3d185b7e0a43a472e298ac0afdc5287d7ad124cd9405a62cd1ca08b282efef7f928df0852525251ac5353aceaa5ff1200000000",
            txid: "221d40597b563f206458f9736b8549237c5554242c419022435a3350a0708ea1",
            amounts: &[1076763594431866, 316847576141144, 1780844721475339],
            script_pubkeys: &["006551ac65630053", "63520053", "acac00656a6351"],
            transparent_input: 2,
            sighashes: &[
                (0x01, "9d802c8ace5bc5f5b03c7dac8a29ef33424def82cbc4fc8400c0bb9d5e084147"),
                (0x02, "ae2db14c57781fb55c2f6658f6141883f7ebe0d39f439cf566dc550fb9da7168"),
                (0x81, "e711834b5a4eb84aa2986a53c2b2ba2fd6374e666f0311bfca8cbceefdde1e38"),
                (0x82, "5c3a0839fdc73417ae7d5f15dceed6c1a4cbe80c6c25ce0386fef7c058a8b943"),
            ],
        },
    ];

    struct Reader<'a> {
        bytes: &'a [u8],
        pos: usize,
    }

    impl<'a> Reader<'a> {
        fn take(&mut self, len: usize) -> &'a [u8] {
            let out = &self.bytes[self.pos..self.pos + len];
            self.pos += len;
            out
        }

        fn u32(&mut self) -> u32 {
            u32::from_le_bytes(self.take(4).try_into().unwrap())
        }

        fn compact(&mut self) -> usize {
            match self.take(1)[0] {
                0xfd => u16::from_le_bytes(self.take(2).try_into().unwrap()) as usize,
                0xfe => self.u32() as usize,
                0xff => u64::from_le_bytes(self.take(8).try_into().unwrap()) as usize,
                n => n as usize,
            }
        }

        fn script(&mut self) -> Script {
            let len = self.compact();
            Script::from(self.take(len).to_vec())
        }
    }

    /// Parses a transparent-only v5 transaction.
    fn parse_tx(bytes: &[u8]) -> ZcashTransaction {
        let mut r = Reader { bytes, pos: 0 };
        let version = r.u32();
        let version_group_id = r.u32();
        let branch_id = H32::try_from(r.take(4)).unwrap();
        let locktime = r.u32();
        let expiry_height = r.u32();

        let transparent_inputs = (0..r.compact())
            .map(|_| TransactionInput {
                previous_output: OutPoint {
                    hash: H256::try_from(r.take(32)).unwrap(),
                    index: r.u32(),
                },
                script_sig: r.script(),
                sequence: r.u32(),
                witness: Witness::default(),
            })
            .collect();
        let transparent_outputs = (0..r.compact())
            .map(|_| TransactionOutput {
                value: i64::from_le_bytes(r.take(8).try_into().unwrap()),
                script_pubkey: r.script(),
            })
            .collect();

        // No Sapling spends, Sapling outputs and Orchard actions.
        assert_eq!(r.take(3), [0, 0, 0]);
        assert_eq!(r.pos, bytes.len());

        ZcashTransaction {
            version,
            version_group_id,
            transparent_inputs,
            transparent_outputs,
            locktime,
            expiry_height,
            sapling_value_balance: 0,
            branch_id,
        }
    }

    fn preimage_args(vector: &TestVector, raw_sighash: u32) -> UtxoPreimageArgs {
        preimage_args_with(vector, SighashType::from_u32(raw_sighash).unwrap())
    }

    fn preimage_args_with(vector: &TestVector, sighash_ty: SighashType) -> UtxoPreimageArgs {
        let spent_script_pubkeys: Vec<Script> = vector
            .script_pubkeys
            .iter()
            .map(|s| Script::from(s.decode_hex().unwrap()))
            .collect();
        UtxoPreimageArgs {
            input_index: vector.transparent_input,
            script_pubkey: spent_script_pubkeys[vector.transparent_input].clone(),
            amount: vector.amounts[vector.transparent_input],
            sighash_ty,
            tx_hasher: Hasher::Sha256d,
            signing_method: SigningMethod::Legacy,
            spent_amounts: vector.amounts.to_vec(),
            spent_script_pubkeys,
            taproot_args: UtxoTaprootPreimageArgs {
                leaf_hash_code_separator: None,
            },
        }
    }

    #[test]
    fn test_zip244_encoding_and_txid() {
        for vector in VECTORS {
            let tx_bytes = vector.tx.decode_hex().unwrap();
            let tx = parse_tx(&tx_bytes);
            assert!(tx.is_v5());
            assert_eq!(tx.encode_out(), tx_bytes);
            assert_eq!(tx.encoded_size(), tx_bytes.len());
            assert_eq!(ZcashSighashV5::txid_digest(&tx).to_hex(), vector.txid);

            // `txid()` returns the reversed (display) byte order.
            let mut expected_txid = vector.txid.decode_hex().unwrap();
            expected_txid.reverse();
            assert_eq!(tx.txid(Hasher::Sha256d), expected_txid);
        }
    }

    #[test]
    fn test_zip244_sighash() {
        for vector in VECTORS {
            let tx = parse_tx(&vector.tx.decode_hex().unwrap());
            for (raw_sighash, expected) in vector.sighashes {
                let args = preimage_args(vector, *raw_sighash);
                let sighash = ZcashSighashV5::sighash_tx(&tx, &args).unwrap();
                assert_eq!(sighash.to_hex(), *expected, "sighash type {raw_sighash:#x}");
            }
        }
    }

    #[test]
    fn test_zip244_sighash_invalid_type() {
        let vector = &VECTORS[0];
        let tx = parse_tx(&vector.tx.decode_hex().unwrap());
        // `SighashType` rejects most invalid values itself, but accepts e.g. `0x00` as `SIGHASH_ALL`.
        let mut checked = 0;
        for raw_sighash in [0x00, 0x04, 0x40, 0x41, 0x84] {
            let Ok(sighash_ty) = SighashType::from_u32(raw_sighash) else {
                continue;
            };
            let args = preimage_args_with(vector, sighash_ty);
            ZcashSighashV5::sighash_tx(&tx, &args).unwrap_err();
            checked += 1;
        }
        assert!(checked > 0);
    }

    #[test]
    fn test_zip244_sighash_single_without_output() {
        // Vector 2 signs input 2, but there are fewer outputs.
        let vector = &VECTORS[2];
        let tx = parse_tx(&vector.tx.decode_hex().unwrap());
        assert!(tx.transparent_outputs.len() <= vector.transparent_input);
        for raw_sighash in [0x03, 0x83] {
            let args = preimage_args(vector, raw_sighash);
            let err = ZcashSighashV5::sighash_tx(&tx, &args).unwrap_err();
            assert_eq!(err.error_type(), &SigningErrorType::Error_invalid_params);
        }
    }

    #[test]
    fn test_zip244_sighash_all_matches_per_input() {
        for vector in VECTORS {
            let tx = parse_tx(&vector.tx.decode_hex().unwrap());
            let args: Vec<_> = vector
                .sighashes
                .iter()
                .map(|(raw_sighash, _)| preimage_args(vector, *raw_sighash))
                .collect();
            let all = ZcashSighashV5::sighash_all(&tx, &args).unwrap();
            let expected: Vec<_> = vector
                .sighashes
                .iter()
                .map(|(_, sighash)| sighash.to_string())
                .collect();
            let actual: Vec<_> = all.iter().map(|h| h.to_hex()).collect();
            assert_eq!(actual, expected);
        }
        assert!(
            ZcashSighashV5::sighash_all(&parse_tx(&VECTORS[0].tx.decode_hex().unwrap()), &[])
                .unwrap()
                .is_empty()
        );
    }
}
