use std::io::{BufRead, BufReader, Read};

#[cfg(test)]
mod tests;

type Result<T> = std::result::Result<T, Error>;

trait ParseTree {
    type Derivation;

    fn derive<'a>(driver: Driver<'a>) -> Result<DerivationResult<'a, Self::Derivation>>;
}

#[derive(Debug, Clone)]
struct DerivationResult<'a, T> {
    derived: T,
    driver: DriverFinalized<'a>,
}

#[derive(Debug)]
enum Error {
    Todo,
    Eof,
}

impl<'a> From<&'a str> for Driver<'a> {
    fn from(buffer: &'a str) -> Self {
        Driver { buffer, pos: 0 }
    }
}

#[derive(Debug, Clone)]
struct DriverAwait<'a> {
    buffer: &'a str,
    pos: usize,
}

impl<'a> DriverAwait<'a> {
    fn commit(self, driver: DriverFinalized<'a>) -> Driver<'a> {
        Driver {
            buffer: self.buffer,
            pos: driver.pos,
        }
    }
    fn rollback(self) -> Driver<'a> {
        Driver {
            buffer: self.buffer,
            pos: self.pos,
        }
    }
}

#[derive(Debug, Clone)]
struct DriverFinalized<'a> {
    buffer: &'a str,
    pos: usize,
}

#[derive(Debug, Clone)]
struct Driver<'a> {
    buffer: &'a str,
    pos: usize,
}

impl<'a> Driver<'a> {
    fn scope(self) -> (DriverAwait<'a>, Driver<'a>) {
        (
            DriverAwait {
                buffer: self.buffer,
                pos: self.pos,
            },
            Driver {
                buffer: self.buffer,
                pos: 0,
            },
        )
    }
    fn finalized(self) -> DriverFinalized<'a> {
        DriverFinalized {
            buffer: self.buffer,
            pos: self.pos,
        }
    }
    fn read_until<P>(mut self) -> Result<(String, DriverScopeUsed<'a>)>
    where
        P: ParseTree,
    {
        let string = &self.buffer[self.pos..];

        for counter in 0..=string.len() {
            let slice = &string[counter..];

            dbg!(counter, slice);

            let driver = Driver::from(slice);
            if P::derive(driver).is_ok() {
                let target = string[..counter].to_string();
                dbg!(&target);

                return Ok((
                    target,
                    DriverScopeUsed {
                        buffer: self.buffer,
                        amt_read: counter,
                        pos: self.pos + counter,
                    },
                ));
            }
        }

        Err(Error::Todo)
    }
    fn read_amt(mut self, amt: usize) -> Result<(Option<String>, DriverScopeUsed<'a>)> {
        dbg!(&self.buffer, &self.buffer[self.pos..]);
        let string = &self.buffer[self.pos..];
        if string.len() < amt {
            return Ok((
                None,
                DriverScopeUsed {
                    buffer: self.buffer,
                    amt_read: 0,
                    pos: self.pos,
                },
            ));
        }

        let slice = &string[..amt];
        dbg!(slice);

        if slice.is_empty() {
            return Ok((
                None,
                DriverScopeUsed {
                    buffer: self.buffer,
                    amt_read: 0,
                    pos: self.pos,
                },
            ));
        }

        Ok((
            Some(slice.to_string()),
            DriverScopeUsed {
                buffer: self.buffer,
                amt_read: amt,
                pos: self.pos + amt,
            },
        ))
    }
}

#[derive(Debug, Clone)]
struct DriverScopeUsed<'a> {
    buffer: &'a str,
    amt_read: usize,
    pos: usize,
}

impl<'a> DriverScopeUsed<'a> {
    fn commit(mut self) -> Driver<'a> {
        Driver {
            buffer: self.buffer,
            pos: self.pos + self.amt_read,
        }
    }
    fn rollback(self) -> Driver<'a> {
        // Do nothing with `amt_read`.
        Driver {
            buffer: self.buffer,
            pos: self.pos,
        }
    }
}

enum EitherOr<T, D> {
    Either(T),
    Or(D),
}

