use crate::claim::{ClaimLocation, TransactionSigner};
use crate::input::*;
use crate::output::*;
use crate::{Error, Result};
use bitcoin::blockdata::locktime::absolute::{Height, LockTime};
use bitcoin::consensus::Encodable;
use bitcoin::sighash::{EcdsaSighashType, SighashCache, TapSighashType};
use bitcoin::taproot::{LeafVersion, TapLeafHash};
use bitcoin::transaction::Transaction;
use bitcoin::{secp256k1, Address, ScriptBuf, TxIn, TxOut, Witness};
use secp256k1::hashes::Hash;
use std::borrow::Cow;
use tw_misc::traits::ToBytesVec;
use tw_proto::Utxo::Proto::{self, SighashType};
use tw_utxo::compiler::{Compiler, StandardBitcoinContext};

#[derive(Debug, Clone)]
pub struct TransactionBuilder {
    pub version: i32,
    pub lock_time: LockTime,
    inputs: Vec<TxInput>,
    outputs: Vec<TxOutput>,
    miner_fee: Option<u64>,
    return_address: Option<Address>,
    contains_taproot: bool,
}

impl Default for TransactionBuilder {
    fn default() -> Self {
        TransactionBuilder {
            version: 2,
            // No lock time, transaction is immediately spendable.
            lock_time: LockTime::Blocks(Height::ZERO),
            inputs: vec![],
            outputs: vec![],
            miner_fee: None,
            return_address: None,
            contains_taproot: false,
        }
    }
}

