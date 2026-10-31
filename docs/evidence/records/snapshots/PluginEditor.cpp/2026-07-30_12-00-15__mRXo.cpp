#include "PluginEditor.h"

//==============================================================================
// Colour constants
static const juce::Colour accentColour  { 0xFF599DA6 };
static const juce::Colour darkBg        { 0xFF1E1E1E };
static const juce::Colour panelBg       { 0xFF2A2A2A };
static const juce::Colour textColour    { 0xFFE0E0E0 };
static const juce::Colour muteText      { 0xFF888888 };
static const juce::Colour gridColour    { 0xFF333333 };
static const juce::Colour pinkColour    { 0xFFD05699 };

//==============================================================================
// DynamicResonatorLookAndFeel
//==============================================================================
DynamicResonatorLookAndFeel::DynamicResonatorLookAndFeel()
    : LookAndFeel_V4(),
      accentColour (::accentColour),
      bgColour     (panelBg),
      sliderFillColour (0xFF477D85)
{
    setColour (juce::Slider::rotarySliderFillColourId,     accentColour);
    setColour (juce::Slider::rotarySliderOutlineColourId,  muteText);
    setColour (juce::Slider::textBoxTextColourId,          textColour);
    setColour (juce::Slider::textBoxBackgroundColourId,    darkBg);
    setColour (juce::Slider::textBoxOutlineColourId,       darkBg);
    setColour (juce::Label::textColourId,                  textColour);
}

void DynamicResonatorLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y,
                                                     int width, int height,
                                                     float sliderPos,
                                                     float rotaryStartAngle,
                                                     float rotaryEndAngle,
                                                     juce::Slider& /*s*/)
{
    const float diameter = float (juce::jmin (width, height)) - 8.0f;
    const float cx       = float (x) + float (width)  * 0.5f;
    const float cy       = float (y) + float (height) * 0.5f;
    const float radius   = diameter * 0.5f * 0.75f;
    const float lineW    = radius * 0.17f;
    const float borderW  = lineW * 0.5f;

    g.setColour (bgColour);
    g.fillEllipse (cx - radius, cy - radius, diameter * 0.75f, diameter * 0.75f);

    g.setColour (muteText);
    g.drawEllipse (cx - radius, cy - radius, diameter * 0.75f, diameter * 0.75f, borderW);

    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    juce::Path arc;
    arc.addCentredArc (cx, cy, radius - borderW, radius - borderW,
                       0.0f, rotaryStartAngle, angle, true);
    g.setColour (accentColour);
    g.strokePath (arc, juce::PathStrokeType (borderW, juce::PathStrokeType::curved,
                                              juce::PathStrokeType::butt));

    juce::Path tick;
    tick.startNewSubPath (cx + radius * 0.25f * std::sin (angle),
                          cy - radius * 0.25f * std::cos (angle));
    tick.lineTo (cx + (radius - borderW - 1.0f) * std::sin (angle),
                 cy - (radius - borderW - 1.0f) * std::cos (angle));
    const float tickW = lineW * 0.9f;
    g.setColour (accentColour);
    g.strokePath (tick, juce::PathStrokeType (tickW, juce::PathStrokeType::curved,
                                              juce::PathStrokeType::butt));
}

void DynamicResonatorLookAndFeel::drawLinearSlider (juce::Graphics& g, int x, int y,
                                                     int width, int height,
                                                     float sliderPos,
                                                     float /*minSliderPos*/,
                                                     float /*maxSliderPos*/,
                                                     const juce::Slider::SliderStyle /*style*/,
                                                     juce::Slider& /*slider*/)
{
    const float trackW = std::max (3.0f, float (width) * 0.15f);
    const float trackX = float (x) + (float (width) - trackW) * 0.5f;

    // Above thumb → #2A2A2A
    g.setColour (bgColour);
    g.fillRoundedRectangle (trackX, float (y), trackW, std::max (0.0f, sliderPos - float (y)), trackW * 0.5f);

    // Below thumb → #599DA6
    g.setColour (accentColour);
    g.fillRoundedRectangle (trackX, sliderPos, trackW, std::max (0.0f, float (y + height) - sliderPos), trackW * 0.5f);

    // Thumb: horizontal rectangle (50 % narrower)
    const float thumbH = 8.0f;
    const float thumbW = (float (width) - 4.0f) * 0.5f;
    const float thumbX = float (x) + (float (width) - thumbW) * 0.5f;
    const float thumbY = sliderPos - thumbH * 0.5f;
    g.setColour (accentColour.brighter (0.25f));
    g.fillRoundedRectangle (thumbX, thumbY, thumbW, thumbH, 2.0f);
}

