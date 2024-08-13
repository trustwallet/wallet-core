// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use crate::message::external_message::wallet_v4::ExternalMessageWalletV4;
use crate::message::external_message::wallet_v5::{ExternalMessageWalletV5, V5R1OpCode};
use crate::message::internal_message::InternalMessage;
use crate::message::out_list::out_action::{OutAction, OutActionType};
use crate::message::signed_message::SignedMessage;
use crate::resources::{BASE_WORKCHAIN, DEFAULT_WALLET_ID, WALLET_ID_V5R1_TON_MAINNET};
use crate::transaction::SignedTransaction;
use tw_coin_entry::error::prelude::*;
use tw_hash::H256;
use tw_keypair::ed25519::sha512::{KeyPair, PrivateKey, PublicKey};
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait};
use tw_number::U256;
use tw_proto::TheOpenNetwork::Proto;
use tw_proto::TheOpenNetwork::Proto::WalletVersion::WALLET_V4_R2;
use tw_proto::TheOpenNetwork::Proto::WalletVersion::WALLET_V5_R1;
use tw_ton_sdk::cell::{Cell, CellArc};
use tw_ton_sdk::error::{cell_to_signing_error, CellError, CellErrorType, CellResult};
use tw_ton_sdk::message::state_init::StateInit;

pub mod wallet_v4;
pub mod wallet_v5;

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

/// Notes: Rust specialization for generic code is not completed. See https://github.com/rust-lang/rust/issues/31844
/// Currently, we use a workaround to implement the versioned TonWallet struct.
/// After the Rust specialization feature is finished, maybe we can remove this workaround.
pub enum VersionedTonWallet {
    V4(TonWallet<wallet_v4::WalletV4>),
    V5(TonWallet<wallet_v5::WalletV5>),
}

impl VersionedTonWallet {
    /// Creates a standard TON wallet from the given public key.
    /// Please note when created with public key only, wallet cannot be used to sign messages.
    pub fn std_with_public_key(
        wallet_version: Proto::WalletVersion,
        public_key: PublicKey,
    ) -> CellResult<Self> {
        let wallet_id = Self::determine_wallet_id(wallet_version)?;
        Self::with_public_key(BASE_WORKCHAIN, wallet_version, public_key, wallet_id)
    }

    /// Creates a standard TON wallet from the given key-pair.
    pub fn std_with_key_pair(
        wallet_version: Proto::WalletVersion,
        key_pair: &KeyPair,
    ) -> CellResult<Self> {
        let wallet_id = Self::determine_wallet_id(wallet_version)?;
        Self::with_key_pair(BASE_WORKCHAIN, wallet_version, key_pair, wallet_id)
    }

    /// Creates a TON wallet from the given public key.
    /// Please note when created with public key only, wallet cannot be used to sign messages.
    pub fn with_public_key(
        workchain: i32,
        wallet_version: Proto::WalletVersion,
        public_key: PublicKey,
        wallet_id: i32,
    ) -> CellResult<Self> {
        Self::new(workchain, wallet_version, public_key, None, wallet_id)
    }

    /// Creates a TON wallet from the given key-pair.
    pub fn with_key_pair(
        workchain: i32,
        wallet_version: Proto::WalletVersion,
        key_pair: &KeyPair,
        wallet_id: i32,
    ) -> CellResult<Self> {
        let public = key_pair.public().clone();
        let private = key_pair.private().clone();
        Self::new(workchain, wallet_version, public, Some(private), wallet_id)
    }

    pub fn address(&self) -> &TonAddress {
        match self {
            Self::V4(wallet_v4) => &wallet_v4.address,
            Self::V5(wallet_v5) => &wallet_v5.address,
        }
    }

    pub fn state_init(&self) -> CellResult<StateInit> {
        match self {
            Self::V4(wallet_v4) => Self::state_init_impl(
                &wallet_v4.version,
                &wallet_v4.public_key,
                wallet_v4.wallet_id,
            ),
            Self::V5(wallet_v5) => Self::state_init_impl(
                &wallet_v5.version,
                &wallet_v5.public_key,
                wallet_v5.wallet_id,
            ),
        }
    }

    pub fn create_external_body(
        &self,
        expire_at: u32,
        seqno: u32,
        internal_messages: Vec<InternalMessage>,
    ) -> CellResult<Cell> {
        match self {
            Self::V4(wallet_v4) => {
                wallet_v4.create_external_body(expire_at, seqno, internal_messages)
            },
            Self::V5(wallet_v5) => {
                wallet_v5.create_external_body(expire_at, seqno, internal_messages)
            },
        }
    }

    pub fn sign_external_message(&self, external_message: Cell) -> SigningResult<SignedMessage> {
        let message_hash = external_message.cell_hash();
        let sig = match self {
            Self::V4(wallet_v4) => wallet_v4.private_key.as_ref(),
            Self::V5(wallet_v5) => wallet_v5.private_key.as_ref(),
        }
        .or_tw_err(SigningErrorType::Error_internal)
        .context("'TonWallet' should be initialized with a key-pair to be able to sign a message")?
        .sign(message_hash.to_vec())?;
        Ok(SignedMessage {
            signature: sig.to_bytes(),
            external_message,
        })
    }

