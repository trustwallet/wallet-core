// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use crate::message::external_message::wallet_v4::ExternalMessageWalletV4;
use crate::message::internal_message::InternalMessage;
use crate::resources::{BASE_WORKCHAIN, DEFAULT_WALLET_ID, WALLET_V4R2_CODE};
use std::sync::Arc;
use tw_keypair::ed25519::sha512::{KeyPair, PrivateKey, PublicKey};
use tw_keypair::traits::KeyPairTrait;
use tw_ton_sdk::cell::cell_builder::CellBuilder;
use tw_ton_sdk::cell::Cell;
use tw_ton_sdk::error::CellResult;
use tw_ton_sdk::message::state_init::StateInit;

pub struct WalletV4R2 {
    pub public_key: PublicKey,
    pub(crate) private_key: Option<PrivateKey>,
    /// TON address derived from the [`TonWallet::public_key`].
    pub address: TonAddress,
    pub wallet_id: i32,
}

impl WalletV4R2 {
    /// Creates a standard TON wallet from the given public key.
    /// Please note when created with public key only, wallet cannot be used to sign messages.
    pub fn std_with_public_key(public_key: PublicKey) -> CellResult<Self> {
        let wallet_id = DEFAULT_WALLET_ID;
        Self::with_public_key(BASE_WORKCHAIN, public_key, wallet_id)
    }

    /// Creates a standard TON wallet from the given key-pair.
    pub fn std_with_key_pair(key_pair: &KeyPair) -> CellResult<Self> {
        let wallet_id = DEFAULT_WALLET_ID;
        Self::with_key_pair(BASE_WORKCHAIN, key_pair, wallet_id)
    }

    /// Creates a TON wallet from the given public key.
    /// Please note when created with public key only, wallet cannot be used to sign messages.
    pub(crate) fn with_public_key(
        workchain: i32,
        public_key: PublicKey,
        wallet_id: i32,
    ) -> CellResult<Self> {
        Self::new(workchain, public_key, None, wallet_id)
    }

    /// Creates a TON wallet from the given key-pair.
    fn with_key_pair(workchain: i32, key_pair: &KeyPair, wallet_id: i32) -> CellResult<Self> {
        let public = key_pair.public().clone();
        let private = key_pair.private().clone();
        Self::new(workchain, public, Some(private), wallet_id)
    }

    /// Private function to create the VersionedTonWallet with the given public and optional private keys.
    /// Do not make it public as the function caller can provide unrelated keys.
    fn new(
        workchain: i32,
        public_key: PublicKey,
        private_key: Option<PrivateKey>,
        wallet_id: i32,
    ) -> CellResult<Self> {
        let state_init_hash = Self::state_init_impl(&public_key, wallet_id)?.create_account_id()?;
        let address = TonAddress::new(workchain, state_init_hash);

        Ok(Self {
            public_key,
            private_key,
            address,
            wallet_id,
        })
    }

    /// Return the stateInit for the wallet.
    pub fn state_init(&self) -> CellResult<StateInit> {
        Self::state_init_impl(&self.public_key, self.wallet_id)
    }

    fn state_init_impl(public_key: &PublicKey, wallet_id: i32) -> CellResult<StateInit> {
        let seqno = 0;

        let mut builder = CellBuilder::new();
        builder
            .store_u32(32, seqno)?
            .store_i32(32, wallet_id)?
            .store_slice(public_key.as_slice())?
            // empty plugin dict
            .store_bit(false)?;

        let initial_data = builder.build()?.into_arc();
        let code = WALLET_V4R2_CODE.single_root().map(Arc::clone)?;

        Ok(StateInit::default().set_code(code).set_data(initial_data))
    }

    pub(crate) fn create_external_body(
        &self,
        expire_at: u32,
        seqno: u32,
        internal_messages: Vec<InternalMessage>,
    ) -> CellResult<Cell> {
        ExternalMessageWalletV4 {
            wallet_id: self.wallet_id,
            expire_at,
            seqno,
            internal_messages,
        }
        .build()
    }
}
