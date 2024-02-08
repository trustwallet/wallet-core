use bitcoin::ScriptBuf;
use tw_encoding::hex;
use tw_misc::traits::ToBytesVec;
use tw_utxo::{
    encode::{stream::Stream, Encodable}, script::Script, signer::{TransactionSigner, TxSigningArgs, UtxoToSign}, signing_mode::SigningMethod, transaction::{
        standard_transaction::{Transaction, TransactionInput, TransactionOutput},
        transaction_parts::OutPoint,
    }
};

#[test]
fn signer() {
    let mut tx = Transaction {
        version: 2,
        inputs: Vec::new(),
        outputs: Vec::new(),
        locktime: 0,
    };

	let script_pubkey = {
		let pubkey_bytes = hex::decode("036666dd712e05a487916384bfcd5973eb53e8038eccbbf97f7eed775b87389536").unwrap();
		let pubkey = bitcoin::PublicKey::from_slice(&pubkey_bytes).unwrap();
		let script_pubkey = ScriptBuf::new_p2pkh(&pubkey.pubkey_hash());

		Script::from(script_pubkey.to_vec())
	};

	let utx_arg = UtxoToSign {
		script_pubkey,
		signing_method: SigningMethod::Legacy,
		amount: 50 * 100_000_000,
	};

	let utxo_args = TxSigningArgs {
		utxos_to_sign: vec![utx_arg],
		..Default::default()
	};

	// Prepare TX input

    let txid: Vec<u8> =
        hex::decode("1e1cdc48aa990d7e154a161d5b5f1cad737742e97d2712ab188027bb42e6e47b")
            .unwrap()
            .into_iter()
            .rev()
            .collect();

	let txid: [u8; 32] = txid.try_into().unwrap();

    let utxo = TransactionInput {
        previous_output: OutPoint {
            hash: txid.into(),
            index: 0,
        },
        sequence: u32::MAX,
        script_sig: Script::new(),
        witness: Vec::new(),
    };

	tx.inputs.push(utxo);

	// Prepare TX output

	let script_pubkey = {
		let pubkey_bytes = hex::decode("037ed9a436e11ec4947ac4b7823787e24ba73180f1edd2857bff19c9f4d62b65bf").unwrap();
		let pubkey = bitcoin::PublicKey::from_slice(&pubkey_bytes).unwrap();
		let script_pubkey = ScriptBuf::new_p2pkh(&pubkey.pubkey_hash());

		Script::from(script_pubkey.to_vec())
	};

	let output = TransactionOutput {
		value: 50 * 100_000_000 - 1_000_000,
		script_pubkey,
	};

	tx.outputs.push(output);

	// Sign preimages

	let mut signer = TransactionSigner::new(tx);
	signer.set_signing_args(utxo_args);

	let primage = signer.preimage_tx().unwrap();

	dbg!(primage);

	panic!();

	// Construct final... TODO

}
