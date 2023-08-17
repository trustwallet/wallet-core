use crate::{Error, Result};
use crate::entry::aliases::*;
use super::brc20::{BRC20TransferInscription, Brc20Ticker};
use bitcoin::taproot::{ControlBlock, LeafVersion};
use bitcoin::{ScriptBuf, Witness};
use tw_coin_entry::coin_entry::SignatureBytes;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;
use std::borrow::Cow;

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
                    let ticker = Brc20Ticker::new(brc20.ticker.to_string())?;

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
                return Err(Error::from(Proto::Error::Error_missing_recipient))
            },
        };

        // Create Utxo.proto structure.
        let claim = UtxoProto::TxInClaim {
            txid: input.txid.to_vec().into(),
            vout: input.vout,
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
