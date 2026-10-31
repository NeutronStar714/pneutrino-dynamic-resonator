# Dynamic Resonator

An input-responsive four-band equalizer for guitar, sound design and music production, from **Pneutrino by NeutronStar714**. Inspired by the BOSS FT-2 dynamic filter pedal's auto-wah behavior, it extends amplitude-driven frequency movement to a high-pass, two peaking EQ bands and a low-pass. Each band has independent frequency, Q, sensitivity and Up/Down sweep controls.

**Start here:** [How it was made](docs/PROCESS.md) · [Complete project source](Source) · [Build instructions](BUILDING.md) · [Contributions and AI assistance](docs/CONTRIBUTIONS.md)

## Demonstration status

**Video and downloadable demo are pending.** The author has an existing self-recorded guitar demonstration recoverable in FL Studio. A short recording comparing the identical guitar phrase with the plugin bypassed/enabled will be added here. This repository currently provides the source and process documentation; the working-demo part of the portfolio submission is not yet complete.

## Implemented in the recovered source

- Four stereo biquad bands with independent amplitude-driven frequency modulation.
- RMS/AM detection, attack/release, a selectable input-loudness window, dry/wet and pre/post gain.
- Live spectra, filter response, raw detector and modulation-envelope meters.
- Parameter-state save/restore; VST3 and standalone build targets.

Version **1.3.0** is the existing project declaration. [Testing and limitations](docs/TESTING.md) separates historical user reports from checks performed for this source import.

## Development record

The original work is documented by surviving July 27–August 2, 2026 prompts and saved editor revisions. [Selected evidence](docs/evidence/README.md) explains the design changes and a recovered bug-fix diff. [CHANGELOG.md](CHANGELOG.md) labels its historical entries as reconstructed. No original project Git history was recovered; this source import starts present-day version control.

## Source and dependencies

All four project-owned C++ files and the CMake build configuration are included. JUCE is referenced as an upstream submodule at an exact commit; use the recursive clone instructions in [BUILDING.md](BUILDING.md). No machine-specific build outputs or raw private conversation archive are included.

The repository's existing [MIT license](LICENSE) is retained for project-owned material. JUCE and its bundled dependencies retain their own terms; see [third-party notices](THIRD_PARTY_NOTICES.md).
