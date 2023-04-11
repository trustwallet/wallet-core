// Automatically generated rust module for 'Solana.proto' file

#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(unused_imports)]
#![allow(unknown_lints)]
#![allow(clippy::all)]
#![cfg_attr(rustfmt, rustfmt_skip)]


use std::borrow::Cow;
use quick_protobuf::{MessageInfo, MessageRead, MessageWrite, BytesReader, Writer, WriterBackend, Result};
use quick_protobuf::sizeofs::*;
use super::super::super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transfer<'a> {
    pub recipient: Cow<'a, str>,
    pub value: u64,
    pub memo: Cow<'a, str>,
    pub references: Vec<Cow<'a, str>>,
}

impl<'a> MessageRead<'a> for Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.recipient = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.value = r.read_uint64(bytes)?,
                Ok(26) => msg.memo = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.references.push(r.read_string(bytes).map(Cow::Borrowed)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Transfer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.recipient == "" { 0 } else { 1 + sizeof_len((&self.recipient).len()) }
        + if self.value == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.value) as u64) }
        + if self.memo == "" { 0 } else { 1 + sizeof_len((&self.memo).len()) }
        + self.references.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.recipient != "" { w.write_with_tag(10, |w| w.write_string(&**&self.recipient))?; }
        if self.value != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.value))?; }
        if self.memo != "" { w.write_with_tag(26, |w| w.write_string(&**&self.memo))?; }
        for s in &self.references { w.write_with_tag(34, |w| w.write_string(&**s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DelegateStake<'a> {
    pub validator_pubkey: Cow<'a, str>,
    pub value: u64,
    pub stake_account: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for DelegateStake<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.validator_pubkey = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.value = r.read_uint64(bytes)?,
                Ok(26) => msg.stake_account = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DelegateStake<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.validator_pubkey == "" { 0 } else { 1 + sizeof_len((&self.validator_pubkey).len()) }
        + if self.value == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.value) as u64) }
        + if self.stake_account == "" { 0 } else { 1 + sizeof_len((&self.stake_account).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.validator_pubkey != "" { w.write_with_tag(10, |w| w.write_string(&**&self.validator_pubkey))?; }
        if self.value != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.value))?; }
        if self.stake_account != "" { w.write_with_tag(26, |w| w.write_string(&**&self.stake_account))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DeactivateStake<'a> {
    pub stake_account: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for DeactivateStake<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.stake_account = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DeactivateStake<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.stake_account == "" { 0 } else { 1 + sizeof_len((&self.stake_account).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.stake_account != "" { w.write_with_tag(10, |w| w.write_string(&**&self.stake_account))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DeactivateAllStake<'a> {
    pub stake_accounts: Vec<Cow<'a, str>>,
}

impl<'a> MessageRead<'a> for DeactivateAllStake<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.stake_accounts.push(r.read_string(bytes).map(Cow::Borrowed)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DeactivateAllStake<'a> {
    fn get_size(&self) -> usize {
        0
        + self.stake_accounts.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.stake_accounts { w.write_with_tag(10, |w| w.write_string(&**s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct WithdrawStake<'a> {
    pub stake_account: Cow<'a, str>,
    pub value: u64,
}

impl<'a> MessageRead<'a> for WithdrawStake<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.stake_account = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.value = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for WithdrawStake<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.stake_account == "" { 0 } else { 1 + sizeof_len((&self.stake_account).len()) }
        + if self.value == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.value) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.stake_account != "" { w.write_with_tag(10, |w| w.write_string(&**&self.stake_account))?; }
        if self.value != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.value))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct StakeAccountValue<'a> {
    pub stake_account: Cow<'a, str>,
    pub value: u64,
}

impl<'a> MessageRead<'a> for StakeAccountValue<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.stake_account = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.value = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for StakeAccountValue<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.stake_account == "" { 0 } else { 1 + sizeof_len((&self.stake_account).len()) }
        + if self.value == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.value) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.stake_account != "" { w.write_with_tag(10, |w| w.write_string(&**&self.stake_account))?; }
        if self.value != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.value))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct WithdrawAllStake<'a> {
    pub stake_accounts: Vec<TW::Solana::Proto::StakeAccountValue<'a>>,
}

