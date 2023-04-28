// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::grammar::{
    GFunctionDecl, GKeyword, GMarker, GMarkers, GParamItem, GPrimitive, GReturnValue, GStructName,
    GType, GTypeCategory,
};
use crate::{must_err, must_ok};

#[test]
fn test_func_params_separator_handling() {
    let expected = GParamItem {
        ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
        name: GKeyword::from("my_var"),
        markers: GMarkers(vec![]),
    };

    must_ok!(GParamItem, "int my_var", expected);
    must_ok!(GParamItem, " int my_var", expected);
    must_ok!(GParamItem, "int my_var ", expected);
    must_ok!(GParamItem, "int my_var\n", expected);
    must_ok!(GParamItem, "int\nmy_var", expected);
    must_ok!(GParamItem, "int\nmy_var\n", expected);

    must_err!(GParamItem, "\nint my_var");
    must_err!(GParamItem, "intmy_var");
}

#[test]
fn test_func_params() {
    must_ok!(
        GParamItem,
        "int my_var",
        GParamItem {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
            name: GKeyword::from("my_var"),
            markers: GMarkers(vec![]),
        }
    );
    must_ok!(
        GParamItem,
        "bool \nsome_bool",
        GParamItem {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
            name: GKeyword::from("some_bool"),
            markers: GMarkers(vec![]),
        }
    );
    must_ok!(
        GParamItem,
        "int _Nonnull my_var\n",
        GParamItem {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
            name: GKeyword::from("my_var"),
            markers: GMarkers(vec![GMarker::NonNull]),
        }
    );
    must_ok!(
        GParamItem,
        "bool\n_Nonnull some_bool\n",
        GParamItem {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
            name: GKeyword::from("some_bool"),
            markers: GMarkers(vec![GMarker::NonNull]),
        }
    );
}

#[test]
fn test_function_declaration() {
    let expected = GFunctionDecl {
        name: GKeyword::from("some_function"),
        params: vec![
            GParamItem {
                ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
                name: GKeyword::from("some_int"),
                markers: GMarkers(vec![]),
            },
            GParamItem {
                ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
                name: GKeyword::from("some_bool"),
                markers: GMarkers(vec![]),
            },
        ],
        return_value: GReturnValue {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Void)),
            markers: GMarkers(vec![]),
        },
        markers: GMarkers(vec![]),
    };

    must_ok!(
        GFunctionDecl,
        "void some_function(int some_int, bool some_bool);",
        expected
    );
    must_ok!(
        GFunctionDecl,
        "void some_function(int some_int,bool some_bool);",
        expected
    );
    must_ok!(
        GFunctionDecl,
        "void some_function ( int some_int , bool some_bool ) ;",
        expected
    );
    must_ok!(
        GFunctionDecl,
        "void\nsome_function\n(\nint\nsome_int\n,\nbool\nsome_bool\n)\n;",
        expected
    );

    // ERR
    must_err!(
        GFunctionDecl,
        "voidsome_function(int some_int, bool some_bool);"
    );
    // No comma.
    must_err!(
        GFunctionDecl,
        "void some_function(int some_int bool some_bool);"
    );
}

#[test]
fn test_function_declaration_with_markers() {
    let expected = GFunctionDecl {
        name: GKeyword::from("some_function"),
        params: vec![
            GParamItem {
                ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
                name: GKeyword::from("some_int"),
                markers: GMarkers(vec![]),
            },
            GParamItem {
                ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
                name: GKeyword::from("some_bool"),
                markers: GMarkers(vec![]),
            },
        ],
        return_value: GReturnValue {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Void)),
            markers: GMarkers(vec![]),
        },
        markers: GMarkers(vec![GMarker::TwExportStruct, GMarker::TWVisibilityDefault]),
    };

    must_ok!(
        GFunctionDecl,
        "TW_EXPORT_STRUCT void some_function(int some_int, bool some_bool) TW_VISIBILITY_DEFAULT;",
        expected
    );
}

#[test]
fn test_function_declaration_struct_return_value() {
    let expected = GFunctionDecl {
        name: GKeyword::from("some_function"),
        params: vec![
            GParamItem {
                ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Int)),
                name: GKeyword::from("some_int"),
                markers: GMarkers(vec![]),
            },
            GParamItem {
                ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Bool)),
                name: GKeyword::from("some_bool"),
                markers: GMarkers(vec![]),
            },
        ],
        return_value: GReturnValue {
            ty: GType::Mutable(GTypeCategory::Pointer(Box::new(GTypeCategory::Struct(
                GStructName::from("SomeStruct"),
            )))),
            markers: GMarkers(vec![GMarker::Nullable]),
        },
        markers: GMarkers(vec![
            GMarker::TwExportStaticMethod,
            GMarker::TWVisibilityDefault,
        ]),
    };

    must_ok!(
        GFunctionDecl,
        "TW_EXPORT_STATIC_METHOD struct SomeStruct* _Nullable some_function(int some_int, bool some_bool) TW_VISIBILITY_DEFAULT;",
        expected
    );

    must_ok!(
        GFunctionDecl,
        "struct TWStoredKey* _Nullable TWStoredKeyLoad(TWString* _Nonnull path);"
    );
}

#[test]
fn test_function_no_params() {
    let expected = GFunctionDecl {
        name: GKeyword::from("some_function"),
        params: vec![],
        return_value: GReturnValue {
            ty: GType::Mutable(GTypeCategory::Scalar(GPrimitive::Void)),
            markers: GMarkers(vec![]),
        },
        markers: GMarkers(vec![]),
    };

    must_ok!(GFunctionDecl, "void some_function();", expected);

    must_ok!(GFunctionDecl, "void some_function( );", expected);
}
