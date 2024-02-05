// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_encoding::base58;
use tw_encoding::base58::Alphabet;
use tw_hash::H256;
use tw_keypair::ed25519;
use tw_solana::modules::TxSigner;
use tw_solana::transaction::versioned::VersionedTransaction;

#[test]
fn test_resign_transfer() {
    let encoded_tx = base58::decode("3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZUjikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDzsW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM", Alphabet::BITCOIN).unwrap();
    let mut signed_tx: VersionedTransaction = bincode::deserialize(&encoded_tx).unwrap();

    // Update the transaction's blockhash.
    let new_blockhash = base58::decode(
        "CyPYVsYWrsJNfVpi8aazu7WsrswNFuDd385z6GNoBGUg",
        Alphabet::BITCOIN,
    )
    .unwrap();
    let new_blockhash = H256::try_from(new_blockhash.as_slice()).unwrap();
    signed_tx.message.set_recent_blockhash(new_blockhash);

    // Re-sign the transaction.
    let my_private_key_bytes = base58::decode(
        "A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr",
        Alphabet::BITCOIN,
    )
    .unwrap();
    let my_private_key =
        ed25519::sha512::PrivateKey::try_from(my_private_key_bytes.as_slice()).unwrap();
    let resigned_transaction = TxSigner::sign_versioned(signed_tx, &[my_private_key]).unwrap();

    let resigned_encoded = base58::encode(
        &bincode::serialize(&resigned_transaction).unwrap(),
        Alphabet::BITCOIN,
    );
    let expected = "64jPp6qjXpssJf8KVHR1eFR8zZ6eMXuDHDevBNSgTGkx3Egzk9tkAgTc6Ez5EHeQcH2GyWie6NdLB9sLTd2Fa76STWtN2Af9i7yCtZTBptNNgMb3XavTHvcEPPZoeAr6ZByfFfGR3dshXv3oNVtJfAEtV6wM98GxUSkubruBbjoDmoMSZPCBPn7W7D9t9GAE76DrK7kqsaFx6TxZVa2MXsbme6Hnyiyvs6jv46V7GBbTYsEKtZG1htyNNBW8LcbyyLme5ZoqauTZQVePBjnNjobPCW7r8ESWevZPD";
    assert_eq!(resigned_encoded, expected);
}
