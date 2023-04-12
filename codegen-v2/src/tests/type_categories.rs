use crate::grammar::{GPrimitive, GStruct, GTypeCategory, ParseTree};
use crate::reader::Reader;
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
    must_ok!(
        GTypeCategory,
        "Unknown",
        GTypeCategory::Unknown("Unknown".to_string())
    );
    must_ok!(
        GTypeCategory,
        "Unknown **",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Unknown("Unknown".to_string())
        ))))
    );
    must_ok!(
        GTypeCategory,
        "Unknown * * *",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Unknown("Unknown".to_string())))
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
        GTypeCategory::Unknown("Unknown".to_string())
    );
    must_ok!(
        GTypeCategory,
        "Unknown **",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Unknown("Unknown".to_string())
        ))))
    );
    must_ok!(
        GTypeCategory,
        "Unknown * * *",
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Unknown("Unknown".to_string())))
        ))))
    );
}
