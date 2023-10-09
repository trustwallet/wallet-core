# About

This is a _work-in-progress_ parser meant to deprecate the existing Ruby parser
in `codegen/`. As of now, we only support Swift binding generation. This project
will progress over multiple stages (PRs).

## Execution

```bash
$ cd codegen-v2
$ cargo run -- swift
```

The bindings are saved to `bindings/`.
