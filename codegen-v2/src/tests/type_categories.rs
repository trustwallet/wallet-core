use crate::grammar::{GKeyword, GPrimitive, GStructName, GTypeCategory, ParseTree};
use crate::must_ok;

#[test]
fn test_types_categories() {
    #[rustfmt::skip]
    must_ok!(
        GTypeCategory,
        "int",
        GTypeCategory::Scalar(GPrimitive::Int)
    );
    must_ok!(
        GTypeCategory,
        "int*",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Scalar(GPrimitive::Int)))
    );
    must_ok!(
        GTypeCategory,
        "int **",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Scalar(GPrimitive::Int)
        ))))
    );
    must_ok!(
        GTypeCategory,
        "int * * *",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Scalar(GPrimitive::Int)))
        ))))
    );
}

#[test]
fn test_types_categories_struct() {
    must_ok!(
        GTypeCategory,
        "struct SomeStruct",
        GTypeCategory::Struct(GStructName::from("SomeStruct"))
    );
    must_ok!(
        GTypeCategory,
        "struct SomeStruct*",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Struct(GStructName::from(
            "SomeStruct"
        ))))
    );
    must_ok!(
        GTypeCategory,
        "struct SomeStruct **",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Struct(GStructName::from("SomeStruct"))
        ))))
    );
    must_ok!(
        GTypeCategory,
        "struct SomeStruct * * *",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Struct(GStructName::from(
                "SomeStruct"
            ))))
        ))))
    );
}

#[test]
fn test_types_categories_unrecognized() {
    must_ok!(
        GTypeCategory,
        "SomeType",
        GTypeCategory::Unrecognized(GKeyword::from("SomeType"))
    );
    must_ok!(
        GTypeCategory,
        "SomeType*",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Unrecognized(GKeyword::from(
            "SomeType"
        ))))
    );
    must_ok!(
        GTypeCategory,
        "SomeType **",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Unrecognized(GKeyword::from("SomeType"))
        ))))
    );
    must_ok!(
        GTypeCategory,
        "SomeType * * *",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Unrecognized(GKeyword::from(
                "SomeType"
            ))))
        ))))
    );
}
