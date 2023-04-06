use super::grammar::ParseTree;
use super::{Error, Result};

#[derive(Debug, Clone)]
pub struct Reader<'a> {
    buffer: &'a str,
    pos: usize,
}

impl<'a> Reader<'a> {
    pub fn checkout(self) -> (ReaderPending<'a>, Reader<'a>) {
        (
            ReaderPending {
                buffer: self.buffer,
                pos: self.pos,
            },
            Reader {
                buffer: self.buffer,
                pos: self.pos,
            },
        )
    }
    pub fn into_branch(self) -> ReaderBranch<'a> {
        ReaderBranch {
            buffer: self.buffer,
            pos: self.pos,
        }
    }
    pub fn read_until<P>(self) -> Result<(String, ReaderStaged<'a>)>
    where
        P: ParseTree,
    {
        let string = &self.buffer[self.pos..];

        for counter in 0..=string.len() {
            let slice = &string[counter..];

            dbg!(counter, slice);

            let reader = Reader::from(slice);
            if P::derive(reader).is_ok() {
                let target = string[..counter].to_string();
                dbg!(&target);

                return Ok((
                    target,
                    ReaderStaged {
                        buffer: self.buffer,
                        amt_read: counter,
                        pos: self.pos,
                    },
                ));
            }
        }

        Err(Error::Todo)
    }
    pub fn read_amt(mut self, amt: usize) -> Result<(Option<String>, ReaderStaged<'a>)> {
        dbg!(&self.buffer, &self.buffer[self.pos..]);
        let string = &self.buffer[self.pos..];
        if string.len() < amt {
            return Ok((
                None,
                ReaderStaged {
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
                ReaderStaged {
                    buffer: self.buffer,
                    amt_read: 0,
                    pos: self.pos,
                },
            ));
        }

        Ok((
            Some(slice.to_string()),
            ReaderStaged {
                buffer: self.buffer,
                amt_read: amt,
                pos: self.pos,
            },
        ))
    }
}

impl<'a> From<&'a str> for Reader<'a> {
    fn from(buffer: &'a str) -> Self {
        Reader { buffer, pos: 0 }
    }
}

#[derive(Debug, Clone)]
pub struct ReaderStaged<'a> {
    buffer: &'a str,
    amt_read: usize,
    pos: usize,
}

impl<'a> ReaderStaged<'a> {
    pub fn commit(mut self) -> Reader<'a> {
        Reader {
            buffer: self.buffer,
            pos: self.pos + self.amt_read,
        }
    }
    pub fn reset(self) -> Reader<'a> {
        // Do nothing with `amt_read`.
        Reader {
            buffer: self.buffer,
            pos: self.pos,
        }
    }
}

#[derive(Debug, Clone)]
pub struct ReaderPending<'a> {
    buffer: &'a str,
    pos: usize,
}

impl<'a> ReaderPending<'a> {
    pub fn merge(self, reader: ReaderBranch<'a>) -> Reader<'a> {
        Reader {
            buffer: self.buffer,
            pos: reader.pos,
        }
    }
    pub fn discard(self) -> Reader<'a> {
        Reader {
            buffer: self.buffer,
            pos: self.pos,
        }
    }
}

#[derive(Debug, Clone)]
pub struct ReaderBranch<'a> {
    buffer: &'a str,
    pos: usize,
}
