#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
// Parameter IDs
const juce::String DynamicResonatorAudioProcessor::paramDryID     = "dry";
const juce::String DynamicResonatorAudioProcessor::paramWetID     = "wet";
const juce::String DynamicResonatorAudioProcessor::paramAttackID  = "attack";
const juce::String DynamicResonatorAudioProcessor::paramReleaseID = "release";
const juce::String DynamicResonatorAudioProcessor::paramModeID    = "mode";
const juce::String DynamicResonatorAudioProcessor::paramDRMinID   = "dr_min";
const juce::String DynamicResonatorAudioProcessor::paramDRMaxID   = "dr_max";

const juce::String DynamicResonatorAudioProcessor::paramB1FreqID  = "b1_freq";
const juce::String DynamicResonatorAudioProcessor::paramB1QID     = "b1_q";
const juce::String DynamicResonatorAudioProcessor::paramB1LevelID = "b1_level";
const juce::String DynamicResonatorAudioProcessor::paramB1SensID  = "b1_sens";
const juce::String DynamicResonatorAudioProcessor::paramB1DirID   = "b1_dir";

const juce::String DynamicResonatorAudioProcessor::paramB2FreqID  = "b2_freq";
const juce::String DynamicResonatorAudioProcessor::paramB2QID     = "b2_q";
const juce::String DynamicResonatorAudioProcessor::paramB2LevelID = "b2_level";
const juce::String DynamicResonatorAudioProcessor::paramB2SensID  = "b2_sens";
const juce::String DynamicResonatorAudioProcessor::paramB2DirID   = "b2_dir";

const juce::String DynamicResonatorAudioProcessor::paramB3FreqID  = "b3_freq";
const juce::String DynamicResonatorAudioProcessor::paramB3QID     = "b3_q";
const juce::String DynamicResonatorAudioProcessor::paramB3LevelID = "b3_level";
const juce::String DynamicResonatorAudioProcessor::paramB3SensID  = "b3_sens";
const juce::String DynamicResonatorAudioProcessor::paramB3DirID   = "b3_dir";

const juce::String DynamicResonatorAudioProcessor::paramB4FreqID  = "b4_freq";
const juce::String DynamicResonatorAudioProcessor::paramB4QID     = "b4_q";
const juce::String DynamicResonatorAudioProcessor::paramB4LevelID = "b4_level";
const juce::String DynamicResonatorAudioProcessor::paramB4SensID  = "b4_sens";
const juce::String DynamicResonatorAudioProcessor::paramB4DirID   = "b4_dir";

//==============================================================================
// NormalisableRange for log-frequency knobs
static juce::NormalisableRange<float> freqRange (20.0f, 20000.0f,
    [] (float min, float max, float norm) { return min * std::pow (max / min, norm); },
    [] (float min, float max, float val)  { return std::log (val / min) / std::log (max / min); },
    [] (float, float, float val)          { return val; });

