// Automatically generated rust module for 'IoTeX.proto' file

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
    pub amount: Cow<'a, str>,
    pub recipient: Cow<'a, str>,
    pub payload: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.recipient = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.payload = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.amount == "" { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.recipient == "" { 0 } else { 1 + sizeof_len((&self.recipient).len()) }
        + if self.payload == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.payload).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.amount != "" { w.write_with_tag(10, |w| w.write_string(&**&self.amount))?; }
        if self.recipient != "" { w.write_with_tag(18, |w| w.write_string(&**&self.recipient))?; }
        if self.payload != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.payload))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Staking<'a> {
    pub message: TW::IoTeX::Proto::mod_Staking::OneOfmessage<'a>,
}

impl<'a> MessageRead<'a> for Staking<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.message = TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeCreate(r.read_message::<TW::IoTeX::Proto::mod_Staking::Create>(bytes)?),
                Ok(18) => msg.message = TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeUnstake(r.read_message::<TW::IoTeX::Proto::mod_Staking::Reclaim>(bytes)?),
                Ok(26) => msg.message = TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeWithdraw(r.read_message::<TW::IoTeX::Proto::mod_Staking::Reclaim>(bytes)?),
                Ok(34) => msg.message = TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeAddDeposit(r.read_message::<TW::IoTeX::Proto::mod_Staking::AddDeposit>(bytes)?),
                Ok(42) => msg.message = TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeRestake(r.read_message::<TW::IoTeX::Proto::mod_Staking::Restake>(bytes)?),
                Ok(50) => msg.message = TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeChangeCandidate(r.read_message::<TW::IoTeX::Proto::mod_Staking::ChangeCandidate>(bytes)?),
                Ok(58) => msg.message = TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeTransferOwnership(r.read_message::<TW::IoTeX::Proto::mod_Staking::TransferOwnership>(bytes)?),
                Ok(66) => msg.message = TW::IoTeX::Proto::mod_Staking::OneOfmessage::candidateRegister(r.read_message::<TW::IoTeX::Proto::mod_Staking::CandidateRegister>(bytes)?),
                Ok(74) => msg.message = TW::IoTeX::Proto::mod_Staking::OneOfmessage::candidateUpdate(r.read_message::<TW::IoTeX::Proto::mod_Staking::CandidateBasicInfo>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Staking<'a> {
    fn get_size(&self) -> usize {
        0
        + match self.message {
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeCreate(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeUnstake(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeWithdraw(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeAddDeposit(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeRestake(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeChangeCandidate(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeTransferOwnership(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::candidateRegister(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::candidateUpdate(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        match self.message {            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeCreate(ref m) => { w.write_with_tag(10, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeUnstake(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeWithdraw(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeAddDeposit(ref m) => { w.write_with_tag(34, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeRestake(ref m) => { w.write_with_tag(42, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeChangeCandidate(ref m) => { w.write_with_tag(50, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::stakeTransferOwnership(ref m) => { w.write_with_tag(58, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::candidateRegister(ref m) => { w.write_with_tag(66, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::candidateUpdate(ref m) => { w.write_with_tag(74, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_Staking::OneOfmessage::None => {},
    }        Ok(())
    }
}

pub mod mod_Staking {

use std::borrow::Cow;
use super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Create<'a> {
    pub candidateName: Cow<'a, str>,
    pub stakedAmount: Cow<'a, str>,
    pub stakedDuration: u32,
    pub autoStake: bool,
    pub payload: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Create<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.candidateName = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.stakedAmount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.stakedDuration = r.read_uint32(bytes)?,
                Ok(32) => msg.autoStake = r.read_bool(bytes)?,
                Ok(42) => msg.payload = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Create<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.candidateName == "" { 0 } else { 1 + sizeof_len((&self.candidateName).len()) }
        + if self.stakedAmount == "" { 0 } else { 1 + sizeof_len((&self.stakedAmount).len()) }
        + if self.stakedDuration == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.stakedDuration) as u64) }
        + if self.autoStake == false { 0 } else { 1 + sizeof_varint(*(&self.autoStake) as u64) }
        + if self.payload == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.payload).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.candidateName != "" { w.write_with_tag(10, |w| w.write_string(&**&self.candidateName))?; }
        if self.stakedAmount != "" { w.write_with_tag(18, |w| w.write_string(&**&self.stakedAmount))?; }
        if self.stakedDuration != 0u32 { w.write_with_tag(24, |w| w.write_uint32(*&self.stakedDuration))?; }
        if self.autoStake != false { w.write_with_tag(32, |w| w.write_bool(*&self.autoStake))?; }
        if self.payload != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.payload))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Reclaim<'a> {
    pub bucketIndex: u64,
    pub payload: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Reclaim<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.bucketIndex = r.read_uint64(bytes)?,
                Ok(18) => msg.payload = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Reclaim<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.bucketIndex == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.bucketIndex) as u64) }
        + if self.payload == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.payload).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.bucketIndex != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.bucketIndex))?; }
        if self.payload != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.payload))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct AddDeposit<'a> {
    pub bucketIndex: u64,
    pub amount: Cow<'a, str>,
    pub payload: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for AddDeposit<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.bucketIndex = r.read_uint64(bytes)?,
                Ok(18) => msg.amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.payload = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for AddDeposit<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.bucketIndex == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.bucketIndex) as u64) }
        + if self.amount == "" { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.payload == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.payload).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.bucketIndex != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.bucketIndex))?; }
        if self.amount != "" { w.write_with_tag(18, |w| w.write_string(&**&self.amount))?; }
        if self.payload != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.payload))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Restake<'a> {
    pub bucketIndex: u64,
    pub stakedDuration: u32,
    pub autoStake: bool,
    pub payload: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Restake<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.bucketIndex = r.read_uint64(bytes)?,
                Ok(16) => msg.stakedDuration = r.read_uint32(bytes)?,
                Ok(24) => msg.autoStake = r.read_bool(bytes)?,
                Ok(34) => msg.payload = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Restake<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.bucketIndex == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.bucketIndex) as u64) }
        + if self.stakedDuration == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.stakedDuration) as u64) }
        + if self.autoStake == false { 0 } else { 1 + sizeof_varint(*(&self.autoStake) as u64) }
        + if self.payload == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.payload).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.bucketIndex != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.bucketIndex))?; }
        if self.stakedDuration != 0u32 { w.write_with_tag(16, |w| w.write_uint32(*&self.stakedDuration))?; }
        if self.autoStake != false { w.write_with_tag(24, |w| w.write_bool(*&self.autoStake))?; }
        if self.payload != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.payload))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ChangeCandidate<'a> {
    pub bucketIndex: u64,
    pub candidateName: Cow<'a, str>,
    pub payload: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for ChangeCandidate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.bucketIndex = r.read_uint64(bytes)?,
                Ok(18) => msg.candidateName = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.payload = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ChangeCandidate<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.bucketIndex == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.bucketIndex) as u64) }
        + if self.candidateName == "" { 0 } else { 1 + sizeof_len((&self.candidateName).len()) }
        + if self.payload == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.payload).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.bucketIndex != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.bucketIndex))?; }
        if self.candidateName != "" { w.write_with_tag(18, |w| w.write_string(&**&self.candidateName))?; }
        if self.payload != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.payload))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransferOwnership<'a> {
    pub bucketIndex: u64,
    pub voterAddress: Cow<'a, str>,
    pub payload: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for TransferOwnership<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.bucketIndex = r.read_uint64(bytes)?,
                Ok(18) => msg.voterAddress = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.payload = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransferOwnership<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.bucketIndex == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.bucketIndex) as u64) }
        + if self.voterAddress == "" { 0 } else { 1 + sizeof_len((&self.voterAddress).len()) }
        + if self.payload == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.payload).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.bucketIndex != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.bucketIndex))?; }
        if self.voterAddress != "" { w.write_with_tag(18, |w| w.write_string(&**&self.voterAddress))?; }
        if self.payload != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.payload))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct CandidateBasicInfo<'a> {
    pub name: Cow<'a, str>,
    pub operatorAddress: Cow<'a, str>,
    pub rewardAddress: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for CandidateBasicInfo<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.name = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.operatorAddress = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.rewardAddress = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for CandidateBasicInfo<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.name == "" { 0 } else { 1 + sizeof_len((&self.name).len()) }
        + if self.operatorAddress == "" { 0 } else { 1 + sizeof_len((&self.operatorAddress).len()) }
        + if self.rewardAddress == "" { 0 } else { 1 + sizeof_len((&self.rewardAddress).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.name != "" { w.write_with_tag(10, |w| w.write_string(&**&self.name))?; }
        if self.operatorAddress != "" { w.write_with_tag(18, |w| w.write_string(&**&self.operatorAddress))?; }
        if self.rewardAddress != "" { w.write_with_tag(26, |w| w.write_string(&**&self.rewardAddress))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct CandidateRegister<'a> {
    pub candidate: Option<TW::IoTeX::Proto::mod_Staking::CandidateBasicInfo<'a>>,
    pub stakedAmount: Cow<'a, str>,
    pub stakedDuration: u32,
    pub autoStake: bool,
    pub ownerAddress: Cow<'a, str>,
    pub payload: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for CandidateRegister<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.candidate = Some(r.read_message::<TW::IoTeX::Proto::mod_Staking::CandidateBasicInfo>(bytes)?),
                Ok(18) => msg.stakedAmount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.stakedDuration = r.read_uint32(bytes)?,
                Ok(32) => msg.autoStake = r.read_bool(bytes)?,
                Ok(42) => msg.ownerAddress = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.payload = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for CandidateRegister<'a> {
    fn get_size(&self) -> usize {
        0
        + self.candidate.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.stakedAmount == "" { 0 } else { 1 + sizeof_len((&self.stakedAmount).len()) }
        + if self.stakedDuration == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.stakedDuration) as u64) }
        + if self.autoStake == false { 0 } else { 1 + sizeof_varint(*(&self.autoStake) as u64) }
        + if self.ownerAddress == "" { 0 } else { 1 + sizeof_len((&self.ownerAddress).len()) }
        + if self.payload == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.payload).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.candidate { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.stakedAmount != "" { w.write_with_tag(18, |w| w.write_string(&**&self.stakedAmount))?; }
        if self.stakedDuration != 0u32 { w.write_with_tag(24, |w| w.write_uint32(*&self.stakedDuration))?; }
        if self.autoStake != false { w.write_with_tag(32, |w| w.write_bool(*&self.autoStake))?; }
        if self.ownerAddress != "" { w.write_with_tag(42, |w| w.write_string(&**&self.ownerAddress))?; }
        if self.payload != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.payload))?; }
        Ok(())
    }
}

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmessage<'a> {
    stakeCreate(TW::IoTeX::Proto::mod_Staking::Create<'a>),
    stakeUnstake(TW::IoTeX::Proto::mod_Staking::Reclaim<'a>),
    stakeWithdraw(TW::IoTeX::Proto::mod_Staking::Reclaim<'a>),
    stakeAddDeposit(TW::IoTeX::Proto::mod_Staking::AddDeposit<'a>),
    stakeRestake(TW::IoTeX::Proto::mod_Staking::Restake<'a>),
    stakeChangeCandidate(TW::IoTeX::Proto::mod_Staking::ChangeCandidate<'a>),
    stakeTransferOwnership(TW::IoTeX::Proto::mod_Staking::TransferOwnership<'a>),
    candidateRegister(TW::IoTeX::Proto::mod_Staking::CandidateRegister<'a>),
    candidateUpdate(TW::IoTeX::Proto::mod_Staking::CandidateBasicInfo<'a>),
    None,
}

impl<'a> Default for OneOfmessage<'a> {
    fn default() -> Self {
        OneOfmessage::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ContractCall<'a> {
    pub amount: Cow<'a, str>,
    pub contract: Cow<'a, str>,
    pub data: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for ContractCall<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.contract = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.data = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ContractCall<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.amount == "" { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.contract == "" { 0 } else { 1 + sizeof_len((&self.contract).len()) }
        + if self.data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.amount != "" { w.write_with_tag(10, |w| w.write_string(&**&self.amount))?; }
        if self.contract != "" { w.write_with_tag(18, |w| w.write_string(&**&self.contract))?; }
        if self.data != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.data))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub version: u32,
    pub nonce: u64,
    pub gasLimit: u64,
    pub gasPrice: Cow<'a, str>,
    pub privateKey: Cow<'a, [u8]>,
    pub action: TW::IoTeX::Proto::mod_SigningInput::OneOfaction<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.version = r.read_uint32(bytes)?,
                Ok(16) => msg.nonce = r.read_uint64(bytes)?,
                Ok(24) => msg.gasLimit = r.read_uint64(bytes)?,
                Ok(34) => msg.gasPrice = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.privateKey = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(82) => msg.action = TW::IoTeX::Proto::mod_SigningInput::OneOfaction::transfer(r.read_message::<TW::IoTeX::Proto::Transfer>(bytes)?),
                Ok(98) => msg.action = TW::IoTeX::Proto::mod_SigningInput::OneOfaction::call(r.read_message::<TW::IoTeX::Proto::ContractCall>(bytes)?),
                Ok(322) => msg.action = TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeCreate(r.read_message::<TW::IoTeX::Proto::mod_Staking::Create>(bytes)?),
                Ok(330) => msg.action = TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeUnstake(r.read_message::<TW::IoTeX::Proto::mod_Staking::Reclaim>(bytes)?),
                Ok(338) => msg.action = TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeWithdraw(r.read_message::<TW::IoTeX::Proto::mod_Staking::Reclaim>(bytes)?),
                Ok(346) => msg.action = TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeAddDeposit(r.read_message::<TW::IoTeX::Proto::mod_Staking::AddDeposit>(bytes)?),
                Ok(354) => msg.action = TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeRestake(r.read_message::<TW::IoTeX::Proto::mod_Staking::Restake>(bytes)?),
                Ok(362) => msg.action = TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeChangeCandidate(r.read_message::<TW::IoTeX::Proto::mod_Staking::ChangeCandidate>(bytes)?),
                Ok(370) => msg.action = TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeTransferOwnership(r.read_message::<TW::IoTeX::Proto::mod_Staking::TransferOwnership>(bytes)?),
                Ok(378) => msg.action = TW::IoTeX::Proto::mod_SigningInput::OneOfaction::candidateRegister(r.read_message::<TW::IoTeX::Proto::mod_Staking::CandidateRegister>(bytes)?),
                Ok(386) => msg.action = TW::IoTeX::Proto::mod_SigningInput::OneOfaction::candidateUpdate(r.read_message::<TW::IoTeX::Proto::mod_Staking::CandidateBasicInfo>(bytes)?),
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
        + if self.version == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.version) as u64) }
        + if self.nonce == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.nonce) as u64) }
        + if self.gasLimit == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.gasLimit) as u64) }
        + if self.gasPrice == "" { 0 } else { 1 + sizeof_len((&self.gasPrice).len()) }
        + if self.privateKey == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.privateKey).len()) }
        + match self.action {
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::call(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeCreate(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeUnstake(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeWithdraw(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeAddDeposit(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeRestake(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeChangeCandidate(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeTransferOwnership(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::candidateRegister(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::candidateUpdate(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.version != 0u32 { w.write_with_tag(8, |w| w.write_uint32(*&self.version))?; }
        if self.nonce != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.nonce))?; }
        if self.gasLimit != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.gasLimit))?; }
        if self.gasPrice != "" { w.write_with_tag(34, |w| w.write_string(&**&self.gasPrice))?; }
        if self.privateKey != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.privateKey))?; }
        match self.action {            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::transfer(ref m) => { w.write_with_tag(82, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::call(ref m) => { w.write_with_tag(98, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeCreate(ref m) => { w.write_with_tag(322, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeUnstake(ref m) => { w.write_with_tag(330, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeWithdraw(ref m) => { w.write_with_tag(338, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeAddDeposit(ref m) => { w.write_with_tag(346, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeRestake(ref m) => { w.write_with_tag(354, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeChangeCandidate(ref m) => { w.write_with_tag(362, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::stakeTransferOwnership(ref m) => { w.write_with_tag(370, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::candidateRegister(ref m) => { w.write_with_tag(378, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::candidateUpdate(ref m) => { w.write_with_tag(386, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_SigningInput::OneOfaction::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfaction<'a> {
    transfer(TW::IoTeX::Proto::Transfer<'a>),
    call(TW::IoTeX::Proto::ContractCall<'a>),
    stakeCreate(TW::IoTeX::Proto::mod_Staking::Create<'a>),
    stakeUnstake(TW::IoTeX::Proto::mod_Staking::Reclaim<'a>),
    stakeWithdraw(TW::IoTeX::Proto::mod_Staking::Reclaim<'a>),
    stakeAddDeposit(TW::IoTeX::Proto::mod_Staking::AddDeposit<'a>),
    stakeRestake(TW::IoTeX::Proto::mod_Staking::Restake<'a>),
    stakeChangeCandidate(TW::IoTeX::Proto::mod_Staking::ChangeCandidate<'a>),
    stakeTransferOwnership(TW::IoTeX::Proto::mod_Staking::TransferOwnership<'a>),
    candidateRegister(TW::IoTeX::Proto::mod_Staking::CandidateRegister<'a>),
    candidateUpdate(TW::IoTeX::Proto::mod_Staking::CandidateBasicInfo<'a>),
    None,
}

impl<'a> Default for OneOfaction<'a> {
    fn default() -> Self {
        OneOfaction::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, [u8]>,
    pub hash: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.encoded == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.encoded).len()) }
        + if self.hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.hash).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.encoded))?; }
        if self.hash != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.hash))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ActionCore<'a> {
    pub version: u32,
    pub nonce: u64,
    pub gasLimit: u64,
    pub gasPrice: Cow<'a, str>,
    pub action: TW::IoTeX::Proto::mod_ActionCore::OneOfaction<'a>,
}

impl<'a> MessageRead<'a> for ActionCore<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.version = r.read_uint32(bytes)?,
                Ok(16) => msg.nonce = r.read_uint64(bytes)?,
                Ok(24) => msg.gasLimit = r.read_uint64(bytes)?,
                Ok(34) => msg.gasPrice = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(82) => msg.action = TW::IoTeX::Proto::mod_ActionCore::OneOfaction::transfer(r.read_message::<TW::IoTeX::Proto::Transfer>(bytes)?),
                Ok(98) => msg.action = TW::IoTeX::Proto::mod_ActionCore::OneOfaction::execution(r.read_message::<TW::IoTeX::Proto::ContractCall>(bytes)?),
                Ok(322) => msg.action = TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeCreate(r.read_message::<TW::IoTeX::Proto::mod_Staking::Create>(bytes)?),
                Ok(330) => msg.action = TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeUnstake(r.read_message::<TW::IoTeX::Proto::mod_Staking::Reclaim>(bytes)?),
                Ok(338) => msg.action = TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeWithdraw(r.read_message::<TW::IoTeX::Proto::mod_Staking::Reclaim>(bytes)?),
                Ok(346) => msg.action = TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeAddDeposit(r.read_message::<TW::IoTeX::Proto::mod_Staking::AddDeposit>(bytes)?),
                Ok(354) => msg.action = TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeRestake(r.read_message::<TW::IoTeX::Proto::mod_Staking::Restake>(bytes)?),
                Ok(362) => msg.action = TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeChangeCandidate(r.read_message::<TW::IoTeX::Proto::mod_Staking::ChangeCandidate>(bytes)?),
                Ok(370) => msg.action = TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeTransferOwnership(r.read_message::<TW::IoTeX::Proto::mod_Staking::TransferOwnership>(bytes)?),
                Ok(378) => msg.action = TW::IoTeX::Proto::mod_ActionCore::OneOfaction::candidateRegister(r.read_message::<TW::IoTeX::Proto::mod_Staking::CandidateRegister>(bytes)?),
                Ok(386) => msg.action = TW::IoTeX::Proto::mod_ActionCore::OneOfaction::candidateUpdate(r.read_message::<TW::IoTeX::Proto::mod_Staking::CandidateBasicInfo>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ActionCore<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.version == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.version) as u64) }
        + if self.nonce == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.nonce) as u64) }
        + if self.gasLimit == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.gasLimit) as u64) }
        + if self.gasPrice == "" { 0 } else { 1 + sizeof_len((&self.gasPrice).len()) }
        + match self.action {
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::execution(ref m) => 1 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeCreate(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeUnstake(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeWithdraw(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeAddDeposit(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeRestake(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeChangeCandidate(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeTransferOwnership(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::candidateRegister(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::candidateUpdate(ref m) => 2 + sizeof_len((m).get_size()),
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.version != 0u32 { w.write_with_tag(8, |w| w.write_uint32(*&self.version))?; }
        if self.nonce != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.nonce))?; }
        if self.gasLimit != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.gasLimit))?; }
        if self.gasPrice != "" { w.write_with_tag(34, |w| w.write_string(&**&self.gasPrice))?; }
        match self.action {            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::transfer(ref m) => { w.write_with_tag(82, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::execution(ref m) => { w.write_with_tag(98, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeCreate(ref m) => { w.write_with_tag(322, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeUnstake(ref m) => { w.write_with_tag(330, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeWithdraw(ref m) => { w.write_with_tag(338, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeAddDeposit(ref m) => { w.write_with_tag(346, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeRestake(ref m) => { w.write_with_tag(354, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeChangeCandidate(ref m) => { w.write_with_tag(362, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::stakeTransferOwnership(ref m) => { w.write_with_tag(370, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::candidateRegister(ref m) => { w.write_with_tag(378, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::candidateUpdate(ref m) => { w.write_with_tag(386, |w| w.write_message(m))? },
            TW::IoTeX::Proto::mod_ActionCore::OneOfaction::None => {},
    }        Ok(())
    }
}

pub mod mod_ActionCore {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfaction<'a> {
    transfer(TW::IoTeX::Proto::Transfer<'a>),
    execution(TW::IoTeX::Proto::ContractCall<'a>),
    stakeCreate(TW::IoTeX::Proto::mod_Staking::Create<'a>),
    stakeUnstake(TW::IoTeX::Proto::mod_Staking::Reclaim<'a>),
    stakeWithdraw(TW::IoTeX::Proto::mod_Staking::Reclaim<'a>),
    stakeAddDeposit(TW::IoTeX::Proto::mod_Staking::AddDeposit<'a>),
    stakeRestake(TW::IoTeX::Proto::mod_Staking::Restake<'a>),
    stakeChangeCandidate(TW::IoTeX::Proto::mod_Staking::ChangeCandidate<'a>),
    stakeTransferOwnership(TW::IoTeX::Proto::mod_Staking::TransferOwnership<'a>),
    candidateRegister(TW::IoTeX::Proto::mod_Staking::CandidateRegister<'a>),
    candidateUpdate(TW::IoTeX::Proto::mod_Staking::CandidateBasicInfo<'a>),
    None,
}

impl<'a> Default for OneOfaction<'a> {
    fn default() -> Self {
        OneOfaction::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Action<'a> {
    pub core: Option<TW::IoTeX::Proto::ActionCore<'a>>,
    pub senderPubKey: Cow<'a, [u8]>,
    pub signature: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Action<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.core = Some(r.read_message::<TW::IoTeX::Proto::ActionCore>(bytes)?),
                Ok(18) => msg.senderPubKey = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.signature = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Action<'a> {
    fn get_size(&self) -> usize {
        0
        + self.core.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.senderPubKey == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.senderPubKey).len()) }
        + if self.signature == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.signature).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.core { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.senderPubKey != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.senderPubKey))?; }
        if self.signature != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.signature))?; }
        Ok(())
    }
}

