use crate::entry::aliases::*;
use crate::Result;
use tw_proto::BitcoinV2::Proto;
use tw_proto::Utxo::Proto as UtxoProto;

use crate::brc20::{BRC20TransferInscription, Ticker};
use bitcoin::address::{NetworkChecked, Payload};
use bitcoin::key::{TapTweak, TweakedKeyPair};
use bitcoin::taproot::{ControlBlock, LeafVersion, TapLeafHash, TapNodeHash};
use bitcoin::{PubkeyHash, ScriptBuf, WPubkeyHash, Witness};
use secp256k1::hashes::Hash;
use secp256k1::{KeyPair, Message, Secp256k1, XOnlyPublicKey};
use std::borrow::Cow;
use std::fmt::Display;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::{CoinAddress, CoinEntry, PublicKeyBytes, SignatureBytes};
use tw_coin_entry::derivation::Derivation;
use tw_coin_entry::error::AddressResult;
use tw_coin_entry::modules::json_signer::JsonSigner;
use tw_coin_entry::modules::plan_builder::NoPlanBuilder;
use tw_coin_entry::prefix::NoPrefix;
use tw_keypair::tw::{PrivateKey, PublicKey, PublicKeyType};
use tw_misc::traits::ToBytesVec;

pub struct InputBuilder {}

impl InputBuilder {
    pub fn txin_from_proto<'a>(input: &Proto::Input<'a>) -> Result<UtxoProto::TxIn<'static>> {
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

        let proto = UtxoProto::TxIn {
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

        Ok(proto)
    }
}
