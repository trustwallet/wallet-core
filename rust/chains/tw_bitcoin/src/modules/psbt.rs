// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use bitcoin::psbt::Psbt;
use tw_encoding::hex::DecodeHex;
use tw_utxo::transaction::standard_transaction::Transaction;

/// Finalizes the [Partially Signed Bitcoin Transaction](Psbt)
/// by updating the final `script_sig` and/or `witness`.
pub fn update_psbt_signed(psbt: &mut Psbt, signed_tx: &Transaction) {
    for (signed_txin, utxo_psbt) in signed_tx.inputs.iter().zip(psbt.inputs.iter_mut()) {
        if !signed_txin.script_sig.is_empty() {
            utxo_psbt.final_script_sig = Some(bitcoin::ScriptBuf::from_bytes(
                signed_txin.script_sig.to_vec(),
            ));
        }

        if !signed_txin.witness.is_empty() {
            let mut final_witness = bitcoin::Witness::new();
            for witness_item in signed_txin.witness.as_items() {
                final_witness.push(bitcoin::ScriptBuf::from_bytes(witness_item.to_vec()));
            }
            utxo_psbt.final_script_witness = Some(final_witness);
        }
    }
}

// 11c64b7f21cc44a64c982da18db19f5fb31e6512610d0f0f79b12814d53c200cd5589fd869efa4d4f6143b7dba4f7884a2bf8f1db20afa4ff7724bad2a84681a
// 22349d945a8627ab359cc4a4aa747a00c44282d6234bf932256794e77ebf5f244f4dc5ccd8c1a91d050a5107bcf863cc12428e7c34b295ed9318014a7026cbb0
// 885a26f7ccaa1c02239deb8ff94560c44491b4a7ddc5158be1f79a07fd82485879158323910c71e64562240e56f9542be3354be5ddfb10445146bfd60e25a78b
// 2042d4ed84856b24fd73a3e5fd91878cc13d8811dd6ee97258a627e52e639d0b37ad2017921cf156ccb4e73d428f996ed11b245313e37e27c978ac4d2cc21eca4672e4ac2049766ccd9e3cd94343e2040474a77fb37cdfd30530d05f9f1e96ae1e2102c86eba2076d1ae01f8fb6bf30108731c884cddcf57ef6eef2d9d9559e130894e0e40c62cba529c
// c150929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac0b50d6b23527ea75dfcadcee29f64568f1b1772f2456649cc8c33201fcff0c93f5eed1a7cb2bd4335325aa437ec9ec5f2effe1f51aa9dc41da14b0642482cc0c7
#[test]
fn test_foo() {
    use tw_encoding::hex::DecodeHex;
    use tw_encoding::hex::ToHex;
    use tw_hash::H256;
    use tw_keypair::schnorr;
    use tw_keypair::traits::VerifyingKeyTrait;

    let x = "70736274ff01005e0200000001f29e073986649ca2c5b9896d9cc606790df58587d4a267e9f90610bc4bffe8690000000000960000000191740000000000002251205d1b83f2e2991c2d80226a54d89255768a77905d63d0d5f51d18476143f90a8e000000000001012b3075000000000000225120fd1b7665d4c5211d0b98600d4915ae7181b0cd7fcac1edc973e2fbc954360a126215c050929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac08cec39f47bbd70eeda791f6b48a4bf906e878c87f6e5ca3f650008f39f1d4a51e77ab126d8d0a6c6ca6c2f1c0d31141352fb95112aa2bbd4390a7e4481e3d8dc27209789cdd12bc90bbd73445718f8a709956eb3cce362716a3425610abb75ea1132ad029600b2c001172050929b74c1a04954b78b4b6035e97a5e078a5a0f28ec96d547bfee9ace803ac00000".decode_hex().unwrap();
    let y = Psbt::deserialize(&x).unwrap();
    println!("PSBT:");
    println!("{}", serde_json::to_string(&y).unwrap());

    // bitcoin::taproot::TaprootSpendInfo::control_block()
    // println!("merkle_root: {}", y.inputs[0].tap_scripts.iter().next().unwrap().)

    let sighash = H256::from("478673e3ea9442f7f51072f0516ebeb1d4b9e5e812b0d052ee9edfa3dba431a2");
    let pk = schnorr::PublicKey::try_from(
        "039789cdd12bc90bbd73445718f8a709956eb3cce362716a3425610abb75ea1132"
            .decode_hex()
            .unwrap()
            .as_slice(),
    )
    .unwrap();
    let sig = schnorr::Signature::from_bytes("2856dd1927e5f95fdc45346261f17455130244f5d3db225ceaef292ed233b66104fea4eb343f7f9ee99a8ee6d67b4e5a95d660bf0299b67d37a96e08e6839be8".decode_hex().unwrap().as_slice()).unwrap();
    assert!(pk.verify(sig, sighash));
}
