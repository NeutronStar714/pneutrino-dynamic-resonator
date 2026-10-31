# Development records

This directory contains the complete recovered local `development-history` directory, published on October 31, 2026. Start with the [development process](../README.md), [full log](prompt-records.md) or [saved source versions](snapshot-index.md).

The original directory contained 129 files. All prompts, snapshots, diffs, build records, source copies and supporting records are retained. This directory index replaces the earlier recovery index. A publication manifest records the copied files; it is additional to the recovery files.

## Files

- `prompt-records.md`: readable log of all 114 prompts and 10 commands.
- `recovered-prompts.jsonl`: all project records with original line references and session identifiers.
- `original-history-lines.jsonl`: the corresponding original JSONL entries, with the same personal-path redactions as the readable log.
- `snapshots/` and `snapshot-index.md`: all 36 editor records, including the editor's `entries.json` files.
- `diffs/`: comparisons between successive recovered file versions.
- `last-snapshot-to-current-*.diff`: comparisons with the code at recovery.
- `snapshot-manifest.csv`: original editor dates, file locations and hashes.
- `current-source/` and `current-source-manifest.json`: the six project source/configuration files and their original metadata.
- `build-evidence/` and `build-evidence-manifest.json`: the 35 recovered build records and their original metadata.
- `related-project-references.jsonl`, `adjacent-spacer-context.jsonl` and `related-project-notes/`: later references and nearby build/styling context.
- `dependency-provenance.json`: the JUCE revision in use at recovery.
- `recovery-manifest.json`: the original recovery's file list and hashes, referring to the private copies before publication edits. It also lists two summary documents outside this directory as they existed at recovery.
- `publication-manifest.json`: original and published hashes, plus encoding/path-redaction information for these copies.

The original local username and personal workspace/download prefixes have been replaced with placeholders. Seventeen UTF-16 build logs were converted to UTF-8. Other source contents are preserved, including all saved C++ versions. Paths and hashes in older manifests describe the recovery; use `publication-manifest.json` to check the published files.

The records do not contain the missing assistant replies or early DSP source versions. Local timestamps document the stored records, rather than independently certifying when a change first worked. The historical `current-source` configuration and defaults are retained as records; use the repository-root CMake setup and `Source/` for the published build.
