# Development process

Dynamic Resonator developed from a volume-controlled filter into a four-band dynamic EQ through listening tests, changes to the detector and modulation controls, and repeated work on the interface. The main development records run from **July 27 to August 2, 2026**. They were recovered on **October 31, 2026**.

The [summary](../PROCESS.md) follows the work by date. All recovered records are included below, including unsuccessful attempts, repeated bug reports, small UI adjustments and ideas that were put aside.

## Full record

| Record | Contents |
|---|---|
| [Development log](records/prompt-records.md) | All 124 project-associated entries: 114 prompts and 10 commands, in order, with dates and session identifiers. |
| [Saved source versions](records/snapshot-index.md) | All 36 VS Code entries: 34 versions of `PluginEditor.cpp` and two of `PluginEditor.h`, with the changes between them. |
| [Build records](records/build-evidence) | All 35 recovered configuration, compiler/linker tracking and VST3 metadata files. |
| [Source at recovery](records/current-source) | The four C++ files, original CMake configuration and defaults file as they existed when recovered. |
| [Raw prompt records](records/recovered-prompts.jsonl) | The same complete project log in JSONL form, including the original history-file line references. |
| [Other plugin references](records/related-project-references.jsonl) | Four prompts from later sibling-plugin work that refer to Dynamic Resonator's interface. |
| [Spacer context](records/adjacent-spacer-context.jsonl) | Five nearby prompts about sharing build and styling conventions; their connection to this project is inferred from the surrounding records. |
| [Later project notes](records/related-project-notes) | Two later assistant memory notes about the series' shared UI conventions. |

## What changed over the week

- **July 27:** the initial filter brief, compilation problems and the move to the MSVC build recipe.
- **July 28:** FL Studio tests, wet-signal distortion, envelope behavior, live meters and spectra.
- **July 29:** Pneutrino styling, spectrum/resonance problems and the four-band redesign.
- **July 30:** per-band sensitivity and direction, pre/post gain, defaults, layout changes, state persistence and attack/release debugging.
- **July 31–August 1:** Dynamic Range behavior, the distinction between raw loudness and modulation level, and the two meters.
- **August 2:** the 512-sample underrun report, more layout work and the frequency-text initialization fix.

The full log also retains later `/resume` entries. Those commands do not establish further development on their own.

## Source comparisons

The [snapshot index](records/snapshot-index.md) links every saved file and successive diff. The comparisons with the source at recovery are also available for [PluginEditor.cpp](records/last-snapshot-to-current-PluginEditor.cpp.diff) and [PluginEditor.h](records/last-snapshot-to-current-PluginEditor.h.diff).

These are individual file versions. They do not form complete historical project builds. The original DSP implementations from earlier in the week were not recovered.

## About the records

Prompt timestamps come from Claude Code's local history; source-version timestamps come from VS Code's local history. Times are shown in **Asia/Shanghai, UTC+08:00**. The prompts retain what was requested or reported; the original assistant replies are missing. One pasted build output survives only as a hash reference. No earlier Git repository was found.

Personal directory prefixes are redacted. Seventeen UTF-16 build logs were converted to UTF-8 so they can be read here. The saved C++ files are unchanged. [Archive details](records/README.md) and the [publication manifest](records/publication-manifest.json) record the file copies and these text changes. The original private copies remain preserved.

The [contributions page](../CONTRIBUTIONS.md) describes implementation assistance and the later record recovery. [Testing status](../TESTING.md) separates historical observations from the Release build check performed during publication.
