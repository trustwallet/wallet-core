// Automatically generated rust module for 'TransactionCompiler.proto' file

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
pub struct PreSigningOutput<'a> {
    pub data_hash: Cow<'a, [u8]>,
    pub data: Cow<'a, [u8]>,
    pub error: TW::Common::Proto::SigningError,
    pub error_message: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for PreSigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.data_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.data = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.error = r.read_enum(bytes)?,
                Ok(34) => msg.error_message = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for PreSigningOutput<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.data_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data_hash).len()) }
        + if self.data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data).len()) }
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
        + if self.error_message == "" { 0 } else { 1 + sizeof_len((&self.error_message).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.data_hash != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.data_hash))?; }
        if self.data != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.data))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(24, |w| w.write_enum(*&self.error as i32))?; }
        if self.error_message != "" { w.write_with_tag(34, |w| w.write_string(&**&self.error_message))?; }
        Ok(())
    }
}