//==============================================================================
// LoudnessMeter
//==============================================================================
LoudnessMeter::LoudnessMeter() {}

void LoudnessMeter::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds().reduced (4);
    const auto w = bounds.getWidth();
    const auto h = bounds.getHeight();

    g.setColour (darkBg);
    g.fillRoundedRectangle (bounds.toFloat(), 3.0f);

    const float level = juce::jlimit (0.0f, 1.0f, envelopeLevel);
    const int fillW = int (level * float (w - 4));

    if (fillW > 0)
    {
        g.setColour (accentColour);
        g.fillRoundedRectangle (bounds.getX() + 2.0f, bounds.getY() + 2.0f,
                                float (fillW), float (h - 4), 2.0f);
    }

    g.setColour (muteText);
    g.setFont (juce::Font (juce::FontOptions (10.0f)));
    g.drawFittedText ("Level", bounds.reduced (6, 0), juce::Justification::left, 1);

    g.setColour (textColour);
    g.drawFittedText (juce::String (level, 2), bounds.reduced (6, 0),
                      juce::Justification::right, 1);
}

//==============================================================================
// FilterResponse
//==============================================================================
FilterResponse::FilterResponse()
    : accentColour (::accentColour)
{
}

void FilterResponse::setSpectrumData (const float* inputSpec, const float* wetSpec,
                                       int numBins, double sampleRate)
{
    inputSpectrum.assign (inputSpec, inputSpec + numBins);
    wetSpectrum.assign   (wetSpec,   wetSpec   + numBins);
    numSpectrumBins    = numBins;
    spectrumSampleRate = sampleRate;
}

void FilterResponse::update (const std::array<BandCoeffs, 4>& bands, double sampleRate)
{
    currentBands      = bands;
    responseSampleRate = sampleRate;
    rebuildPath();
    repaint();
}

void FilterResponse::rebuildPath()
{
    compositePath.clear();

    const float logMin   = std::log10 (freqMin);
    const float logRange = std::log10 (freqMax / freqMin);

    const auto  area  = getLocalBounds().toFloat();
    const float pad   = 12.0f;
    const float plotW = area.getWidth()  - pad * 2.0f;
    const float plotH = area.getHeight() - pad * 2.0f;
    const float oX    = area.getX() + pad;
    const float oY    = area.getY() + pad + plotH;
    const float dbRange = dbMax - dbMin;

    bool first = true;

    for (int i = 0; i < numCurvePts; ++i)
    {
        const float frac = float (i) / float (numCurvePts - 1);
        const float f    = std::pow (10.0f, logMin + frac * logRange);

        // Cascade magnitude across all 4 bands
        float totalMag = 1.0f;
        for (int b = 0; b < 4; ++b)
        {
            if (currentBands[b].coeffs != nullptr)
            {
                totalMag *= currentBands[b].coeffs->getMagnitudeForFrequency (f, responseSampleRate);
                totalMag *= currentBands[b].postTrim;
            }
        }

        const float dB = 20.0f * std::log10 (std::max (totalMag, 1e-12f));
        const float c  = juce::jlimit (dbMin, dbMax, dB);

        const float x = oX + frac * plotW;
        const float y = oY - ((c - dbMin) / dbRange) * plotH;

        if (first) { compositePath.startNewSubPath (x, y); first = false; }
        else        compositePath.lineTo (x, y);
    }
}

