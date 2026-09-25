use tw_any_coin::test_utils::transaction_calc_tx_hash_utils::TransactionUtilHelper;
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_bitcoin_transaction_util_calc_tx_hash() {
    let encoded_tx = "02000000000101089098890d2653567b9e8df2d1fbe5c3c8bf1910ca7184e301db0ad3b495c88e0100000000ffffffff02581b000000000000225120e8b706a97732e705e22ae7710703e7f589ed13c636324461afa443016134cc051040000000000000160014e311b8d6ddff856ce8e9a4e03bc6d4fe5050a83d02483045022100a44aa28446a9a886b378a4a65e32ad9a3108870bd725dc6105160bed4f317097022069e9de36422e4ce2e42b39884aa5f626f8f94194d1013007d5a1ea9220a06dce0121030f209b6ada5edb42c77fd2bc64ad650ae38314c8f451f3e36d80bc8e26f132cb00000000";
    let tx_hash = TransactionUtilHelper::calc_tx_hash(CoinType::Bitcoin, encoded_tx);

    assert_eq!(
        tx_hash,
        "797d17d47ae66e598341f9dfdea020b04d4017dcf9cc33f0e51f7a6082171fb1"
    );
}