//==============================================================================
DynamicResonatorAudioProcessor::DynamicResonatorAudioProcessor()
    : AudioProcessor (BusesProperties().withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "Parameters",
      {
          std::make_unique<juce::AudioParameterFloat> (paramDryID, "Dry", 0.0f, 1.0f, 0.5f),
          std::make_unique<juce::AudioParameterFloat> (paramWetID, "Wet", 0.0f, 1.0f, 0.5f),
          std::make_unique<juce::AudioParameterInt> (paramAttackID,  "Attack",  1,  100, 23),
          std::make_unique<juce::AudioParameterInt> (paramReleaseID, "Release", 20, 300, 110),
          std::make_unique<juce::AudioParameterBool> (paramModeID, "Mode", false),  // false=RMS, true=AM
          std::make_unique<juce::AudioParameterFloat> (paramDRMinID, "DR Min", 0.0f, 1.0f, 0.0f),
          std::make_unique<juce::AudioParameterFloat> (paramDRMaxID, "DR Max", 0.0f, 1.0f, 1.0f),

          std::make_unique<juce::AudioParameterFloat> (paramB1FreqID,  "B1 Freq",  freqRange,    20.0f),
          std::make_unique<juce::AudioParameterFloat> (paramB1QID,     "B1 Q",     0.1f, 10.0f,  0.707f),
          std::make_unique<juce::AudioParameterFloat> (paramB1LevelID, "B1 PreAmp", -12.0f, 12.0f, 0.0f),
          std::make_unique<juce::AudioParameterFloat> (paramB1SensID,  "B1 Sens",  0.0f, 1.0f,   0.0f),
          std::make_unique<juce::AudioParameterBool> (paramB1DirID,  "B1 Dir",   false),  // false=Up, true=Down

          std::make_unique<juce::AudioParameterFloat> (paramB2FreqID,  "B2 Freq",  freqRange,    200.0f),
          std::make_unique<juce::AudioParameterFloat> (paramB2QID,     "B2 Q",     0.1f, 10.0f,  0.707f),
          std::make_unique<juce::AudioParameterFloat> (paramB2LevelID, "B2 Level", -24.0f, 24.0f, 0.0f),
          std::make_unique<juce::AudioParameterFloat> (paramB2SensID,  "B2 Sens",  0.0f, 1.0f,   0.7f),
          std::make_unique<juce::AudioParameterBool> (paramB2DirID,  "B2 Dir",   false),

          std::make_unique<juce::AudioParameterFloat> (paramB3FreqID,  "B3 Freq",  freqRange,    2000.0f),
          std::make_unique<juce::AudioParameterFloat> (paramB3QID,     "B3 Q",     0.1f, 10.0f,  0.707f),
          std::make_unique<juce::AudioParameterFloat> (paramB3LevelID, "B3 Level", -24.0f, 24.0f, 0.0f),
          std::make_unique<juce::AudioParameterFloat> (paramB3SensID,  "B3 Sens",  0.0f, 1.0f,   0.7f),
          std::make_unique<juce::AudioParameterBool> (paramB3DirID,  "B3 Dir",   false),

          std::make_unique<juce::AudioParameterFloat> (paramB4FreqID,  "B4 Freq",  freqRange,    100.0f),
          std::make_unique<juce::AudioParameterFloat> (paramB4QID,     "B4 Q",     0.1f, 10.0f,  0.707f),
          std::make_unique<juce::AudioParameterFloat> (paramB4LevelID, "B4 Level", -24.0f, 24.0f, 0.0f),
          std::make_unique<juce::AudioParameterFloat> (paramB4SensID,  "B4 Sens",  0.0f, 1.0f,   1.0f),
          std::make_unique<juce::AudioParameterBool> (paramB4DirID,  "B4 Dir",   false),
      })
{
    bands[0].type = FilterType::HP;
    bands[1].type = FilterType::Peak;
    bands[2].type = FilterType::Peak;
    bands[3].type = FilterType::LP;

    // Precompute Hann window
    for (int i = 0; i < fftSize; ++i)
        fftWindow[static_cast<size_t> (i)] =
            0.5f * (1.0f - std::cos (juce::MathConstants<float>::twoPi * float (i) / float (fftSize - 1)));
}

DynamicResonatorAudioProcessor::~DynamicResonatorAudioProcessor() = default;

//==============================================================================
void DynamicResonatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate       = sampleRate;
    spec.maximumBlockSize = juce::uint32 (samplesPerBlock);
    spec.numChannels      = 1;

    for (auto& band : bands)
        band.prepare (spec);

    for (auto& band : bands)
        band.reset();

    rmsSq       = 0.0f;
    envelope    = 0.0f;
    rawEnvelope = 0.0f;
    fifoIndex = 0;

    inputFIFO.fill (0.0f);
    wetFIFO.fill  (0.0f);
    inputSpectrum.fill (0.0f);
    wetSpectrum.fill   (0.0f);
}

void DynamicResonatorAudioProcessor::releaseResources() {}