void FilterResponse::paint (juce::Graphics& g)
{
    const auto area     = getLocalBounds().toFloat();
    const float pad     = 12.0f;
    const float plotW   = area.getWidth()  - pad * 2.0f;
    const float plotH   = area.getHeight() - pad * 2.0f;
    const float oX      = area.getX() + pad;
    const float oY      = area.getY() + pad + plotH;
    const float dbRange = dbMax - dbMin;

    // Background
    g.setColour (panelBg);
    auto bgArea = area.reduced (2.0f);
    bgArea.setBottom (bgArea.getBottom() + 2.0f);
    g.fillRoundedRectangle (bgArea, 4.0f);

    // Grid lines every 12 dB
    g.setColour (gridColour);
    g.setFont (juce::Font (juce::FontOptions (9.0f)));
    for (float db = dbMin; db <= dbMax; db += 12.0f)
    {
        const float y = oY - ((db - dbMin) / dbRange) * plotH;
        g.drawHorizontalLine (int (y), oX, oX + plotW);
        if (db != dbMax)
        {
            g.setColour (muteText);
            g.drawSingleLineText (juce::String (int (db)) + "dB",
                                  int (oX + 4.0f), int (y - 2.0f));
            g.setColour (gridColour);
        }
    }

    // Frequency labels
    g.setColour (muteText);
    g.setFont (juce::Font (juce::FontOptions (9.0f)));
    auto drawFreqLabel = [&](float freqHz)
    {
        const float frac = std::log10 (freqHz / freqMin) / std::log10 (freqMax / freqMin);
        const float x = oX + frac * plotW;
        g.drawSingleLineText (freqHz >= 1000.0f
                                ? juce::String (freqHz / 1000.0f, 1) + "k"
                                : juce::String (int (freqHz)),
                              int (x), int (oY + 12.0f));
    };
    drawFreqLabel (20.0f);
    drawFreqLabel (100.0f);
    drawFreqLabel (1000.0f);
    drawFreqLabel (10000.0f);

    // Composite EQ curve
    {
        juce::Path fillPath (compositePath);
        fillPath.lineTo (oX + plotW, oY);
        fillPath.lineTo (oX,         oY);
        fillPath.closeSubPath();

        g.setColour (accentColour.withAlpha (0.15f));
        g.fillPath (fillPath);

        g.setColour (accentColour);
        g.strokePath (compositePath, juce::PathStrokeType (1.8f));
    }

    // Border
    g.setColour (gridColour);
    g.drawRect (area.reduced (2.0f), 1.0f);

    // Spectrum overlay
    if (numSpectrumBins > 0 && ! inputSpectrum.empty() && ! wetSpectrum.empty())
    {
        float currentPeak = 1e-12f;
        for (auto v : inputSpectrum) if (v > currentPeak) currentPeak = v;
        for (auto v : wetSpectrum)   if (v > currentPeak) currentPeak = v;
        if (currentPeak > masterPeak) masterPeak = currentPeak;
        else                          masterPeak *= 0.997f;
        masterPeak = juce::jlimit (1e-12f, 1e10f, masterPeak);
        const float refVal = std::max (dbfsRef, masterPeak);

        const float binToFreq = float (spectrumSampleRate) / float (numSpectrumBins * 2);
        constexpr float eps  = 1e-12f;
        constexpr int interpSteps = 6;

        struct Pt { float x, y; };

        auto catmullRom = [](const Pt& p0, const Pt& p1, const Pt& p2, const Pt& p3, float t)
        {
            const float t2 = t * t, t3 = t2 * t;
            return Pt {
                0.5f * (2.0f * p1.x + (-p0.x + p2.x) * t
                      + (2.0f * p0.x - 5.0f * p1.x + 4.0f * p2.x - p3.x) * t2
                      + (-p0.x + 3.0f * p1.x - 3.0f * p2.x + p3.x) * t3),
                0.5f * (2.0f * p1.y + (-p0.y + p2.y) * t
                      + (2.0f * p0.y - 5.0f * p1.y + 4.0f * p2.y - p3.y) * t2
                      + (-p0.y + 3.0f * p1.y - 3.0f * p2.y + p3.y) * t3) };
        };

        auto buildSmoothPath = [&](const std::vector<float>& data, juce::Path& path)
        {
            path.clear();
            std::vector<Pt> pts;
            pts.reserve (256);

            for (int i = 0; i < numSpectrumBins; ++i)
            {
                const float freq = (float (i) + 0.5f) * binToFreq;
                if (freq > freqMax) break;
                const float mag = data[static_cast<size_t> (i)];
                const float dB  = 20.0f * std::log10 (mag / refVal + eps);
                const float y   = juce::jlimit (oY - plotH, oY,
                                                oY - ((dB - dbMin) / dbRange) * plotH);
                const float frac = logFreqToFrac (freq);
                const float x    = std::max (oX, oX + frac * plotW);
                if (x > oX + plotW) break;
                pts.push_back ({ x, y });
            }

            if (pts.empty()) return;
            if (pts[0].x > oX + 1.0f)
                pts.insert (pts.begin(), { oX, pts[0].y });

            path.startNewSubPath (pts[0].x, pts[0].y);
            for (size_t i = 0; i + 1 < pts.size(); ++i)
            {
                const Pt& p0 = (i == 0) ? pts[i]   : pts[i - 1];
                const Pt& p1 = pts[i];
                const Pt& p2 = pts[i + 1];
                const Pt& p3 = (i + 2 < pts.size()) ? pts[i + 2] : pts[i + 1];
                for (int s = 1; s <= interpSteps; ++s)
                {
                    auto pt = catmullRom (p0, p1, p2, p3, float (s) / float (interpSteps));
                    path.lineTo (pt.x, pt.y);
                }
            }
        };

        // Dry spectrum — white
        {
            juce::Path p;
            buildSmoothPath (inputSpectrum, p);
            const float alpha = 0.25f + dryGain * 0.5f;
            g.setColour (juce::Colours::white.withAlpha (alpha));
            g.strokePath (p, juce::PathStrokeType (1.5f));
        }

        // Wet spectrum — pink
        {
            juce::Path p;
            buildSmoothPath (wetSpectrum, p);
            const float alpha = 0.25f + wetGain * 0.5f;
            g.setColour (pinkColour.withAlpha (alpha));
            g.strokePath (p, juce::PathStrokeType (1.5f));
        }
    }
}

