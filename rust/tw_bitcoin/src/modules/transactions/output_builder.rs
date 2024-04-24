use std::str::FromStr;

use super::brc20::{BRC20TransferInscription, Brc20Ticker};
use super::OrdinalNftInscription;
use crate::aliases::*;
use crate::{Error, Result};
use bitcoin::address::{Payload, WitnessVersion};
use bitcoin::key::TweakedPublicKey;
use bitcoin::taproot::{LeafVersion, TapNodeHash};
use bitcoin::{Address, PubkeyHash, ScriptBuf, ScriptHash, WPubkeyHash, WScriptHash};
use secp256k1::hashes::Hash;
use secp256k1::XOnlyPublicKey;
use tw_misc::traits::ToBytesVec;
use tw_proto::BitcoinV2::Proto;

pub struct OutputBuilder;

// Convenience varibles used solely for readability.
const NO_CONTROL_BLOCK: Option<Vec<u8>> = None;
const NO_TAPROOT_PAYLOAD: Option<Vec<u8>> = None;

impl OutputBuilder {
    /// Creates the spending condition (_scriptPubkey_) for a given output.
    pub fn utxo_from_proto(
        output: &Proto::Output<'_>,
    ) -> Result<Proto::mod_PreSigningOutput::TxOut<'static>> {
        let secp = secp256k1::Secp256k1::new();

        let (script_pubkey, control_block, taproot_payload) = match &output.to_recipient {
            // Script spending condition was passed on directly.
            ProtoOutputRecipient::custom_script_pubkey(script) => (
                ScriptBuf::from_bytes(script.to_vec()),
                NO_CONTROL_BLOCK,
                NO_TAPROOT_PAYLOAD,
            ),
            // Process builder methods. We construct the Script spending
            // conditions by using the specified parameters.
            ProtoOutputRecipient::builder(builder) => match &builder.variant {
                ProtoOutputBuilder::p2sh(script_or_hash) => {
                    let script_hash = redeem_script_or_hash(script_or_hash)?;
                    (
                        ScriptBuf::new_p2sh(&script_hash),
                        NO_CONTROL_BLOCK,
                        NO_TAPROOT_PAYLOAD,
                    )
                },
                ProtoOutputBuilder::p2pkh(pubkey_or_hash) => {
                    let pubkey_hash = pubkey_hash_from_proto(pubkey_or_hash)?;
                    (
                        ScriptBuf::new_p2pkh(&pubkey_hash),
                        NO_CONTROL_BLOCK,
                        NO_TAPROOT_PAYLOAD,
                    )
                },
                ProtoOutputBuilder::p2wsh(script_or_hash) => {
                    let wscript_hash = witness_redeem_script_or_hash(script_or_hash)?;
                    (
                        ScriptBuf::new_v0_p2wsh(&wscript_hash),
                        NO_CONTROL_BLOCK,
                        NO_TAPROOT_PAYLOAD,
                    )
                },
                ProtoOutputBuilder::p2wpkh(pubkey_or_hash) => {
                    let wpubkey_hash = witness_pubkey_hash_from_proto(pubkey_or_hash)?;
                    (
                        ScriptBuf::new_v0_p2wpkh(&wpubkey_hash),
                        NO_CONTROL_BLOCK,
                        NO_TAPROOT_PAYLOAD,
                    )
                },
                ProtoOutputBuilder::p2tr_key_path(pubkey) => {
                    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_ref())?;
                    let xonly = XOnlyPublicKey::from(pubkey.inner);
                    (
                        ScriptBuf::new_v1_p2tr(&secp, xonly, None),
                        NO_CONTROL_BLOCK,
                        NO_TAPROOT_PAYLOAD,
                    )
                },
                ProtoOutputBuilder::p2tr_script_path(complex) => {
                    let node_hash = TapNodeHash::from_slice(complex.merkle_root.as_ref())
                        .map_err(|_| Error::from(Proto::Error::Error_invalid_taproot_root))?;

                    let pubkey = bitcoin::PublicKey::from_slice(complex.internal_key.as_ref())?;
                    let xonly = XOnlyPublicKey::from(pubkey.inner);

                    (
                        ScriptBuf::new_v1_p2tr(&secp, xonly, Some(node_hash)),
                        NO_CONTROL_BLOCK,
                        NO_TAPROOT_PAYLOAD,
                    )
                },
                ProtoOutputBuilder::p2tr_dangerous_assume_tweaked(tweaked_pubkey) => {
                    let xonly = XOnlyPublicKey::from_slice(tweaked_pubkey).map_err(|_| {
                        Error::from(Proto::Error::Error_invalid_taproot_tweaked_pubkey)
                    })?;

                    let tweaked = TweakedPublicKey::dangerous_assume_tweaked(xonly);

                    (
                        ScriptBuf::new_v1_p2tr_tweaked(tweaked),
                        NO_CONTROL_BLOCK,
                        NO_TAPROOT_PAYLOAD,
                    )
                },
                ProtoOutputBuilder::ordinal_inscribe(ordinal) => {
                    let pubkey = bitcoin::PublicKey::from_slice(ordinal.inscribe_to.as_ref())?;
                    let xonly = XOnlyPublicKey::from(pubkey.inner);
                    let mime_type = ordinal.mime_type.as_ref();
                    let data = ordinal.payload.as_ref();

                    let nft = OrdinalNftInscription::new(mime_type.as_bytes(), data, pubkey)
                        .expect("badly constructed Ordinal inscription");

                    // Construct the control block.
                    let control_block = nft
                        .inscription()
                        .spend_info()
                        .control_block(&(
                            nft.inscription().taproot_program().to_owned(),
                            LeafVersion::TapScript,
                        ))
                        .expect("badly constructed control block");

                    // Construct the merkle root.
                    let merkle_root = nft
                        .inscription()
                        .spend_info()
                        .merkle_root()
                        .expect("badly constructed Taproot merkle root");

                    (
                        ScriptBuf::new_v1_p2tr(&secp, xonly, Some(merkle_root)),
                        Some(control_block.serialize()),
                        Some(nft.inscription().taproot_program().to_vec()),
                    )
                },
                ProtoOutputBuilder::brc20_inscribe(brc20) => {
                    let pubkey = bitcoin::PublicKey::from_slice(brc20.inscribe_to.as_ref())?;
                    let xonly = XOnlyPublicKey::from(pubkey.inner);

                    let ticker = Brc20Ticker::new(brc20.ticker.to_string())?;
                    let transfer = BRC20TransferInscription::new(
                        pubkey,
                        ticker,
                        brc20.transfer_amount.to_string(),
                    )
                    .expect("invalid BRC20 transfer construction");

                    // Construct the control block.
                    let control_block = transfer
                        .inscription()
                        .spend_info()
                        .control_block(&(
                            transfer.inscription().taproot_program().to_owned(),
                            LeafVersion::TapScript,
                        ))
                        .expect("badly constructed control block");

                    // Construct the merkle root.
                    let merkle_root = transfer
                        .inscription()
                        .spend_info()
                        .merkle_root()
                        .expect("badly constructed Taproot merkle root");

                    (
                        ScriptBuf::new_v1_p2tr(&secp, xonly, Some(merkle_root)),
                        Some(control_block.serialize()),
                        Some(transfer.inscription().taproot_program().to_vec()),
                    )
                },
                ProtoOutputBuilder::None => {
                    return Err(Error::from(Proto::Error::Error_missing_output_builder))
                },
            },
            // We derive the transaction type from the address.
            ProtoOutputRecipient::from_address(addr) => {
                let proto = output_from_address(output.value, addr.as_ref())?;

                // Recursive call, will initiate the appropraite builder.
                return Self::utxo_from_proto(&proto);
            },
            ProtoOutputRecipient::None => {
                return Err(Error::from(Proto::Error::Error_missing_recipient))
            },
        };

        let utxo = Proto::mod_PreSigningOutput::TxOut {
            value: output.value,
            script_pubkey: script_pubkey.to_vec().into(),
            control_block: control_block.map(|cb| cb.into()).unwrap_or_default(),
            taproot_payload: taproot_payload.map(|cb| cb.into()).unwrap_or_default(),
        };

        Ok(utxo)
    }
}

