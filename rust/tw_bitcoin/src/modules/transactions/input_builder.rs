use super::brc20::{BRC20TransferInscription, Brc20Ticker};
use crate::entry::aliases::*;
use crate::{Error, Result};
use bitcoin::taproot::{ControlBlock, LeafVersion, TapLeafHash};
use bitcoin::{ScriptBuf, Witness};
use secp256k1::XOnlyPublicKey;
use std::borrow::Cow;
use tw_coin_entry::coin_entry::SignatureBytes;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

pub struct InputBuilder;

impl InputBuilder {
    pub fn utxo_from_proto(input: &Proto::Input<'_>) -> Result<UtxoProto::TxIn<'static>> {
        let (signing_method, script_pubkey, leaf_hash, weight) = match &input.to_recipient {
            ProtoInputRecipient::builder(builder) => match &builder.variant {
                ProtoInputBuilder::p2sh(_) => todo!(),
                ProtoInputBuilder::p2pkh(pubkey) => {
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref())?;

                    (
                        UtxoProto::SigningMethod::Legacy,
                        ScriptBuf::new_p2pkh(&pubkey.pubkey_hash()),
                        None,
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
                ProtoInputBuilder::p2wsh(_) => todo!(),
                ProtoInputBuilder::p2wpkh(pubkey) => {
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref())?;

                    (
                        UtxoProto::SigningMethod::Segwit,
                        ScriptBuf::new_v0_p2wpkh(&pubkey.wpubkey_hash().ok_or_else(|| {
                            Error::from(Proto::Error::Error_invalid_witness_pubkey_hash)
                        })?),
                        None,
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

                    (
                        signing_method,
                        ScriptBuf::new_v1_p2tr(&secp256k1::Secp256k1::new(), xonly, None),
                        None,
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
                    let script_buf = ScriptBuf::from_bytes(complex.payload.to_vec());
                    let leaf_hash = Some(TapLeafHash::from_script(
                        script_buf.as_script(),
                        bitcoin::taproot::LeafVersion::TapScript,
                    ));

                    let signing_method = if complex.one_prevout {
                        UtxoProto::SigningMethod::TaprootOnePrevout
                    } else {
                        UtxoProto::SigningMethod::TaprootAll
                    };

                    (
                        signing_method,
                        script_buf,
                        leaf_hash,
                        // witness bytes, scale factor NOT applied.
                        (
                            // indicator of witness item
                            1 +
                            // the payload (TODO: should this be `repeated bytes`?)
                            complex.payload.len() as u64 +
                            // length + control block (pubkey + Merkle path), roughly
                            1 + 64
                        ),
                    )
                },
                ProtoInputBuilder::brc20_inscribe(brc20) => {
                    let pubkey = bitcoin::PublicKey::from_slice(brc20.inscribe_to.as_ref())?;
                    let ticker = Brc20Ticker::new(brc20.ticker.to_string())?;

                    let transfer =
                        BRC20TransferInscription::new(pubkey.into(), ticker, brc20.transfer_amount)
                            .expect("invalid BRC20 transfer construction");

                    let leaf_hash = Some(TapLeafHash::from_script(
                        transfer.inscription().taproot_program(),
                        bitcoin::taproot::LeafVersion::TapScript,
                    ));

                    let signing_method = if brc20.one_prevout {
                        UtxoProto::SigningMethod::TaprootOnePrevout
                    } else {
                        UtxoProto::SigningMethod::TaprootAll
                    };

                    (
                        signing_method,
                        ScriptBuf::from(transfer.inscription().taproot_program()),
                        leaf_hash,
                        // witness bytes, scale factor NOT applied.
                        (
                            // indicator of witness item count.
                            1 +
                            // the payload (TODO: should this be `repeated bytes`?)
                            transfer.inscription().taproot_program().len() as u64 +
                            // length + control block (pubkey + Merkle path), roughly
                            1 + 64
                        ),
                    )
                },
                ProtoInputBuilder::None => {
                    return Err(Error::from(Proto::Error::Error_missing_input_builder))
                },
            },
            ProtoInputRecipient::custom(custom) => {
                let script_pubkey = ScriptBuf::from_bytes(custom.script_pubkey.to_vec());

                let leaf_hash = if let UtxoProto::SigningMethod::TaprootAll
                | UtxoProto::SigningMethod::TaprootOnePrevout =
                    custom.signing_method
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
            amount: input.amount,
            sequence: input.sequence,
            script_pubkey: script_pubkey.to_vec().into(),
            signing_method,
            sighash_type: input.sighash_type,
            weight_projection: weight,
            leaf_hash: leaf_hash
                .map(|hash| hash.to_vec().into())
                .unwrap_or_default(),
        };

        Ok(utxo)
    }
}
