use super::test_cases::TEST_CASES;
use tw_any_coin::test_utils::transaction_calc_tx_hash_utils::TransactionUtilHelper;
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_pactus_transaction_util_calc_tx_hash() {
    for case in TEST_CASES {
        let tx_hash = TransactionUtilHelper::calc_tx_hash(CoinType::Pactus, case.signed_data);
        assert_eq!(tx_hash, case.transaction_id);

        let tx_hash = TransactionUtilHelper::calc_tx_hash(CoinType::Pactus, case.signed_data);
        assert_eq!(tx_hash, case.transaction_id);
    }
}
