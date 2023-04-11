// Automatically generated rust module for 'Nervos.proto' file

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
pub struct TransactionPlan<'a> {
    pub cell_deps: Vec<TW::Nervos::Proto::CellDep<'a>>,
    pub header_deps: Vec<Cow<'a, [u8]>>,
    pub selected_cells: Vec<TW::Nervos::Proto::Cell<'a>>,
    pub outputs: Vec<TW::Nervos::Proto::CellOutput<'a>>,
    pub outputs_data: Vec<Cow<'a, [u8]>>,
    pub error: TW::Common::Proto::SigningError,
}

impl<'a> MessageRead<'a> for TransactionPlan<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.cell_deps.push(r.read_message::<TW::Nervos::Proto::CellDep>(bytes)?),
                Ok(18) => msg.header_deps.push(r.read_bytes(bytes).map(Cow::Borrowed)?),
                Ok(26) => msg.selected_cells.push(r.read_message::<TW::Nervos::Proto::Cell>(bytes)?),
                Ok(34) => msg.outputs.push(r.read_message::<TW::Nervos::Proto::CellOutput>(bytes)?),
                Ok(42) => msg.outputs_data.push(r.read_bytes(bytes).map(Cow::Borrowed)?),
                Ok(48) => msg.error = r.read_enum(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionPlan<'a> {
    fn get_size(&self) -> usize {
        0
        + self.cell_deps.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.header_deps.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
        + self.selected_cells.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.outputs.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.outputs_data.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.cell_deps { w.write_with_tag(10, |w| w.write_message(s))?; }
        for s in &self.header_deps { w.write_with_tag(18, |w| w.write_bytes(&**s))?; }
        for s in &self.selected_cells { w.write_with_tag(26, |w| w.write_message(s))?; }
        for s in &self.outputs { w.write_with_tag(34, |w| w.write_message(s))?; }
        for s in &self.outputs_data { w.write_with_tag(42, |w| w.write_bytes(&**s))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(48, |w| w.write_enum(*&self.error as i32))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct CellDep<'a> {
    pub dep_type: Cow<'a, str>,
    pub out_point: Option<TW::Nervos::Proto::OutPoint<'a>>,
}

impl<'a> MessageRead<'a> for CellDep<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.dep_type = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.out_point = Some(r.read_message::<TW::Nervos::Proto::OutPoint>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for CellDep<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.dep_type == "" { 0 } else { 1 + sizeof_len((&self.dep_type).len()) }
        + self.out_point.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.dep_type != "" { w.write_with_tag(10, |w| w.write_string(&**&self.dep_type))?; }
        if let Some(ref s) = self.out_point { w.write_with_tag(18, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OutPoint<'a> {
    pub tx_hash: Cow<'a, [u8]>,
    pub index: u32,
}

impl<'a> MessageRead<'a> for OutPoint<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.tx_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.index = r.read_uint32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OutPoint<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.tx_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.tx_hash).len()) }
        + if self.index == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.index) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.tx_hash != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.tx_hash))?; }
        if self.index != 0u32 { w.write_with_tag(16, |w| w.write_uint32(*&self.index))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct CellOutput<'a> {
    pub capacity: u64,
    pub lock: Option<TW::Nervos::Proto::Script<'a>>,
    pub type_pb: Option<TW::Nervos::Proto::Script<'a>>,
}

impl<'a> MessageRead<'a> for CellOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.capacity = r.read_uint64(bytes)?,
                Ok(18) => msg.lock = Some(r.read_message::<TW::Nervos::Proto::Script>(bytes)?),
                Ok(26) => msg.type_pb = Some(r.read_message::<TW::Nervos::Proto::Script>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for CellOutput<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.capacity == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.capacity) as u64) }
        + self.lock.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.type_pb.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.capacity != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.capacity))?; }
        if let Some(ref s) = self.lock { w.write_with_tag(18, |w| w.write_message(s))?; }
        if let Some(ref s) = self.type_pb { w.write_with_tag(26, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Script<'a> {
    pub code_hash: Cow<'a, [u8]>,
    pub hash_type: Cow<'a, str>,
    pub args: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Script<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.code_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.hash_type = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.args = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Script<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.code_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.code_hash).len()) }
        + if self.hash_type == "" { 0 } else { 1 + sizeof_len((&self.hash_type).len()) }
        + if self.args == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.args).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.code_hash != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.code_hash))?; }
        if self.hash_type != "" { w.write_with_tag(18, |w| w.write_string(&**&self.hash_type))?; }
        if self.args != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.args))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct NativeTransfer<'a> {
    pub to_address: Cow<'a, str>,
    pub change_address: Cow<'a, str>,
    pub amount: u64,
    pub use_max_amount: bool,
}

