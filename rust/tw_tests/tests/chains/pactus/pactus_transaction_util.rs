use tw_any_coin::test_utils::transaction_calc_tx_hash_utils::TransactionUtilHelper;
use tw_coin_registry::coin_type::CoinType;

use crate::chains::pactus::test_cases::transfer_1::{
    TRANSACTION_NOT_SIGNED_DATA, TRANSACTION_SIGNED_DATA, TX_ID,
};

#[test]
fn test_pactus_transaction_util_calc_tx_hash_signed() {
    let encoded_tx = TRANSACTION_SIGNED_DATA;
    let tx_hash = TransactionUtilHelper::calc_tx_hash(CoinType::Pactus, encoded_tx);

    assert_eq!(tx_hash, TX_ID);
}

#[test]
fn test_pactus_transaction_util_calc_tx_hash_not_signed() {
    let encoded_tx = TRANSACTION_NOT_SIGNED_DATA;
    let tx_hash = TransactionUtilHelper::calc_tx_hash(CoinType::Pactus, encoded_tx);

    assert_eq!(tx_hash, TX_ID);
}
