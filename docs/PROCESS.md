# Development summary

These notes summarize the records recovered on October 31, 2026. The [development process and full log](evidence/README.md) include every recovered project prompt, saved source revision, diff and build record. The original assistant replies and early DSP source versions were not recovered.

## July 27 — the first filter and build setup

The starting idea was an amplitude-driven filter, inspired by the BOSS FT-2's auto-wah behavior. The first brief specified dry/wet, sensitivity and resonance controls, a teal theme and attention to buffer size and Nyquist limits. Compilation ran into toolchain problems. A working recipe from Spacer helped establish the Visual Studio/MSVC build.

## July 28 — listening tests and displays

The first FL Studio test was followed by a report of severe wet-signal distortion with polyphonic input and drums. Questions about envelope wobble, RMS response and simultaneous notes led to more work on the detector and its displays. A loudness meter, live filter response and dry/wet spectra made it easier to see what the plugin was doing. The spectrum went through several revisions: refresh behavior, a fill that was later removed, interpolation and corrections to which signal each trace represented.

## July 29 — four bands and the Pneutrino interface

The interface took on the series' knob style, typography, attribution and colors. Spectrum-level accuracy and resonance behavior still needed attention. That evening, the design changed substantially: the original filter gave way to a high-pass, two peaking EQ bands and a low-pass, with independent controls. The redesign request referenced Fruity Parametric EQ 2's biquad peaking-filter approach.

## July 30 — band controls and envelope timing

Independent sensitivity and Up/Down controls replaced the earlier dynamic-link checkboxes. Frequency motion became exponential. Pre/post gain, defaults, vertical gain sliders, band boxes and graph labels were revised. Saved UI source versions show the small layout and styling changes as well as the larger additions.

Testing also exposed Pre/Post values not being restored, attack settings changing the apparent destination frequency, and release behavior that did not follow the control. The prompts record repeated attempts to understand and correct those problems.

## July 31–August 1 — the loudness window

The first Dynamic Range specification scaled the envelope into a chosen output interval. It was then changed to an input window: below the lower bound, modulation is zero; above the upper bound, it is full; inside the window, the value maps to 0–1. The current DSP implements that normalization and clamp.

A second meter separated the raw RMS/AM detector value from the mapped modulation value. “Perceived” is the project's label for that detector output, not a standardized LUFS measurement. Version labeling was discussed. A remote-disable idea was put aside.

## August 2 — performance and frequency text

FL Studio/ASIO4ALL testing produced a report of underruns at 512 samples but none at 1024. Current code reduces coefficient-update frequency, but no recovered benchmark establishes whether that resolved the problem.

Further UI work addressed spacing and frequency fields reverting to floating-point text on reopening. The latest recovered editor file and current source show the added `updateText()` call after the integer formatter is assigned. The [full record](evidence/README.md) includes that diff, all other saved UI revisions and the complete prompt sequence. [Testing status](TESTING.md) records the later Release build check and the runtime checks still pending.
