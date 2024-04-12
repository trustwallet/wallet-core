// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::prelude::{TWError, TWErrorKind};
use std::fmt;

pub type TWResult<T> = Result<T, TWError>;

pub trait ResultContext<T> {
    /// Wrap the error value with additional context.
    fn context<C>(self, context: C) -> TWResult<T>
    where
        C: fmt::Display;

    /// Wrap the error value with additional context that is evaluated lazily
    /// only once an error does occur.
    fn with_context<C, F>(self, f: F) -> TWResult<T>
    where
        C: fmt::Display,
        F: FnOnce() -> C;
}

pub trait IntoTWError<T> {
    fn into_tw(self) -> TWResult<T>;
}

pub trait MapTWError<T, E> {
    fn tw_err<F>(self, f: F) -> TWResult<T>
    where
        F: FnOnce(E) -> TWErrorKind;
}

pub trait OrTWError<T> {
    fn or_tw_err(self, error: TWErrorKind) -> TWResult<T>;
}

impl<T, E> ResultContext<T> for Result<T, E>
where
    TWError: From<E>,
{
    fn context<C>(self, context: C) -> TWResult<T>
    where
        C: fmt::Display,
    {
        self.map_err(|e| TWError::from(e).context(context))
    }

    fn with_context<C, F>(self, f: F) -> TWResult<T>
    where
        C: fmt::Display,
        F: FnOnce() -> C,
    {
        self.map_err(|e| TWError::from(e).context(f()))
    }
}

impl<T, E> IntoTWError<T> for Result<T, E>
where
    TWError: From<E>,
{
    fn into_tw(self) -> TWResult<T> {
        self.map_err(TWError::from)
    }
}

impl<T, E> MapTWError<T, E> for Result<T, E> {
    fn tw_err<F>(self, f: F) -> TWResult<T>
    where
        F: FnOnce(E) -> TWErrorKind,
    {
        self.map_err(|e| TWError::new(f(e)))
    }
}

impl<T> OrTWError<T> for Option<T> {
    fn or_tw_err(self, error: TWErrorKind) -> TWResult<T> {
        self.ok_or(TWError::new(error))
    }
}
