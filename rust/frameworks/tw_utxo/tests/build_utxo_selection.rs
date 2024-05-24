use tw_encoding::hex;
use tw_hash::H256;
use tw_keypair::ecdsa;
// TODO: Consider using ecdsa directly.
use tw_keypair::tw::{PublicKey, PublicKeyType};
use tw_utxo::dust::DustPolicy;
use tw_utxo::modules::utxo_selector::exact_selector::ExactInputSelector;
use tw_utxo::modules::utxo_selector::{InputSelector, SelectResult};
use tw_utxo::sighash::{SighashBase, SighashType};
use tw_utxo::transaction::standard_transaction::builder::OutputBuilder;
use tw_utxo::transaction::standard_transaction::builder::TransactionBuilder;
use tw_utxo::transaction::standard_transaction::builder::UtxoBuilder;
use tw_utxo::transaction::transaction_interface::TransactionInterface;

const SATS_PER_VBYTE: i64 = 2;

#[test]
fn build_tx_input_selection() {
    let alice_pubkey =
        hex::decode("036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536").unwrap();
    let bob_pubkey =
        hex::decode("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf").unwrap();

    let alice_ecdsa_pubkey =
        ecdsa::secp256k1::PublicKey::try_from(alice_pubkey.as_slice()).unwrap();
    let alice_pubkey = PublicKey::new(alice_pubkey, PublicKeyType::Secp256k1).unwrap();
    let bob_pubkey = PublicKey::new(bob_pubkey, PublicKeyType::Secp256k1).unwrap();

    let (utxo1, arg1) = UtxoBuilder::new()
        .prev_txid(H256::from([1; 32]))
        .prev_index(0)
        .amount(1_000)
        .sighash_type(SighashType::new(SighashBase::All))
        .p2pkh(&alice_ecdsa_pubkey)
        .unwrap();

    let (utxo2, arg2) = UtxoBuilder::new()
        .prev_txid(H256::from([2; 32]))
        .prev_index(0)
        .amount(3_000)
        .sighash_type(SighashType::new(SighashBase::All))
        .p2pkh(&alice_ecdsa_pubkey)
        .unwrap();

    let (utxo3, arg3) = UtxoBuilder::new()
        .prev_txid(H256::from([3; 32]))
        .prev_index(0)
        .amount(4_000)
        .sighash_type(SighashType::new(SighashBase::All))
        .p2pkh(&alice_ecdsa_pubkey)
        .unwrap();

    let out1 = OutputBuilder::new(1_000).p2pkh(&bob_pubkey);

    let out2 = OutputBuilder::new(1_000).p2pkh(&bob_pubkey);

    // Create the change output. The exact amount will be calculated in the
    // `SelectionBuilder`.
    let change_output = OutputBuilder::new(0).p2pkh(&alice_pubkey);

    let mut builder = TransactionBuilder::new();
    builder
        .push_input(utxo1.clone(), arg1.clone())
        .push_input(utxo2.clone(), arg2.clone())
        .push_input(utxo3, arg3)
        .push_output(out1)
        .push_output(out2);
    let unsigned_tx = builder.build().unwrap();

    // Select the inputs and build the final transaction that includes the
    // change amount.
    let SelectResult { unsigned_tx, plan } = ExactInputSelector::new(unsigned_tx)
        .maybe_change_output(Some(change_output))
        .select_inputs(
            // TODO move to a constant
            DustPolicy::FixedAmount(546),
            InputSelector::Ascending,
            SATS_PER_VBYTE,
        )
        .unwrap();

    assert_eq!(unsigned_tx.inputs().len(), 2);
    assert_eq!(
        unsigned_tx.inputs()[0].previous_output.hash,
        utxo1.previous_output.hash
    );
    assert_eq!(
        unsigned_tx.inputs()[1].previous_output.hash,
        utxo2.previous_output.hash
    );
    assert_eq!(plan.fee_estimate, 820);

    // NOTE: During UTXO selection, a dummy signature is used. Since the
    // DER-encoded signature lengths (plus Sighash type) can slightly vary (71
    // to 73 bytes), the projected fee during change output calculation can be
    // slightly off from the final fee.

    let tx_outputs = unsigned_tx.transaction().outputs();

    let total_input = arg1.amount + arg2.amount;
    let total_output = tx_outputs[0].value + tx_outputs[1].value + tx_outputs[2].value;
    assert_eq!(total_input, total_output + plan.fee_estimate);

    let total_input = 1_000 + 3_000;
    let total_output = 1_000 + 1_000 + 1186; // 1186 = change_output
    assert_eq!(total_input, total_output + 814);
}
