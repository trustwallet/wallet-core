// use common::hex;
// use tw_bitcoin::aliases::*;
// use tw_bitcoin::entry::BitcoinEntry;
// use tw_coin_entry::coin_entry::CoinEntry;
// use tw_coin_entry::test_utils::test_context::TestCoinContext;
// use tw_proto::BitcoinV2::Proto;
// use tw_proto::Utxo::Proto as UtxoProto;
//
// #[test]
// fn coin_entry_sign_ordinal_nft_commit_reveal_transfer() {
//     let coin = TestCoinContext::default();
//
//     let alice_private_key = hex("e253373989199da27c48680e3a3fc0f648d50f9a727ef17a7fe6a4dc3b159129");
//     let alice_pubkey = hex("030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb");
//
//     let txid: Vec<u8> = hex("579590c3227253ad423b1e7e3c5b073b8a280d307c68aecd779df2600daa2f99")
//         .into_iter()
//         .rev()
//         .collect();
//
//     let tx1 = Proto::Input {
//         txid: txid.as_slice().into(),
//         vout: 0,
//         value: 32_400,
//         sighash_type: UtxoProto::SighashType::All,
//         to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
//             variant: ProtoInputBuilder::p2wpkh(alice_pubkey.as_slice().into()),
//         }),
//         ..Default::default()
//     };
//
//     let out1 = Proto::Output {
//         value: 31_100,
//         to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
//             variant: ProtoOutputBuilder::ordinal_inscribe(
//                 Proto::mod_Output::OutputOrdinalInscription {
//                     inscribe_to: alice_pubkey.as_slice().into(),
//                     mime_type: "image/png".into(),
//                     payload: hex(common::data::NFT_INSCRIPTION_IMAGE_DATA).into(),
//                 },
//             ),
//         }),
//     };
//
//     let signing = Proto::SigningInput {
//         private_key: alice_private_key.as_slice().into(),
//         inputs: vec![tx1],
//         outputs: vec![out1],
//         input_selector: UtxoProto::InputSelector::UseAll,
//         disable_change_output: true,
//         ..Default::default()
//     };
//
//     let signed = BitcoinEntry.sign(&coin, signing);
//     assert_eq!(signed.error, Proto::Error::OK);
//
//     // https://www.blockchain.com/explorer/transactions/btc/f1e708e5c5847339e16accf8716c14b33717c14d6fe68f9db36627cecbde7117
//     assert_eq!(
//         signed.txid,
//         hex("f1e708e5c5847339e16accf8716c14b33717c14d6fe68f9db36627cecbde7117")
//     );
//
//     let encoded = tw_encoding::hex::encode(signed.encoded, false);
//     let transaction = signed.transaction.unwrap();
//
//     assert_eq!(transaction.inputs.len(), 1);
//     assert_eq!(transaction.outputs.len(), 1);
//     assert_eq!(&encoded, "02000000000101992faa0d60f29d77cdae687c300d288a3b075b3c7e1e3b42ad537222c39095570000000000ffffffff017c790000000000002251202ac69a7e9dba801e9fcba826055917b84ca6fba4d51a29e47d478de603eedab602473044022054212984443ed4c66fc103d825bfd2da7baf2ab65d286e3c629b36b98cd7debd022050214cfe5d3b12a17aaaf1a196bfeb2f0ad15ffb320c4717eb7614162453e4fe0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000");
//
//     let txid: Vec<u8> = hex("f1e708e5c5847339e16accf8716c14b33717c14d6fe68f9db36627cecbde7117")
//         .into_iter()
//         .rev()
//         .collect();
//
//     let tx1 = Proto::Input {
//         txid: txid.as_slice().into(),
//         vout: 0,
//         value: 31_100,
//         sighash_type: UtxoProto::SighashType::UseDefault,
//         to_recipient: ProtoInputRecipient::builder(Proto::mod_Input::InputBuilder {
//             variant: ProtoInputBuilder::ordinal_inscribe(
//                 Proto::mod_Input::InputOrdinalInscription {
//                     one_prevout: false,
//                     inscribe_to: alice_pubkey.as_slice().into(),
//                     mime_type: "image/png".into(),
//                     payload: hex(common::data::NFT_INSCRIPTION_IMAGE_DATA).into(),
//                 },
//             ),
//         }),
//         ..Default::default()
//     };
//
//     let out1 = Proto::Output {
//         value: 546,
//         to_recipient: ProtoOutputRecipient::builder(Proto::mod_Output::OutputBuilder {
//             variant: ProtoOutputBuilder::p2wpkh(Proto::ToPublicKeyOrHash {
//                 to_address: ProtoPubkeyOrHash::pubkey(alice_pubkey.as_slice().into()),
//             }),
//         }),
//     };
//
//     let signing = Proto::SigningInput {
//         private_key: alice_private_key.as_slice().into(),
//         inputs: vec![tx1],
//         outputs: vec![out1],
//         input_selector: UtxoProto::InputSelector::UseAll,
//         disable_change_output: true,
//         // We enable deterministic Schnorr signatures here
//         dangerous_use_fixed_schnorr_rng: true,
//         ..Default::default()
//     };
//
//     let signed = BitcoinEntry.sign(&coin, signing);
//     assert_eq!(signed.error, Proto::Error::OK);
//
//     // https://www.blockchain.com/explorer/transactions/btc/173f8350b722243d44cc8db5584de76b432eb6d0888d9e66e662db51584f44ac
//     assert_eq!(
//         signed.txid,
//         hex("173f8350b722243d44cc8db5584de76b432eb6d0888d9e66e662db51584f44ac")
//     );
//
//     let encoded = tw_encoding::hex::encode(signed.encoded, false);
//     let transaction = signed.transaction.unwrap();
//
//     assert_eq!(encoded, common::data::NFT_INSCRIPTION_RAW_HEX);
//     assert_eq!(transaction.inputs.len(), 1);
//     assert_eq!(transaction.outputs.len(), 1);
// }
