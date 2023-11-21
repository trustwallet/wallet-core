// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{Error, Result};
use std::fs;
use std::path::{Path, PathBuf};

pub fn read_lines<P: AsRef<Path>>(path: P) -> Result<Vec<String>> {
    let lines = fs::read_to_string(path)?
        .split('\n')
        .map(|line| line.to_string())
        .collect();
    Ok(lines)
}

pub fn write_lines<P: AsRef<Path>>(path: P, lines: Vec<String>) -> Result<()> {
    let content = lines.join("\n");
    fs::write(path, content).map_err(Error::from)
}

pub struct FileContent {
    path: PathBuf,
    lines: Vec<String>,
}

impl FileContent {
    pub fn read(path: PathBuf) -> Result<FileContent> {
        read_lines(&path).map(|lines| FileContent { path, lines })
    }

    pub fn find_region_with_comments(
        &mut self,
        start_comment: &str,
        end_comment: &str,
    ) -> Result<FileRegion<'_>> {
        // Find the position of the `start_comment`.
        let start_comment_at = self
            .lines
            .iter()
            .position(|line| line.contains(start_comment))
            .ok_or_else(|| {
                Error::io_error_other(format!("Cannot find the `{start_comment}` line"))
            })?;
        let end_comment_at = self
            .lines
            .iter()
            .skip(start_comment_at)
            .position(|line| line.contains(end_comment))
            .ok_or_else(|| {
                Error::io_error_other(format!("Cannot find the `{end_comment}` line"))
            })?
            + start_comment_at;

        let region_starts_at = start_comment_at + 1;
        let region_ends_at = end_comment_at - 1;

        if region_starts_at > region_ends_at {
            return Err(Error::io_error_other(format!(
                "There must be the content between {start_comment} and {end_comment}"
            )));
        }

        Ok(FileRegion {
            lines: &mut self.lines,
            region_starts_at,
            region_ends_at,
        })
    }

    pub fn rfind_line<F>(&mut self, f: F) -> Result<LinePointer<'_>>
    where
        F: Fn(&str) -> bool,
    {
        let line_idx = self
            .lines
            .iter()
            .rposition(|line| f(&line))
            .ok_or_else(|| {
                Error::io_error_other(format!(
                    "{:?} file does not contain a required pattern",
                    self.path
                ))
            })?;
        Ok(LinePointer {
            lines: &mut self.lines,
            line_idx,
        })
    }

    pub fn write(self) -> Result<()> {
        write_lines(self.path, self.lines)
    }
}

pub struct FileRegion<'a> {
    lines: &'a mut Vec<String>,
    region_starts_at: usize,
    region_ends_at: usize,
}

impl<'a> FileRegion<'a> {
    pub fn push_line(&mut self, line: String) {
        self.lines.insert(self.region_ends_at + 1, line);
    }

    pub fn sort(&mut self) {
        self.lines[self.region_starts_at..self.region_ends_at].sort()
    }
}

pub struct LinePointer<'a> {
    lines: &'a mut Vec<String>,
    line_idx: usize,
}

impl<'a> LinePointer<'a> {
    pub fn push_line_after(&mut self, line: String) {
        self.lines.insert(self.line_idx + 1, line);
    }
}
