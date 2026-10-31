# Selected development evidence

Recovered on **2026-10-31**. Original record times below are **UTC+08:00 (Asia/Shanghai)**. The full local recovery contains 114 user prompts, 10 commands and 36 UI-file snapshots. This page selects a few relevant items and explains their limits. The original project Git repository and complete assistant transcripts were not recovered.

[selected-prompts.json](selected-prompts.json) preserves six dated user prompts. Local machine paths were replaced with descriptive placeholders; no assistant replies or outcomes have been invented. H identifiers refer to the source history-file line at recovery.

## Concept and redesign

**H591 — July 27, 19:48:** the initial brief requested a filter tracking input volume, dry/wet/sensitivity/resonance controls and a teal theme, with attention to buffer length and Nyquist frequencies.

**H644 — July 29, 23:38:** the author requested an overhaul to four independently controlled bands and referenced Fruity Parametric EQ 2's biquad peaking-filter approach. The current source uses HP → Peak → Peak → LP.

**What this establishes:** a dated change in the author's design direction. The earlier DSP implementation was not recovered, so these prompts do not provide executable before/after DSP versions.

**H655 — July 30, 02:32:** the next refinements specified independent sensitivities, Up/Down motion and exponential frequency movement. These controls are present in the current source.

## Dynamic Range correction

**H709 — July 31, 22:15:** the author clarified that only loudness within the selected input window should move the modulation value from 0 to 1; lower/higher inputs should clamp to the corresponding endpoint.

The current implementation in [PluginProcessor.cpp](../../Source/PluginProcessor.cpp) reads:

```cpp
const float drRange = std::max (drMax - drMin, 0.001f);
// ... rawEnvelope is computed by the RMS/AM detector ...
envelope = (rawEnvelope - drMin) / drRange;
envelope = juce::jlimit (0.0f, 1.0f, envelope);
```

This excerpt omits intervening code; it is an explanation of the current implementation, not a historical patch. It demonstrates how a listening/control requirement became a precise normalization and clamp. Separate raw and mapped meters support interpreting this behavior.

## Frequency text fix

**H717 — August 2, 19:24:** the author reported that frequency number fields showed values such as `20.0000000` on reopening, until a small adjustment restored integer formatting.

[frequency-text-refresh.diff](frequency-text-refresh.diff) compares the latest recovered `PluginEditor.cpp` snapshot (editor record **August 2, 19:22:05.532**) with the current file. It adds an explicit `bandFreqKnob[b].updateText()` after assigning the integer formatter, with a comment explaining that the attachment's initial update had already painted the default float formatting.

**What this establishes:** actual saved source before and after the small initialization fix, consistent with the reported bug. It does not supply a complete historical project build or a fresh runtime test.

## Performance report and historical defaults

**H714 — August 2, 13:04:** the author reported FL Studio/ASIO4ALL underruns at 512 samples but none at 1024. This is a user report, not a benchmark proving current performance. [Testing status](../TESTING.md) keeps the issue explicit.

[defaults-2026-07-30.txt](defaults-2026-07-30.txt) is the surviving defaults reference, last written July 30 according to local filesystem metadata. It predates later controls and is included as a design artifact. The parameter declarations in [PluginProcessor.cpp](../../Source/PluginProcessor.cpp) are authoritative for the published code's actual defaults.

## Provenance

Raw prompts came from Claude Code's local history, and historical UI files came from VS Code local history. These are local records, not independently certified timestamps. The private originals and their hashes were retained during recovery. [SOURCE_PROVENANCE.json](../SOURCE_PROVENANCE.json) records hashes of the published project source and the exact JUCE dependency revision. Retrospective explanations were prepared with Codex; see [contributions](../CONTRIBUTIONS.md).
