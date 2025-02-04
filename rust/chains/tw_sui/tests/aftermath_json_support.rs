use tw_encoding::base64::{self, STANDARD};
use tw_encoding::bcs;
use tw_sui::transaction::transaction_builder::TransactionBuilder;

#[test]
fn test_aftermath_json_support() {
    let raw_json = include_str!("./fixtures/aftermath_tx_1.json");
    let result = TransactionBuilder::raw_json(raw_json, 0, 0);
    assert!(result.is_ok());

    let data = result.unwrap();
    let bytes = base64::encode(&bcs::encode(&data).unwrap(), STANDARD);

    let expected_json = include_str!("./fixtures/aftermath_tx_1_serialized.json"); // Generated via aftermath-sdk
    let expected: serde_json::Value = serde_json::from_str(expected_json).unwrap();
    let expected_bytes = expected["serializedTransaction"].as_str().unwrap();
    assert_eq!(expected_bytes, bytes);

    let raw_json_2 = include_str!("./fixtures/aftermath_tx_2.json");
    let result_2 = TransactionBuilder::raw_json(raw_json_2, 0, 0);
    assert!(result_2.is_ok());
}
