# Changelog

Historical entries were **reconstructed on 2026-10-31**, using surviving prompts, editor-file snapshots and current source. They describe documented development activity, not verified release dates. No original project Git history was recovered.

## 1.3.0 release — 2026-11-01

- Published separate Windows x64 standalone and VST3 packages built from the tagged source, with installation notes and checksums.
- Added the recorded guitar audio demo to the README and release downloads.
- Kept the existing 1.3.0 DSP/UI implementation unchanged.

## Development record — 2026-10-31

- Added all recovered project prompts, saved source revisions, diffs, build records and related notes.
- Replaced the shorter evidence page with a development-process summary linked to the complete recovered record. Personal directory paths are redacted; original records remain preserved locally.

## Source import — 2026-10-31

- Imported the existing project version declaration, **1.3.0**, and all four current C++ files unchanged.
- Replaced the machine-specific sibling JUCE path with an explicit portable dependency path and pinned upstream revision.
- Added build instructions, concise process/evidence documentation and detailed contribution/tool attribution.
- Preserved the repository's existing license and acknowledged separate dependency terms.
- Working guitar video and distributable demo remain pending; see [testing status](docs/TESTING.md).

## 2026-08-02

- Investigated the author's reported 512-sample buffer underruns; no before/after benchmark survives.
- Refined the UI and addressed frequency textbox initialization. A recovered/current code diff shows the explicit integer-text refresh.

## 2026-07-31 to 2026-08-01

- Specified the Dynamic Range control, then clarified its input-window-to-0–1 mapping; current DSP matches the later specification.
- Separated raw detector and mapped-envelope meters. Discussed version labeling; the exact suggested version is unavailable.

## 2026-07-30

- Refined per-band sensitivity/direction, defaults, pre/post gain, controls and visual grouping.
- Investigated persistence and envelope timing through user-reported tests. Actual saved UI-file revisions survive from this date.

## 2026-07-29

- Directed the four-band EQ redesign and Pneutrino series styling; investigated spectrum and resonance behavior.

## 2026-07-28

- Reported early FL Studio operation and wet-signal distortion; iterated on live meters, spectra, FFT display and envelope behavior.

## 2026-07-27

- Defined the initial dynamic-filter brief and investigated the Windows build toolchain using Spacer's working recipe.

[Development process](docs/evidence/README.md) includes the full recovered record and explains its gaps.