// Convenience helper function.
fn redeem_script_or_hash(
    script_or_hash: &Proto::mod_Output::OutputRedeemScriptOrHash,
) -> Result<ScriptHash> {
    let pubkey_hash = match &script_or_hash.variant {
        ProtoRedeemScriptOrHash::hash(hash) => ScriptHash::from_slice(hash.as_ref())
            .map_err(|_| Error::from(Proto::Error::Error_invalid_redeem_script))?,
        ProtoRedeemScriptOrHash::redeem_script(script) => {
            ScriptBuf::from_bytes(script.to_vec()).script_hash()
        },
        ProtoRedeemScriptOrHash::None => {
            return Err(Error::from(Proto::Error::Error_missing_recipient))
        },
    };

    Ok(pubkey_hash)
}

// Convenience helper function.
fn witness_redeem_script_or_hash(
    script_or_hash: &Proto::mod_Output::OutputRedeemScriptOrHash,
) -> Result<WScriptHash> {
    let pubkey_hash = match &script_or_hash.variant {
        ProtoRedeemScriptOrHash::hash(hash) => WScriptHash::from_slice(hash)
            .map_err(|_| Error::from(Proto::Error::Error_invalid_witness_redeem_script_hash))?,
        ProtoRedeemScriptOrHash::redeem_script(script) => {
            ScriptBuf::from_bytes(script.to_vec()).wscript_hash()
        },
        ProtoRedeemScriptOrHash::None => {
            return Err(Error::from(Proto::Error::Error_missing_recipient))
        },
    };

    Ok(pubkey_hash)
}

