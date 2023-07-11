// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::SigningErrorType;
use tw_encoding::hex;
use tw_ethereum::signer::Signer;
use tw_number::U256;
use tw_proto::Ethereum::Proto;
use tw_proto::Ethereum::Proto::TransactionMode;

#[test]
fn test_encode_transaction_non_typed() {
    let private =
        hex::decode("0x4646464646464646464646464646464646464646464646464646464646464646").unwrap();

    let erc20_transfer = Proto::mod_Transaction::ERC20Transfer {
        to: "0x5322b34c88ed0691971bf52a7047448f0f4efc84".into(),
        amount: U256::encode_be_compact(2_000_000_000_000_000_000),
    };

    let input = Proto::SigningInput {
        chain_id: U256::encode_be_compact(0x34),
        tx_mode: TransactionMode::Legacy,
        // 42000000000
        gas_price: U256::encode_be_compact(0x09_c765_2400),
        // 78009
        gas_limit: U256::encode_be_compact(0x01_30B9),
        // DAI
        to_address: "0x6b175474e89094c44da98b954eedeac495271d0f".into(),
        transaction: Some(Proto::Transaction {
            transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::erc20_transfer(
                erc20_transfer,
            ),
        }),
        private_key: private.into(),
        ..Proto::SigningInput::default()
    };

    let output = Signer::sign_proto(input);
    assert_eq!(output.error, SigningErrorType::OK);
    assert!(output.error_message.is_empty());

    let expected = "f8ab808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000818ba0c34040ff76f6d5e397b54b47f7fa2b3a7213f3c2a39a750260211fa15249ae8aa01ac5061e9bcf05aebef461864662652f25c45ee99240e3bb91b31f456208a6cd";
    assert_eq!(hex::encode(output.encoded, false), expected);

    assert_eq!(
        hex::encode(output.pre_hash, false),
        "b3525019dc367d3ecac48905f9a95ff3550c25a24823db765f92cae2dec7ebfd"
    );
}
