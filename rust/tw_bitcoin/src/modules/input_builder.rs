use crate::brc20::{BRC20TransferInscription, Ticker};
use crate::entry::aliases::*;
use crate::Result;
use bitcoin::taproot::{ControlBlock, TapLeafHash};
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
        let mut leaf_hash = None;

        let (signing_method, script_pubkey) = match &input.to_recipient {
            ProtoInputRecipient::builder(builder) => match &builder.variant {
                ProtoInputBuilder::p2sh(_) => todo!(),
                ProtoInputBuilder::p2pkh(pubkey) => {
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref()).unwrap();

                    (
                        UtxoProto::SigningMethod::Legacy,
                        ScriptBuf::new_p2pkh(&pubkey.pubkey_hash()),
                    )
                },
                ProtoInputBuilder::p2wsh(_) => todo!(),
                ProtoInputBuilder::p2wpkh(pubkey) => {
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref()).unwrap();

                    (
                        UtxoProto::SigningMethod::Segwit,
                        ScriptBuf::new_v0_p2wpkh(&pubkey.wpubkey_hash().unwrap()),
                    )
                },
                ProtoInputBuilder::p2tr_key_path(pubkey) => {
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref()).unwrap();
                    let xonly = XOnlyPublicKey::from(pubkey.inner);

                    (
                        UtxoProto::SigningMethod::Taproot,
                        ScriptBuf::new_v1_p2tr(&secp256k1::Secp256k1::new(), xonly, None),
                    )
                },
                ProtoInputBuilder::p2tr_script_path(complex) => {
                    let script_buf = ScriptBuf::from_bytes(complex.payload.to_vec());
                    leaf_hash = Some(TapLeafHash::from_script(
                        script_buf.as_script(),
                        bitcoin::taproot::LeafVersion::TapScript,
                    ));

                    (UtxoProto::SigningMethod::Taproot, script_buf)
                },
                ProtoInputBuilder::brc20_inscribe(brc20) => {
                    let pubkey =
                        bitcoin::PublicKey::from_slice(brc20.inscribe_to.as_ref()).unwrap();
                    let ticker = Ticker::new(brc20.ticker.to_string()).unwrap();

                    let brc20 =
                        BRC20TransferInscription::new(pubkey.into(), ticker, brc20.transfer_amount)
                            .unwrap();

                    leaf_hash = Some(TapLeafHash::from_script(
                        brc20.inscription().taproot_program(),
                        bitcoin::taproot::LeafVersion::TapScript,
                    ));

                    (
                        UtxoProto::SigningMethod::Taproot,
                        ScriptBuf::from(brc20.inscription().taproot_program()),
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
            leaf_hash: leaf_hash
                .map(|hash| hash.to_vec().into())
                .unwrap_or_default(),
            one_prevout: input.one_prevout,
        };

        Ok(utxo)
    }
    pub fn utxo_claim_from_proto(
        input: &Proto::Input<'_>,
        signature: SignatureBytes,
    ) -> Result<UtxoProto::TxInClaim<'static>> {
        let (script_sig, witness) = match &input.to_recipient {
            ProtoInputRecipient::builder(variant) => match &variant.variant {
                ProtoInputBuilder::None => panic!(),
                ProtoInputBuilder::p2sh(_) => todo!(),
                ProtoInputBuilder::p2pkh(pubkey) => {
                    let sig = bitcoin::ecdsa::Signature::from_slice(signature.as_ref()).unwrap();
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref()).unwrap();

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
                    let sig = bitcoin::ecdsa::Signature::from_slice(signature.as_ref()).unwrap();
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref()).unwrap();

                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(sig.serialize());
                        w.push(pubkey.to_bytes());
                        w
                    })
                },
                ProtoInputBuilder::p2tr_key_path(_) => {
                    let sig = bitcoin::taproot::Signature::from_slice(signature.as_ref()).unwrap();

                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(sig.to_vec());
                        w
                    })
                },
                ProtoInputBuilder::p2tr_script_path(taproot) => {
                    let sig = bitcoin::taproot::Signature::from_slice(signature.as_ref()).unwrap();
                    let control_block =
                        ControlBlock::decode(taproot.control_block.as_ref()).unwrap();

                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(sig.to_vec());
                        w.push(taproot.payload.as_ref());
                        w.push(control_block.serialize());
                        w
                    })
                },
                ProtoInputBuilder::brc20_inscribe(brc20) => {
                    let pubkey =
                        bitcoin::PublicKey::from_slice(brc20.inscribe_to.as_ref()).unwrap();
                    let ticker = Ticker::new(brc20.ticker.to_string()).unwrap();
                    let control_block = ControlBlock::decode(brc20.control_block.as_ref()).unwrap();

                    let brc20 =
                        BRC20TransferInscription::new(pubkey.into(), ticker, brc20.transfer_amount)
                            .unwrap();

                    let sig = bitcoin::taproot::Signature::from_slice(signature.as_ref()).unwrap();

                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(sig.to_vec());
                        w.push(brc20.inscription().taproot_program());
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
