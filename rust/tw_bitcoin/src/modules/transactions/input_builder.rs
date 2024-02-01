use super::brc20::{BRC20TransferInscription, Brc20Ticker};
use crate::aliases::*;
use crate::modules::transactions::OrdinalNftInscription;
use crate::{Error, Result};
use bitcoin::taproot::{LeafVersion, TapLeafHash};
use bitcoin::ScriptBuf;
use secp256k1::XOnlyPublicKey;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

// Convenience varibles used solely for readability.
const NO_LEAF_HASH: Option<TapLeafHash> = None;

pub struct InputBuilder;

impl InputBuilder {
    pub fn utxo_from_proto(input: &Proto::Input<'_>) -> Result<UtxoProto::TxIn<'static>> {
        let (signing_method, script_pubkey, leaf_hash, weight) = match &input.to_recipient {
            ProtoInputRecipient::builder(builder) => match &builder.variant {
                ProtoInputBuilder::p2sh(redeem_script) => {
                    // The scriptPubkey is the redeem script directly.
                    let script_pubkey = ScriptBuf::from_bytes(redeem_script.to_vec());

                    (
                        UtxoProto::SigningMethod::Legacy,
                        script_pubkey,
                        NO_LEAF_HASH,
                        // scale factor applied to non-witness bytes
                        4 * (
                            // length + redeem script.
                            1 + redeem_script.len() as u64
                        ),
                    )
                },
                ProtoInputBuilder::p2pkh(pubkey) => {
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref())?;
                    let script_pubkey = ScriptBuf::new_p2pkh(&pubkey.pubkey_hash());

                    (
                        UtxoProto::SigningMethod::Legacy,
                        script_pubkey,
                        NO_LEAF_HASH,
                        // scale factor applied to non-witness bytes
                        4 * (
                            // length + ECDSA signature
                            1 + 72 +
                            // length + public key
                            1 + {
                                if pubkey.compressed {
                                    33
                                } else {
                                    65
                                }
                            }
                        ),
                    )
                },
                ProtoInputBuilder::p2wsh(redeem_script) => {
                    // The scriptPubkey is the redeem script directly.
                    let script_pubkey = ScriptBuf::from_bytes(redeem_script.to_vec());

                    (
                        UtxoProto::SigningMethod::Segwit,
                        script_pubkey,
                        NO_LEAF_HASH,
                        // witness bytes, scale factor NOT applied.
                        (
                            // length + redeem script.
                            1 + redeem_script.len() as u64
                        ),
                    )
                },
                ProtoInputBuilder::p2wpkh(pubkey) => {
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref())?;

                    let script_pubkey =
                        ScriptBuf::new_v0_p2wpkh(&pubkey.wpubkey_hash().ok_or_else(|| {
                            Error::from(Proto::Error::Error_invalid_witness_pubkey_hash)
                        })?)
                        // Special script code requirement for claiming P2WPKH outputs.
                        .p2wpkh_script_code()
                        .ok_or_else(|| Error::from(Proto::Error::Error_invalid_wpkh_script_code))?;

                    (
                        UtxoProto::SigningMethod::Segwit,
                        script_pubkey,
                        NO_LEAF_HASH,
                        // witness bytes, scale factor NOT applied.
                        (
                            // indicator of witness item (2)
                            1 +
                            // length + ECDSA signature (can be 71 or 72)
                            1 + 72 +
                            // length + public key
                            1 + {
                                if pubkey.compressed {
                                    33
                                } else {
                                    65
                                }
                            }
                        ),
                    )
                },
                ProtoInputBuilder::p2tr_key_path(key_path) => {
                    let pubkey = bitcoin::PublicKey::from_slice(key_path.public_key.as_ref())?;
                    let xonly = XOnlyPublicKey::from(pubkey.inner);

                    let signing_method = if key_path.one_prevout {
                        UtxoProto::SigningMethod::TaprootOnePrevout
                    } else {
                        UtxoProto::SigningMethod::TaprootAll
                    };

                    let script_pubkey =
                        ScriptBuf::new_v1_p2tr(&secp256k1::Secp256k1::new(), xonly, None);

                    (
                        signing_method,
                        script_pubkey,
                        NO_LEAF_HASH,
                        // witness bytes, scale factor NOT applied.
                        (
                            // indicator of witness item (1)
                            1 +
                            // length + Schnorr signature (can be 71 or 72)
                            1 + 72
                            // NO public key
                        ),
                    )
                },
                ProtoInputBuilder::p2tr_script_path(complex) => {
                    let script_pubkey = ScriptBuf::from_bytes(complex.payload.to_vec());
                    let leaf_hash = Some(TapLeafHash::from_script(
                        script_pubkey.as_script(),
                        bitcoin::taproot::LeafVersion::TapScript,
                    ));

                    let signing_method = if complex.one_prevout {
                        UtxoProto::SigningMethod::TaprootOnePrevout
                    } else {
                        UtxoProto::SigningMethod::TaprootAll
                    };

                    (
                        signing_method,
                        script_pubkey,
                        leaf_hash,
                        // witness bytes, scale factor NOT applied.
                        (
                            // indicator of witness item
                            1 +
                            // the payload/witness
                            complex.payload.len() as u64
                        ),
                    )
                },
                ProtoInputBuilder::ordinal_inscribe(ordinal) => {
                    let pubkey = bitcoin::PublicKey::from_slice(ordinal.inscribe_to.as_ref())?;
                    let mime_type = ordinal.mime_type.as_ref();
                    let data = ordinal.payload.as_ref();

                    let nft = OrdinalNftInscription::new(mime_type.as_bytes(), data, pubkey)
                        .expect("badly constructed Ordinal inscription");

                    // We construct a control block to estimate the fee,
                    // otherwise we do not need it here.
                    let control_block = nft
                        .inscription()
                        .spend_info()
                        .control_block(&(
                            nft.inscription().taproot_program().to_owned(),
                            LeafVersion::TapScript,
                        ))
                        .expect("badly constructed control block");

                    let leaf_hash = Some(TapLeafHash::from_script(
                        nft.inscription().taproot_program(),
                        bitcoin::taproot::LeafVersion::TapScript,
                    ));

                    let signing_method = if ordinal.one_prevout {
                        UtxoProto::SigningMethod::TaprootOnePrevout
                    } else {
                        UtxoProto::SigningMethod::TaprootAll
                    };

                    let script_pubkey = ScriptBuf::from(nft.inscription().taproot_program());

                    (
                        signing_method,
                        // TODO: This is technically not needed here, since
                        // Sighash only includes the leaf hash, not the actual
                        // payload. Remove this (same for other complex scripts).
                        script_pubkey,
                        leaf_hash,
                        // witness bytes, scale factor NOT applied.
                        (
                            // indicator of witness item (1)
                            1 +
                            // length + Schnorr signature (can be 71 or 72)
                            1 + 72 +
                            // the payload/witness
                            nft.inscription().taproot_program().len() as u64 +
                            // length + control block
                            1 + control_block.size() as u64
                        ),
                    )
                },
                // TODO: Unify this and `ordinal_inscribe` somehow
                ProtoInputBuilder::brc20_inscribe(brc20) => {
                    let pubkey = bitcoin::PublicKey::from_slice(brc20.inscribe_to.as_ref())?;
                    let ticker = Brc20Ticker::new(brc20.ticker.to_string())?;

                    let transfer = BRC20TransferInscription::new(
                        pubkey,
                        ticker,
                        brc20.transfer_amount.to_string(),
                    )
                    .expect("invalid BRC20 transfer construction");

                    // We construct a control block to estimate the fee,
                    // otherwise we do not need it here.
                    let control_block = transfer
                        .inscription()
                        .spend_info()
                        .control_block(&(
                            transfer.inscription().taproot_program().to_owned(),
                            LeafVersion::TapScript,
                        ))
                        .expect("badly constructed control block");

                    let leaf_hash = Some(TapLeafHash::from_script(
                        transfer.inscription().taproot_program(),
                        bitcoin::taproot::LeafVersion::TapScript,
                    ));

                    let signing_method = if brc20.one_prevout {
                        UtxoProto::SigningMethod::TaprootOnePrevout
                    } else {
                        UtxoProto::SigningMethod::TaprootAll
                    };

                    let script_pubkey = ScriptBuf::from(transfer.inscription().taproot_program());

                    (
                        signing_method,
                        script_pubkey,
                        leaf_hash,
                        // witness bytes, scale factor NOT applied.
                        (
                            // indicator of witness item (1)
                            1 +
                            // length + Schnorr signature (can be 71 or 72)
                            1 + 72 +
                            // the payload/witness
                            transfer.inscription().taproot_program().len() as u64 +
                            // length + control block
                            1 + control_block.size() as u64
                        ),
                    )
                },
                ProtoInputBuilder::None => {
                    return Err(Error::from(Proto::Error::Error_missing_input_builder))
                },
            },
            ProtoInputRecipient::custom_script(custom) => {
                let script_pubkey = ScriptBuf::from_bytes(custom.script_pubkey.to_vec());

                #[rustfmt::skip]
                let leaf_hash = if let UtxoProto::SigningMethod::TaprootAll
                    | UtxoProto::SigningMethod::TaprootOnePrevout = custom.signing_method
                {
                    Some(TapLeafHash::from_script(
                        &script_pubkey,
                        bitcoin::taproot::LeafVersion::TapScript,
                    ))
                } else {
                    None
                };

                (
                    custom.signing_method,
                    script_pubkey,
                    leaf_hash,
                    (
                        // scale factor applied to non-witness bytes
                        4 * custom.script_sig.len() as u64
                        // indicator of witness item count.
                        + 1
                        // witness bytes, scale factor NOT applied.
                        + custom
                            .witness_items
                            .iter()
                            .map(|item| item.len() as u64)
                            .sum::<u64>()
                    ),
                )
            },
            ProtoInputRecipient::None => {
                return Err(Error::from(Proto::Error::Error_missing_input_builder))
            },
        };

        // Create Utxo.proto structure.
        let utxo = UtxoProto::TxIn {
            txid: input.txid.to_vec().into(),
            vout: input.vout,
            value: input.value,
            sequence: input.sequence,
            script_pubkey: script_pubkey.to_vec().into(),
            signing_method,
            sighash_type: input.sighash_type,
            weight_estimate: weight,
            leaf_hash: leaf_hash
                .map(|hash| hash.to_vec().into())
                .unwrap_or_default(),
        };

        Ok(utxo)
    }
}
