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
// PerceivedLoudnessMeter
//==============================================================================
PerceivedLoudnessMeter::PerceivedLoudnessMeter() {}

void PerceivedLoudnessMeter::paint (juce::Graphics& g)
{
    const auto bounds = getLocalBounds().reduced (4);
    const auto w = bounds.getWidth();
    const auto h = bounds.getHeight();

    g.setColour (darkBg);
    g.fillRoundedRectangle (bounds.toFloat(), 3.0f);

    // Dynamic Range window band
    const float drX1 = bounds.getX() + 2.0f + drMin * float (w - 4);
    const float drX2 = bounds.getX() + 2.0f + drMax * float (w - 4);
    g.setColour (accentColour.withAlpha (0.35f));
    g.fillRoundedRectangle (drX1, bounds.getY() + 2.0f,
                            std::max (drX2 - drX1, 2.0f), float (h - 4), 2.0f);

    // Level fill
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
    g.drawFittedText ("Perceived", bounds.reduced (6, 0), juce::Justification::left, 1);

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
    const float pad   = 16.0f;
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
    const float pad     = 16.0f;
    const float plotW   = area.getWidth()  - pad * 2.0f;
    const float plotH   = area.getHeight() - pad * 2.0f;
    const float oX      = area.getX() + pad;
    const float oY      = area.getY() + pad + plotH;
    const float dbRange = dbMax - dbMin;

    // Background
    g.setColour (panelBg);
    auto bgArea = area.reduced (2.0f);
    g.fillRoundedRectangle (bgArea, 4.0f);

    // Grid lines every 12 dB
    g.setColour (gridColour);
    g.setFont (juce::Font (juce::FontOptions (9.0f)));
    for (float db = dbMin; db <= dbMax; db += 12.0f)
    {
        const float y = oY - ((db - dbMin) / dbRange) * plotH;
        g.drawHorizontalLine (int (y), oX, oX + plotW);
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
    auto drawFreqLabel = [&](float freqHz, float yOff)
    {
        const float frac = std::log10 (freqHz / freqMin) / std::log10 (freqMax / freqMin);
        const float x = oX + frac * plotW;
        g.drawSingleLineText (freqHz >= 1000.0f
                                ? juce::String (freqHz / 1000.0f, 1) + "k"
                                : juce::String (int (freqHz)),
                              int (x), int (oY + yOff));
    };
    drawFreqLabel (20.0f,    11.0f);
    drawFreqLabel (50.0f,    11.0f);
    drawFreqLabel (100.0f,   11.0f);
    drawFreqLabel (200.0f,   11.0f);
    drawFreqLabel (500.0f,   11.0f);
    drawFreqLabel (1000.0f,  11.0f);
    drawFreqLabel (2000.0f,  11.0f);
    drawFreqLabel (5000.0f,  11.0f);
    drawFreqLabel (10000.0f, 11.0f);

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

    // Band marker circles at (freq, level)
    static const juce::Colour markerColours[] = {
        juce::Colour (0xFF365E63),
        juce::Colour (0xFF599DA6),
        juce::Colour (0xFF9CC4C9),
        juce::Colour (0xFFDEEBED)
    };

    for (int b = 0; b < 4; ++b)
    {
        if (currentBands[b].freqHz < 1.0f) continue;

        const float frac = logFreqToFrac (currentBands[b].freqHz);
        const float x    = oX + frac * plotW;
        const float dB   = (b == 0) ? 0.0f : currentBands[b].levelDb;
        const float y    = oY - ((dB - dbMin) / dbRange) * plotH;

        g.setColour (markerColours[b]);
        g.fillEllipse (x - 5.0f, y - 5.0f, 10.0f, 10.0f);
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

    // --- Row 1: six global knobs (Dry, Wet, Pre, Post, Attack, Release) ---
    auto setupKnob6 = [&](juce::Slider& knob, juce::Label& lbl,
                           const juce::String& text, double def, double lo, double hi, double step)
    {
        knob.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        knob.setRange (lo, hi, step);
        knob.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 40, 16);
        knob.setDoubleClickReturnValue (true, def);
        knob.setLookAndFeel (&lnf);
        addAndMakeVisible (knob);
        lbl.setText (text, juce::dontSendNotification);
        lbl.setFont (juce::Font (juce::FontOptions ("Helvetica", 10.0f, juce::Font::bold)));
        lbl.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (lbl);
    };

    setupKnob6 (dryKnob,   dryLbl,   "Dry",    0.5, 0.0, 1.0,   0.001);
    setupKnob6 (wetKnob,   wetLbl,   "Wet",    0.5, 0.0, 1.0,   0.001);
    setupKnob6 (bandLevelKnob[0], bandLevelLbl[0], "Pre",   0.0, -12.0, 12.0, 0.1);
    setupKnob6 (bandLevelKnob[3], bandLevelLbl[3], "Post",  0.0, -24.0, 24.0, 0.1);
    setupKnob6 (attackKnob,  attackLbl,  "Attack",  23,   1, 100,    1.0);
    setupKnob6 (releaseKnob, releaseLbl, "Release", 110, 20, 300,   1.0);

    bandLevelKnob[0].setVisible (true);
    bandLevelLbl[0].setVisible (true);
    bandLevelKnob[3].setVisible (true);
    bandLevelLbl[3].setVisible (true);

    dryLbl.setColour (juce::Label::textColourId, juce::Colours::white);
    wetLbl.setColour (juce::Label::textColourId, juce::Colour (0xFFEF80C0));
    bandLevelLbl[0].setColour (juce::Label::textColourId, muteText);
    bandLevelLbl[3].setColour (juce::Label::textColourId, muteText);

    dryAttach    = std::make_unique<SliderAttach> (apvts, DynamicResonatorAudioProcessor::paramDryID,     dryKnob);
    wetAttach    = std::make_unique<SliderAttach> (apvts, DynamicResonatorAudioProcessor::paramWetID,     wetKnob);
    attackLbl.setColour (juce::Label::textColourId, muteText);
    releaseLbl.setColour (juce::Label::textColourId, muteText);
    attackAttach = std::make_unique<SliderAttach> (apvts, DynamicResonatorAudioProcessor::paramAttackID,  attackKnob);
    releaseAttach= std::make_unique<SliderAttach> (apvts, DynamicResonatorAudioProcessor::paramReleaseID, releaseKnob);
    // levelAttachments[0] and [3] are already set in the loop

    // --- Row 2: Dynamic Range slider + RMS/AM toggle ---
    {
        drSlider.setSliderStyle (juce::Slider::TwoValueHorizontal);
        drSlider.setRange (0.0, 1.0, 0.001);
        drSlider.setMinValue (0.0, juce::dontSendNotification);
        drSlider.setMaxValue (1.0, juce::dontSendNotification);
        drSlider.setColour (juce::Slider::trackColourId,       accentColour);
        drSlider.setColour (juce::Slider::backgroundColourId,  panelBg);
        drSlider.setColour (juce::Slider::thumbColourId,       accentColour);
        drSlider.setTextBoxStyle (juce::Slider::NoTextBox, 0, 0, 0);
        drSlider.addListener (this);
        addAndMakeVisible (drSlider);

        auto setupNumBox = [&](juce::Label& lbl, float defVal) {
            lbl.setText (juce::String (defVal, 3), juce::dontSendNotification);
            lbl.setFont (juce::Font (juce::FontOptions (12.0f)));
            lbl.setEditable (true, false, true);
            lbl.setColour (juce::Label::textColourId,       textColour);
            lbl.setColour (juce::Label::backgroundColourId, darkBg);
            lbl.setColour (juce::Label::outlineColourId,    darkBg);
            lbl.setColour (juce::Label::textWhenEditingColourId, textColour);
            lbl.setColour (juce::Label::backgroundWhenEditingColourId, darkBg);
            lbl.setJustificationType (juce::Justification::centred);
            lbl.addListener (this);
            addAndMakeVisible (lbl);
        };
        setupNumBox (drMinVal, 0.0f);
        setupNumBox (drMaxVal, 1.0f);

        drLbl.setText ("DR", juce::dontSendNotification);
        drLbl.setFont (juce::Font (juce::FontOptions ("Helvetica", 10.0f, juce::Font::bold)));
        drLbl.setColour (juce::Label::textColourId, muteText);
        drLbl.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (drLbl);

        // RMS / AM toggle
        modeBtn.setButtonText ("RMS");
        modeBtn.setClickingTogglesState (true);
        modeBtn.setColour (juce::TextButton::textColourOffId,   juce::Colours::white);
        modeBtn.setColour (juce::TextButton::textColourOnId,    accentColour);
        modeBtn.setColour (juce::TextButton::buttonColourId,    accentColour);
        modeBtn.setColour (juce::TextButton::buttonOnColourId, panelBg);
        modeBtn.setLookAndFeel (&lnf);
        addAndMakeVisible (modeBtn);

        modeAttach = std::make_unique<ButtonAttach> (apvts, DynamicResonatorAudioProcessor::paramModeID, modeBtn);
        modeBtn.setButtonText (modeBtn.getToggleState() ? "AM" : "RMS");
        modeBtn.onClick = [btn = &modeBtn]() {
            btn->setButtonText (btn->getToggleState() ? "AM" : "RMS");
        };
    }

    // --- Displays ---
    addAndMakeVisible (loudnessMeter);
    addAndMakeVisible (perceivedMeter);
    addAndMakeVisible (filterResponse);

    startTimerHz (30);
    setSize (520, 823);
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
    attackKnob.setLookAndFeel (nullptr);
    releaseKnob.setLookAndFeel (nullptr);
    modeBtn.setLookAndFeel (nullptr);
}

//==============================================================================
void DynamicResonatorAudioEditor::paint (juce::Graphics& g)
{
    g.fillAll (darkBg);

    // Band boxes — rounded rectangles around each column's controls
    static const juce::Colour boxColours[] = {
        juce::Colour (0xFF365E63),  // HP
        juce::Colour (0xFF599DA6),  // PEAK 1
        juce::Colour (0xFF9CC4C9),  // PEAK 2
        juce::Colour (0xFFDEEBED)   // LP
    };

    const int colKnobH  = 65;
    const int colHeader = 22;
    const int colRow    = colKnobH + 14 + 4;    // 83
    const int colH      = colHeader + colRow * 3 + 36;  // 305
    const int colW      = (getWidth() - 24) / 4;
    const int boxY      = 42;  // 2px above header bottom
    const float margin  = 7.0f;
    const float radius  = 10.0f;
    const float borderW = 2.5f;

    for (int b = 0; b < 4; ++b)
    {
        auto box = juce::Rectangle<float> (12.0f + b * colW, boxY, colW, colH);
        g.setColour (boxColours[b]);
        g.drawRoundedRectangle (box.reduced (margin), radius, borderW);
    }

}

void DynamicResonatorAudioEditor::resized()
{
    auto area = getLocalBounds().reduced (12);

    // === Header ===
    nameLabel.setBounds (area.removeFromTop (34).withY (6).reduced (0, 4));

    // === Attribution (always at bottom) ===
    attributionLabel.setBounds (area.removeFromBottom (18).withY (800) .reduced (4, 0));

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
            const int sliderH = 2 * colRow + colKnobH - 2;  // text box bottom aligns with Sens text box
            const int belowTxt = col.getY() + sliderH + 2;  // label below component = same Y as Sens label
            auto sr = col.withLeft (col.getX() + colW - sliderW - 15)
                        .withTop (col.getY())
                        .withWidth (sliderW).withHeight (sliderH);
            bandLevelKnob[b].setBounds (sr.reduced (2, 0));
            bandLevelLbl[b].setBounds (sr.withTop (belowTxt).withHeight (14).reduced (4, 0));
        }

        bandDirBtn[b].setBounds (col.withTop (yPos).withHeight (20).reduced (11, 0));
    }

    // === Row 1: six global knobs (Dry, Wet, Pre, Post, Attack, Release) ===
    const int row1H = colRow + 4;  // 87
    auto row1Area = area.removeFromTop (row1H).reduced (4, 4);
    const int kw6 = row1Area.getWidth() / 6;

    auto placeKnob6 = [&](juce::Slider& knob, juce::Label& lbl, int col)
    {
        auto r = row1Area.withX (row1Area.getX() + col * kw6).withWidth (kw6);
        knob.setBounds (r.withHeight (colKnobH).reduced (4, 2));
        lbl.setBounds (r.withTop (r.getY() + colKnobH).withHeight (14).reduced (4, 0));
    };
    placeKnob6 (dryKnob,           dryLbl,           0);
    placeKnob6 (wetKnob,           wetLbl,           1);
    placeKnob6 (bandLevelKnob[0], bandLevelLbl[0],   2);
    placeKnob6 (bandLevelKnob[3], bandLevelLbl[3],   3);
    placeKnob6 (attackKnob,        attackLbl,        4);
    placeKnob6 (releaseKnob,       releaseLbl,       5);

    // === Row 2: Dynamic Range slider + RMS/AM toggle (compact) ===
    const int row2H = 43;
    auto row2Area = area.removeFromTop (row2H).reduced (4, 4);

    const int nbW    = 40;      // number box width (matches Dry knob's text box)
    const int btnW   = 70;      // mode button width
    const int drSlW  = row2Area.getWidth() - nbW * 2 - btnW - 4;
    const int midX  = row2Area.getX() + nbW;

    // Left number box (min) — matches Dry knob's text box dimensions
    drMinVal.setBounds (row2Area.withWidth (nbW).withHeight (16).translated (0, (row2Area.getHeight() - 16) / 2));

    // Slider
    drSlider.setBounds (midX, row2Area.getY() + 4, drSlW, row2Area.getHeight() - 8);
    drLbl.setBounds (midX, row2Area.getY() + row2Area.getHeight() - 12, drSlW, 12);

    // Right number box (max)
    drMaxVal.setBounds (row2Area.withLeft (midX + drSlW + 2).withWidth (nbW).withHeight (16)
                                 .translated (0, (row2Area.getHeight() - 16) / 2));

    // Mode toggle
    modeBtn.setBounds (row2Area.withLeft (row2Area.getRight() - btnW).withWidth (btnW)
                                .reduced (6, 6));

    // === Monitors: Perceived (with DR window) + Level (compact) ===
    perceivedMeter.setBounds (area.removeFromTop (22).reduced (4, 1));
    loudnessMeter.setBounds (area.removeFromTop (22).reduced (4, 1));

    // === Filter response (all remaining space) ===
    filterResponse.setBounds (area.reduced (4, 2));
}

