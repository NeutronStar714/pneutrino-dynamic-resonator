#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
    Dynamic Resonator — a 4-band parametric EQ whose bands can be
    dynamically modulated by the input signal's amplitude envelope.

    Bands:  HP → Peaking → Peaking → LP
    Each band has independent freq, Q, level and an optional envelope link.
*/
class DynamicResonatorAudioProcessor final : public juce::AudioProcessor
{
public:
    //==============================================================================
    DynamicResonatorAudioProcessor();
    ~DynamicResonatorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override { return "Dynamic Resonator"; }

    bool acceptsMidi() const override   { return false; }
    bool producesMidi() const override  { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    //==============================================================================
    int getNumPrograms() override        { return 1; }
    int getCurrentProgram() override     { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // --- Parameter tree ----------------------------------------------------------
    juce::AudioProcessorValueTreeState apvts;

    static const juce::String paramDryID;
    static const juce::String paramWetID;
    static const juce::String paramAttackID;
    static const juce::String paramReleaseID;
    static const juce::String paramModeID;
    static const juce::String paramDRMinID;
    static const juce::String paramDRMaxID;

    static const juce::String paramB1FreqID;
    static const juce::String paramB1QID;
    static const juce::String paramB1LevelID;
    static const juce::String paramB1SensID;
    static const juce::String paramB1DirID;

    static const juce::String paramB2FreqID;
    static const juce::String paramB2QID;
    static const juce::String paramB2LevelID;
    static const juce::String paramB2SensID;
    static const juce::String paramB2DirID;

    static const juce::String paramB3FreqID;
    static const juce::String paramB3QID;
    static const juce::String paramB3LevelID;
    static const juce::String paramB3SensID;
    static const juce::String paramB3DirID;

    static const juce::String paramB4FreqID;
    static const juce::String paramB4QID;
    static const juce::String paramB4LevelID;
    static const juce::String paramB4SensID;
    static const juce::String paramB4DirID;

    // --- Visualisation data (read by editor) ------------------------------------
    float getCurrentEnvelope() const noexcept { return envelope; }        // post-DR (drives the filter)
    float getPerceivedLoudness() const noexcept { return rawEnvelope; }   // pre-DR (raw RMS/AM)

    static constexpr int fftOrder  = 11;              // 2^11 = 2048
    static constexpr int fftSize   = 1 << fftOrder;
    static constexpr int numSpectrumBins = fftSize / 2;

    const float* getInputSpectrum() const noexcept { return inputSpectrum.data(); }
    const float* getWetSpectrum()   const noexcept { return wetSpectrum.data(); }
    int getSpectrumSize() const noexcept            { return numSpectrumBins; }

private:
    //==============================================================================
    // Band types
    enum class FilterType { HP, Peak, LP };

    //==============================================================================
    // A single stereo biquad filter band with lazy coefficient update
    struct FilterBand
    {
        juce::dsp::IIR::Filter<float> filterL;
        juce::dsp::IIR::Filter<float> filterR;
        FilterType type = FilterType::Peak;

        float cachedFreq   = -1.0f;
        float cachedQ      = -1.0f;
        float cachedGainDb = -1000.0f;
        double cachedSr    = -1.0;

        float postTrim = 1.0f;

        void prepare (const juce::dsp::ProcessSpec& spec)
        {
            filterL.prepare (spec);
            filterR.prepare (spec);
        }

        void reset()
        {
            filterL.reset();
            filterR.reset();
        }

        void updateCoefficients (float freq, float q, float levelDb, double sampleRate)
        {
            constexpr float freqTol  = 1.0f;
            constexpr float qTol     = 0.01f;
            constexpr float gainTol  = 0.1f;

            if (std::abs (freq - cachedFreq) < freqTol
                && std::abs (q - cachedQ) < qTol
                && std::abs (levelDb - cachedGainDb) < gainTol
                && sampleRate == cachedSr)
                return;

            using Coefficients = juce::dsp::IIR::Coefficients<float>;
            Coefficients::Ptr newCoeffs;

            switch (type)
            {
                case FilterType::HP:
                    newCoeffs = Coefficients::makeHighPass (sampleRate, freq, q);
                    postTrim = 1.0f;  // HP level is used as pre-amp, not post-trim
                    break;

                case FilterType::Peak:
                    newCoeffs = Coefficients::makePeakFilter (sampleRate, freq, q,
                                    juce::Decibels::decibelsToGain (levelDb));
                    postTrim = 1.0f;
                    break;

                case FilterType::LP:
                    newCoeffs = Coefficients::makeLowPass (sampleRate, freq, q);
                    postTrim = juce::Decibels::decibelsToGain (levelDb);
                    break;
            }

            filterL.coefficients = newCoeffs;
            filterR.coefficients = newCoeffs;

            cachedFreq   = freq;
            cachedQ      = q;
            cachedGainDb = levelDb;
            cachedSr     = sampleRate;
        }

        float processSample (int channel, float input)
        {
            float out = (channel == 0) ? filterL.processSample (input)
                                       : filterR.processSample (input);
            return out * postTrim;
        }
    };

    //==============================================================================
    // The 4 EQ bands
    static constexpr int numBands = 4;
    std::array<FilterBand, numBands> bands;

    // Envelope follower (RMS power detection)
    float rmsSq       = 0.0f;
    float envelope    = 0.0f;  // post-DR — drives the filter
    float rawEnvelope = 0.0f;  // pre-DR — perceived loudness

    // Cached sample rate
    double currentSampleRate = 44100.0;

    //==============================================================================
    // Real-time spectrum analysis
    juce::dsp::FFT forwardFFT{ fftOrder };

    std::array<float, fftSize> inputFIFO{};
    std::array<float, fftSize> wetFIFO{};
    int fifoIndex = 0;

    std::array<float, fftSize * 2> fftWork{};
    std::array<float, fftSize>     fftWindow{};

    std::array<float, numSpectrumBins> inputSpectrum{};
    std::array<float, numSpectrumBins> wetSpectrum{};

    void performFFT (const std::array<float, fftSize>& fifo,
                     std::array<float, numSpectrumBins>& dest) noexcept;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DynamicResonatorAudioProcessor)
};