//==============================================================================
void DynamicResonatorAudioProcessor::performFFT (const std::array<float, fftSize>& fifo,
                                                  std::array<float, numSpectrumBins>& dest) noexcept
{
    for (int i = 0; i < fftSize; ++i)
        fftWork[static_cast<size_t> (i)] =
            fifo[static_cast<size_t> (i)] * fftWindow[static_cast<size_t> (i)];

    std::fill (fftWork.begin() + fftSize, fftWork.end(), 0.0f);

    forwardFFT.performRealOnlyForwardTransform (fftWork.data());

    for (int i = 0; i < numSpectrumBins; ++i)
    {
        const auto idx = static_cast<size_t> (i);
        const float real = fftWork[idx * 2];
        const float imag = fftWork[idx * 2 + 1];
        dest[idx] = std::sqrt (real * real + imag * imag);
    }
}

//==============================================================================
void DynamicResonatorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                                    juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    const auto numChannels = buffer.getNumChannels();
    const auto numSamples  = buffer.getNumSamples();

    // --- Read parameters (once per block) ----------------------------------------
    const float dryGain  = apvts.getRawParameterValue (paramDryID)->load();
    const float wetGain  = apvts.getRawParameterValue (paramWetID)->load();
    const float attackMs = float (apvts.getRawParameterValue (paramAttackID)->load());
    const float releaseMs= float (apvts.getRawParameterValue (paramReleaseID)->load());
    const bool  useRMS   = apvts.getRawParameterValue (paramModeID)->load() < 0.5f;
    const float drMin    = apvts.getRawParameterValue (paramDRMinID)->load();
    const float drMax    = apvts.getRawParameterValue (paramDRMaxID)->load();
    const float drRange  = std::max (drMax - drMin, 0.001f);  // guard against divide-by-zero

    // Per-band parameter arrays
    float bandFreq[numBands], bandQ[numBands], bandLevel[numBands];
    float bandSens[numBands];
    bool  bandDirUp[numBands];  // true = Up, false = Down

    auto readBand = [&](int b, const juce::String& freqId, const juce::String& qId,
                         const juce::String& lvlId, const juce::String& sensId,
                         const juce::String& dirId)
    {
        bandFreq[b]   = apvts.getRawParameterValue (freqId)->load();
        bandQ[b]      = apvts.getRawParameterValue (qId)   ->load();
        bandLevel[b]  = apvts.getRawParameterValue (lvlId) ->load();
        bandSens[b]   = apvts.getRawParameterValue (sensId)->load();
        bandDirUp[b]  = apvts.getRawParameterValue (dirId)->load() < 0.5f;  // 0=Up, 1=Down
    };

    readBand (0, paramB1FreqID,  paramB1QID,  paramB1LevelID, paramB1SensID, paramB1DirID);
    readBand (1, paramB2FreqID,  paramB2QID,  paramB2LevelID, paramB2SensID, paramB2DirID);
    readBand (2, paramB3FreqID,  paramB3QID,  paramB3LevelID, paramB3SensID, paramB3DirID);
    readBand (3, paramB4FreqID,  paramB4QID,  paramB4LevelID, paramB4SensID, paramB4DirID);

    // --- Envelope follower coefficients (user-controlled attack/release) ---------
    const float attackCoef  = 1.0f - std::exp (-1.0f / float (currentSampleRate * attackMs * 0.001f));
    const float releaseCoef = 1.0f - std::exp (-1.0f / float (currentSampleRate * releaseMs * 0.001f));

    // --- Pointers ---------------------------------------------------------------
    const float* const inL  = buffer.getReadPointer (0);
    const float* const inR  = numChannels > 1 ? buffer.getReadPointer (1) : inL;
          float* const outL = buffer.getWritePointer (0);
          float* const outR = numChannels > 1 ? buffer.getWritePointer (1) : outL;

    // --- Pre-amp gain (from band 1 level / HP knob, labelled "Pre") ---
    const float preAmp = juce::Decibels::decibelsToGain (bandLevel[0]);

    // --- Pre-compute modulation ranges (constant per block) ---------------------
    float maxOctUp[numBands], maxOctDown[numBands];
    for (int b = 0; b < numBands; ++b)
    {
        maxOctUp[b]   = std::log2 (50000.0f / bandFreq[b]);
        maxOctDown[b] = std::log2 (bandFreq[b] / 10.0f);
    }

    // --- Per-sample processing --------------------------------------------------
    for (int s = 0; s < numSamples; ++s)
    {
        // Apply pre-amp to the wet signal and envelope path
        const float preL = inL[s] * preAmp;
        const float preR = inR[s] * preAmp;

        // ---- Envelope follower (RMS p=2 or AM p=1, user-selectable) ----
        float instantLevel;
        if (useRMS)
        {
            if (numChannels > 1)
                instantLevel = (preL * preL + preR * preR) * 0.5f;  // p=2
            else
                instantLevel = preL * preL;
        }
        else
        {
            if (numChannels > 1)
                instantLevel = (std::abs (preL) + std::abs (preR)) * 0.5f;  // p=1
            else
                instantLevel = std::abs (preL);
        }

        const float coef = (instantLevel > rmsSq) ? attackCoef : releaseCoef;
        rmsSq += coef * (instantLevel - rmsSq);
        rawEnvelope = useRMS ? std::sqrt (rmsSq) : rmsSq;
        // Dynamic Range window: only raw envelope within [drMin, drMax] maps to 0..1.
        // Below drMin → 0, above drMax → 1.
        envelope = (rawEnvelope - drMin) / drRange;
        envelope = juce::jlimit (0.0f, 1.0f, envelope);

        // ---- Pre-compute modulated frequencies (per-band direction + sensitivity) ----
        float modFreq[numBands];
        for (int b = 0; b < numBands; ++b)
        {
            float f = bandFreq[b];
            const float sens = bandSens[b];

            if (sens > 0.001f)
            {
                if (bandDirUp[b])  // Up mode: freq rises with loudness
                {
                    f *= std::exp2 (envelope * sens * maxOctUp[b]);
                    f = std::min (f, 20000.0f);
                }
                else  // Down mode: freq falls with loudness
                {
                    f /= std::exp2 (envelope * sens * maxOctDown[b]);
                    f = std::max (f, 20.0f);
                }
            }

            modFreq[b] = f;
        }

        // ---- Left channel: update coefficients + process ----
        // Coefficients are only recomputed every 4 samples — the filter's natural
        // response masks the ~90 µs granularity, but avoids heap allocations
        // (IIR::Coefficients creation) in the audio thread.
        float sampleL = preL;
        if ((s & 3) == 0)
        {
            for (int b = 0; b < numBands; ++b)
            {
                bands[b].updateCoefficients (modFreq[b], bandQ[b], bandLevel[b], currentSampleRate);
                sampleL = bands[b].processSample (0, sampleL);
            }
        }
        else
        {
            for (int b = 0; b < numBands; ++b)
                sampleL = bands[b].processSample (0, sampleL);
        }

        // ---- Right channel: coefficients already set, just process ----
        float sampleR = preR;
        for (int b = 0; b < numBands; ++b)
            sampleR = bands[b].processSample (1, sampleR);

        // ---- Soft clip ----
        sampleL = std::tanh (sampleL);
        sampleR = std::tanh (sampleR);

        // ---- FFT capture (pre-amped signal) ----
        const float inputMono = (preL + preR) * 0.5f;
        const float wetMono   = (sampleL + sampleR) * 0.5f;

        const auto idx = static_cast<size_t> (fifoIndex);
        inputFIFO[idx] = inputMono;
        wetFIFO[idx]   = wetMono;
        ++fifoIndex;

        if (fifoIndex >= fftSize)
        {
            performFFT (inputFIFO, inputSpectrum);
            performFFT (wetFIFO,   wetSpectrum);
            fifoIndex = 0;
        }

        // ---- Output ----
        outL[s] = inL[s] * dryGain + sampleL * wetGain;
        outR[s] = inR[s] * dryGain + sampleR * wetGain;

        outL[s] = juce::jlimit (-1.0f, 1.0f, outL[s]);
        outR[s] = juce::jlimit (-1.0f, 1.0f, outR[s]);
    }
}

//==============================================================================
juce::AudioProcessorEditor* DynamicResonatorAudioProcessor::createEditor()
{
    return new DynamicResonatorAudioEditor (*this);
}

bool DynamicResonatorAudioProcessor::hasEditor() const { return true; }

//==============================================================================
void DynamicResonatorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void DynamicResonatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DynamicResonatorAudioProcessor();
}