impl TransactionBuilder {
    pub fn new() -> Self {
        Self::default()
    }
    pub fn version(mut self, version: i32) -> Self {
        self.version = version;
        self
    }
    pub fn lock_time_native(mut self, lock_time: LockTime) -> Self {
        self.lock_time = lock_time;
        self
    }
    pub fn lock_time_height(mut self, height: u32) -> Result<Self> {
        self.lock_time = LockTime::Blocks(Height::from_consensus(height).map_err(|_| Error::Todo)?);
        Ok(self)
    }
    pub fn return_address(mut self, address: Address) -> Self {
        self.return_address = Some(address);
        self
    }
    pub fn miner_fee(mut self, satoshis: u64) -> Self {
        self.miner_fee = Some(satoshis);
        self
    }
    pub fn add_input(mut self, input: TxInput) -> Self {
        match input {
            TxInput::P2TRKeyPath(_) | TxInput::P2TRScriptPath(_) => self.contains_taproot = true,
            _ => {},
        }

        self.inputs.push(input);
        self
    }
    pub fn add_output(mut self, output: TxOutput) -> Self {
        self.outputs.push(output);
        self
    }
    pub fn sign_inputs<S>(self, signer: S) -> Result<TransactionSigned>
    where
        S: TransactionSigner,
    {
        self.sign_inputs_fn(|input, sighash| match input {
            TxInput::P2PKH(p) => signer.claim_p2pkh(p, sighash, EcdsaSighashType::All),
            TxInput::P2WPKH(p) => signer.claim_p2wpkh(p, sighash, EcdsaSighashType::All),
            TxInput::P2TRKeyPath(p) => {
                signer.claim_p2tr_key_path(p, sighash, TapSighashType::Default)
            },
            TxInput::P2TRScriptPath(p) => {
                signer.claim_p2tr_script_path(p, sighash, TapSighashType::Default)
            },
        })
    }
    pub fn sign_inputs_fn<F>(self, signer: F) -> Result<TransactionSigned>
    where
        F: Fn(&TxInput, secp256k1::Message) -> Result<(ScriptBuf, Witness)>,
    {
        let mut signing = Proto::SigningInput {
            version: self.version,
            // TODO
            lock_time: Proto::mod_SigningInput::OneOflock_time::blocks(0),
            ..Default::default()
        };

        for input in &self.inputs {
            let (sighash, sighash_method) = match input {
                TxInput::P2PKH(_) => (
                    SighashType::All,
                    Proto::mod_TxIn::OneOfsighash_method::legacy(Proto::mod_TxIn::Legacy {
                        script_pubkey: input.ctx().script_pubkey.as_bytes().into(),
                    }),
                ),
                TxInput::P2WPKH(_) => (
                    SighashType::All,
                    Proto::mod_TxIn::OneOfsighash_method::segwit(Proto::mod_TxIn::Segwit {
                        value: input.ctx().value,
                        script_pubkey: input.ctx().script_pubkey.as_bytes().into(),
                    }),
                ),
                TxInput::P2TRKeyPath(_) => (
                    SighashType::UseDefault,
                    Proto::mod_TxIn::OneOfsighash_method::taproot(Proto::mod_TxIn::Taproot {
                        leaf_hash: Cow::default(),
                        prevout: Proto::mod_TxIn::mod_Taproot::OneOfprevout::all(
                            Proto::mod_TxIn::mod_Taproot::AllPrevouts {
                                all: self
                                    .inputs
                                    .iter()
                                    .map(|input| Proto::mod_TxIn::mod_Taproot::Prevout {
                                        value: input.ctx().value,
                                        script_pubkey: input.ctx().script_pubkey.as_bytes().into(),
                                    })
                                    .collect(),
                            },
                        ),
                    }),
                ),
                TxInput::P2TRScriptPath(p2tr) => {
                    (
                        SighashType::UseDefault,
                        Proto::mod_TxIn::OneOfsighash_method::taproot({
                            let leaf_hash =
                                TapLeafHash::from_script(p2tr.witness(), LeafVersion::TapScript);

                            Proto::mod_TxIn::Taproot {
                                // TODO: Can `to_vec()` be avoided?
                                leaf_hash: leaf_hash.as_byte_array().to_vec().into(),
                                prevout: Proto::mod_TxIn::mod_Taproot::OneOfprevout::all(
                                    Proto::mod_TxIn::mod_Taproot::AllPrevouts {
                                        all: self
                                            .inputs
                                            .iter()
                                            .map(|input| Proto::mod_TxIn::mod_Taproot::Prevout {
                                                value: input.ctx().value,
                                                script_pubkey: input
                                                    .ctx()
                                                    .script_pubkey
                                                    .as_bytes()
                                                    .into(),
                                            })
                                            .collect(),
                                    },
                                ),
                            }
                        }),
                    )
                },
            };

            signing.inputs.push(Proto::TxIn {
                // TODO: Can `to_vec()` be avoided?
                txid: input.ctx().previous_output.txid.to_vec().into(),
                vout: input.ctx().previous_output.vout,
                amount: input.ctx().value,
                sighash,
                sighash_method,
            });
        }

        for output in &self.outputs {
            signing.outputs.push(Proto::TxOut {
                value: output.satoshis(),
                script_pubkey: output.script_pubkey().as_bytes().into(),
            });
        }

        let proto_output = Compiler::<StandardBitcoinContext>::preimage_hashes(&signing);
        assert_eq!(proto_output.error, Proto::Error::OK);
        let sighashes = proto_output.sighashes;

        let mut claims = vec![];
        for (input, sighash) in self.inputs.iter().zip(sighashes.iter()) {
            let message = secp256k1::Message::from_slice(sighash)
                .expect("Sighash must always convert to secp256k1::Message");

            // TODO: This should call the methods directly.
            let (script_sig, witness) = signer(input, message)?;

            // Prepare witness items.
            let items = witness
                .into_iter()
                .map(|item| Cow::Owned(item.to_vec()))
                .collect();

            claims.push(Proto::TxInClaim {
                txid: input.ctx().previous_output.txid.to_vec().into(),
                vout: input.ctx().previous_output.vout,
                sequence: input.ctx().sequence.to_consensus_u32(),
                script_sig: script_sig.to_bytes().into(),
                witness_items: items,
            })
        }

        let signing = Proto::PreSerialization {
            version: signing.version,
            inputs: claims,
            outputs: signing.outputs,
            lock_time: Proto::mod_PreSerialization::OneOflock_time::blocks(0),
        };

        let proto_output = Compiler::<StandardBitcoinContext>::compile(&signing);
        assert_eq!(proto_output.error, Proto::Error::OK);

        Ok(TransactionSigned {
            inner: proto_output.encoded.to_vec(),
        })
    }
}

// TODO: Deprecate
pub struct TransactionSigned {
    pub inner: Vec<u8>,
}

impl TransactionSigned {
    pub fn serialize(&self) -> Result<Vec<u8>> {
        Ok(self.inner.to_vec())
    }
}