//==============================================================================
// DynamicResonatorAudioEditor — constructor
//==============================================================================
DynamicResonatorAudioEditor::DynamicResonatorAudioEditor (DynamicResonatorAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processorRef (p)
{
    // --- Header ---
    nameLabel.setText ("Dynamic Resonator", juce::dontSendNotification);
    nameLabel.setFont (juce::Font (juce::FontOptions ("Courier New", 30.0f, juce::Font::bold)));
    nameLabel.setColour (juce::Label::textColourId, juce::Colour (0xFF7AB0B8));
    nameLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (nameLabel);

    attributionLabel.setText ("from Pneutrino by NeutronStar714", juce::dontSendNotification);
    attributionLabel.setFont (juce::Font (juce::FontOptions ("Courier New", 20.0f, juce::Font::plain)));
    attributionLabel.setColour (juce::Label::textColourId, accentColour);
    attributionLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (attributionLabel);

    // --- Helper: set up one knob + label ---
    auto setupKnob = [&](juce::Slider& knob, juce::Label& label,
                          const juce::String& text, double defaultValue,
                          double rangeMin = 0.0, double rangeMax = 1.0,
                          double interval = 0.001)
    {
        knob.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        knob.setRange (rangeMin, rangeMax, interval);
        knob.setValue (defaultValue, juce::dontSendNotification);
        knob.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 40, 16);
        knob.setDoubleClickReturnValue (true, defaultValue);
        knob.setLookAndFeel (&lnf);
        addAndMakeVisible (knob);

        label.setText (text, juce::dontSendNotification);
        label.setFont (juce::Font (juce::FontOptions ("Helvetica", 10.0f, juce::Font::bold)));
        label.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (label);
    };

    // --- Band controls ---
    const float freqDefaults[]  = { 20.0f, 200.0f, 2000.0f, 100.0f };
    const float qDefault        = 0.707f;
    const juce::String bandNames[] = { "HP", "PEAK 1", "PEAK 2", "LP" };

    auto& apvts = processorRef.apvts;

    // Attachments
    const juce::String freqIds[] = {
        DynamicResonatorAudioProcessor::paramB1FreqID,
        DynamicResonatorAudioProcessor::paramB2FreqID,
        DynamicResonatorAudioProcessor::paramB3FreqID,
        DynamicResonatorAudioProcessor::paramB4FreqID
    };
    const juce::String qIds[] = {
        DynamicResonatorAudioProcessor::paramB1QID,
        DynamicResonatorAudioProcessor::paramB2QID,
        DynamicResonatorAudioProcessor::paramB3QID,
        DynamicResonatorAudioProcessor::paramB4QID
    };
    const juce::String lvlIds[] = {
        DynamicResonatorAudioProcessor::paramB1LevelID,
        DynamicResonatorAudioProcessor::paramB2LevelID,
        DynamicResonatorAudioProcessor::paramB3LevelID,
        DynamicResonatorAudioProcessor::paramB4LevelID
    };
    const juce::String sensIds[] = {
        DynamicResonatorAudioProcessor::paramB1SensID,
        DynamicResonatorAudioProcessor::paramB2SensID,
        DynamicResonatorAudioProcessor::paramB3SensID,
        DynamicResonatorAudioProcessor::paramB4SensID
    };
    const juce::String dirIds[] = {
        DynamicResonatorAudioProcessor::paramB1DirID,
        DynamicResonatorAudioProcessor::paramB2DirID,
        DynamicResonatorAudioProcessor::paramB3DirID,
        DynamicResonatorAudioProcessor::paramB4DirID
    };

    for (int b = 0; b < 4; ++b)
    {
        bandHeaderLbl[b].setText (bandNames[b], juce::dontSendNotification);
        bandHeaderLbl[b].setFont (juce::Font (juce::FontOptions ("Helvetica", 12.0f, juce::Font::bold)));
        bandHeaderLbl[b].setColour (juce::Label::textColourId, textColour);
        bandHeaderLbl[b].setJustificationType (juce::Justification::centred);
        addAndMakeVisible (bandHeaderLbl[b]);

        setupKnob (bandFreqKnob[b], bandFreqLbl[b], "Freq", freqDefaults[b], 20.0, 20000.0, 1.0);
        bandFreqLbl[b].setColour (juce::Label::textColourId, muteText);

        setupKnob (bandQKnob[b], bandQLbl[b], "Q", qDefault, 0.1, 10.0, 0.01);
        bandQLbl[b].setColour (juce::Label::textColourId, muteText);

        // Level (PEAK bands only — vertical slider; HP & LP are global Pre/Post)
        if (b == 0 || b == 3)
        {
            bandLevelKnob[b].setVisible (false);
            bandLevelLbl[b].setVisible (false);
        }
        else
        {
            bandLevelKnob[b].setSliderStyle (juce::Slider::LinearVertical);
            bandLevelKnob[b].setRange (-24.0, 24.0, 0.1);
            bandLevelKnob[b].setValue (0.0, juce::dontSendNotification);
            bandLevelKnob[b].setDoubleClickReturnValue (true, 0.0);
            bandLevelKnob[b].setTextBoxStyle (juce::Slider::TextBoxBelow, false, 36, 16);
            bandLevelKnob[b].setColour (juce::Slider::textBoxTextColourId, textColour);
            bandLevelKnob[b].setColour (juce::Slider::textBoxBackgroundColourId, darkBg);
            bandLevelKnob[b].setColour (juce::Slider::textBoxOutlineColourId, darkBg);
            bandLevelKnob[b].setLookAndFeel (&lnf);
            addAndMakeVisible (bandLevelKnob[b]);

            bandLevelLbl[b].setText ("Level", juce::dontSendNotification);
            bandLevelLbl[b].setFont (juce::Font (juce::FontOptions ("Helvetica", 10.0f, juce::Font::bold)));
            bandLevelLbl[b].setColour (juce::Label::textColourId, muteText);
            bandLevelLbl[b].setJustificationType (juce::Justification::centred);
            addAndMakeVisible (bandLevelLbl[b]);
        }

        // Sensitivity (per-band, B1 defaults to 0)
        const float sensDefaults[] = { 0.0f, 0.7f, 0.7f, 1.0f };
        setupKnob (bandSensKnob[b], bandSensLbl[b], "Sens", sensDefaults[b], 0.0, 1.0, 0.001);
        bandSensLbl[b].setColour (juce::Label::textColourId, muteText);

        // Direction toggle: Up / Down
        bandDirBtn[b].setClickingTogglesState (true);
        // Off (Up): teal fill, white text  /  On (Down): grey fill, teal text
        bandDirBtn[b].setColour (juce::TextButton::textColourOffId,   juce::Colours::white);
        bandDirBtn[b].setColour (juce::TextButton::textColourOnId,    accentColour);
        bandDirBtn[b].setColour (juce::TextButton::buttonColourId,    accentColour);
        bandDirBtn[b].setColour (juce::TextButton::buttonOnColourId, panelBg);
        bandDirBtn[b].setLookAndFeel (&lnf);
        addAndMakeVisible (bandDirBtn[b]);

        // Attachments
        freqAttachments[b]  = std::make_unique<SliderAttach> (apvts, freqIds[b],  bandFreqKnob[b]);
        bandFreqKnob[b].textFromValueFunction = [] (double val) { return juce::String (int (val + 0.5)); };
        qAttachments[b]     = std::make_unique<SliderAttach> (apvts, qIds[b],     bandQKnob[b]);
        levelAttachments[b] = std::make_unique<SliderAttach> (apvts, lvlIds[b],   bandLevelKnob[b]);
        sensAttachments[b]  = std::make_unique<SliderAttach> (apvts, sensIds[b],  bandSensKnob[b]);
        dirAttachments[b]   = std::make_unique<ButtonAttach> (apvts, dirIds[b],   bandDirBtn[b]);
        // Sync button text with toggle state
        bandDirBtn[b].setButtonText (bandDirBtn[b].getToggleState() ? "DOWN" : "UP");
        bandDirBtn[b].onClick = [btn = &bandDirBtn[b]]() {
            btn->setButtonText (btn->getToggleState() ? "DOWN" : "UP");
        };
    }

    // --- Global controls (Dry, Wet, Pre, Post) ---
    auto setupGlobal = [&](juce::Slider& knob, juce::Label& lbl,
                            const juce::String& text, double def, double lo, double hi)
    {
        knob.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        knob.setRange (lo, hi, (hi - lo < 50.0 ? 0.1 : 0.001));
        knob.setValue (def, juce::dontSendNotification);
        knob.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 40, 16);
        knob.setDoubleClickReturnValue (true, def);
        knob.setLookAndFeel (&lnf);
        addAndMakeVisible (knob);

        lbl.setText (text, juce::dontSendNotification);
        lbl.setFont (juce::Font (juce::FontOptions ("Helvetica", 10.0f, juce::Font::bold)));
        lbl.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (lbl);
    };

    setupGlobal (dryKnob,   dryLbl,   "Dry",  0.5, 0.0, 1.0);
    setupGlobal (wetKnob,   wetLbl,   "Wet",  0.5, 0.0, 1.0);
    setupGlobal (bandLevelKnob[0], bandLevelLbl[0], "Pre",  0.0, -12.0, 12.0);
    setupGlobal (bandLevelKnob[3], bandLevelLbl[3], "Post", 0.0, -24.0, 24.0);

    bandLevelKnob[0].setVisible (true);
    bandLevelLbl[0].setVisible (true);
    bandLevelKnob[3].setVisible (true);
    bandLevelLbl[3].setVisible (true);

    dryLbl.setColour (juce::Label::textColourId, juce::Colours::white);
    wetLbl.setColour (juce::Label::textColourId, juce::Colour (0xFFEF80C0));
    bandLevelLbl[0].setColour (juce::Label::textColourId, muteText);
    bandLevelLbl[3].setColour (juce::Label::textColourId, muteText);

    dryAttach = std::make_unique<SliderAttach> (apvts, DynamicResonatorAudioProcessor::paramDryID, dryKnob);
    wetAttach = std::make_unique<SliderAttach> (apvts, DynamicResonatorAudioProcessor::paramWetID, wetKnob);
    // levelAttachments[0] and [3] are already set in the loop (attach to bandLevelKnob[0]/[3])

    // --- Displays ---
    addAndMakeVisible (loudnessMeter);
    addAndMakeVisible (filterResponse);

    startTimerHz (30);
    setSize (520, 740);
    setResizable (false, false);
}

