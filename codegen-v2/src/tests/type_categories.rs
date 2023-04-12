use crate::grammar::{GPrimitive, GStruct, GTypeCategory, ParseTree};
use crate::reader::Reader;

#[test]
fn test_types_categories() {
    let driver = Reader::from("int");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(der.derived, GTypeCategory::Scalar(GPrimitive::Int));

    let driver = Reader::from("int*");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Scalar(GPrimitive::Int)))
    );

    let driver = Reader::from("int **");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Scalar(GPrimitive::Int)
        ))))
    );

    let driver = Reader::from("int * * *");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Scalar(GPrimitive::Int)))
        ))))
    );

    let driver = Reader::from("Unknown");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(der.derived, GTypeCategory::Unknown("Unknown".to_string()));

    let driver = Reader::from("Unknown **");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Unknown("Unknown".to_string())
        ))))
    );

    let driver = Reader::from("Unknown * * *");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Unknown("Unknown".to_string())))
        ))))
    );
}

#[test]
fn test_types_categories_struct() {
    let driver = Reader::from("struct SomeStruct");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Struct(GStruct::from("SomeStruct"))
    );

    let driver = Reader::from("struct SomeStruct*");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Struct(GStruct::from("SomeStruct"))))
    );

    let driver = Reader::from("struct SomeStruct **");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Struct(GStruct::from("SomeStruct"))
        ))))
    );

    let driver = Reader::from("struct SomeStruct * * *");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Struct(GStruct::from("SomeStruct"))))
        ))))
    );

    let driver = Reader::from("Unknown");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(der.derived, GTypeCategory::Unknown("Unknown".to_string()));

    let driver = Reader::from("Unknown **");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Unknown("Unknown".to_string())
        ))))
    );

    let driver = Reader::from("Unknown * * *");
    let der = GTypeCategory::derive(driver).unwrap();
    assert_eq!(
        der.derived,
        GTypeCategory::Pointer(Box::new(GTypeCategory::Pointer(Box::new(
            GTypeCategory::Pointer(Box::new(GTypeCategory::Unknown("Unknown".to_string())))
        ))))
    );
}
