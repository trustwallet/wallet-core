// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use crate::wallet::wallet_v4::WalletV4;
use crate::wallet::TonWallet;
use tw_number::U256;

pub mod builder;

pub struct TransferRequest {
    /// TON recipient address.
    /// Also determines whether the transaction is bounceable or not.
    pub dest: TonAddress,
    /// Amount to send in nanotons.
    pub ton_amount: U256,
    /// Message counter.
    /// https://ton.org/docs/develop/smart-contracts/guidelines/external-messages
    pub seqno: u32,
    /// Send mode.
    /// https://ton.org/docs/develop/func/stdlib#send_raw_message
    pub mode: u8,
    /// Expiration UNIX timestamp.
    pub expire_at: u32,
    /// Transfer comment message.
    pub comment: Option<String>,
}

pub struct SigningRequest {
    /// Wallet initialized with the user's key-pair or public key.
    pub wallet: TonWallet<WalletV4>,
    pub transfer_request: TransferRequest,
}
