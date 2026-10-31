# Build Dynamic Resonator

## Dependencies

- CMake 3.22 or newer and a C++17 compiler.
- For the Windows build checked for this import: Visual Studio 2022 Build Tools, the Desktop development with C++ workload and a Windows SDK.
- JUCE at commit `3ba67d4585e9d1fbcdb26a877c7978608b1f802e` (its CMake project declares 8.0.13). The exact revision is pinned by the `external/JUCE` submodule, rather than a moving branch.

JUCE supplies the VST3 integration used here; a separate local Steinberg SDK checkout is not required. Required module/framework source is available through the public JUCE dependency. Retain its license notices and use it under the applicable terms.

## Windows x64

```powershell
git clone --recurse-submodules https://github.com/NeutronStar714/pneutrino-dynamic-resonator.git
cd pneutrino-dynamic-resonator
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --target DynamicResonator_VST3 DynamicResonator_Standalone --parallel 1 -- /nodeReuse:false
```

For an already-cloned repository, run `git submodule update --init --recursive` before configuring. GitHub's automatic source ZIP does not populate submodules; use the recursive clone to obtain the complete dependency tree.

Expected outputs:

- `build/DynamicResonator_artefacts/Release/Standalone/Dynamic Resonator.exe`
- `build/DynamicResonator_artefacts/Release/VST3/Dynamic Resonator.vst3/`

The VST3 is a bundle directory: copy the entire directory when installing it into your DAW's scanned VST3 location. Automatic installation is not required for building. The standalone lets you select an audio device and use an input without a plugin host.

An existing matching JUCE checkout can be reused without downloading another copy:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DPNEUTRINO_JUCE_SOURCE_DIR="D:/path/to/JUCE"
```

Use the pinned revision when reproducing the documented build. This explicit option replaces the original machine-specific sibling-directory assumption. The four DSP/UI source files are unchanged by the import.

macOS/Linux build and runtime behavior have not been verified for this portfolio package. See [testing status](docs/TESTING.md).
