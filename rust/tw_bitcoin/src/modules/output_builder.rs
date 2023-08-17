use std::str::FromStr;

use crate::brc20::{BRC20TransferInscription, Ticker};
use crate::entry::aliases::*;
use crate::{Error, Result};
use bitcoin::address::{Payload, WitnessVersion};
use bitcoin::taproot::{LeafVersion, TapNodeHash};
use bitcoin::{Address, PubkeyHash, ScriptBuf, WPubkeyHash};
use secp256k1::hashes::Hash;
use secp256k1::XOnlyPublicKey;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;

pub struct OutputBuilder;

impl OutputBuilder {
    /// Creates the spending condition (_scriptPubkey_) for a given output.
    pub fn utxo_from_proto(
        output: &Proto::Output<'_>,
    ) -> Result<Proto::mod_PreSigningOutput::TxOut<'static>> {
        let secp = secp256k1::Secp256k1::new();
        let (script_pubkey, control_block, taproot_payload) = match &output.to_recipient {
            // Script spending condition was passed on directly.
            ProtoOutputRecipient::script_pubkey(script) => {
                (ScriptBuf::from_bytes(script.to_vec()), None, None)
            },
            // Process builder methods. We construct the Script spending
            // conditions by using the specified parameters.
            ProtoOutputRecipient::builder(builder) => match &builder.variant {
                ProtoOutputBuilder::p2sh(_) => {
                    todo!()
                },
                ProtoOutputBuilder::p2pkh(pubkey_or_hash) => {
                    let pubkey_hash = pubkey_hash_from_proto(pubkey_or_hash)?;
                    (ScriptBuf::new_p2pkh(&pubkey_hash), None, None)
                },
                ProtoOutputBuilder::p2wsh(_) => {
                    todo!()
                },
                ProtoOutputBuilder::p2wpkh(pubkey_or_hash) => {
                    let wpubkey_hash = witness_pubkey_hash_from_proto(pubkey_or_hash)?;
                    (ScriptBuf::new_v0_p2wpkh(&wpubkey_hash), None, None)
                },
                ProtoOutputBuilder::p2tr_key_path(pubkey) => {
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref())?;
                    let xonly = XOnlyPublicKey::from(pubkey.inner);
                    (ScriptBuf::new_v1_p2tr(&secp, xonly, None), None, None)
                },
                ProtoOutputBuilder::p2tr_script_path(complex) => {
                    let node_hash = TapNodeHash::from_slice(complex.node_hash.as_ref())
                        .map_err(|_| Error::from(Proto::Error::Error_invalid_taproot_root))?;

                    let pubkey = bitcoin::PublicKey::from_slice(complex.public_key.as_ref())?;
                    let xonly = XOnlyPublicKey::from(pubkey.inner);

                    (
                        ScriptBuf::new_v1_p2tr(&secp, xonly, Some(node_hash)),
                        None,
                        None,
                    )
                },
                ProtoOutputBuilder::brc20_inscribe(brc20) => {
                    let pubkey = bitcoin::PublicKey::from_slice(brc20.inscribe_to.as_ref())?;
                    let xonly = XOnlyPublicKey::from(pubkey.inner);

                    let ticker = Ticker::new(brc20.ticker.to_string())?;
                    let transfer =
                        BRC20TransferInscription::new(pubkey.into(), ticker, brc20.transfer_amount)
                            .expect("invalid BRC20 transfer construction");

                    // Explicit check
                    let control_block = transfer
                        .inscription()
                        .spend_info()
                        .control_block(&(
                            transfer.inscription().taproot_program().to_owned(),
                            LeafVersion::TapScript,
                        ))
                        .expect("incorrectly constructed control block");

                    let merkle_root = transfer
                        .inscription()
                        .spend_info()
                        .merkle_root()
                        .expect("incorrectly constructed Taproot merkle root");

                    (
                        ScriptBuf::new_v1_p2tr(&secp, xonly, Some(merkle_root)),
                        Some(control_block.serialize()),
                        Some(transfer.inscription().taproot_program().to_vec()),
                    )
                },
                ProtoOutputBuilder::None => todo!(),
            },
            // We derive the transaction type from the address.
            ProtoOutputRecipient::from_address(addr) => {
                let string = String::from_utf8(addr.to_vec()).unwrap();
                // TODO: Network.
                let addr = Address::from_str(&string)
                    .unwrap()
                    .require_network(bitcoin::Network::Bitcoin)
                    .unwrap();

                let proto = match addr.payload {
                    // Identified a "PubkeyHash" address (i.e. P2PKH).
                    Payload::PubkeyHash(pubkey_hash) => Proto::Output {
                        amount: output.amount,
                        to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::Builder {
                            variant: ProtoOutputBuilder::p2pkh(Proto::ToPublicKeyOrHash {
                                to_address: Proto::mod_ToPublicKeyOrHash::OneOfto_address::hash(
                                    pubkey_hash.to_vec().into(),
                                ),
                            }),
                        }),
                    },
                    // Identified a witness program (i.e. Segwit or Taproot).
                    Payload::WitnessProgram(progam) => {
                        match progam.version() {
                            // Identified version 0, i.e. Segwit
                            WitnessVersion::V0 => {
                                let wpubkey_hash = progam.program().as_bytes().to_vec();
                                // TODO:
                                assert_eq!(wpubkey_hash.len(), 20);

                                Proto::Output {
                                    amount: output.amount,
                                    to_recipient: ProtoOutputRecipient::builder(
                                        Proto::mod_Output::Builder {
                                            variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                                                to_address:
                                                    Proto::mod_ToPublicKeyOrHash::OneOfto_address::hash(
                                                        wpubkey_hash.into(),
                                                    ),
                                            }),
                                        },
                                    ),
                                }
                            },
                            // Identified version 1, i.e P2TR key-path (Taproot)
                            WitnessVersion::V1 => {
                                let pubkey = progam.program().as_bytes().to_vec();
                                // TODO:
                                assert_eq!(pubkey.len(), 32);

                                Proto::Output {
                                    amount: output.amount,
                                    to_recipient: ProtoOutputRecipient::builder(
                                        Proto::mod_Output::Builder {
                                            variant: ProtoOutputBuilder::p2tr_key_path(
                                                pubkey.into(),
                                            ),
                                        },
                                    ),
                                }
                            },
                            _ => todo!(),
                        }
                    },
                    Payload::ScriptHash(_hash) => todo!(),
                    _ => todo!(),
                };

                // Recursive call, will initiate the appropraite builder.
                return Self::utxo_from_proto(&proto);
            },
            ProtoOutputRecipient::None => todo!(),
        };

        let utxo = Proto::mod_PreSigningOutput::TxOut {
            value: output.amount,
            script_pubkey: script_pubkey.to_vec().into(),
            control_block: control_block.map(|cb| cb.into()).unwrap_or_default(),
            taproot_payload: taproot_payload.map(|cb| cb.into()).unwrap_or_default(),
        };

        Ok(utxo)
    }
}

