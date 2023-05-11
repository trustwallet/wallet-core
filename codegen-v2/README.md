# About

This is a _work-in-progress_ parser meant to deprecate the existing Ruby parser
in `codegen/`. This project will progess over multiple stages (PRs).

## Execution

```bash
$ cd codegen-v2
$ cargo run
```

This command reads the C headers in `include/` and generates a
`include_manifest.json` file.