impl<T: ParseTree, D: ParseTree> ParseTree for EitherOr<T, D> {
    type Derivation = EitherOr<T::Derivation, D::Derivation>;

    fn derive<'a>(driver: Driver<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        let (pending, scoped) = driver.scope();
        if let Ok(res) = T::derive(scoped) {
            return Ok(DerivationResult {
                derived: EitherOr::Either(res.derived),
                driver: res.driver,
            });
        }

        let driver = pending.rollback();

        if let Ok(res) = D::derive(driver) {
            return Ok(DerivationResult {
                derived: EitherOr::Or(res.derived),
                driver: res.driver,
            });
        }

        Err(Error::Todo)
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
struct GEof;

impl ParseTree for GEof {
    type Derivation = Self;

    fn derive<'a>(driver: Driver<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        let (slice, handle) = driver.read_amt(1)?;

        if slice.is_some() {
            return Err(Error::Todo);
        }

        Ok(DerivationResult {
            derived: GEof,
            driver: handle.commit().finalized(),
        })
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
enum GType {
    Bool,
    Char,
    Int,
    Struct(GStruct),
}

impl ParseTree for GType {
    type Derivation = Self;

    fn derive<'a>(mut driver: Driver<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        // Read data until the separator is reached, then return the sub-slice
        // leading up to it.
        let (slice, handle) = driver.read_until::<EitherOr<GSeparator, GEof>>()?;
        dbg!(&slice);

        let derived = match slice.as_str() {
            "bool" => GType::Bool,
            "char" => GType::Char,
            "int" => GType::Int,
            _ => {
                // Rollback driver, retry with the next derivation attempt.
                let mut driver = handle.rollback();

                if let Ok(res) = GStruct::derive(driver) {
                    return Ok(DerivationResult {
                        derived: GType::Struct(res.derived),
                        driver: res.driver,
                    });
                }

                return Err(Error::Todo);
            }
        };

        Ok(DerivationResult {
            derived,
            driver: handle.commit().finalized(),
        })
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
struct GStruct;

impl ParseTree for GStruct {
    type Derivation = Self;

    fn derive<'a>(_driver: Driver<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        todo!()
    }
}

type GSeparator = Continuum<GSeparatorItem>;

#[derive(Debug, Clone, Eq, PartialEq)]
enum GSeparatorItem {
    Space,
    Newline,
    Tab,
}

impl ParseTree for GSeparatorItem {
    type Derivation = Self;

    fn derive<'a>(driver: Driver<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        let (slice, handle) = driver.read_amt(1)?;
        dbg!(&slice, &handle);
        let slice = match slice {
            Some(string) => string,
            None => {
                return Err(Error::Todo);
            }
        };

        let derived = match slice.as_str() {
            " " => GSeparatorItem::Space,
            "\n" => GSeparatorItem::Newline,
            "\t" => GSeparatorItem::Tab,
            _ => return Err(Error::Todo),
        };

        Ok(DerivationResult {
            derived,
            driver: handle.commit().finalized(),
        })
    }
}

enum GParam {
    Item(GParamItem),
    Continued(GParamContinued),
}

enum GParamItem {}

#[derive(Debug, Clone, Eq, PartialEq)]
struct GParamItemWithMarker {
    ty: GType,
    marker: GMarker,
    name: GParamName,
}

impl ParseTree for GParamItemWithMarker {
    type Derivation = Self;

    fn derive<'a>(mut driver: Driver<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        // Derive parameter type.
        let (pending, scoped) = driver.scope();
        let ty_res = GType::derive(scoped)?;
        dbg!(&ty_res);

        driver = pending.commit(ty_res.driver);

        // Ignore leading separators.
        let (pending, scoped) = driver.scope();
        if let Ok(res) = EitherOr::<GSeparator, GEof>::derive(scoped) {
            driver = pending.commit(res.driver);
        } else {
            driver = pending.rollback();
        }

        // Derive marker, ignore leading separators.
        let (pending, scoped) = driver.scope();
        let marker_res = GMarker::derive(scoped)?;
        dbg!(&marker_res);

        driver = pending.commit(marker_res.driver);

