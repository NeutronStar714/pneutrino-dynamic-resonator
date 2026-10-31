#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
// Custom rotary-slider look-and-feel (unchanged)
class DynamicResonatorLookAndFeel final : public juce::LookAndFeel_V4
{
public:
    DynamicResonatorLookAndFeel();

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                           juce::Slider& slider) override;

    void drawLinearSlider (juce::Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const juce::Slider::SliderStyle style, juce::Slider& slider) override;

    juce::Font getTextButtonFont (juce::TextButton&, int) override
    {
        return juce::Font (juce::FontOptions ("Courier New", 20.0f, juce::Font::bold));
    }

private:
    juce::Colour accentColour;
    juce::Colour bgColour;
    juce::Colour sliderFillColour;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicResonatorLookAndFeel)
};

//==============================================================================
// Loudness meter
class LoudnessMeter final : public juce::Component
{
public:
    LoudnessMeter();
    void setLevel (float level) noexcept { envelopeLevel = level; repaint(); }
    void paint (juce::Graphics& g) override;
private:
    float envelopeLevel = 0.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LoudnessMeter)
};

//==============================================================================
// Perceived loudness meter — same as LoudnessMeter but also highlights
// the Dynamic Range window [drMin, drMax] as a band on the bar.
class PerceivedLoudnessMeter final : public juce::Component
{
public:
    PerceivedLoudnessMeter();
    void setLevel (float level) noexcept { envelopeLevel = level; repaint(); }
    void setDRRange (float min, float max) noexcept { drMin = min; drMax = max; repaint(); }
    void paint (juce::Graphics& g) override;
private:
    float envelopeLevel = 0.0f;
    float drMin = 0.0f;
    float drMax = 1.0f;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PerceivedLoudnessMeter)
};

//==============================================================================
// Filter response graph — composite 4-band EQ curve + spectrum overlays
class FilterResponse final : public juce::Component
{
public:
    FilterResponse();

    struct BandCoeffs
    {
        juce::dsp::IIR::Coefficients<float>::Ptr coeffs;
        float postTrim = 1.0f;
        float freqHz   = 0.0f;
        float levelDb  = 0.0f;
    };

    void update (const std::array<BandCoeffs, 4>& bands, double sampleRate);
    void setBandPositions (const std::array<float, 4>& freqs, const std::array<float, 4>& levels);
    void setMixGains (float dry, float wet) noexcept { dryGain = dry; wetGain = wet; }
    void setSpectrumData (const float* inputSpec, const float* wetSpec,
                          int numBins, double sampleRate);

    void paint (juce::Graphics& g) override;

private:
    void rebuildPath();

    std::array<BandCoeffs, 4> currentBands;
    double responseSampleRate = 44100.0;
    juce::Path compositePath;

    // Spectrum data
    std::vector<float> inputSpectrum;
    std::vector<float> wetSpectrum;
    int   numSpectrumBins    = 0;
    double spectrumSampleRate = 44100.0;

    float dryGain = 0.5f;
    float wetGain = 0.5f;
    float masterPeak = 1e-12f;

    static constexpr float freqMin  = 20.0f;
    static constexpr float freqMax  = 20000.0f;
    static constexpr float dbMin    = -48.0f;
    static constexpr float dbMax    = +24.0f;
    static constexpr int   numCurvePts = 200;
    static constexpr float dbfsRef    = 512.0f;

    float logFreqToFrac (float f) const noexcept
    {
        return std::log10 (f / freqMin) / std::log10 (freqMax / freqMin);
    }

    juce::Colour accentColour;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterResponse)
};

//==============================================================================
// The plugin's editor — 4-band column layout
class DynamicResonatorAudioEditor final : public juce::AudioProcessorEditor,
                                          private juce::Timer,
                                          private juce::Slider::Listener,
                                          private juce::Label::Listener
{
public:
    explicit DynamicResonatorAudioEditor (DynamicResonatorAudioProcessor&);
    ~DynamicResonatorAudioEditor() override;

    void resized() override;
    void paint (juce::Graphics& g) override;

private:
    void timerCallback() override;
    void sliderValueChanged (Slider* slider) override;
    void labelTextChanged (Label* label) override;

    DynamicResonatorAudioProcessor& processorRef;
    DynamicResonatorLookAndFeel lnf;

    // 4-band controls
    static constexpr int numBands = 4;
    juce::Slider bandFreqKnob[numBands];
    juce::Slider bandQKnob[numBands];
    juce::Slider bandLevelKnob[numBands];
    juce::Slider bandSensKnob[numBands];
    juce::TextButton bandDirBtn[numBands];

    juce::Label bandHeaderLbl[numBands];
    juce::Label bandFreqLbl[numBands];
    juce::Label bandQLbl[numBands];
    juce::Label bandLevelLbl[numBands];
    juce::Label bandSensLbl[numBands];

    // Global controls
    juce::Slider dryKnob;
    juce::Slider wetKnob;
    juce::Slider attackKnob;
    juce::Slider releaseKnob;
    juce::Slider drSlider;
    juce::TextButton modeBtn;

    juce::Label dryLbl;
    juce::Label wetLbl;
    juce::Label attackLbl;
    juce::Label releaseLbl;
    juce::Label drLbl;
    juce::Label drMinVal;
    juce::Label drMaxVal;

    juce::Label nameLabel;
    juce::Label attributionLabel;

    // Displays
    LoudnessMeter  loudnessMeter;
    PerceivedLoudnessMeter perceivedMeter;
    FilterResponse filterResponse;

    // Attachments
    using SliderAttach = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttach = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::array<std::unique_ptr<SliderAttach>, numBands> freqAttachments;
    std::array<std::unique_ptr<SliderAttach>, numBands> qAttachments;
    std::array<std::unique_ptr<SliderAttach>, numBands> levelAttachments;
    std::array<std::unique_ptr<SliderAttach>, numBands> sensAttachments;
    std::array<std::unique_ptr<ButtonAttach>, numBands>  dirAttachments;

    std::unique_ptr<SliderAttach> dryAttach;
    std::unique_ptr<SliderAttach> wetAttach;
    std::unique_ptr<SliderAttach> attackAttach;
    std::unique_ptr<SliderAttach> releaseAttach;
    std::unique_ptr<ButtonAttach> modeAttach;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicResonatorAudioEditor)
};
