use crate::grammar::{GKeyword, GPrimitive, GStruct, GTypeCategory, ParseTree};
use crate::must_ok;
use crate::reader::Reader;

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
    must_ok!(
        GTypeCategory,
        "Unknown",
        GTypeCategory::Unknown(GKeyword::from("Unknown"))
    );
    must_ok!(
        GTypeCategory,
        "Unknown **",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Unknown(GKeyword::from("Unknown"))
        ))))
    );
    must_ok!(
        GTypeCategory,
        "Unknown * * *",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Unknown(GKeyword::from("Unknown"))))
        ))))
    );
}

#[test]
fn test_types_categories_struct() {
    must_ok!(
        GTypeCategory,
        "struct SomeStruct",
        GTypeCategory::Struct(GStruct::from("SomeStruct"))
    );
    must_ok!(
        GTypeCategory,
        "struct SomeStruct*",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Struct(GStruct::from("SomeStruct"))))
    );
    must_ok!(
        GTypeCategory,
        "struct SomeStruct **",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Struct(GStruct::from("SomeStruct"))
        ))))
    );
    must_ok!(
        GTypeCategory,
        "struct SomeStruct * * *",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Struct(GStruct::from("SomeStruct"))))
        ))))
    );
    must_ok!(
        GTypeCategory,
        "Unknown",
        GTypeCategory::Unknown(GKeyword::from("Unknown"))
    );
    must_ok!(
        GTypeCategory,
        "Unknown **",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Unknown(GKeyword::from("Unknown"))
        ))))
    );
    must_ok!(
        GTypeCategory,
        "Unknown * * *",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Unknown(GKeyword::from("Unknown"))))
        ))))
    );
}
