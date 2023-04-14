use crate::grammar::{
    GKeyword, GMarkers, GPrimitive, GStructDecl, GStructName, GType, GTypeCategory,
};
use crate::{must_err, must_ok};

#[test]
fn test_struct_decls_separator_handling() {
    // Variants without values.
    let expected = GStructDecl {
        name: GStructName::from("SomeStruct"),
        fields: vec![
            (
                GKeyword::from("one"),
                GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
            ),
            (
                GKeyword::from("two"),
                GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
            ),
            (
                GKeyword::from("three"),
                GType::Mutable(GTypeCategory::Scalar(GPrimitive::Char)),
            ),
        ],
        markers: GMarkers(vec![]),
    };

    must_ok!(
        GStructDecl,
        "struct SomeStruct { int one; bool two; char three; };",
        expected
    );
    must_ok!(
        GStructDecl,
        " struct SomeStruct { int one; bool two; char three; };",
        expected
    );
    must_ok!(
        GStructDecl,
        "struct SomeStruct { int one ; bool two ; char three ; } ;",
        expected
    );
    must_ok!(
        GStructDecl,
        "struct SomeStruct{int one;bool two;char three;};",
        expected
    );
    must_ok!(
        GStructDecl,
        "struct\nSomeStruct\n{\nint\none\n;\nbool\ntwo\n;\nchar\nthree\n;\n}\n;",
        expected
    );

    // ERR!
    must_err!(
        GStructDecl,
        "structSomeStruct { int one; bool two; char three; };"
    );
    must_err!(
        GStructDecl,
        "struct SomeStruct { int; bool two; char three; };"
    );
    // No semicolon termination.
    must_err!(
        GStructDecl,
        "struct SomeStruct { int one; bool two; char three; }"
    );
}