// Conenience helper function.
fn pubkey_hash_from_proto(pubkey_or_hash: &Proto::ToPublicKeyOrHash) -> Result<PubkeyHash> {
    let pubkey_hash = match &pubkey_or_hash.to_address {
        ProtoPubkeyOrHash::hash(hash) => PubkeyHash::from_slice(hash.as_ref())
            .map_err(|_| Error::from(Proto::Error::Error_invalid_pubkey_hash))?,
        ProtoPubkeyOrHash::pubkey(pubkey) => {
            bitcoin::PublicKey::from_slice(pubkey.as_ref())?.pubkey_hash()
        },
        ProtoPubkeyOrHash::None => todo!(),
    };

    Ok(pubkey_hash)
}

// Conenience helper function.
fn witness_pubkey_hash_from_proto(
    pubkey_or_hash: &Proto::ToPublicKeyOrHash,
) -> Result<WPubkeyHash> {
    let wpubkey_hash = match &pubkey_or_hash.to_address {
        ProtoPubkeyOrHash::hash(hash) => WPubkeyHash::from_slice(hash.as_ref())
            .map_err(|_| Error::from(Proto::Error::Error_invalid_witness_pubkey_hash))?,
        ProtoPubkeyOrHash::pubkey(pubkey) => bitcoin::PublicKey::from_slice(pubkey.as_ref())?
            .wpubkey_hash()
            .ok_or_else(|| Error::from(Proto::Error::Error_invalid_witness_pubkey_hash))?,
        ProtoPubkeyOrHash::None => todo!(),
    };

    Ok(wpubkey_hash)
}
