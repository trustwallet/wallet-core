// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use crate::wallet::wallet_v4::WalletV4;
use crate::wallet::TonWallet;
use tw_number::U256;

pub mod builder;
pub mod cell_creator;

pub enum TransferPayload {
    /// Jetton Transfer message payload.
    JettonTransfer(JettonTransferRequest),
    /// Custom Transfer message payload.
    Custom(TransferCustomRequest),
}

pub struct TransferRequest {
    /// TON recipient address.
    /// Also determines whether the transaction is bounceable or not.
    pub dest: TonAddress,
    /// Amount to send in nanotons.
    pub ton_amount: U256,
    /// Send mode.
    /// https://ton.org/docs/develop/func/stdlib#send_raw_message
    pub mode: u8,
    /// Transfer comment message.
    pub comment: Option<String>,
    /// Transfer payload.
    pub payload: Option<TransferPayload>,
}

pub struct JettonTransferRequest {
    /// Arbitrary request number.
    pub query_id: u64,
    /// Amount of transferred jettons in elementary integer units.
    /// The real value transferred is `jetton_amount` multiplied by ten to the power of token decimal precision.
    pub jetton_amount: U256,
    /// Address of the new owner of the jettons.
    pub dest: TonAddress,
    /// Address where to send a response with confirmation of a successful transfer and the rest of the incoming message Toncoins.
    /// Usually the sender should get back their toncoins.
    pub response_address: TonAddress,
    /// Amount in nanotons to forward to recipient. Basically minimum amount - 1 nanoton should be used.
    pub forward_ton_amount: U256,
}

pub struct TransferCustomRequest {
    /// (string base64, optional): raw one-cell BoC encoded in Base64.
    /// Can be used to deploy a smart contract.
    pub state_init: Option<String>,
    /// (string base64, optional): raw one-cell BoC encoded in Base64.
    pub payload: Option<String>,
}

pub struct SigningRequest {
    /// Wallet initialized with the user's key-pair or public key.
    pub wallet: TonWallet<WalletV4>,
    pub messages: Vec<TransferRequest>,
    /// External message counter.
    /// https://ton.org/docs/develop/smart-contracts/guidelines/external-messages
    pub seqno: u32,
    /// Expiration UNIX timestamp.
    pub expire_at: u32,
}
