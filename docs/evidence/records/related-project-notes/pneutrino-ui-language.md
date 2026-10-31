---
name: pneutrino-ui-language
description: "The shared Pneutrino series UI design language (from Dynamic Resonator) - palette roles, fonts, knob/button/box drawing, header & attribution"
metadata: 
  node_type: memory
  type: reference
  originSessionId: a03a879b-7523-4049-8ddd-824eae083cbe
  modified: 2026-08-08T15:46:15.134Z
---

The Pneutrino series (NeutronStar714) unifies UI styling across plugins. Reference implementation: `[workspace]\Dynamic Resonator\Source\PluginEditor.cpp` (DynamicResonatorLookAndFeel + editor). Extreme Shifter was restyled to this language 2026-08-08, keeping its own warm theme (#543526 family) while adopting the structure.

**Palette roles** (per-plugin theme supplies the accent; roles are fixed):
- `accentColour` — theme; knob arc? no: see below. Used for: button OFF fill, (DR: arc/tick/attribution — only when the theme is mid-light enough to read)
- `accentBright`/`accentLightest` — lighter tints of the theme: knob arc + tick, attribution (bright), header name (lightest)
- `panelBg` = window bg lightened one step — knob recessed well, button ON fill
- `darkBg` — window background
- `textColour` — primary text (labels of key knobs, box captions)
- `muteText` — mid grey/warm-grey: knob outline, secondary knob labels
- Box borders use the accent family in a dark→light gradient across columns

**Fonts:** header name = "Courier New" 30 bold, lightest accent tint, centered top (bounds: removeFromTop(34).withY(4)); attribution = "Courier New" 20 plain, accentBright, centered bottom (y = height-26, h 18) — text "from Pneutrino by NeutronStar714"; text buttons = Courier New 20 bold via `getTextButtonFont` override; box captions = Helvetica 12 bold, textColour; knob labels = Helvetica 10 bold, muteText.

**Knobs:** `drawRotarySlider`: recessed well (panelBg fill circle) + muteText outline (borderW = lineW*0.5, lineW = radius*0.17, radius = diameter*0.5*0.75) + accent value arc (addCentredArc, butt caps, curved) + accent tick (inner r*0.25 → outer r-borderW-1, width lineW*0.9). NO centre cap, NO drag glow. Real TextBoxBelow (40-48 × 16, text=textColour, bg/outline=darkBg) + label beneath (14px). LNF set per-component (`slider.setLookAndFeel(&lnf)`), NOT global default; reset to nullptr in dtor.
- **GOTCHA — text box formats revert to raw floats on reopen:** the `SliderAttachment` is constructed in the member init list and pushes the value while `textFromValueFunction` is still null, so the box paints JUCE's default format ("0.92" instead of "92%"). CURE: after setting `textFromValueFunction`, call `slider.updateText()` in the ctor (2026-08-08, Extreme Shifter — DR already encoded this with `knob.updateText()`). Formatters that depend on another param (e.g. semitone shows "%+d st" in STEP, "%+.2f st" in SMOOTH via a captured `getRawParameterValue` atomic) must be re-rendered with `slider.updateText()` when that param changes (mode button's onStateChange).

**Text buttons:** toggle; off = buttonColourId accent + textColourOffId white/cream; on = buttonOnColourId panelBg + textColourOnId accentBright; label set via onStateChange. **Must override `drawButtonBackground`** (fill from toggle state + border = theme bright copper, 2.5 px, radius 6) — the default LookAndFeel_V4 derives the border as a *contrast* colour which falls outside the theme family (Extreme Shifter, 2026-08-08).

**Boxes:** rounded rectangles (radius 10, borderW 2.5, margin 7) outline-only, per-box accent shade; caption label inside at top 22px.

**Window:** fixed size (setResizable false).

**Why:** any new Pneutrino plugin should reuse this language so the series looks like one product; adapt the accent family per plugin, never import DR's teal.

**How to apply:** copy the LNF structure + editor scaffolding from Dynamic Resonator's PluginEditor; swap the palette block for the plugin's theme. See [[voice-architecture]] for ES specifics.
