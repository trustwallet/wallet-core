use crate::aliases::*;
use tw_proto::BitcoinV2::Proto;

// Convenience function: our protobuf library wraps certain types (such as
// `bytes`) in `Cow`, but given that calling `clone()` on a `Cow::Borrowed(T)`
// does not actually clone the underlying data (but the smart pointer instead),
// we must hard-clone individual fields manually. This is unfortunately required
// due to how protobuf library works and our use of the 'static constraints.
pub fn hard_clone_proto_output(proto: Proto::Output<'_>) -> Proto::Output<'static> {
    fn new_builder(
        variant: Proto::mod_Output::mod_OutputBuilder::OneOfvariant<'static>,
    ) -> Proto::mod_Output::OneOfto_recipient<'static> {
        Proto::mod_Output::OneOfto_recipient::builder(Proto::mod_Output::OutputBuilder { variant })
    }

    fn new_script_or_hash(
        proto: Proto::mod_Output::OutputRedeemScriptOrHash<'_>,
    ) -> Proto::mod_Output::OutputRedeemScriptOrHash<'static> {
        let variant = match proto.variant {
            Proto::mod_Output::mod_OutputRedeemScriptOrHash::OneOfvariant::redeem_script(
                script,
            ) => Proto::mod_Output::mod_OutputRedeemScriptOrHash::OneOfvariant::redeem_script(
                script.to_vec().into(),
            ),
            Proto::mod_Output::mod_OutputRedeemScriptOrHash::OneOfvariant::hash(hash) => {
                Proto::mod_Output::mod_OutputRedeemScriptOrHash::OneOfvariant::hash(
                    hash.to_vec().into(),
                )
            },
            Proto::mod_Output::mod_OutputRedeemScriptOrHash::OneOfvariant::None => todo!(),
        };

        Proto::mod_Output::OutputRedeemScriptOrHash { variant }
    }

    fn new_pubkey_or_hash(
        proto: Proto::ToPublicKeyOrHash<'_>,
    ) -> Proto::ToPublicKeyOrHash<'static> {
        let to_address = match proto.to_address {
            Proto::mod_ToPublicKeyOrHash::OneOfto_address::pubkey(pubkey) => {
                Proto::mod_ToPublicKeyOrHash::OneOfto_address::pubkey(pubkey.to_vec().into())
            },
            Proto::mod_ToPublicKeyOrHash::OneOfto_address::hash(hash) => {
                Proto::mod_ToPublicKeyOrHash::OneOfto_address::hash(hash.to_vec().into())
            },
            Proto::mod_ToPublicKeyOrHash::OneOfto_address::None => todo!(),
        };

        Proto::ToPublicKeyOrHash { to_address }
    }

    let to_recipient = match proto.to_recipient {
        Proto::mod_Output::OneOfto_recipient::builder(builder) => match builder.variant {
            Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2sh(script_or_hash) => {
                new_builder(Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2sh(
                    new_script_or_hash(script_or_hash),
                ))
            },
            Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2pkh(pubkey_or_hash) => {
                new_builder(Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2pkh(
                    new_pubkey_or_hash(pubkey_or_hash),
                ))
            },
            Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2wsh(script_or_hash) => {
                new_builder(Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2wsh(
                    new_script_or_hash(script_or_hash),
                ))
            },
            Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2wpkh(pubkey_or_hash) => {
                new_builder(Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2wpkh(
                    new_pubkey_or_hash(pubkey_or_hash),
                ))
            },
            Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_key_path(pubkey) => {
                new_builder(
                    Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_key_path(
                        pubkey.to_vec().into(),
                    ),
                )
            },
            Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_script_path(script_path) => {
                new_builder(
                    Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_script_path(
                        Proto::mod_Output::OutputTaprootScriptPath {
                            internal_key: script_path.internal_key.to_vec().into(),
                            merkle_root: script_path.merkle_root.to_vec().into(),
                        },
                    ),
                )
            },
            Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_dangerous_assume_tweaked(
                tweaked,
            ) => new_builder(
                Proto::mod_Output::mod_OutputBuilder::OneOfvariant::p2tr_dangerous_assume_tweaked(
                    tweaked.to_vec().into(),
                ),
            ),
            Proto::mod_Output::mod_OutputBuilder::OneOfvariant::brc20_inscribe(brc20) => {
                new_builder(
                    Proto::mod_Output::mod_OutputBuilder::OneOfvariant::brc20_inscribe(
                        Proto::mod_Output::OutputBrc20Inscription {
                            inscribe_to: brc20.inscribe_to.to_vec().into(),
                            ticker: brc20.ticker.to_string().into(),
                            transfer_amount: brc20.transfer_amount,
                        },
                    ),
                )
            },
            Proto::mod_Output::mod_OutputBuilder::OneOfvariant::ordinal_inscribe(ord) => {
                new_builder(
                    Proto::mod_Output::mod_OutputBuilder::OneOfvariant::ordinal_inscribe(
                        Proto::mod_Output::OutputOrdinalInscription {
                            inscribe_to: ord.inscribe_to.to_vec().into(),
                            mime_type: ord.mime_type.to_string().into(),
                            payload: ord.payload.to_vec().into(),
                        },
                    ),
                )
            },
            Proto::mod_Output::mod_OutputBuilder::OneOfvariant::None => todo!(),
        },
        Proto::mod_Output::OneOfto_recipient::custom_script_pubkey(custom) => {
            Proto::mod_Output::OneOfto_recipient::custom_script_pubkey(custom.to_vec().into())
        },
        Proto::mod_Output::OneOfto_recipient::from_address(address) => {
            Proto::mod_Output::OneOfto_recipient::from_address(address.to_string().into())
        },
        Proto::mod_Output::OneOfto_recipient::None => todo!(),
    };

    Proto::Output {
        value: proto.value,
        to_recipient,
    }
}

// Convenience function: our protobuf library wraps certain types (such as
// `bytes`) in `Cow`, but given that calling `clone()` on a `Cow::Borrowed(T)`
// does not actually clone the underlying data (but the smart pointer instead),
// we must hard-clone individual fields manually. This is unfortunately required
// due to how protobuf library works and our use of the 'static constraints.
pub fn hard_clone_proto_input(proto: Proto::Input<'_>) -> Proto::Input<'static> {
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
            ProtoInputBuilder::None => todo!(),
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
        ProtoInputRecipient::None => todo!(),
    };

    Proto::Input {
        private_key: proto.private_key.to_vec().into(),
        txid: proto.txid.to_vec().into(),
        to_recipient,
        ..proto
    }
}