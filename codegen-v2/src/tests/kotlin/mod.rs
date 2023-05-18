use crate::codegen::kotlin::{render_to_strings, RenderIntput};
use crate::manifest::parse_str;

// Convenience function.
fn create_input(yaml: &str) -> RenderIntput {
    let file_info = parse_str(yaml).unwrap();

    RenderIntput {
        file_info,
        android_main_struct: include_str!("../../codegen/kotlin/templates/android_main_struct.hbs"),
        android_main_enum: include_str!("../../codegen/kotlin/templates/android_main_enum.hbs"),
        common_main_struct: include_str!("../../codegen/kotlin/templates/common_main_struct.hbs"),
        common_main_enum: include_str!("../../codegen/kotlin/templates/common_main_enum.hbs"),
    }
}

#[test]
// TODO: Check whether non-classes should render properties.
// TODO: Check non-class struct with methods.
fn androidmain_single_struct() {
    const INPUT: &str = include_str!("../manifest/struct.input.yaml");
    const EXPECTED: &str = include_str!("bindings/struct.kt");

    let input = create_input(INPUT);
    let rendered = render_to_strings(input).unwrap();

    assert_eq!(rendered.android_main.structs.len(), 1);

    // Check generated enum.
    let (name, output) = &rendered.android_main.structs[0];
    assert_eq!(name, "MainStruct");
    println!("{output}");
    assert_eq!(output, EXPECTED);
}

#[test]
fn androidmain_single_class() {
    const INPUT: &str = include_str!("../manifest/class.input.yaml");
    const EXPECTED: &str = include_str!("bindings/class.kt");

    let input = create_input(INPUT);
    let rendered = render_to_strings(input).unwrap();

    assert_eq!(rendered.android_main.structs.len(), 1);

    // Check generated enum.
    let (name, output) = &rendered.android_main.structs[0];
    println!("{output}");
    assert_eq!(name, "MainStruct");
    assert_eq!(output, EXPECTED);
}

#[test]
fn androidmain_optional() {
    const INPUT: &str = include_str!("../manifest/optional.input.yaml");
    const EXPECTED: &str = include_str!("bindings/optional.kt");

    let input = create_input(INPUT);
    let rendered = render_to_strings(input).unwrap();

    assert_eq!(rendered.android_main.structs.len(), 1);

    // Check generated enum.
    let (name, output) = &rendered.android_main.structs[0];
    println!("{output}");
    assert_eq!(name, "MainStruct");
    assert_eq!(output, EXPECTED);
}

#[test]
fn androidmain_enum_single() {
    const INPUT: &str = include_str!("../manifest/enum.input.yaml");
    const EXPECTED: &str = include_str!("bindings/enum.kt");

    let input = create_input(INPUT);
    let rendered = render_to_strings(input).unwrap();

    assert!(rendered.android_main.structs.is_empty());
    assert_eq!(rendered.common_main.enums.len(), 1);

    // Check generated enum.
    let (name, output) = &rendered.common_main.enums[0];
    println!("{output}");
    assert_eq!(name, "MainEnum");
    assert_eq!(output, EXPECTED);
}

#[test]
fn androidmain_enum_with_descriptions() {
    const INPUT: &str = include_str!("../manifest/enum_descriptions.yaml");
    const EXPECTED: &str = include_str!("bindings/enum_descriptions.kt");

    let input = create_input(INPUT);
    let rendered = render_to_strings(input).unwrap();

    assert!(rendered.android_main.structs.is_empty());
    assert_eq!(rendered.common_main.enums.len(), 1);

    // Check generated enum.
    let (name, output) = &rendered.common_main.enums[0];
    println!("{output}");
    assert_eq!(name, "MainEnum");
    assert_eq!(output, EXPECTED);
}

#[test]
// TODO: Check non-static
fn androidmain_enum_with_methods() {
    const INPUT: &str = include_str!("../manifest/enum_extension.input.yaml");
    const EXPECTED: &str = include_str!("bindings/enum_methods.kt");

    let input = create_input(INPUT);
    let rendered = render_to_strings(input).unwrap();

    assert!(rendered.android_main.structs.is_empty());
    assert_eq!(rendered.common_main.enums.len(), 1);

    // Check generated enum.
    let (name, output) = &rendered.common_main.enums[0];
    println!("{output}");
    assert_eq!(name, "MainEnum");
    assert_eq!(output, EXPECTED);
}
