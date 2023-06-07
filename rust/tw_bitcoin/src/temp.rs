use bitcoin::blockdata::locktime::absolute::{Height, LockTime};
use bitcoin::consensus::Encodable;
use bitcoin::key::{KeyPair, PrivateKey, TapTweak, TweakedKeyPair, UntweakedKeyPair};
use bitcoin::script::ScriptBuf;
use bitcoin::sighash::{Prevouts, SighashCache, TapSighashType};
use bitcoin::transaction::Transaction;
use bitcoin::{secp256k1, OutPoint, Sequence, TxIn, TxOut, Txid, Witness};
use std::str::FromStr;

#[test]
// FOR: https://github.com/rust-bitcoin/rust-bitcoin/issues/1903
fn sign_p2tr_key_path() {
    let secp = secp256k1::Secp256k1::new();

    // WIF-formatted private keys (*regtest mode!*)
    let alice_wif = "cNDFvH3TXCjxgWeVc7vbu4Jw5m2Lu8FkQ69Z2XvFUD9D9rGjofN1";
    let bob_wif = "cNt3XNHiJdJpoX5zt3CXY8ncgrCted8bxmFBzcGeTZbBw6jkByWB";

    // Public keys:
    // alice: mvBCW6bHUpxrLD6ProHDSbPiBZHr1QrHE8
    // bob: mpHjC7jP4DApXZe9DbaqM6J8wMrCUoQRo6

    // Construct keypairs.
    let pk = PrivateKey::from_wif(alice_wif).unwrap();
    let alice = KeyPair::from_secret_key(&secp, &pk.inner);

    let pk = PrivateKey::from_wif(bob_wif).unwrap();
    let bob = KeyPair::from_secret_key(&secp, &pk.inner);

    // ## Create input
    let txid =
        Txid::from_str("9a582032f6a50cedaff77d3d5604b33adf8bc31bdaef8de977c2187e395860ac").unwrap();
    let vout = 0;
    let amount_sent = 4_999_000_000;

    // Here, Alice sent `amount_sent` to Bob.
    let input = TxIn {
        previous_output: OutPoint { txid, vout },
        // Empty scriptSig
        script_sig: ScriptBuf::new(),
        sequence: Sequence::default(),
        witness: Witness::new(),
    };

    let (bob_recipient, _) = bob.x_only_public_key();

    // Prepare prevouts, which are part of the signing payload.
    let prevouts = [TxOut {
        value: amount_sent,
        script_pubkey: ScriptBuf::new_v1_p2tr(&secp, bob_recipient, None),
    }];

    // >> For debugging, lets tweak Bob's pubkey and display it.
    {
        let (tweaked, _) = bob_recipient.tap_tweak(&secp, None);
        println!("{}", hex::encode(tweaked.serialize()));
    }

    // ## Create new P2TR key-path output (zeroed merkle root).

    // Bob wants to claim the output and send (some) of the amount back to Alice.
    let (alice_recipient, _) = alice.x_only_public_key();

    let output = TxOut {
        // This is `amount_sent` - miner-fee
        value: 4_998_000_000,
        script_pubkey: ScriptBuf::new_v1_p2tr(&secp, alice_recipient, None),
    };

    // Prepare transaction.
    let tx = Transaction {
        version: 2,
        // No lock
        lock_time: LockTime::Blocks(Height::ZERO),
        input: vec![input],
        output: vec![output],
    };

    // ## Construct the sighash `TapSighashType::Default`.
    let mut signer = SighashCache::new(tx);
    let sighash = signer
        .taproot_key_spend_signature_hash(0, &Prevouts::All(&prevouts), TapSighashType::Default)
        .unwrap();

    // Bob signs the message.
    let message = secp256k1::Message::from_slice(sighash.as_ref()).unwrap();

    // NEW:
    let tweaked: TweakedKeyPair = bob.tap_tweak(&secp, None);
    let bob = KeyPair::from(tweaked);
    // ^^Is there a nicer way to do this?

    let sig = bitcoin::taproot::Signature {
        sig: secp.sign_schnorr(&message, &bob),
        hash_ty: TapSighashType::Default,
    };

    // ## Update transaction with the witness containing the (serialized) Schnorr signature.
    let mut tx = signer.into_transaction();
    tx.input[0].witness = Witness::from_slice(&[sig.to_vec().as_slice()]);

    // Serialize and encode transaction.
    let mut buffer = vec![];
    tx.consensus_encode(&mut buffer).unwrap();

    let hex = hex::encode(buffer);

    println!("{hex}");
}
