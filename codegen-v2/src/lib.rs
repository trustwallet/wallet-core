use std::io::{BufRead, BufReader, Read};

#[cfg(test)]
mod tests;

type Result<T> = std::result::Result<T, Error>;

trait ParseTree {
    type Derivation;

    fn derive<'a>(driver: DriverScope<'a>) -> Result<DerivationResult<'a, Self::Derivation>>;
}

#[derive(Debug, Clone)]
struct DerivationResult<'a, T> {
    derived: T,
    driver: DriverScope<'a>,
}

#[derive(Debug)]
enum Error {
    Todo,
    Eof,
}

impl<'a> From<&'a str> for DriverScope<'a> {
    fn from(buffer: &'a str) -> Self {
        DriverScope { buffer, pos: 0 }
    }
}

#[derive(Debug, Clone)]
struct DriverScope<'a> {
    buffer: &'a str,
    pos: usize,
}

impl<'a> DriverScope<'a> {
    fn scope(&self) -> Self {
        DriverScope {
            buffer: self.buffer,
            pos: 0,
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

            let driver = DriverScope::from(slice);
            if P::derive(driver).is_ok() {
                let target = string[..counter].to_string();
                dbg!(&target);

                return Ok((
                    target,
                    DriverScopeUsed {
                        buffer: self.buffer,
                        amt_read: counter,
                        pos: self.pos,
                    },
                ));
            }
        }

        Err(Error::Todo)
    }
    fn read_amt(mut self, amt: usize) -> Result<(Option<String>, DriverScopeUsed<'a>)> {
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
                pos: self.pos,
            },
        ))
    }
}

struct DriverScopeUsed<'a> {
    buffer: &'a str,
    amt_read: usize,
    pos: usize,
}

impl<'a> DriverScopeUsed<'a> {
    fn commit(mut self) -> DriverScope<'a> {
        DriverScope {
            buffer: self.buffer,
            pos: self.pos + self.amt_read,
        }
    }
    fn rollback(self) -> DriverScope<'a> {
        // Do nothing with `amt_read`.
        DriverScope {
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

    fn derive<'a>(driver: DriverScope<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        if let Ok(res) = T::derive(driver.scope()) {
            return Ok(DerivationResult {
                derived: EitherOr::Either(res.derived),
                driver: res.driver,
            });
        }

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

    fn derive<'a>(driver: DriverScope<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        let (slice, handle) = driver.read_amt(1)?;

        if slice.is_some() {
            return Err(Error::Todo);
        }

        Ok(DerivationResult {
            derived: GEof,
            driver: handle.commit(),
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

    fn derive<'a>(mut driver: DriverScope<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
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

        // Commit driver.
        driver = handle.commit();

        Ok(DerivationResult { derived, driver })
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
struct GStruct;

impl ParseTree for GStruct {
    type Derivation = Self;

    fn derive<'a>(_driver: DriverScope<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        todo!()
    }
}

type GSeparator = Continuum<GSeparatorItem>;

#[derive(Debug)]
enum GSeparatorItem {
    Space,
    Newline,
    Tab,
    Eof,
}

impl ParseTree for GSeparatorItem {
    type Derivation = Self;

    fn derive<'a>(driver: DriverScope<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        let (slice, handle) = driver.read_amt(1)?;
        dbg!(&slice);
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

        let driver = handle.commit();

        Ok(DerivationResult { derived, driver })
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

    fn derive<'a>(mut driver: DriverScope<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        // Derive parameter type.
        let ty_res = GType::derive(driver.scope())?;
        dbg!(&ty_res);

        // Ignore leading separators.
        if let Ok(res) = EitherOr::<GSeparator, GEof>::derive(ty_res.driver.scope()) {
            driver = res.driver;
        }

        // Derive marker, ignore leading separators.
        let marker_res = GMarker::derive(driver)?;
        dbg!(&marker_res);

        // TODO
        //let driver = EitherOr::<GSeparator, GEof>::derive(marker_der.driver).ignore_result();

        // Derive parameter name.
        let name_res = GParamName::derive(marker_res.driver)?;
        dbg!(&name_res);

        // Everything derived successfully, return.
        Ok(DerivationResult {
            derived: GParamItemWithMarker {
                ty: ty_res.derived,
                marker: marker_res.derived,
                name: name_res.derived,
            },
            driver: name_res.driver,
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

    fn derive<'a>(mut driver: DriverScope<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        // Derive parameter type.
        let ty_res = GType::derive(driver.scope())?;
        dbg!(&ty_res);

        // Ignore leading separators.
        if let Ok(res) = EitherOr::<GSeparator, GEof>::derive(ty_res.driver.scope()) {
            driver = res.driver;
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

    fn derive<'a>(driver: DriverScope<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        let (string, handle) = driver.read_until::<EitherOr<GSeparator, GEof>>()?;

        Ok(DerivationResult {
            derived: GParamName(string),
            driver: handle.commit(),
        })
    }
}

struct GParamContinued {
    item: GParamItem,
    next: Box<GParam>,
}

impl ParseTree for GParam {
    type Derivation = Self;

    fn derive<'a>(driver: DriverScope<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        todo!()
    }
}

#[derive(Debug, Clone, Eq, PartialEq)]
struct GMarker(String);

impl ParseTree for GMarker {
    type Derivation = Self;

    fn derive<'a>(driver: DriverScope<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        let (string, handle) = driver.read_until::<GSeparator>()?;

        Ok(DerivationResult {
            derived: GMarker(string),
            driver: handle.commit(),
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

    fn derive<'a>(mut driver: DriverScope<'a>) -> Result<DerivationResult<'a, Self::Derivation>> {
        let mut sep_items: Option<Continuum<T::Derivation>> = None;
        loop {
            if let Ok(res) = T::derive(driver.scope()) {
                driver = res.driver;

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
                        driver,
                    });
                } else {
                    return Err(Error::Todo);
                }
            }
        }
    }
}
