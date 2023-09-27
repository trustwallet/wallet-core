// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::abi::token::Token;
use tw_memory::Data;

pub fn encode_tokens<I: IntoIterator<Item = Token>>(tokens: I) -> Data {
    let ethabi_tokens: Vec<_> = tokens.into_iter().map(Token::into_ethabi_token).collect();
    ethabi::encode(&ethabi_tokens)
}