DynamicResonatorAudioEditor::~DynamicResonatorAudioEditor()
{
    stopTimer();
    for (int b = 0; b < 4; ++b)
    {
        bandFreqKnob[b].setLookAndFeel  (nullptr);
        bandQKnob[b].setLookAndFeel     (nullptr);
        bandLevelKnob[b].setLookAndFeel (nullptr);
        bandSensKnob[b].setLookAndFeel  (nullptr);
        bandDirBtn[b].setLookAndFeel    (nullptr);
    }
    dryKnob.setLookAndFeel (nullptr);
    wetKnob.setLookAndFeel (nullptr);
}

//==============================================================================
void DynamicResonatorAudioEditor::paint (juce::Graphics& g)
{
    g.fillAll (darkBg);

    // Header separator
    g.setColour (accentColour.withAlpha (0.25f));
    g.fillRect (30.0f, 44.0f, getWidth() - 60.0f, 1.0f);

    // Band boxes — rounded rectangles around each column's controls
    const int colKnobH  = 65;
    const int colHeader = 22;
    const int colRow    = colKnobH + 14 + 4;    // 83
    const int colH      = colHeader + colRow * 3 + 24;  // 295
    const int colW      = (getWidth() - 24) / 4;
    const int boxY      = 52;  // 12 reduced + 40 header
    const float margin  = 2.5f;
    const float radius  = 10.0f;

    g.setColour (accentColour.withAlpha (0.15f));
    for (int b = 0; b < 4; ++b)
    {
        auto box = juce::Rectangle<float> (12.0f + b * colW, boxY, colW, colH);
        g.drawRoundedRectangle (box.reduced (margin), radius, 1.0f);
    }

    // Separator between band columns and global controls
    g.setColour (accentColour.withAlpha (0.15f));
    g.fillRect (30.0f, 347.0f, getWidth() - 60.0f, 1.0f);
}

