// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_proto::Solana::Proto;

/// The two Unicode separators that break a line without being control characters, so
/// `char::is_control` does not catch them. `Zl` and `Zp` have exactly one member each.
const LINE_SEPARATOR: char = '\u{2028}';
const PARAGRAPH_SEPARATOR: char = '\u{2029}';

/// Renders a [`Proto::StructuredMessage`] into the text a wallet displays and a key signs.
///
/// The layout is exact, because the bytes are the contract with whoever rebuilds them: the
/// header on its own line if present, then one `"<label>: <value>"` line per field **in the
/// order given**, then, if a statement is present, one blank line and the statement. UTF-8, LF
/// line endings, a single space after each colon, and no trailing newline — the statement is the
/// last line and the message ends with it.
pub struct StructuredMessageRenderer;

impl StructuredMessageRenderer {
    pub fn render(msg: &Proto::StructuredMessage) -> SigningResult<String> {
        let mut lines = Vec::with_capacity(msg.fields.len() + 3);

        if !msg.header.is_empty() {
            lines.push(msg.header.to_string());
        }

        for (index, field) in msg.fields.iter().enumerate() {
            if field.label.is_empty() {
                return SigningError::err(SigningErrorType::Error_invalid_params)
                    .context(format!("'fields[{index}]' has an empty label"));
            }

            // An empty value would render a bare "Label:" line, which reads like a truncated
            // message to anyone auditing the signature — and silently disagrees with a peer
            // implementation that writes a placeholder there instead. The caller passes its own
            // placeholder, such as "-", so both sides render the same bytes.
            if field.value.is_empty() {
                return SigningError::err(SigningErrorType::Error_invalid_params).context(format!(
                "'fields[{index}]' ({}) has an empty value; pass a placeholder such as \"-\" instead",
                field.label
            ));
            }

            lines.push(format!("{}: {}", field.label, field.value));
        }

        if !msg.statement.is_empty() {
            lines.push(String::new());
            lines.push(msg.statement.to_string());
        }

        if lines.is_empty() {
            return SigningError::err(SigningErrorType::Error_invalid_params)
            .context("'structured_message' renders to nothing: it has no header, no fields and no statement");
        }

        // Every line is one line, checked before they are joined.
        //
        // This is a security check, not input hygiene. The body is one "Label: value" line per
        // field, so nothing in that shape distinguishes a line this renderer emitted from a line
        // a *value* contained: a value carrying a newline forges further labeled lines — a second
        // "Purpose:", an "Anchor:" naming another account — in the message the wallet displays
        // and in whatever consent record the verifier stores. The signature then covers those
        // bytes, which is what makes it dangerous: the signature is genuine and the message reads
        // as something it is not. The body is text, so rejecting the character is the only place
        // this can be stopped.
        //
        // It also pins the statement to a single line, which the layout requires anyway.
        for line in &lines {
            Self::check_single_line(line)?;
        }

        Ok(lines.join("\n"))
    }

    /// Requires a rendered line to carry no control character and no Unicode line separator.
    ///
    /// It screens characters, never meaning: a confusable or a bidi-reordered value is the
    /// caller's business, as is whether a value belongs in the message at all.
    fn check_single_line(line: &str) -> SigningResult<()> {
        for (at, ch) in line.char_indices() {
            if ch.is_control() || ch == LINE_SEPARATOR || ch == PARAGRAPH_SEPARATOR {
                return SigningError::err(SigningErrorType::Error_invalid_params).context(format!(
                    "'structured_message' carries {ch:?} at byte {at} of the line {line:?}; a signed value may not contain a control character"
                ));
            }
        }

        Ok(())
    }
}
