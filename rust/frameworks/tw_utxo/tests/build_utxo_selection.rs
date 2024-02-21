use tw_encoding::hex;
use tw_hash::H256;
use tw_keypair::ecdsa::secp256k1::PrivateKey;
use tw_keypair::ecdsa::signature::Signature;
use tw_keypair::traits::SigningKeyTrait;
// TODO: Consider using ecdsa directly.
use tw_keypair::tw::{PublicKey, PublicKeyType};
use tw_utxo::sighash::SighashBase;
use tw_utxo::sighash::SighashType;
use tw_utxo::sighash_computer::SighashComputer;
use tw_utxo::transaction::standard_transaction::builder::OutputBuilder;
use tw_utxo::transaction::standard_transaction::builder::SpendingScriptBuilder;
use tw_utxo::transaction::standard_transaction::builder::TransactionBuilder;
use tw_utxo::transaction::standard_transaction::builder::UtxoBuilder;
use tw_utxo::transaction::transaction_fee::TransactionFee;
use tw_utxo::utxo_selector::{InputSelector, SelectionBuilder};

const SATS_PER_VBYTE: i64 = 2;

#[test]
fn build_tx_input_selection() {
    let alice_private_key =
        hex::decode("56429688a1a6b00b90ccd22a0de0a376b6569d8684022ae92229a28478bfb657").unwrap();
    let alice_pubkey =
        hex::decode("036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536").unwrap();
    let bob_pubkey =
        hex::decode("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf").unwrap();

    let alice_private_key = PrivateKey::try_from(alice_private_key.as_slice()).unwrap();
    let alice_pubkey = PublicKey::new(alice_pubkey, PublicKeyType::Secp256k1).unwrap();
    let bob_pubkey = PublicKey::new(bob_pubkey, PublicKeyType::Secp256k1).unwrap();

    let (utxo1, arg1) = UtxoBuilder::new()
        .prev_txid(H256::from([1; 32]))
        .prev_index(0)
        .amount(1_000)
        .p2pkh(alice_pubkey.clone())
        .unwrap();

    let (utxo2, arg2) = UtxoBuilder::new()
        .prev_txid(H256::from([2; 32]))
        .prev_index(0)
        .amount(3_000)
        .p2pkh(alice_pubkey.clone())
        .unwrap();

    let (utxo3, arg3) = UtxoBuilder::new()
        .prev_txid(H256::from([3; 32]))
        .prev_index(0)
        .amount(4_000)
        .p2pkh(alice_pubkey.clone())
        .unwrap();

    let out1 = OutputBuilder::new()
        .amount(1_000)
        .p2pkh(bob_pubkey.clone())
        .unwrap();

    let out2 = OutputBuilder::new()
        .amount(1_000)
        .p2pkh(bob_pubkey)
        .unwrap();

    // Create the change output. The exact amount will be calculated in the
    // `SelectionBuilder`.
    let change_output = OutputBuilder::new()
        .amount(0)
        .p2pkh(alice_pubkey.clone())
        .unwrap();

    let (tx, args) = TransactionBuilder::new()
        .push_input(utxo1.clone(), arg1.clone())
        .push_input(utxo2.clone(), arg2.clone())
        .push_input(utxo3, arg3)
        .push_output(out1)
        .push_output(out2)
        .push_output(change_output)
        .build();

    let mut dummy_claims = vec![];
    for _ in &tx.inputs {
        let dummy_sig = Signature::from_bytes(&vec![1; 65]).unwrap();

        // Build the spending script.
        let claim = SpendingScriptBuilder::new()
            .sighash_ty(SighashType::new(SighashBase::All))
            .p2pkh(dummy_sig, alice_pubkey.clone())
            .unwrap();

        dummy_claims.push(claim);
    }

    // Select the inputs and build the final transaction that includes the
    // change amount.
    let (tx, args) = SelectionBuilder::new(tx, args)
        .compile(dummy_claims)
        .unwrap()
        .select_inputs(InputSelector::Ascending, SATS_PER_VBYTE)
        .unwrap();

    // Compute the final preimage.
    let computer = SighashComputer::new(tx, args);
    let preimage = computer.preimage_tx().unwrap();

    // Create the final spending scripts.
    let mut claims = vec![];
    for sighash in preimage.into_h256_list().unwrap() {
        // Sign the sighash.
        let sig = alice_private_key.sign(sighash).unwrap();

        // Build the spending script.
        let claim = SpendingScriptBuilder::new()
            .sighash_ty(SighashType::new(SighashBase::All))
            .p2pkh(sig, alice_pubkey.clone())
            .unwrap();

        claims.push(claim);
    }

    // Select the inputs and build the transaction.
    let tx = computer.compile(claims).unwrap();

    // Two inputs were selected.
    assert_eq!(tx.inputs.len(), 2);
    assert_eq!(
        tx.inputs[0].previous_output.hash,
        utxo1.previous_output.hash
    );
    assert_eq!(
        tx.inputs[1].previous_output.hash,
        utxo2.previous_output.hash
    );

    // Check the amounts and fee precision.
    let fee = tx.fee(SATS_PER_VBYTE);
    assert_eq!(fee, 814);

    // NOTE: During UTXO selection, a dummy signature is used. Since the
    // DER-encoded signature lengths (plus Sighash type) can slightly vary (71
    // to 73 bytes), the projected fee during change output calculation can be
    // slightly off from the final fee.

    let total_input = arg1.amount + arg2.amount;
    let total_output = tx.outputs[0].value + tx.outputs[1].value + tx.outputs[2].value;
    assert_eq!(total_input, total_output + fee - 2); // off by 2

    let total_input = 1_000 + 3_000;
    let total_output = 1_000 + 1_000 + 1186; // 1186 = change_output
    assert_eq!(total_input, total_output + 814);

    // TODO:
}
