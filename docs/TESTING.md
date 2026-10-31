# Testing and current limits

## Historical user reports

- July 28, 2026: the author reported the plugin working in FL Studio, followed by distortion with polyphonic/drum input at full wet.
- July 30–August 2: reports covered envelope timing, parameter persistence, display formatting and UI layout.
- August 2: the author reported ASIO4ALL/FL Studio underruns at 512 samples, with none at 1024. No recovered benchmark establishes later resolution.

These are contemporaneous observations from the recovered prompts, not newly reproduced test results.

## Source-import checks — October 31, 2026

- The four published C++ files are byte-for-byte copies of the recovered current project source.
- The portable CMake configuration uses the exact installed JUCE revision, pinned as a public submodule.
- CMake configuration and both Windows x64 Release targets (`DynamicResonator_VST3` and `DynamicResonator_Standalone`) completed successfully with exit code 0. The VST3 bundle and standalone executable were produced; the VST3 manifest helper also completed.
- Toolchain: CMake 4.4.0-rc1, Visual Studio 2022 Build Tools / MSVC 19.44.35228, Windows SDK 10.0.26100.0. The check used the existing clean JUCE checkout at the pinned revision through `PNEUTRINO_JUCE_SOURCE_DIR`.
- MSVC emitted warnings, including source-code-page and numeric-conversion warnings. They were not treated as evidence of runtime correctness, and no DSP/UI source changes were made to silence them.

## Pending

Fresh FL Studio listening tests, state save/reopen checks, sample-rate/buffer-size measurements, a portable executable download, and the guitar comparison video have not been completed for this import. No macOS/Linux runtime claim is made. No historical automated DSP test suite was recovered for this project.

The current audio callback still constructs JUCE coefficient objects when filter updates are needed. Reduced update frequency is not proof of allocation-free processing or a particular real-time performance level.
