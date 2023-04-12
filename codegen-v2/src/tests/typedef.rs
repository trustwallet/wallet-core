use crate::grammar::{
    GKeyword, GMarker, GMarkers, GPrimitive, GType, GTypeCategory, GTypedef, ParseTree,
};
use crate::reader::Reader;
use crate::{must_err, must_ok};

#[test]
// TODO: Backslashes should be handled.
fn test_keyword_separator_handling() {
    let expected = GTypedef {
        ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
        name: "var".to_string(),
        markers: GMarkers(vec![]),
    };

    must_ok!(GTypedef, "typedef int var;", expected);
    must_ok!(GTypedef, " typedef int var;", expected);
    must_ok!(GTypedef, "typedef int var; ", expected);
    must_ok!(GTypedef, "typedef int var;\n", expected);
    must_ok!(GTypedef, "typedef int var ;", expected);
    must_ok!(GTypedef, "typedef\nint\nvar\n;", expected);

    // ERR
    must_err!(GTypedef, "\ntypedef int var ;");
    // No semicolon.
    must_err!(GTypedef, "typedef int var");
    must_err!(GTypedef, "typedef int var word;");
}

#[test]
// TODO: Update/expand
fn test_typedef() {
    must_ok!(
        GTypedef,
        "typedef int *_Nonnull TW_Aeternity_Proto_SigningInput;",
        GTypedef {
            ty: GType::Mutable(GTypeCategory::Pointer(Box::new(GTypeCategory::Scalar(
                GPrimitive::Int
            )))),
            name: "TW_Aeternity_Proto_SigningInput".to_string(),
            markers: GMarkers(vec![GMarker::NonNull])
        }
    );
    must_ok!(
        GTypedef,
        "typedef const void TWData;",
        GTypedef {
            ty: GType::Const(GTypeCategory::Scalar(GPrimitive::Void)),
            name: "TWData".to_string(),
            markers: GMarkers(vec![])
        }
    );
}