impl<'a> MessageRead<'a> for WithdrawAllStake<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.stake_accounts.push(r.read_message::<TW::Solana::Proto::StakeAccountValue>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for WithdrawAllStake<'a> {
    fn get_size(&self) -> usize {
        0
        + self.stake_accounts.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.stake_accounts { w.write_with_tag(10, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct CreateTokenAccount<'a> {
    pub main_address: Cow<'a, str>,
    pub token_mint_address: Cow<'a, str>,
    pub token_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for CreateTokenAccount<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.main_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.token_mint_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.token_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for CreateTokenAccount<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.main_address == "" { 0 } else { 1 + sizeof_len((&self.main_address).len()) }
        + if self.token_mint_address == "" { 0 } else { 1 + sizeof_len((&self.token_mint_address).len()) }
        + if self.token_address == "" { 0 } else { 1 + sizeof_len((&self.token_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.main_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.main_address))?; }
        if self.token_mint_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.token_mint_address))?; }
        if self.token_address != "" { w.write_with_tag(26, |w| w.write_string(&**&self.token_address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TokenTransfer<'a> {
    pub token_mint_address: Cow<'a, str>,
    pub sender_token_address: Cow<'a, str>,
    pub recipient_token_address: Cow<'a, str>,
    pub amount: u64,
    pub decimals: u32,
    pub memo: Cow<'a, str>,
    pub references: Vec<Cow<'a, str>>,
}

impl<'a> MessageRead<'a> for TokenTransfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.token_mint_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.sender_token_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.recipient_token_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(32) => msg.amount = r.read_uint64(bytes)?,
                Ok(40) => msg.decimals = r.read_uint32(bytes)?,
                Ok(50) => msg.memo = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.references.push(r.read_string(bytes).map(Cow::Borrowed)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TokenTransfer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.token_mint_address == "" { 0 } else { 1 + sizeof_len((&self.token_mint_address).len()) }
        + if self.sender_token_address == "" { 0 } else { 1 + sizeof_len((&self.sender_token_address).len()) }
        + if self.recipient_token_address == "" { 0 } else { 1 + sizeof_len((&self.recipient_token_address).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.decimals == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.decimals) as u64) }
        + if self.memo == "" { 0 } else { 1 + sizeof_len((&self.memo).len()) }
        + self.references.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.token_mint_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.token_mint_address))?; }
        if self.sender_token_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.sender_token_address))?; }
        if self.recipient_token_address != "" { w.write_with_tag(26, |w| w.write_string(&**&self.recipient_token_address))?; }
        if self.amount != 0u64 { w.write_with_tag(32, |w| w.write_uint64(*&self.amount))?; }
        if self.decimals != 0u32 { w.write_with_tag(40, |w| w.write_uint32(*&self.decimals))?; }
        if self.memo != "" { w.write_with_tag(50, |w| w.write_string(&**&self.memo))?; }
        for s in &self.references { w.write_with_tag(58, |w| w.write_string(&**s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct CreateAndTransferToken<'a> {
    pub recipient_main_address: Cow<'a, str>,
    pub token_mint_address: Cow<'a, str>,
    pub recipient_token_address: Cow<'a, str>,
    pub sender_token_address: Cow<'a, str>,
    pub amount: u64,
    pub decimals: u32,
    pub memo: Cow<'a, str>,
    pub references: Vec<Cow<'a, str>>,
}

impl<'a> MessageRead<'a> for CreateAndTransferToken<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.recipient_main_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.token_mint_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.recipient_token_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.sender_token_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.amount = r.read_uint64(bytes)?,
                Ok(48) => msg.decimals = r.read_uint32(bytes)?,
                Ok(58) => msg.memo = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.references.push(r.read_string(bytes).map(Cow::Borrowed)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for CreateAndTransferToken<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.recipient_main_address == "" { 0 } else { 1 + sizeof_len((&self.recipient_main_address).len()) }
        + if self.token_mint_address == "" { 0 } else { 1 + sizeof_len((&self.token_mint_address).len()) }
        + if self.recipient_token_address == "" { 0 } else { 1 + sizeof_len((&self.recipient_token_address).len()) }
        + if self.sender_token_address == "" { 0 } else { 1 + sizeof_len((&self.sender_token_address).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.decimals == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.decimals) as u64) }
        + if self.memo == "" { 0 } else { 1 + sizeof_len((&self.memo).len()) }
        + self.references.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.recipient_main_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.recipient_main_address))?; }
        if self.token_mint_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.token_mint_address))?; }
        if self.recipient_token_address != "" { w.write_with_tag(26, |w| w.write_string(&**&self.recipient_token_address))?; }
        if self.sender_token_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.sender_token_address))?; }
        if self.amount != 0u64 { w.write_with_tag(40, |w| w.write_uint64(*&self.amount))?; }
        if self.decimals != 0u32 { w.write_with_tag(48, |w| w.write_uint32(*&self.decimals))?; }
        if self.memo != "" { w.write_with_tag(58, |w| w.write_string(&**&self.memo))?; }
        for s in &self.references { w.write_with_tag(66, |w| w.write_string(&**s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub private_key: Cow<'a, [u8]>,
    pub recent_blockhash: Cow<'a, str>,
    pub v0_msg: bool,
    pub transaction_type: TW::Solana::Proto::mod_SigningInput::OneOftransaction_type<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.recent_blockhash = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.v0_msg = r.read_bool(bytes)?,
                Ok(34) => msg.transaction_type = TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::transfer_transaction(r.read_message::<TW::Solana::Proto::Transfer>(bytes)?),
                Ok(42) => msg.transaction_type = TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::delegate_stake_transaction(r.read_message::<TW::Solana::Proto::DelegateStake>(bytes)?),
                Ok(50) => msg.transaction_type = TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::deactivate_stake_transaction(r.read_message::<TW::Solana::Proto::DeactivateStake>(bytes)?),
                Ok(58) => msg.transaction_type = TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::deactivate_all_stake_transaction(r.read_message::<TW::Solana::Proto::DeactivateAllStake>(bytes)?),
                Ok(66) => msg.transaction_type = TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::withdraw_transaction(r.read_message::<TW::Solana::Proto::WithdrawStake>(bytes)?),
                Ok(74) => msg.transaction_type = TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::withdraw_all_transaction(r.read_message::<TW::Solana::Proto::WithdrawAllStake>(bytes)?),
                Ok(82) => msg.transaction_type = TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::create_token_account_transaction(r.read_message::<TW::Solana::Proto::CreateTokenAccount>(bytes)?),
                Ok(90) => msg.transaction_type = TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::token_transfer_transaction(r.read_message::<TW::Solana::Proto::TokenTransfer>(bytes)?),
                Ok(98) => msg.transaction_type = TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::create_and_transfer_token_transaction(r.read_message::<TW::Solana::Proto::CreateAndTransferToken>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SigningInput<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + if self.recent_blockhash == "" { 0 } else { 1 + sizeof_len((&self.recent_blockhash).len()) }
        + if self.v0_msg == false { 0 } else { 1 + sizeof_varint(*(&self.v0_msg) as u64) }
        + match self.transaction_type {
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::transfer_transaction(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::delegate_stake_transaction(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::deactivate_stake_transaction(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::deactivate_all_stake_transaction(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::withdraw_transaction(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::withdraw_all_transaction(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::create_token_account_transaction(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::token_transfer_transaction(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::create_and_transfer_token_transaction(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.private_key))?; }
        if self.recent_blockhash != "" { w.write_with_tag(18, |w| w.write_string(&**&self.recent_blockhash))?; }
        if self.v0_msg != false { w.write_with_tag(24, |w| w.write_bool(*&self.v0_msg))?; }
        match self.transaction_type {            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::transfer_transaction(ref m) => { w.write_with_tag(34, |w| w.write_message(m))? },
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::delegate_stake_transaction(ref m) => { w.write_with_tag(42, |w| w.write_message(m))? },
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::deactivate_stake_transaction(ref m) => { w.write_with_tag(50, |w| w.write_message(m))? },
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::deactivate_all_stake_transaction(ref m) => { w.write_with_tag(58, |w| w.write_message(m))? },
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::withdraw_transaction(ref m) => { w.write_with_tag(66, |w| w.write_message(m))? },
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::withdraw_all_transaction(ref m) => { w.write_with_tag(74, |w| w.write_message(m))? },
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::create_token_account_transaction(ref m) => { w.write_with_tag(82, |w| w.write_message(m))? },
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::token_transfer_transaction(ref m) => { w.write_with_tag(90, |w| w.write_message(m))? },
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::create_and_transfer_token_transaction(ref m) => { w.write_with_tag(98, |w| w.write_message(m))? },
            TW::Solana::Proto::mod_SigningInput::OneOftransaction_type::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOftransaction_type<'a> {
    transfer_transaction(TW::Solana::Proto::Transfer<'a>),
    delegate_stake_transaction(TW::Solana::Proto::DelegateStake<'a>),
    deactivate_stake_transaction(TW::Solana::Proto::DeactivateStake<'a>),
    deactivate_all_stake_transaction(TW::Solana::Proto::DeactivateAllStake<'a>),
    withdraw_transaction(TW::Solana::Proto::WithdrawStake<'a>),
    withdraw_all_transaction(TW::Solana::Proto::WithdrawAllStake<'a>),
    create_token_account_transaction(TW::Solana::Proto::CreateTokenAccount<'a>),
    token_transfer_transaction(TW::Solana::Proto::TokenTransfer<'a>),
    create_and_transfer_token_transaction(TW::Solana::Proto::CreateAndTransferToken<'a>),
    None,
}

impl<'a> Default for OneOftransaction_type<'a> {
    fn default() -> Self {
        OneOftransaction_type::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, str>,
    pub unsigned_tx: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.unsigned_tx = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SigningOutput<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.encoded == "" { 0 } else { 1 + sizeof_len((&self.encoded).len()) }
        + if self.unsigned_tx == "" { 0 } else { 1 + sizeof_len((&self.unsigned_tx).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != "" { w.write_with_tag(10, |w| w.write_string(&**&self.encoded))?; }
        if self.unsigned_tx != "" { w.write_with_tag(18, |w| w.write_string(&**&self.unsigned_tx))?; }
        Ok(())
    }
}

