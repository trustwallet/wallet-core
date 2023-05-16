use crate::codegen::kotlin::{render_to_strings, RenderIntput};
use crate::manifest::parse_str;

// Convenience function.
fn create_input(yaml: &str) -> RenderIntput {
	let file_info = parse_str(yaml).unwrap();

	RenderIntput {
		file_info,
		android_main_template: include_str!("../../codegen/kotlin/templates/androidmain_struct.hbs"),
	}
}

#[test]
fn androidmain_single_struct() {
	const INPUT: &str = include_str!("../manifest/struct.input.yaml");
	const EXPECTED: &str = include_str!("bindings/struct.kt");

    let input = create_input(INPUT);
    let rendered = render_to_strings(input).unwrap();

    assert_eq!(rendered.structs.len(), 1);

    // Check generated enum.
    let (name, output) = &rendered.structs[0];
	assert_eq!(name, "MainStruct");
    assert_eq!(output, EXPECTED);
}

#[test]
fn androidmain_single_class() {
	const INPUT: &str = include_str!("../manifest/class.input.yaml");
	const EXPECTED: &str = include_str!("bindings/class.kt");

    let input = create_input(INPUT);
    let rendered = render_to_strings(input).unwrap();

    assert_eq!(rendered.structs.len(), 1);

    // Check generated enum.
    let (name, output) = &rendered.structs[0];
	println!("{output}");
	assert_eq!(name, "MainStruct");
   	assert_eq!(output, EXPECTED);
}
