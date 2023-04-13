use crate::grammar::{GKeyword};
use crate::{must_err, must_ok};

#[test]
// TODO: Backslashes should be handled.
fn test_keyword_separator_handling() {
    let expected = GKeyword("some_key".to_string());

    must_ok!(GKeyword, "some_key", expected);
    must_ok!(GKeyword, " some_key", expected);
    must_ok!(GKeyword, "some_key ", expected);
    must_ok!(GKeyword, "some_key\n", expected);
    // Note that "other_value" is skipped
    must_ok!(GKeyword, "some_key other_value", expected);
    must_ok!(GKeyword, "some_key\nother_value", expected);

    // ERR
    must_err!(GKeyword, "\nsome_key");
}

#[test]
// TODO: Allow words that start with a number?
fn test_keyword() {
    must_ok!(GKeyword, "key", GKeyword::from("key"));
    must_ok!(GKeyword, "some_key", GKeyword::from("some_key"));
    must_ok!(GKeyword, "some-key", GKeyword::from("some-key"));
    must_ok!(GKeyword, "some_key88", GKeyword::from("some_key88"));
    must_ok!(GKeyword, "some_key(", GKeyword::from("some_key"));

    // Emtpy values not allowed
    must_err!(GKeyword, "");
}
