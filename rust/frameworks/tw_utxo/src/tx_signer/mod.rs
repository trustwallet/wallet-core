// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::marker::PhantomData;

pub struct TxSigner<Transaction> {
    _phantom: PhantomData<Transaction>,
}

// impl<Transaction> TxSigner<Transaction> where Transaction: TransactionPreimage + TransactionInterface {
//     pub fn sign_tx(unsigned_tx: &UnsignedTransaction<Transaction>) -> SigningResult<Transaction> {
//         let x = SighashComputer::new(unsigned_tx.transaction().clone(), unsigned_tx.input_args())
//     }
// }
