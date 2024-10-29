use tw_any_coin::test_utils::transaction_calc_tx_hash_utils::TransactionUtilHelper;
use tw_coin_registry::coin_type::CoinType;

use crate::chains::pactus::test_cases::{bond_test_case, transfer_test_case};

#[test]
fn test_pactus_transaction_util_calc_tx_hash() {
    struct TestCase<'a> {
        tx_hex: &'a str,
        tx_id: &'a str,
    }

    let test_cases = vec![
        TestCase {
            tx_hex: transfer_test_case::NOT_SIGNED_DATA,
            tx_id: transfer_test_case::TX_ID,
        },
        TestCase {
            tx_hex: transfer_test_case::SIGNED_DATA,
            tx_id: transfer_test_case::TX_ID,
        },
        TestCase {
            tx_hex: bond_test_case::NOT_SIGNED_DATA,
            tx_id: bond_test_case::TX_ID,
        },
        TestCase {
            tx_hex: bond_test_case::NOT_SIGNED_DATA,
            tx_id: bond_test_case::TX_ID,
        },
    ];

    for case in test_cases {
        let tx_hash = TransactionUtilHelper::calc_tx_hash(CoinType::Pactus, case.tx_hex);
        assert_eq!(tx_hash, case.tx_id);
    }
}
