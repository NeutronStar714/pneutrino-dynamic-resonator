# Contributions and AI assistance

## NeutronStar714

The author provided the musical concept and detailed feature/UI specifications, directed the transition to four bands, tested in FL Studio, reported audible and visual problems, and refined behavior such as band sensitivity/direction and the input-loudness window. Surviving prompts include requests for explanations, corrections based on listening tests and statements about manual adjustments. Saved editor revisions document actual UI-file changes, but do not identify who typed every line.

## Claude Code during original development

Claude Code was used interactively to generate and edit C++/JUCE implementation code, implement requested features and UI changes, explain signal-processing/code behavior, propose debugging approaches, and assist with compilation/toolchain troubleshooting. The author supplied specifications, evaluated results and requested revisions. This is an AI-assisted project; the source is not presented as entirely manually written.

The recovery retains user prompts but lacks complete assistant replies and tool logs for the original project sessions. Exact line-by-line attribution and the model behind every interaction cannot be established from those records.

## Codex during documentation and source recovery

On October 31, 2026, Codex located and copied surviving prompts and VS Code snapshots, compared them with current code, and drafted retrospective process/changelog documentation. It prepared this publication tree, changed the CMake dependency lookup to a portable path and checked the build as recorded in [TESTING.md](TESTING.md). The original four C++ files were copied unchanged. These later recovery/publication steps are separate from the July/August development.

## Framework and references

JUCE supplies substantial plugin, UI, state-management and DSP infrastructure. BOSS FT-2, Fruity Parametric EQ 2 and the sibling Spacer project contributed the documented inspirations/references described in [third-party notices](../THIRD_PARTY_NOTICES.md). No human collaborator or mentor is identified by the recovered records; that absence does not establish that none existed.
