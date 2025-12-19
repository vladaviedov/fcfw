# fcfw

Frequency Counter Firmware

## Requirements

- ATTinyCore AVR toolchain
- GNU make

## Build
```
make
```

## Development

- `make dev` - Generate `compile_flags.txt` for `clangd`
- `make release` - Build release binary (same as `make`).
- `make debug` - Build debug binary.
- `make clean` - Remove build files
- `make format` - Autoformat files with `clang-format`