void DynamicResonatorAudioEditor::resized()
{
    auto area = getLocalBounds().reduced (12);

    // === Header ===
    nameLabel.setBounds (area.removeFromTop (40).withY (12).reduced (0, 4));

    // === Attribution (always at bottom) ===
    attributionLabel.setBounds (area.removeFromBottom (22).reduced (4, 0));

    // === 4-band columns (3 knobs + dir switch, PEAK bands have vertical level slider) ===
    const int colKnobH  = 65;
    const int colHeader = 22;
    const int colRow    = colKnobH + 14 + 4;           // 83
    const int colH      = colHeader + colRow * 3 + 24; // 22 + 249 + 24 = 295
    const int colW      = area.getWidth() / 4;
    const int sliderW   = 44;   // width reserved for the vertical level slider in PEAK columns

    auto columnArea = area.removeFromTop (colH);

    for (int b = 0; b < 4; ++b)
    {
        const bool isPeak = (b == 1 || b == 2);
        auto col = columnArea.withX (columnArea.getX() + b * colW).withWidth (colW);

        bandHeaderLbl[b].setBounds (col.removeFromTop (colHeader).reduced (4, 0));

        int yPos = col.getY();

        auto placeControl = [&](juce::Slider& knob, juce::Label& lbl)
        {
            auto r = col.withTop (yPos).withHeight (colKnobH);
            if (isPeak) r = r.withWidth (colW - sliderW);
            knob.setBounds (r.reduced (4, 2));
            lbl.setBounds (col.withTop (yPos + colKnobH).withHeight (14)
                              .withWidth (isPeak ? (colW - sliderW) : colW).reduced (6, 0));
            yPos += colRow;
        };

        placeControl (bandFreqKnob[b], bandFreqLbl[b]);
        placeControl (bandQKnob[b],    bandQLbl[b]);
        placeControl (bandSensKnob[b], bandSensLbl[b]);

        // PEAK bands: vertical level slider on the right side
        if (isPeak)
        {
            const int sliderH = 2 * colRow + colKnobH;  // text box bottom aligns with Sens text box
            const int belowTxt = col.getY() + sliderH - 20;  // label below component = same Y as Sens label
            auto sr = col.withLeft (col.getX() + colW - sliderW - 15)
                        .withTop (col.getY())
                        .withWidth (sliderW).withHeight (sliderH);
            bandLevelKnob[b].setBounds (sr.reduced (2, 0));
            bandLevelLbl[b].setBounds (sr.withTop (belowTxt).withHeight (14).reduced (4, 0));
        }

        bandDirBtn[b].setBounds (col.withTop (yPos).withHeight (20).reduced (6, 0));
    }

    // === Global controls (Dry, Wet, Pre, Post) ===
    const int globalH = 65;
    auto globalArea = area.removeFromTop (globalH).reduced (4, 4);
    const int kw4 = globalArea.getWidth() / 4;

    auto placeGlobal = [&](juce::Slider& knob, juce::Label& lbl, int idx)
    {
        auto r = globalArea.withX (globalArea.getX() + idx * kw4).withWidth (kw4);
        knob.setBounds (r.withHeight (48).reduced (6, 2));
        lbl.setBounds (r.withTop (r.getY() + 50).withHeight (14).reduced (6, 0));
    };
    placeGlobal (dryKnob,           dryLbl,           0);
    placeGlobal (wetKnob,           wetLbl,           1);
    placeGlobal (bandLevelKnob[0], bandLevelLbl[0],   2);  // Pre
    placeGlobal (bandLevelKnob[3], bandLevelLbl[3],   3);  // Post

    // === Loudness meter ===
    loudnessMeter.setBounds (area.removeFromTop (26).reduced (4, 2));

    // === Filter response (all remaining space) ===
    filterResponse.setBounds (area.reduced (4, 2));
}

