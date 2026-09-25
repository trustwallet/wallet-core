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
    /// Converts `PrevE` into `E` and wraps it as [`TWError<E>`].
    pub fn new<PrevE>(error: PrevE) -> Self
    where
        E: From<PrevE>,
    {
        TWError {
            error: E::from(error),
            context: Vec::default(),
        }
    }

    /// Converts `PrevE` into `E` and wraps it as [`Err(TWError<E>)`].
    pub fn err<T, PrevE>(error: PrevE) -> TWResult<T, E>
    where
        E: From<PrevE>,
    {
        Err(TWError::new(error))
    }

    /// Adds an error context.
    pub fn context<C>(mut self, context: C) -> Self
    where
        C: fmt::Display,
    {
        self.context.push(context.to_string());
        self
    }

    /// Returns an inner error type.
    pub fn error_type(&self) -> &E {
        &self.error
    }

    /// Converts [`TWError<E>`] into [`TWError<NewE>`].
    pub fn map_err<F, NewE>(self, f: F) -> TWError<NewE>
    where
        F: FnOnce(E) -> NewE,
    {
        TWError {
            error: f(self.error),
            context: self.context,
        }
    }

    fn format_context(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        if self.context.is_empty() {
            return Ok(());
        }
        writeln!(f)?;
        writeln!(f, "Context:")?;
        for (i, context) in self.context.iter().enumerate() {
            write!(f, "{i}. {context}")?;
            if i < self.context.len() - 1 {
                writeln!(f)?;
            }
        }
        Ok(())
    }
}

impl<E> From<E> for TWError<E> {
    fn from(inner: E) -> Self {
        TWError::new(inner)
    }
}

impl<E: fmt::Display> fmt::Display for TWError<E> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.error)?;
        self.format_context(f)
    }
}

impl<E: fmt::Debug> fmt::Debug for TWError<E> {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "{:?}", self.error)?;
        self.format_context(f)
    }
}
