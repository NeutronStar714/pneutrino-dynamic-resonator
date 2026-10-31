# How Dynamic Resonator developed

This account was reconstructed on **October 31, 2026** from surviving user prompts, saved editor revisions and current source. Original development records span July 27–August 2. It distinguishes a requested behavior from an implementation or test outcome that can actually be checked.

## From an auto-wah idea to four bands

The author's existing project description identifies the BOSS FT-2's volume-responsive filter as inspiration. The July 27 brief asked for a filter whose cutoff followed the incoming signal's level, with dry/wet, sensitivity and resonance controls. Getting it built involved toolchain troubleshooting and adapting a working Visual Studio/MSVC recipe from the sibling Spacer plugin.

On July 28, the author reported testing it in FL Studio and hearing severe distortion with polyphonic input or drums at full wet. They questioned the detector's response to constant-volume synths and multiple notes, and requested meters and spectra to make its behavior visible. On July 29, they directed a larger redesign: four independently controlled bands. The surviving implementation consists of HP, two peaking EQ bands and LP. [Original brief and redesign evidence](evidence/README.md#concept-and-redesign)

## Making modulation predictable

Subsequent requests specified each band's sensitivity and Up/Down frequency movement, pre/post gain and controllable attack/release. Listening tests exposed behavior that the author considered wrong: changing envelope timing appeared to change the final frequency, and release sometimes sounded unchanged across settings. These records document iteration; the missing assistant replies prevent a complete reconstruction of every intermediate fix.

A particularly clear correction concerned the **Dynamic Range** control. The first specification scaled the envelope into a chosen output interval. On July 31, the author instead specified an input window: below its lower limit, modulation is zero; above its upper limit, modulation is full; within it, the detector value maps to 0–1. The present code implements that normalization and clamp. Separate raw-detector and post-window meters make the distinction visible. Here “Perceived” names the RMS/AM detector output, not a standardized LUFS measurement. [Specification and code](evidence/README.md#dynamic-range-correction)

## Learning from the interface

The interface also evolved through detailed decisions about colors, knob geometry, typography, band grouping and graph labels. Thirty-six saved UI-file revisions survive. The author reported frequency fields reverting to floating-point text when reopening the interface. Comparing the latest recovered editor snapshot with current code reveals the added `updateText()` call after setting the integer formatter—a concrete, small before/after example. [Recovered diff](evidence/README.md#frequency-text-fix)

## What remains to establish

The author reported buffer underruns at 512 samples but none at 1024 in FL Studio/ASIO4ALL. The current code contains coefficient caching and an update check every four samples, but no recovered measurement proves that the issue was eliminated. Fresh runtime/performance validation and the guitar demonstration remain pending. The four source files are preserved in this import, with a portable build configuration and pinned JUCE dependency. [Testing status](TESTING.md)
