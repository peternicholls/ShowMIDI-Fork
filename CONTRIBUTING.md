# Contributing

Thank you for your interest in improving ShowMIDI.

## DPI-aware UI development

To keep the interface consistent across displays:

- Use `sm::scaled()` for all size calculations.
- Reference values in `layout/Constants.h` instead of hard-coding dimensions.

See [docs/dpi-scaling.md](docs/dpi-scaling.md) for more information on DPI scaling.