impl<'a> MessageRead<'a> for NativeTransfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.change_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.amount = r.read_uint64(bytes)?,
                Ok(32) => msg.use_max_amount = r.read_bool(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for NativeTransfer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.change_address == "" { 0 } else { 1 + sizeof_len((&self.change_address).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.use_max_amount == false { 0 } else { 1 + sizeof_varint(*(&self.use_max_amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to_address))?; }
        if self.change_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.change_address))?; }
        if self.amount != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.amount))?; }
        if self.use_max_amount != false { w.write_with_tag(32, |w| w.write_bool(*&self.use_max_amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SudtTransfer<'a> {
    pub to_address: Cow<'a, str>,
    pub change_address: Cow<'a, str>,
    pub sudt_address: Cow<'a, [u8]>,
    pub amount: Cow<'a, str>,
    pub use_max_amount: bool,
}

impl<'a> MessageRead<'a> for SudtTransfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.change_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.sudt_address = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.use_max_amount = r.read_bool(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SudtTransfer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.change_address == "" { 0 } else { 1 + sizeof_len((&self.change_address).len()) }
        + if self.sudt_address == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.sudt_address).len()) }
        + if self.amount == "" { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.use_max_amount == false { 0 } else { 1 + sizeof_varint(*(&self.use_max_amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to_address))?; }
        if self.change_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.change_address))?; }
        if self.sudt_address != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.sudt_address))?; }
        if self.amount != "" { w.write_with_tag(34, |w| w.write_string(&**&self.amount))?; }
        if self.use_max_amount != false { w.write_with_tag(40, |w| w.write_bool(*&self.use_max_amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DaoDeposit<'a> {
    pub to_address: Cow<'a, str>,
    pub change_address: Cow<'a, str>,
    pub amount: u64,
}

impl<'a> MessageRead<'a> for DaoDeposit<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.change_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.amount = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DaoDeposit<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.change_address == "" { 0 } else { 1 + sizeof_len((&self.change_address).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to_address))?; }
        if self.change_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.change_address))?; }
        if self.amount != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DaoWithdrawPhase1<'a> {
    pub deposit_cell: Option<TW::Nervos::Proto::Cell<'a>>,
    pub change_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for DaoWithdrawPhase1<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.deposit_cell = Some(r.read_message::<TW::Nervos::Proto::Cell>(bytes)?),
                Ok(18) => msg.change_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DaoWithdrawPhase1<'a> {
    fn get_size(&self) -> usize {
        0
        + self.deposit_cell.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.change_address == "" { 0 } else { 1 + sizeof_len((&self.change_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.deposit_cell { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.change_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.change_address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DaoWithdrawPhase2<'a> {
    pub deposit_cell: Option<TW::Nervos::Proto::Cell<'a>>,
    pub withdrawing_cell: Option<TW::Nervos::Proto::Cell<'a>>,
    pub amount: u64,
}

impl<'a> MessageRead<'a> for DaoWithdrawPhase2<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.deposit_cell = Some(r.read_message::<TW::Nervos::Proto::Cell>(bytes)?),
                Ok(18) => msg.withdrawing_cell = Some(r.read_message::<TW::Nervos::Proto::Cell>(bytes)?),
                Ok(24) => msg.amount = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DaoWithdrawPhase2<'a> {
    fn get_size(&self) -> usize {
        0
        + self.deposit_cell.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.withdrawing_cell.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.deposit_cell { w.write_with_tag(10, |w| w.write_message(s))?; }
        if let Some(ref s) = self.withdrawing_cell { w.write_with_tag(18, |w| w.write_message(s))?; }
        if self.amount != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub byte_fee: u64,
    pub private_key: Vec<Cow<'a, [u8]>>,
    pub cell: Vec<TW::Nervos::Proto::Cell<'a>>,
    pub plan: Option<TW::Nervos::Proto::TransactionPlan<'a>>,
    pub operation_oneof: TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.byte_fee = r.read_uint64(bytes)?,
                Ok(18) => msg.private_key.push(r.read_bytes(bytes).map(Cow::Borrowed)?),
                Ok(26) => msg.cell.push(r.read_message::<TW::Nervos::Proto::Cell>(bytes)?),
                Ok(34) => msg.plan = Some(r.read_message::<TW::Nervos::Proto::TransactionPlan>(bytes)?),
                Ok(42) => msg.operation_oneof = TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::native_transfer(r.read_message::<TW::Nervos::Proto::NativeTransfer>(bytes)?),
                Ok(50) => msg.operation_oneof = TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::sudt_transfer(r.read_message::<TW::Nervos::Proto::SudtTransfer>(bytes)?),
                Ok(58) => msg.operation_oneof = TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::dao_deposit(r.read_message::<TW::Nervos::Proto::DaoDeposit>(bytes)?),
                Ok(66) => msg.operation_oneof = TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::dao_withdraw_phase1(r.read_message::<TW::Nervos::Proto::DaoWithdrawPhase1>(bytes)?),
                Ok(74) => msg.operation_oneof = TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::dao_withdraw_phase2(r.read_message::<TW::Nervos::Proto::DaoWithdrawPhase2>(bytes)?),
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
        + if self.byte_fee == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.byte_fee) as u64) }
        + self.private_key.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
        + self.cell.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.plan.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + match self.operation_oneof {
            TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::native_transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::sudt_transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::dao_deposit(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::dao_withdraw_phase1(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::dao_withdraw_phase2(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.byte_fee != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.byte_fee))?; }
        for s in &self.private_key { w.write_with_tag(18, |w| w.write_bytes(&**s))?; }
        for s in &self.cell { w.write_with_tag(26, |w| w.write_message(s))?; }
        if let Some(ref s) = self.plan { w.write_with_tag(34, |w| w.write_message(s))?; }
        match self.operation_oneof {            TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::native_transfer(ref m) => { w.write_with_tag(42, |w| w.write_message(m))? },
            TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::sudt_transfer(ref m) => { w.write_with_tag(50, |w| w.write_message(m))? },
            TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::dao_deposit(ref m) => { w.write_with_tag(58, |w| w.write_message(m))? },
            TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::dao_withdraw_phase1(ref m) => { w.write_with_tag(66, |w| w.write_message(m))? },
            TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::dao_withdraw_phase2(ref m) => { w.write_with_tag(74, |w| w.write_message(m))? },
            TW::Nervos::Proto::mod_SigningInput::OneOfoperation_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfoperation_oneof<'a> {
    native_transfer(TW::Nervos::Proto::NativeTransfer<'a>),
    sudt_transfer(TW::Nervos::Proto::SudtTransfer<'a>),
    dao_deposit(TW::Nervos::Proto::DaoDeposit<'a>),
    dao_withdraw_phase1(TW::Nervos::Proto::DaoWithdrawPhase1<'a>),
    dao_withdraw_phase2(TW::Nervos::Proto::DaoWithdrawPhase2<'a>),
    None,
}

impl<'a> Default for OneOfoperation_oneof<'a> {
    fn default() -> Self {
        OneOfoperation_oneof::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Cell<'a> {
    pub out_point: Option<TW::Nervos::Proto::OutPoint<'a>>,
    pub capacity: u64,
    pub lock: Option<TW::Nervos::Proto::Script<'a>>,
    pub type_pb: Option<TW::Nervos::Proto::Script<'a>>,
    pub data: Cow<'a, [u8]>,
    pub block_number: u64,
    pub block_hash: Cow<'a, [u8]>,
    pub since: u64,
    pub input_type: Cow<'a, [u8]>,
    pub output_type: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Cell<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.out_point = Some(r.read_message::<TW::Nervos::Proto::OutPoint>(bytes)?),
                Ok(16) => msg.capacity = r.read_uint64(bytes)?,
                Ok(26) => msg.lock = Some(r.read_message::<TW::Nervos::Proto::Script>(bytes)?),
                Ok(34) => msg.type_pb = Some(r.read_message::<TW::Nervos::Proto::Script>(bytes)?),
                Ok(42) => msg.data = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(48) => msg.block_number = r.read_uint64(bytes)?,
                Ok(58) => msg.block_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(64) => msg.since = r.read_uint64(bytes)?,
                Ok(74) => msg.input_type = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(82) => msg.output_type = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Cell<'a> {
    fn get_size(&self) -> usize {
        0
        + self.out_point.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.capacity == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.capacity) as u64) }
        + self.lock.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.type_pb.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data).len()) }
        + if self.block_number == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.block_number) as u64) }
        + if self.block_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.block_hash).len()) }
        + if self.since == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.since) as u64) }
        + if self.input_type == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.input_type).len()) }
        + if self.output_type == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.output_type).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.out_point { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.capacity != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.capacity))?; }
        if let Some(ref s) = self.lock { w.write_with_tag(26, |w| w.write_message(s))?; }
        if let Some(ref s) = self.type_pb { w.write_with_tag(34, |w| w.write_message(s))?; }
        if self.data != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.data))?; }
        if self.block_number != 0u64 { w.write_with_tag(48, |w| w.write_uint64(*&self.block_number))?; }
        if self.block_hash != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.block_hash))?; }
        if self.since != 0u64 { w.write_with_tag(64, |w| w.write_uint64(*&self.since))?; }
        if self.input_type != Cow::Borrowed(b"") { w.write_with_tag(74, |w| w.write_bytes(&**&self.input_type))?; }
        if self.output_type != Cow::Borrowed(b"") { w.write_with_tag(82, |w| w.write_bytes(&**&self.output_type))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub transaction_json: Cow<'a, str>,
    pub transaction_id: Cow<'a, str>,
    pub error: TW::Common::Proto::SigningError,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.transaction_json = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.transaction_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.error = r.read_enum(bytes)?,
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
        + if self.transaction_json == "" { 0 } else { 1 + sizeof_len((&self.transaction_json).len()) }
        + if self.transaction_id == "" { 0 } else { 1 + sizeof_len((&self.transaction_id).len()) }
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.transaction_json != "" { w.write_with_tag(10, |w| w.write_string(&**&self.transaction_json))?; }
        if self.transaction_id != "" { w.write_with_tag(18, |w| w.write_string(&**&self.transaction_id))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(24, |w| w.write_enum(*&self.error as i32))?; }
        Ok(())
    }
}

