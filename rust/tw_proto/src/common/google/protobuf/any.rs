// Automatically generated rust module for 'any.proto' file

#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(unused_imports)]
#![allow(unknown_lints)]
#![allow(clippy::all)]
#![cfg_attr(rustfmt, rustfmt_skip)]


use quick_protobuf::{MessageInfo, MessageRead, MessageWrite, BytesReader, Writer, WriterBackend, Result};
use quick_protobuf::sizeofs::*;
use super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Any {
    pub type_url: String,
    pub value: Vec<u8>,
}

impl<'a> MessageRead<'a> for Any {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.type_url = r.read_string(bytes)?.to_owned(),
                Ok(18) => msg.value = r.read_bytes(bytes)?.to_owned(),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl MessageWrite for Any {
    fn get_size(&self) -> usize {
        0
        + if self.type_url == String::default() { 0 } else { 1 + sizeof_len((&self.type_url).len()) }
        + if self.value.is_empty() { 0 } else { 1 + sizeof_len((&self.value).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.type_url != String::default() { w.write_with_tag(10, |w| w.write_string(&**&self.type_url))?; }
        if !self.value.is_empty() { w.write_with_tag(18, |w| w.write_bytes(&**&self.value))?; }
        Ok(())
    }
}