//==============================================================================
void DynamicResonatorAudioEditor::timerCallback()
{
    const float envelope = processorRef.getCurrentEnvelope();
    loudnessMeter.setLevel (envelope);

    const float dry = *processorRef.apvts.getRawParameterValue (DynamicResonatorAudioProcessor::paramDryID);
    const float wet = *processorRef.apvts.getRawParameterValue (DynamicResonatorAudioProcessor::paramWetID);
    const double sr = processorRef.getSampleRate();

    using Coefficients = juce::dsp::IIR::Coefficients<float>;
    std::array<FilterResponse::BandCoeffs, 4> bandCoeffs;

    const juce::String freqIds[] = {
        DynamicResonatorAudioProcessor::paramB1FreqID,
        DynamicResonatorAudioProcessor::paramB2FreqID,
        DynamicResonatorAudioProcessor::paramB3FreqID,
        DynamicResonatorAudioProcessor::paramB4FreqID
    };
    const juce::String qIds[] = {
        DynamicResonatorAudioProcessor::paramB1QID,
        DynamicResonatorAudioProcessor::paramB2QID,
        DynamicResonatorAudioProcessor::paramB3QID,
        DynamicResonatorAudioProcessor::paramB4QID
    };
    const juce::String levelIds[] = {
        DynamicResonatorAudioProcessor::paramB1LevelID,
        DynamicResonatorAudioProcessor::paramB2LevelID,
        DynamicResonatorAudioProcessor::paramB3LevelID,
        DynamicResonatorAudioProcessor::paramB4LevelID
    };
    const juce::String sensIds[] = {
        DynamicResonatorAudioProcessor::paramB1SensID,
        DynamicResonatorAudioProcessor::paramB2SensID,
        DynamicResonatorAudioProcessor::paramB3SensID,
        DynamicResonatorAudioProcessor::paramB4SensID
    };
    const juce::String dirIds[] = {
        DynamicResonatorAudioProcessor::paramB1DirID,
        DynamicResonatorAudioProcessor::paramB2DirID,
        DynamicResonatorAudioProcessor::paramB3DirID,
        DynamicResonatorAudioProcessor::paramB4DirID
    };

    for (int b = 0; b < 4; ++b)
    {
        float freq    = *processorRef.apvts.getRawParameterValue (freqIds[b]);
        float q       = *processorRef.apvts.getRawParameterValue (qIds[b]);
        float level   = *processorRef.apvts.getRawParameterValue (levelIds[b]);
        float sensVal = *processorRef.apvts.getRawParameterValue (sensIds[b]);
        int   dir     = int (*processorRef.apvts.getRawParameterValue (dirIds[b]));
        bool  isUp    = (dir == 0);

        // Apply envelope modulation
        float modFreq = freq;
        if (sensVal > 0.001f)
        {
            if (isUp)
            {
                const float maxOct = std::log2 (50000.0f / freq);
                modFreq = freq * std::pow (2.0f, envelope * sensVal * maxOct);
                if (modFreq > 20000.0f) modFreq = 20000.0f;
            }
            else
            {
                const float maxOct = std::log2 (freq / 10.0f);
                modFreq = freq / std::pow (2.0f, envelope * sensVal * maxOct);
                if (modFreq < 20.0f) modFreq = 20.0f;
            }
        }

        if (b == 0)
        {
            bandCoeffs[b].coeffs   = Coefficients::makeHighPass (sr, modFreq, q);
            bandCoeffs[b].postTrim = 1.0f;  // HP level is pre-amp, not a post-trim
        }
        else if (b == 1 || b == 2)
        {
            bandCoeffs[b].coeffs   = Coefficients::makePeakFilter (sr, modFreq, q,
                                        juce::Decibels::decibelsToGain (level));
            bandCoeffs[b].postTrim = 1.0f;
        }
        else
        {
            bandCoeffs[b].coeffs   = Coefficients::makeLowPass (sr, modFreq, q);
            bandCoeffs[b].postTrim = juce::Decibels::decibelsToGain (level);
        }
    }

    filterResponse.update (bandCoeffs, sr);
    filterResponse.setMixGains (dry, wet);
    filterResponse.setSpectrumData (
        processorRef.getInputSpectrum(),
        processorRef.getWetSpectrum(),
        processorRef.getSpectrumSize(),
        sr);
}
