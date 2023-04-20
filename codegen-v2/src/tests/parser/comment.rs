use crate::grammar::GInlineComment;
use crate::{must_err, must_ok};

#[test]
fn test_inline_comment() {
    let expected = GInlineComment::from("Some comment");

    must_ok!(GInlineComment, "/* Some comment */", expected);
    must_ok!(GInlineComment, "/*Some comment*/", expected);
    must_ok!(GInlineComment, " /* Some comment */ ", expected);

    must_err!(GInlineComment, "\n/* Some comment */");
}
