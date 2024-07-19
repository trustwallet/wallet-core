// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use crate::resources::{BASE_WORKCHAIN, DEFAULT_WALLET_ID};
use tw_hash::H256;
use tw_keypair::ed25519::sha512::{KeyPair, PrivateKey, PublicKey};
use tw_keypair::traits::KeyPairTrait;
use tw_ton_sdk::cell::{Cell, CellArc};
use tw_ton_sdk::error::CellResult;
use tw_ton_sdk::message::state_init::StateInit;

pub mod wallet_v4;

pub trait WalletVersion {
    /// Returns data that is (will be) stored in the wallet smart contract
    /// when it is first deployed to the blockchain.
    fn initial_data(&self, wallet_id: i32, public_key: H256) -> CellResult<Cell>;

    /// Returns this wallet specific version contract code as a Cell.
    fn code(&self) -> CellResult<CellArc>;
}

pub struct TonWallet<Version> {
    public_key: PublicKey,
    private_key: Option<PrivateKey>,
    version: Version,
    /// TON address derived from the [`TonWallet::public_key`].
    address: TonAddress,
    wallet_id: i32,
}

impl TonWallet<wallet_v4::WalletV4> {
    /// Creates a standard TON wallet from the given public key.
    /// Please note when created with public key only, wallet cannot be used to sign messages.
    pub fn std_with_public_key(public_key: PublicKey) -> CellResult<Self> {
        Self::with_public_key(
            BASE_WORKCHAIN,
            wallet_v4::WalletV4::r2()?,
            public_key,
            DEFAULT_WALLET_ID,
        )
    }

    /// Creates a standard TON wallet from the given key-pair.
    pub fn std_with_key_pair(key_pair: &KeyPair) -> CellResult<Self> {
        Self::with_key_pair(
            BASE_WORKCHAIN,
            wallet_v4::WalletV4::r2()?,
            key_pair,
            DEFAULT_WALLET_ID,
        )
    }
}

impl<Version: WalletVersion> TonWallet<Version> {
    /// Creates a TON wallet from the given public key.
    /// Please note when created with public key only, wallet cannot be used to sign messages.
    pub fn with_public_key(
        workchain: i32,
        version: Version,
        public_key: PublicKey,
        wallet_id: i32,
    ) -> CellResult<Self> {
        Self::new(workchain, version, public_key, None, wallet_id)
    }

    /// Creates a TON wallet from the given key-pair.
    pub fn with_key_pair(
        workchain: i32,
        version: Version,
        key_pair: &KeyPair,
        wallet_id: i32,
    ) -> CellResult<Self> {
        let public = key_pair.public().clone();
        let private = key_pair.private().clone();
        Self::new(workchain, version, public, Some(private), wallet_id)
    }

    pub fn address(&self) -> &TonAddress {
        &self.address
    }

    /// Private function to create the TonWallet with the given public and optional private keys.
    /// Do not make it public as the function caller can provide unrelated keys.
    fn new(
        workchain: i32,
        version: Version,
        public_key: PublicKey,
        private_key: Option<PrivateKey>,
        wallet_id: i32,
    ) -> CellResult<Self> {
        let public_key_bytes = public_key.to_bytes();

        let initial_data = version
            .initial_data(wallet_id, public_key_bytes)?
            .into_arc();
        let code = version.code()?;
        let state_init_hash = StateInit::default()
            .set_code(code)
            .set_data(initial_data)
            .create_account_id()?;

        let address = TonAddress::new(workchain, state_init_hash);
        Ok(TonWallet {
            public_key,
            private_key,
            version,
            address,
            wallet_id,
        })
    }
}
