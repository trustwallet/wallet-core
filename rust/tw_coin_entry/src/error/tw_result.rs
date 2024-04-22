// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::error::prelude::TWError;
use std::fmt;

pub type TWResult<T, E> = Result<T, TWError<E>>;

pub trait ResultContext {
    /// Wraps the error value with additional context.
    fn context<C>(self, context: C) -> Self
    where
        C: fmt::Display;

    /// Wraps the error value with additional context that is evaluated lazily
    /// only once an error does occur.
    fn with_context<C, F>(self, f: F) -> Self
    where
        C: fmt::Display,
        F: FnOnce() -> C;
}

pub trait IntoTWError<T, E> {
    /// Wraps the inner `E` error into [`TWError<E>`].
    fn into_tw(self) -> TWResult<T, E>;
}

pub trait MapTWError<T, E, PrevE> {
    /// Maps `PrevE` into [`TWError<E>`] with an `F` mapper.
    fn tw_err<F>(self, f: F) -> TWResult<T, E>
    where
        F: FnOnce(PrevE) -> E;
}

pub trait OrTWError<T, E> {
    /// Transforms the [`Option<T>`] into a [`Result<T, TWError<E>>`], mapping [`Some(v)`] to
    /// [`Ok(v)`] and [`None`] to [`Err(TWError<E>)`].
    fn or_tw_err(self, error: E) -> TWResult<T, E>;
}

impl<T, E> ResultContext for TWResult<T, E> {
    fn context<C>(self, context: C) -> Self
    where
        C: fmt::Display,
    {
        self.map_err(|e| e.context(context))
    }

    fn with_context<C, F>(self, f: F) -> Self
    where
        C: fmt::Display,
        F: FnOnce() -> C,
    {
        self.map_err(|e| e.context(f()))
    }
}

impl<T, E, PrevE> IntoTWError<T, E> for Result<T, PrevE>
where
    TWError<E>: From<PrevE>,
{
    fn into_tw(self) -> TWResult<T, E> {
        self.map_err(TWError::from)
    }
}

impl<T, E, PrevE> MapTWError<T, E, PrevE> for Result<T, PrevE> {
    fn tw_err<F>(self, f: F) -> TWResult<T, E>
    where
        F: FnOnce(PrevE) -> E,
    {
        self.map_err(|e| TWError::new(f(e)))
    }
}

impl<T, E> OrTWError<T, E> for Option<T> {
    fn or_tw_err(self, error: E) -> TWResult<T, E> {
        self.ok_or(TWError::new(error))
    }
}