void DynamicResonatorAudioEditor::sliderValueChanged (Slider* slider)
{
    if (slider == &drSlider)
    {
        auto* paramMin = processorRef.apvts.getParameter (DynamicResonatorAudioProcessor::paramDRMinID);
        auto* paramMax = processorRef.apvts.getParameter (DynamicResonatorAudioProcessor::paramDRMaxID);
        if (paramMin != nullptr) paramMin->setValueNotifyingHost ((float) drSlider.getMinValue());
        if (paramMax != nullptr) paramMax->setValueNotifyingHost ((float) drSlider.getMaxValue());
    }
}

void DynamicResonatorAudioEditor::labelTextChanged (Label* label)
{
    auto setDR = [&](float val, bool isMin) {
        val = juce::jlimit (0.0f, 1.0f, val);
        if (isMin) { val = std::min (val, *processorRef.apvts.getRawParameterValue (DynamicResonatorAudioProcessor::paramDRMaxID) - 0.001f); }
        else       { val = std::max (val, *processorRef.apvts.getRawParameterValue (DynamicResonatorAudioProcessor::paramDRMinID) + 0.001f); }
        auto* param = processorRef.apvts.getParameter (isMin ? DynamicResonatorAudioProcessor::paramDRMinID
                                                             : DynamicResonatorAudioProcessor::paramDRMaxID);
        if (param != nullptr) param->setValueNotifyingHost (param->convertTo0to1 (val));
    };

    if      (label == &drMinVal) setDR (label->getText().getFloatValue(), true);
    else if (label == &drMaxVal) setDR (label->getText().getFloatValue(), false);
}

