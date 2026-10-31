---
name: pneutrino-series
description: "The Pneutrino plugin series — shared UI language, attribution string, and Dynamic Resonator as the styling reference"
metadata:
  node_type: memory
  type: project
  originSessionId: 63101803-aa82-43cf-9d98-af7ce6d705fe
  modified: 2026-10-10T10:28:28.629Z
---

The user builds a series of JUCE audio plugins under the brand **"Pneutrino by NeutronStar714"**, all living in `[workspace]\`. Every plugin in the series must share one visual language, so a new plugin's editor is styled by studying an existing sibling rather than designed from scratch.

Siblings seen so far: Snowfall, Extreme Shifter, HayesPitchShifter, Dynamic Resonator, Spacer, Pitcher, Poly Thrifter, Super Vibrato — plus Snow.

**Conventions (series-wide):**
- Dark theme: bg `0xFF1E1E1E`, panel `0xFF2A2A2A`, text `0xFFE0E0E0`, muted `0xFF888888`.
- Header: plugin name in "Courier New" 30 bold, in a light tint of the plugin's accent colour. Attribution line **"from Pneutrino by NeutronStar714"** in "Courier New" 20 plain, bottom, in the accent colour.
- Knob labels "Helvetica" 10 bold muted; group headers "Helvetica" 12 bold.
- A per-plugin `LookAndFeel_V4` subclass: custom `drawRotarySlider` (panel-filled ellipse, muted border, accent arc + tick), `getTextButtonFont` = "Courier New" 20 bold, sliders `RotaryHorizontalVerticalDrag` with `TextBoxBelow` 40×16 and double-click-to-default, and an explicit `setLookAndFeel(nullptr)` in the destructor.
- Group boxes: rounded rects (radius 10, border 2.5, margin 7) walking a shade ramp of the accent — deepest first, theme colour on the last box.
- Fixed-size window, `setResizable(false, false)`.

Per-plugin accent colours are the one thing that varies — Snowfall `#CCF1FF`, **Snow `#EAF6FF`** (ice white; header tint `#F7FCFF`). Snow's four group boxes walk `#5E7F92` → `#86A8BB` → `#B4C8D5` → `#EAF6FF`. **`[workspace]\Dynamic Resonator\Source\PluginEditor.{h,cpp}` is the canonical reference** to copy patterns from.

`[workspace]\Snow\plan.md` §2 holds the layout Snow settled on after several rounds of the user correcting it (knob size 96 → 48 px, edges aligned, Output box vertically centred).
