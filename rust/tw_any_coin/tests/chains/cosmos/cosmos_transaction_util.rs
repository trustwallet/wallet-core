use tw_any_coin::test_utils::transaction_calc_tx_hash_utils::TransactionUtilHelper;
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_cosmos_transaction_util_calc_tx_hash() {
    let encoded_tx = "CpIBCo8BChwvY29zbW9zLmJhbmsudjFiZXRhMS5Nc2dTZW5kEm8KLWNvc21vczFta3k2OWNuOGVrdHd5MDg0NXZlYzl1cHNkcGhrdHh0MDNna3dseBItY29zbW9zMThzMGhkbnNsbGdjY2x3ZXU5YXltdzRuZ2t0cjJrMHJreWdkemRwGg8KBXVhdG9tEgY0MDAwMDASZQpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAuzvXOQ3owLGf5VGjeSzHzbpEfRn1+alK0HB4T4dVjZJEgQKAggBEhMKDQoFdWF0b20SBDEwMDAQwJoMGkCvvVE6d29P30cO9/lnXyGunWMPxNY12NuqDcCnFkNM0H4CUQdl1Gc9+ogIJbro5nyzZzlv9rl2/GsZox/JXoCX";
    let tx_hash = TransactionUtilHelper::calc_tx_hash(CoinType::Cosmos, encoded_tx);

    assert_eq!(
        tx_hash,
        "85392373F54577562067030BF0D61596C91188AA5E6CA8FFE731BD0349296411"
    );
}
