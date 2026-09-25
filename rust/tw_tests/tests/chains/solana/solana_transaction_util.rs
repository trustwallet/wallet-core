use tw_any_coin::test_utils::transaction_calc_tx_hash_utils::TransactionUtilHelper;
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_solana_transaction_util_calc_tx_hash() {
    let encoded_tx = "AnQTYwZpkm3fs4SdLxnV6gQj3hSLsyacpxDdLMALYWObm722f79IfYFTbZeFK9xHtMumiDOWAM2hHQP4r/GtbARpncaXgOVFv7OgbRLMbuCEJHO1qwcdCbtH72VzyzU8yw9sqqHIAaCUE8xaQTgT6Z5IyZfeyMe2QGJIfOjz65UPAgACBssq8Im1alV3N7wXGODL8jLPWwLhTuCqfGZ1Iz9fb5tXlMOJD6jUvASrKmdtLK/qXNyJns2Vqcvlk+nfJYdZaFpIWiT/tAcEYbttfxyLdYxrLckAKdVRtf1OrNgtZeMCII4SAn6SYaaidrX/AN3s/aVn/zrlEKW0cEUIatHVDKtXO0Qss5EhV/E6kz0BNCgtAytf/s0Botvxt3kGCN8ALqcG3fbh12Whk9nL4UbO63msHLSF7V9bN5E6jPWFfv8Aqe6sdLXiXSDILEtzckCjkjchiSf6zVGpMYiAE5BE2IqHAQUEAgQDAQoMoA8AAAAAAAAG";
    let tx_hash = TransactionUtilHelper::calc_tx_hash(CoinType::Solana, encoded_tx);

    assert_eq!(
        tx_hash,
        "3KbvREZUat76wgWMtnJfWbJL74Vzh4U2eabVJa3Z3bb2fPtW8AREP5pbmRwUrxZCESbTomWpL41PeKDcPGbojsej"
    );
}
