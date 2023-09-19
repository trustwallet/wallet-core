use crate::aliases::*;
use crate::{Error, Result};
use tw_proto::BitcoinV2::Proto;

// Convenience function: our protobuf library wraps certain types (such as
// `bytes`) in `Cow`, but given that calling `clone()` on a `Cow::Borrowed(T)`
// does not actually clone the underlying data (but the smart pointer instead),
// we must hard-clone individual fields manually. This is unfortunately required
// due to how protobuf library works and our use of the 'static constraints.
pub fn hard_clone_proto_input(proto: Proto::Input<'_>) -> Result<Proto::Input<'static>> {
    fn new_builder(variant: ProtoInputBuilder<'static>) -> ProtoInputRecipient<'static> {
        ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder { variant })
    }

    let to_recipient = match proto.to_recipient {
        ProtoInputRecipient::builder(builder) => match builder.variant {
            ProtoInputBuilder::p2sh(script) => {
                new_builder(ProtoInputBuilder::p2sh(script.to_vec().into()))
            },
            ProtoInputBuilder::p2pkh(script) => {
                new_builder(ProtoInputBuilder::p2pkh(script.to_vec().into()))
            },
            ProtoInputBuilder::p2wsh(script) => {
                new_builder(ProtoInputBuilder::p2wsh(script.to_vec().into()))
            },
            ProtoInputBuilder::p2wpkh(script) => {
                new_builder(ProtoInputBuilder::p2wpkh(script.to_vec().into()))
            },
            ProtoInputBuilder::p2tr_key_path(key_path) => new_builder(
                ProtoInputBuilder::p2tr_key_path(Proto::mod_Input::InputTaprootKeyPath {
                    one_prevout: key_path.one_prevout,
                    public_key: key_path.public_key.to_vec().into(),
                }),
            ),
            ProtoInputBuilder::p2tr_script_path(script) => new_builder(
                ProtoInputBuilder::p2tr_script_path(Proto::mod_Input::InputTaprootScriptPath {
                    one_prevout: script.one_prevout,
                    payload: script.payload.to_vec().into(),
                    control_block: script.control_block.to_vec().into(),
                }),
            ),
            ProtoInputBuilder::brc20_inscribe(brc20) => new_builder(
                ProtoInputBuilder::brc20_inscribe(Proto::mod_Input::InputBrc20Inscription {
                    one_prevout: brc20.one_prevout,
                    inscribe_to: brc20.inscribe_to.to_vec().into(),
                    ticker: brc20.ticker.to_string().into(),
                    transfer_amount: brc20.transfer_amount,
                }),
            ),
            ProtoInputBuilder::ordinal_inscribe(ord) => new_builder(
                ProtoInputBuilder::ordinal_inscribe(Proto::mod_Input::InputOrdinalInscription {
                    one_prevout: ord.one_prevout,
                    inscribe_to: ord.inscribe_to.to_vec().into(),
                    mime_type: ord.mime_type.to_string().into(),
                    payload: ord.payload.to_vec().into(),
                }),
            ),
            ProtoInputBuilder::None => {
                return Err(Error::from(Proto::Error::Error_missing_input_builder))
            },
        },
        ProtoInputRecipient::custom_script(custom) => {
            ProtoInputRecipient::custom_script(Proto::mod_Input::InputScriptWitness {
                script_pubkey: custom.script_pubkey.to_vec().into(),
                script_sig: custom.script_sig.to_vec().into(),
                witness_items: custom
                    .witness_items
                    .iter()
                    .map(|item| item.to_vec().into())
                    .collect(),
                signing_method: custom.signing_method,
            })
        },
        ProtoInputRecipient::None => {
            return Err(Error::from(Proto::Error::Error_missing_recipient))
        },
    };

    Ok(Proto::Input {
        private_key: proto.private_key.to_vec().into(),
        txid: proto.txid.to_vec().into(),
        to_recipient,
        ..proto
    })
}

// Convenience function: our protobuf library wraps certain types (such as
// `bytes`) in `Cow`, but given that calling `clone()` on a `Cow::Borrowed(T)`
// does not actually clone the underlying data (but the smart pointer instead),
// we must hard-clone individual fields manually. This is unfortunately required
// due to how protobuf library works and our use of the 'static constraints.
pub fn hard_clone_proto_output(proto: Proto::Output<'_>) -> Result<Proto::Output<'static>> {
    fn new_builder(variant: ProtoOutputBuilder<'static>) -> ProtoOutputRecipient<'static> {
        ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder { variant })
    }

    fn new_script_or_hash(
        proto: Proto::mod_Output::OutputRedeemScriptOrHash<'_>,
    ) -> Result<Proto::mod_Output::OutputRedeemScriptOrHash<'static>> {
        let variant = match proto.variant {
            ProtoOutputRedeemScriptOrHashBuilder::redeem_script(script) => {
                ProtoOutputRedeemScriptOrHashBuilder::redeem_script(script.to_vec().into())
            },
            ProtoOutputRedeemScriptOrHashBuilder::hash(hash) => {
                ProtoOutputRedeemScriptOrHashBuilder::hash(hash.to_vec().into())
            },
            ProtoOutputRedeemScriptOrHashBuilder::None => {
                return Err(Error::from(Proto::Error::Error_missing_recipient))
            },
        };

        Ok(Proto::mod_Output::OutputRedeemScriptOrHash { variant })
    }

    fn new_pubkey_or_hash(
        proto: Proto::ToPublicKeyOrHash<'_>,
    ) -> Result<Proto::ToPublicKeyOrHash<'static>> {
        let to_address = match proto.to_address {
            ProtoPubkeyOrHash::pubkey(pubkey) => ProtoPubkeyOrHash::pubkey(pubkey.to_vec().into()),
            ProtoPubkeyOrHash::hash(hash) => ProtoPubkeyOrHash::hash(hash.to_vec().into()),
            ProtoPubkeyOrHash::None => {
                return Err(Error::from(Proto::Error::Error_missing_recipient))
            },
        };

        Ok(Proto::ToPublicKeyOrHash { to_address })
    }

    let to_recipient = match proto.to_recipient {
        ProtoOutputRecipient::builder(builder) => match builder.variant {
            ProtoOutputBuilder::p2sh(script_or_hash) => new_builder(ProtoOutputBuilder::p2sh(
                new_script_or_hash(script_or_hash)?,
            )),
            ProtoOutputBuilder::p2pkh(pubkey_or_hash) => new_builder(ProtoOutputBuilder::p2pkh(
                new_pubkey_or_hash(pubkey_or_hash)?,
            )),
            ProtoOutputBuilder::p2wsh(script_or_hash) => new_builder(ProtoOutputBuilder::p2wsh(
                new_script_or_hash(script_or_hash)?,
            )),
            ProtoOutputBuilder::p2wpkh(pubkey_or_hash) => new_builder(ProtoOutputBuilder::p2wpkh(
                new_pubkey_or_hash(pubkey_or_hash)?,
            )),
            ProtoOutputBuilder::p2tr_key_path(pubkey) => {
                new_builder(ProtoOutputBuilder::p2tr_key_path(pubkey.to_vec().into()))
            },
            ProtoOutputBuilder::p2tr_script_path(script_path) => new_builder(
                ProtoOutputBuilder::p2tr_script_path(Proto::mod_Output::OutputTaprootScriptPath {
                    internal_key: script_path.internal_key.to_vec().into(),
                    merkle_root: script_path.merkle_root.to_vec().into(),
                }),
            ),
            ProtoOutputBuilder::p2tr_dangerous_assume_tweaked(tweaked) => new_builder(
                ProtoOutputBuilder::p2tr_dangerous_assume_tweaked(tweaked.to_vec().into()),
            ),
            ProtoOutputBuilder::brc20_inscribe(brc20) => new_builder(
                ProtoOutputBuilder::brc20_inscribe(Proto::mod_Output::OutputBrc20Inscription {
                    inscribe_to: brc20.inscribe_to.to_vec().into(),
                    ticker: brc20.ticker.to_string().into(),
                    transfer_amount: brc20.transfer_amount,
                }),
            ),
            ProtoOutputBuilder::ordinal_inscribe(ord) => new_builder(
                ProtoOutputBuilder::ordinal_inscribe(Proto::mod_Output::OutputOrdinalInscription {
                    inscribe_to: ord.inscribe_to.to_vec().into(),
                    mime_type: ord.mime_type.to_string().into(),
                    payload: ord.payload.to_vec().into(),
                }),
            ),
            ProtoOutputBuilder::None => {
                return Err(Error::from(Proto::Error::Error_missing_output_builder))
            },
        },
        ProtoOutputRecipient::custom_script_pubkey(custom) => {
            ProtoOutputRecipient::custom_script_pubkey(custom.to_vec().into())
        },
        ProtoOutputRecipient::from_address(address) => {
            ProtoOutputRecipient::from_address(address.to_string().into())
        },
        ProtoOutputRecipient::None => {
            return Err(Error::from(Proto::Error::Error_missing_output_builder))
        },
    };

    Ok(Proto::Output {
        value: proto.value,
        to_recipient,
    })
}
