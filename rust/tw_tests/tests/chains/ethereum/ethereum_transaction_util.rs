use tw_any_coin::test_utils::transaction_calc_tx_hash_utils::TransactionUtilHelper;
use tw_coin_registry::coin_type::CoinType;

#[test]
fn test_ethereum_transaction_util_calc_tx_hash() {
    let encoded_tx = "f8aa808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec8000025a0724c62ad4fbf47346b02de06e603e013f26f26b56fdc0be7ba3d6273401d98cea0032131cae15da7ddcda66963e8bef51ca0d9962bfef0547d3f02597a4a58c931";
    let tx_hash = TransactionUtilHelper::calc_tx_hash(CoinType::Ethereum, encoded_tx);

    assert_eq!(
        tx_hash,
        "0x199a7829fc5149e49b452c2cab76d8fa5a9682fee6e4891b8acb697ac142513e"
    );
}
