// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::grammar::GPrimitive;
use crate::{must_ok, must_err};

#[test]
fn test_primitives() {
    must_ok!(GPrimitive, "void", GPrimitive::Void);
    must_ok!(GPrimitive, "bool", GPrimitive::Bool);
    must_ok!(GPrimitive, "char", GPrimitive::Char);
    must_ok!(GPrimitive, "short", GPrimitive::ShortInt);
    must_ok!(GPrimitive, "signed", GPrimitive::Int);
    must_ok!(GPrimitive, "int", GPrimitive::Int);
    must_ok!(GPrimitive, "unsigned", GPrimitive::UnsignedInt);
    must_ok!(GPrimitive, "long", GPrimitive::LongInt);
    must_ok!(GPrimitive, "float", GPrimitive::Float);
    must_ok!(GPrimitive, "double", GPrimitive::Double);
    must_ok!(GPrimitive, "uint32_t", GPrimitive::UInt32T);

    must_err!(GPrimitive, "itn");
    must_err!(GPrimitive, "uint32");
}
