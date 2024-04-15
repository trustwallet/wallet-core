// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::tw_result::TWResult;
use std::fmt;

/// The wrapper over the `Common::TWErrorKindType` error for convenient use.
pub struct TWError<E> {
    error: E,
    context: Vec<String>,
}

impl<E> TWError<E> {
    pub fn new<PrevE>(error: PrevE) -> Self
    where
        E: From<PrevE>,
    {
        TWError {
            error: E::from(error),
            context: Vec::default(),
        }
    }

    pub fn err<T, PrevE>(error: PrevE) -> TWResult<T, E>
    where
        E: From<PrevE>,
    {
        Err(TWError::new(error))
    }

    pub fn context<C>(mut self, context: C) -> Self
    where
        C: fmt::Display,
    {
        self.context.push(context.to_string());
        self
    }

    pub fn error_type(&self) -> &E {
        &self.error
    }

    fn format_context(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        for (i, context) in self.context.iter().enumerate() {
            write!(f, "{i}. {context}")?;
            if i == self.context.len() - 1 {
                writeln!(f)?;
            }
        }
        Ok(())
    }
}

impl<E: fmt::Display> fmt::Display for TWError<E> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        writeln!(f, "{}. Context:", self.error)?;
        self.format_context(f)
    }
}

impl<E: fmt::Debug> fmt::Debug for TWError<E> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        writeln!(f, "{:?}. Context:", self.error)?;
        self.format_context(f)
    }
}
