# Sources and third-party notices

## Project-owned source

The repository's existing MIT license and `Copyright (c) 2026 NeutronStar714` notice are retained. They do not replace the license terms of external dependencies.

## JUCE

- Upstream: https://github.com/juce-framework/JUCE
- Pinned source: https://github.com/juce-framework/JUCE/tree/3ba67d4585e9d1fbcdb26a877c7978608b1f802e
- Framework notices: https://github.com/juce-framework/JUCE/blob/3ba67d4585e9d1fbcdb26a877c7978608b1f802e/LICENSE.md

JUCE provides audio/plugin and standalone infrastructure, UI components, parameter-state handling, FFT and biquad-filter facilities. Its license file describes its commercial/AGPLv3 alternatives and lists bundled dependencies and their notices. These terms remain with the upstream source. This repository does not relicense JUCE under its own MIT license.

## Design and development references

- **BOSS FT-2 dynamic filter:** the author's original repository description names its volume-responsive auto-wah behavior as the project's inspiration. This is a design reference, not a source dependency.
- **Image-Line Fruity Parametric EQ 2:** a recovered July 29 request references its biquad peaking-EQ approach during the four-band redesign. The current implementation uses JUCE's filter facilities; no Image-Line implementation source was recovered or included.
- **Spacer:** the author's sibling Pneutrino project supplied a working JUCE/MSVC build recipe and initial series styling conventions. Later sibling plugins used Dynamic Resonator's UI as their reference.

## Assistive tools

Claude Code assisted with implementation, explanation, debugging and build troubleshooting during development. Codex later recovered records and prepared this source/documentation import. See [CONTRIBUTIONS.md](docs/CONTRIBUTIONS.md) for the division of work and evidence limits.

Build tools include CMake and the Microsoft C++ toolchain. FL Studio was used for the author's plugin testing; ASIO4ALL appears in a historical performance report. No DAW, audio-driver or compiler software is redistributed in this repository.