//==============================================================================
void DynamicResonatorAudioEditor::timerCallback()
{
    const float envelope = processorRef.getCurrentEnvelope();       // post-DR
    const float perceived = processorRef.getPerceivedLoudness();    // pre-DR raw
    loudnessMeter.setLevel (envelope);
    perceivedMeter.setLevel (perceived);

    // Sync DR slider from parameter values (in case preset was loaded)
    const float drMin = *processorRef.apvts.getRawParameterValue (DynamicResonatorAudioProcessor::paramDRMinID);
    const float drMax = *processorRef.apvts.getRawParameterValue (DynamicResonatorAudioProcessor::paramDRMaxID);
    drSlider.setMinValue (drMin, juce::dontSendNotification);
    drSlider.setMaxValue (drMax, juce::dontSendNotification);
    perceivedMeter.setDRRange (drMin, drMax);
    // Only update DR number boxes when not being edited
    if (! drMinVal.isBeingEdited()) drMinVal.setText (juce::String (drMin, 3), juce::dontSendNotification);
    if (! drMaxVal.isBeingEdited()) drMaxVal.setText (juce::String (drMax, 3), juce::dontSendNotification);

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

        bandCoeffs[b].freqHz  = modFreq;
        bandCoeffs[b].levelDb = level;

        if (b == 0)
        {
            bandCoeffs[b].coeffs   = Coefficients::makeHighPass (sr, modFreq, q);
            bandCoeffs[b].postTrim = 1.0f;
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