// Convenience helper function.
fn pubkey_hash_from_proto(pubkey_or_hash: &Proto::ToPublicKeyOrHash) -> Result<PubkeyHash> {
    let pubkey_hash = match &pubkey_or_hash.to_address {
        ProtoPubkeyOrHash::hash(hash) => PubkeyHash::from_slice(hash.as_ref())
            .map_err(|_| Error::from(Proto::Error::Error_invalid_pubkey_hash))?,
        ProtoPubkeyOrHash::pubkey(pubkey) => {
            bitcoin::PublicKey::from_slice(pubkey.as_ref())?.pubkey_hash()
        },
        ProtoPubkeyOrHash::None => return Err(Error::from(Proto::Error::Error_missing_recipient)),
    };

    Ok(pubkey_hash)
}

// Convenience helper function.
fn witness_pubkey_hash_from_proto(
    pubkey_or_hash: &Proto::ToPublicKeyOrHash,
) -> Result<WPubkeyHash> {
    let wpubkey_hash = match &pubkey_or_hash.to_address {
        ProtoPubkeyOrHash::hash(hash) => WPubkeyHash::from_slice(hash.as_ref())
            .map_err(|_| Error::from(Proto::Error::Error_invalid_witness_pubkey_hash))?,
        ProtoPubkeyOrHash::pubkey(pubkey) => bitcoin::PublicKey::from_slice(pubkey.as_ref())?
            .wpubkey_hash()
            .ok_or_else(|| Error::from(Proto::Error::Error_invalid_witness_pubkey_hash))?,
        ProtoPubkeyOrHash::None => return Err(Error::from(Proto::Error::Error_missing_recipient)),
    };

    Ok(wpubkey_hash)
}

// Derives the P2* output from the given address.
fn output_from_address(value: u64, addr: &str) -> Result<Proto::Output<'static>> {
    let string = String::from_utf8(addr.to_vec())
        .map_err(|_| Error::from(Proto::Error::Error_bad_address_recipient))?;

    let addr = Address::from_str(&string)
        .map_err(|_| Error::from(Proto::Error::Error_bad_address_recipient))?
        .require_network(bitcoin::Network::Bitcoin)
        .map_err(|_| Error::from(Proto::Error::Error_bad_address_recipient))?;

    let proto = match addr.payload {
        // Identified a "PubkeyHash" address (i.e. P2PKH).
        Payload::PubkeyHash(pubkey_hash) => Proto::Output {
            value,
            to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
                variant: ProtoOutputBuilder::p2pkh(Proto::ToPublicKeyOrHash {
                    to_address: ProtoPubkeyOrHash::hash(pubkey_hash.to_vec().into()),
                }),
            }),
        },
        // Identified a witness program (i.e. Segwit or Taproot).
        Payload::WitnessProgram(progam) => {
            match progam.version() {
                // Identified version 0, i.e. Segwit
                WitnessVersion::V0 => {
                    let payload = progam.program().as_bytes().to_vec();

                    // Check for P2WPKH.
                    if payload.len() == 20 {
                        return Ok(Proto::Output {
                            value,
                            to_recipient: ProtoOutputRecipient::builder(
                                Proto::mod_Output::OutputBuilder {
                                    variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
                                        to_address: ProtoPubkeyOrHash::hash(
                                            // Payload is the WPubkey hash.
                                            payload.into(),
                                        ),
                                    }),
                                },
                            ),
                        });
                    }

                    // Check for P2WSH.
                    if payload.len() == 32 {
                        return Ok(Proto::Output {
                            value,
                            to_recipient: ProtoOutputRecipient::builder(
                                Proto::mod_Output::OutputBuilder {
                                    variant: ProtoOutputBuilder::p2wsh(
                                        Proto::mod_Output::OutputRedeemScriptOrHash {
                                            variant: ProtoOutputRedeemScriptOrHashBuilder::hash(
                                                // Payload is the WScript hash.
                                                payload.to_vec().into(),
                                            ),
                                        },
                                    ),
                                },
                            ),
                        });
                    }

                    return Err(Error::from(Proto::Error::Error_bad_address_recipient));
                },
                // Identified version 1, i.e P2TR key-path (Taproot)
                WitnessVersion::V1 => {
                    let pubkey = progam.program().as_bytes().to_vec();
                    if pubkey.len() != 32 {
                        return Err(Error::from(Proto::Error::Error_bad_address_recipient));
                    }

                    Proto::Output {
                        value,
                        to_recipient: ProtoOutputRecipient::builder(
                            Proto::mod_Output::OutputBuilder {
                                variant: ProtoOutputBuilder::p2tr_dangerous_assume_tweaked(
                                    pubkey.into(),
                                ),
                            },
                        ),
                    }
                },
                _ => {
                    return Err(Error::from(
                        Proto::Error::Error_unsupported_address_recipient,
                    ))
                },
            }
        },
        Payload::ScriptHash(script_hash) => Proto::Output {
            value,
            to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
                variant: ProtoOutputBuilder::p2sh(Proto::mod_Output::OutputRedeemScriptOrHash {
                    variant: ProtoOutputRedeemScriptOrHashBuilder::hash(
                        script_hash.to_vec().into(),
                    ),
                }),
            }),
        },
        _ => {
            return Err(Error::from(
                Proto::Error::Error_unsupported_address_recipient,
            ))
        },
    };

    Ok(proto)
}