        // Ignore leading separators.
        let (pending, scoped) = driver.scope();
        if let Ok(res) = EitherOr::<GSeparator, GEof>::derive(scoped) {
            driver = pending.commit(res.driver);
        } else {
            driver = pending.rollback();
        }

        // Derive parameter name.
        let (pending, scoped) = driver.scope();
        let name_res = GParamName::derive(scoped)?;
        dbg!(&name_res);

        // Everything derived successfully, return.
        Ok(DerivationResult {
            derived: GParamItemWithMarker {
                ty: ty_res.derived,
                marker: marker_res.derived,
                name: name_res.derived,
            },
            driver: pending.commit(name_res.driver).finalized(),
        })
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
struct GParamItemWithoutMarker {
    ty: GType,
    name: GParamName,
}

impl ParseTree for GParamItemWithoutMarker {
    type Derivation = Self;

    fn derive<'a>(mut driver: Driver<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        // Derive parameter type.
        let (pending, scoped) = driver.scope();
        let ty_res = GType::derive(scoped)?;
        dbg!(&ty_res);

        driver = pending.commit(ty_res.driver);

        // Ignore leading separators.
        let (pending, scoped) = driver.scope();
        if let Ok(res) = EitherOr::<GSeparator, GEof>::derive(scoped) {
            driver = pending.commit(res.driver);
        } else {
            driver = pending.rollback();
        }

        // Derive parameter name.
        let name_res = GParamName::derive(driver)?;
        dbg!(&name_res);

        // Everything derived successfully, return.
        Ok(DerivationResult {
            derived: GParamItemWithoutMarker {
                ty: ty_res.derived,
                name: name_res.derived,
            },
            driver: name_res.driver,
        })
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
struct GParamName(String);

impl ParseTree for GParamName {
    type Derivation = Self;

    fn derive<'a>(driver: Driver<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        let (string, handle) = driver.read_until::<EitherOr<GSeparator, GEof>>()?;

        Ok(DerivationResult {
            derived: GParamName(string),
            driver: handle.commit().finalized(),
        })
    }
}

struct GParamContinued {
    item: GParamItem,
    next: Box<GParam>,
}

impl ParseTree for GParam {
    type Derivation = Self;

    fn derive<'a>(driver: Driver<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        todo!()
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
struct GMarker(String);

impl ParseTree for GMarker {
    type Derivation = Self;

    fn derive<'a>(driver: Driver<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        let (string, handle) = driver.read_until::<GSeparator>()?;

        Ok(DerivationResult {
            derived: GMarker(string),
            driver: handle.commit().finalized(),
        })
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
enum Continuum<T> {
    Thing(T),
    Next(ContinuumNext<T>),
}

#[derive(Debug, Clone, Eq, PartialEq)]
struct ContinuumNext<T> {
    thing: T,
    next: Box<Continuum<T>>,
}

impl<T> Continuum<T> {
    fn add(self, new_thing: T) -> Self {
        match self {
            Continuum::Thing(thing) => Continuum::Next(ContinuumNext {
                thing,
                next: Box::new(Continuum::Thing(new_thing)),
            }),
            Continuum::Next(next) => Continuum::Next(ContinuumNext {
                thing: next.thing,
                next: Box::new(next.next.add(new_thing)),
            }),
        }
    }
}

impl<T: ParseTree<Derivation = T> + std::fmt::Debug> ParseTree for Continuum<T> {
    type Derivation = Continuum<T>;

    fn derive<'a>(mut driver: Driver<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        let mut sep_items: Option<Continuum<T::Derivation>> = None;
        loop {
            let (pending, scoped) = driver.scope();
            if let Ok(res) = T::derive(scoped) {
                driver = pending.commit(res.driver);

                if let Some(sep) = sep_items {
                    sep_items = Some(sep.add(res.derived));
                } else {
                    sep_items = Some(Continuum::Thing(res.derived));
                }
            } else {
                dbg!(&sep_items);
                if let Some(items) = sep_items {
                    return Ok(DerivationResult {
                        derived: items,
                        driver: pending.rollback().finalized(),
                    });
                } else {
                    return Err(Error::Todo);
                }
            }
        }
    }
}
