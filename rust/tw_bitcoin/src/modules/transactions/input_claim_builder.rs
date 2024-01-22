use super::brc20::{BRC20TransferInscription, Brc20Ticker};
use super::OrdinalNftInscription;
use crate::aliases::*;
use crate::{Error, Result};
use bitcoin::consensus::Decodable;
use bitcoin::taproot::{ControlBlock, LeafVersion};
use bitcoin::{ScriptBuf, Witness};
use std::borrow::Cow;
use tw_coin_entry::coin_entry::SignatureBytes;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

pub struct InputClaimBuilder;

impl InputClaimBuilder {
    /// Creates the claim script (_scriptSig_ or _Witness_) to be revealed
    /// on-chain for a given input.
    pub fn utxo_claim_from_proto(
        input: &Proto::Input<'_>,
        signature: SignatureBytes,
    ) -> Result<UtxoProto::TxInClaim<'static>> {
        let (script_sig, witness) = match &input.to_recipient {
            ProtoInputRecipient::builder(variant) => match &variant.variant {
                ProtoInputBuilder::p2sh(redeem_script) => (
                    ScriptBuf::from_bytes(redeem_script.to_vec()),
                    Witness::new(),
                ),
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
                ProtoInputBuilder::p2wsh(redeem_script) => {
                    let witness = Witness::consensus_decode(&mut redeem_script.as_ref())
                        .map_err(|_| Error::from(Proto::Error::Error_invalid_witness_encoding))?;

                    (ScriptBuf::new(), witness)
                },
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

                    let sig = bitcoin::taproot::Signature::from_slice(signature.as_ref())?;

                    // The spending script itself.
                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(sig.to_vec());
                        w.push(taproot.payload.as_ref());
                        w.push(control_block.serialize());
                        w
                    })
                },
                ProtoInputBuilder::ordinal_inscribe(ordinal) => {
                    let pubkey = bitcoin::PublicKey::from_slice(ordinal.inscribe_to.as_ref())?;
                    let mime_type = ordinal.mime_type.as_ref();
                    let data = ordinal.payload.as_ref();

                    let nft = OrdinalNftInscription::new(mime_type.as_bytes(), data, pubkey)
                        .expect("badly constructed Ordinal inscription");

                    // Create a control block for that inscription.
                    let control_block = nft
                        .inscription()
                        .spend_info()
                        .control_block(&(
                            nft.inscription().taproot_program().to_owned(),
                            LeafVersion::TapScript,
                        ))
                        .expect("badly constructed control block");

                    let sig = bitcoin::taproot::Signature::from_slice(signature.as_ref())?;

                    // The spending script itself.
                    (ScriptBuf::new(), {
                        let mut w = Witness::new();
                        w.push(sig.to_vec());
                        w.push(nft.inscription().taproot_program());
                        w.push(control_block.serialize());
                        w
                    })
                },
                ProtoInputBuilder::brc20_inscribe(brc20) => {
                    let pubkey = bitcoin::PublicKey::from_slice(brc20.inscribe_to.as_ref())?;
                    let ticker = Brc20Ticker::new(brc20.ticker.to_string())?;

                    // Construct the BRC20 transfer inscription.
                    let transfer = BRC20TransferInscription::new(
                        pubkey,
                        ticker,
                        brc20.transfer_amount.to_string(),
                    )
                    .expect("invalid BRC20 transfer construction");

                    // Create a control block for that inscription.
                    let control_block = transfer
                        .inscription()
                        .spend_info()
                        .control_block(&(
                            transfer.inscription().taproot_program().to_owned(),
                            LeafVersion::TapScript,
                        ))
                        .expect("badly constructed control block");

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
                ProtoInputBuilder::None => {
                    return Err(Error::from(Proto::Error::Error_missing_input_builder))
                },
            },
            ProtoInputRecipient::custom_script(custom) => (
                ScriptBuf::from_bytes(custom.script_sig.to_vec()),
                Witness::from_slice(&custom.witness_items),
            ),
            ProtoInputRecipient::None => {
                return Err(Error::from(Proto::Error::Error_missing_recipient))
            },
        };

        // Create Utxo.proto structure.
        let claim = UtxoProto::TxInClaim {
            txid: input.txid.to_vec().into(),
            vout: input.vout,
            value: input.value,
            sequence: input.sequence,
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