    pub fn sign_transaction(
        &self,
        external_message: Cell,
        state_init: bool,
    ) -> SigningResult<SignedTransaction> {
        let state_init = if state_init {
            let state_init = self.state_init().map_err(cell_to_signing_error)?;
            Some(state_init)
        } else {
            None
        };

        let signed_body = self.sign_external_message(external_message)?;
        Ok(SignedTransaction {
            src_address: TonAddress::null(),
            // The wallet contract address.
            dest_address: self.address().clone(),
            import_fee: U256::zero(),
            state_init,
            signed_body,
            wallet_version: match self {
                Self::V4(_) => WALLET_V4_R2,
                Self::V5(_) => WALLET_V5_R1,
            },
        })
    }

    /// Private function to create the VersionedTonWallet with the given public and optional private keys.
    /// Do not make it public as the function caller can provide unrelated keys.
    fn new(
        workchain: i32,
        wallet_version: Proto::WalletVersion,
        public_key: PublicKey,
        private_key: Option<PrivateKey>,
        wallet_id: i32,
    ) -> CellResult<Self> {
        match wallet_version {
            WALLET_V4_R2 => Ok(VersionedTonWallet::V4(
                TonWallet::<wallet_v4::WalletV4>::new(
                    workchain,
                    public_key,
                    private_key,
                    wallet_id,
                )?,
            )),

            WALLET_V5_R1 => Ok(VersionedTonWallet::V5(
                TonWallet::<wallet_v5::WalletV5>::new(
                    workchain,
                    public_key,
                    private_key,
                    wallet_id,
                )?,
            )),

            _ => CellError::err(CellErrorType::NotSupported).context("Unsupported wallet version"),
        }
    }

    pub fn state_init_impl<Version: WalletVersion>(
        version: &Version,
        public_key: &PublicKey,
        wallet_id: i32,
    ) -> CellResult<StateInit> {
        let public_key_bytes = public_key.to_bytes();
        let initial_data = version
            .initial_data(wallet_id, public_key_bytes)?
            .into_arc();
        let code = version.code()?;
        Ok(StateInit::default().set_code(code).set_data(initial_data))
    }

    /// Determines the wallet ID based on the wallet version.
    fn determine_wallet_id(wallet_version: Proto::WalletVersion) -> CellResult<i32> {
        match wallet_version {
            WALLET_V4_R2 => Ok(DEFAULT_WALLET_ID),
            WALLET_V5_R1 => Ok(WALLET_ID_V5R1_TON_MAINNET),
            _ => CellError::err(CellErrorType::NotSupported).context("Unsupported wallet version"),
        }
    }
}

impl TonWallet<wallet_v4::WalletV4> {
    fn new(
        workchain: i32,
        public_key: PublicKey,
        private_key: Option<PrivateKey>,
        wallet_id: i32,
    ) -> CellResult<Self> {
        let version = wallet_v4::WalletV4::r2()?;
        let state_init_hash = VersionedTonWallet::state_init_impl::<wallet_v4::WalletV4>(
            &version,
            &public_key,
            wallet_id,
        )?
        .create_account_id()?;
        let address = TonAddress::new(workchain, state_init_hash);
        Ok(TonWallet::<wallet_v4::WalletV4> {
            public_key,
            private_key,
            version,
            address,
            wallet_id,
        })
    }

    pub fn create_external_body(
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

impl TonWallet<wallet_v5::WalletV5> {
    fn new(
        workchain: i32,
        public_key: PublicKey,
        private_key: Option<PrivateKey>,
        wallet_id: i32,
    ) -> CellResult<Self> {
        let version = wallet_v5::WalletV5::r1()?;
        let state_init_hash = VersionedTonWallet::state_init_impl::<wallet_v5::WalletV5>(
            &version,
            &public_key,
            wallet_id,
        )?
        .create_account_id()?;
        let address = TonAddress::new(workchain, state_init_hash);
        Ok(TonWallet::<wallet_v5::WalletV5> {
            public_key,
            private_key,
            version,
            address,
            wallet_id,
        })
    }

    pub fn create_external_body(
        &self,
        expire_at: u32,
        seqno: u32,
        internal_messages: Vec<InternalMessage>,
    ) -> CellResult<Cell> {
        // Convert internal_messages to basic_actions
        let basic_actions: Vec<OutAction> = internal_messages
            .into_iter()
            .map(|msg| OutAction {
                typ: OutActionType::SendMsg,
                mode: msg.mode,
                data: msg.message,
            })
            .collect();

        ExternalMessageWalletV5 {
            opcode: V5R1OpCode::AuthSignedExternal,
            wallet_id: self.wallet_id,
            expire_at,
            seqno,
            basic_actions,
        }
        .build()
    }
}
