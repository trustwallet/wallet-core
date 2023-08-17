use super::brc20::{BRC20TransferInscription, Ticker};
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
                    let ticker = Ticker::new(brc20.ticker.to_string())?;

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
                            // indicator of witness item (?)
                            1 +
                            // the payload (TODO: should this be `repeated bytes`?)
                            transfer.inscription().taproot_program().len() as u64 +
                            // length + control block (pubkey + Merkle path), roughly
                            1 + 64
                        ),
                    )
                },
                ProtoInputBuilder::None => todo!(),
            },
            ProtoInputRecipient::custom(_custom) => {
                todo!()
            },
            ProtoInputRecipient::None => todo!(),
        };

        let utxo = UtxoProto::TxIn {
            txid: input.txid.to_vec().into(),
            vout: input.vout,
            amount: input.amount,
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
    /// Creates the claim script (_scriptSig_ or _Witness_) to be revealed
    /// on-chain for a given input.
    pub fn utxo_claim_from_proto(
        input: &Proto::Input<'_>,
        signature: SignatureBytes,
    ) -> Result<UtxoProto::TxInClaim<'static>> {
        let (script_sig, witness) = match &input.to_recipient {
            ProtoInputRecipient::builder(variant) => match &variant.variant {
                ProtoInputBuilder::None => panic!(),
                ProtoInputBuilder::p2sh(_) => todo!(),
                ProtoInputBuilder::p2pkh(pubkey) => {
                    let sig = bitcoin::ecdsa::Signature::from_slice(signature.as_ref())?;
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref())?;

                    // The spending script itself.
                    (
                        ScriptBuf::builder()
                            .push_slice(sig.serialize())
                            .push_key(&pubkey)
                            .into_script(),
                        Witness::new(),
                    )
                },
                ProtoInputBuilder::p2wsh(_) => todo!(),
                ProtoInputBuilder::p2wpkh(pubkey) => {
                    let sig = bitcoin::ecdsa::Signature::from_slice(signature.as_ref())?;
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref())?;

                    // The spending script itself.
                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(sig.serialize());
                        w.push(pubkey.to_bytes());
                        w
                    })
                },
                ProtoInputBuilder::p2tr_key_path(_) => {
                    let sig = bitcoin::taproot::Signature::from_slice(signature.as_ref())?;

                    // The spending script itself.
                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(sig.to_vec());
                        w
                    })
                },
                ProtoInputBuilder::p2tr_script_path(taproot) => {
                    let control_block = ControlBlock::decode(taproot.control_block.as_ref())
                        .map_err(|_| Error::from(Proto::Error::Error_invalid_control_block))?;

                    // The spending script itself.
                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(taproot.payload.as_ref());
                        w.push(control_block.serialize());
                        w
                    })
                },
                ProtoInputBuilder::brc20_inscribe(brc20) => {
                    let pubkey = bitcoin::PublicKey::from_slice(brc20.inscribe_to.as_ref())?;
                    let ticker = Ticker::new(brc20.ticker.to_string())?;

                    // Construct the BRC20 transfer inscription.
                    let transfer =
                        BRC20TransferInscription::new(pubkey.into(), ticker, brc20.transfer_amount)
                            .expect("invalid BRC20 transfer construction");

                    // Create a control block for that inscription.
                    let control_block = transfer
                        .inscription()
                        .spend_info()
                        .control_block(&(
                            transfer.inscription().taproot_program().to_owned(),
                            LeafVersion::TapScript,
                        ))
                        .expect("incorrectly constructed control block");

                    let sig = bitcoin::taproot::Signature::from_slice(signature.as_ref())?;

                    // The spending script itself.
                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(sig.to_vec());
                        w.push(transfer.inscription().taproot_program());
                        w.push(control_block.serialize());
                        w
                    })
                },
            },
            ProtoInputRecipient::custom(custom) => (
                ScriptBuf::from_bytes(custom.script_sig.to_vec()),
                Witness::from_slice(&custom.witness_items),
            ),
            ProtoInputRecipient::None => {
                todo!()
            },
        };

        let claim = UtxoProto::TxInClaim {
            txid: input.txid.to_vec().into(),
            vout: input.vout,
            // TODO
            sequence: u32::MAX,
            script_sig: script_sig.to_vec().into(),
            witness_items: witness
                .to_vec()
                .into_iter()
                .map(Cow::Owned)
                .collect::<Vec<Cow<_>>>(),
        };

        Ok(claim)
    }
}
