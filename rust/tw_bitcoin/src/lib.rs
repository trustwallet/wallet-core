extern crate serde;

pub mod entry;
pub mod modules;

pub use entry::*;

use tw_proto::BitcoinV2::Proto;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub struct Error(Proto::Error);

impl From<Proto::Error> for Error {
    fn from(value: Proto::Error) -> Self {
        Error(value)
    }
}

impl From<Error> for Proto::Error {
    fn from(value: Error) -> Self {
        value.0
    }
}

impl From<bitcoin::key::Error> for Error {
    fn from(_: bitcoin::key::Error) -> Self {
        Error::from(Proto::Error::Error_invalid_public_key)
    }
}

impl From<bitcoin::ecdsa::Error> for Error {
    fn from(_: bitcoin::ecdsa::Error) -> Self {
        Error::from(Proto::Error::Error_invalid_ecdsa_signature)
    }
}

impl From<bitcoin::taproot::Error> for Error {
    fn from(_: bitcoin::taproot::Error) -> Self {
        Error::from(Proto::Error::Error_invalid_schnorr_signature)
    }
}

// Convenience aliases.
pub mod aliases {
    use super::Proto;

    pub type ProtoOutputRecipient<'a> = Proto::mod_Output::OneOfto_recipient<'a>;
    pub type ProtoOutputBuilder<'a> = Proto::mod_Output::mod_OutputBuilder::OneOfvariant<'a>;
    pub type ProtoPubkeyOrHash<'a> = Proto::mod_ToPublicKeyOrHash::OneOfto_address<'a>;
    pub type ProtoRedeemScriptOrHash<'a> =
        Proto::mod_Output::mod_OutputRedeemScriptOrHash::OneOfvariant<'a>;
    pub type ProtoInputRecipient<'a> = Proto::mod_Input::OneOfto_recipient<'a>;
    pub type ProtoInputBuilder<'a> = Proto::mod_Input::mod_InputBuilder::OneOfvariant<'a>;
}
