use crate::grammar::{GPrimitive, ParseTree};
use crate::must_ok;
use crate::reader::Reader;

#[test]
fn test_primitives() {
    must_ok!(GPrimitive, "void", GPrimitive::Void);
    must_ok!(GPrimitive, "bool", GPrimitive::Bool);
    must_ok!(GPrimitive, "char", GPrimitive::Char);
    must_ok!(GPrimitive, "short", GPrimitive::ShortInt);
    must_ok!(GPrimitive, "signed", GPrimitive::Int);
    must_ok!(GPrimitive, "unsigned", GPrimitive::UnsignedInt);
    must_ok!(GPrimitive, "long", GPrimitive::LongInt);
    must_ok!(GPrimitive, "float", GPrimitive::Float);
    must_ok!(GPrimitive, "double", GPrimitive::Double);
    must_ok!(GPrimitive, "uint32_t", GPrimitive::UInt32T);
}
