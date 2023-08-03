use bitcoin::blockdata::locktime::absolute::{Height, LockTime, Time};
use bitcoin::hashes::Hash;
use bitcoin::sighash::{EcdsaSighashType, Prevouts, SighashCache, TapSighashType};
use bitcoin::taproot::TapLeafHash;
use bitcoin::{OutPoint, Script, ScriptBuf, Sequence, Transaction, TxIn, TxOut, Txid, Witness};
use std::borrow::Cow;
use std::marker::PhantomData;
use tw_coin_entry::error::SigningResult;
use tw_proto::Utxo::Proto::{self, SighashType};

pub mod builder;
pub mod compiler;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone)]
pub enum Error {
    SighashFailed(bitcoin::sighash::Error),
    InvalidWpkhScriptPubkey,
    InvalidLeafHash,
    InvalidSighashType,
    InvalidLockTime,
    InvalidTxid,
    OnePrevoutOutOfBound,
}

impl From<bitcoin::sighash::Error> for Error {
    fn from(value: bitcoin::sighash::Error) -> Self {
        Error::SighashFailed(value)
    }
}
