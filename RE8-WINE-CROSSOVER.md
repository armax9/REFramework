# Resident Evil Village VR on macOS / CrossOver

Experimental Windows x64 RE8 build for the Windows/Steam edition of Resident Evil Village, including the original Shadows of Rose VR scripts. Tested on an M3 Max, CrossOver Preview, GPTK 4 / D3DMetal and Quest 3 through MoltenVR/OpenXR.

Download the **RE8** archive from [version 0.2](https://github.com/armax9/REFramework/releases/tag/v0.2.0-wine-crossover-experimental). This DLL is for the Windows game running in CrossOver, not the native macOS release.

## Changes

- Keep Wine/D3DMetal's active D3D12 export intact; inherit the RE7 compatibility fixes.
- Retain a device-validated DIRECT queue across auxiliary VR queue submissions; use the exact swapchain queue when it is recreated.
- Record submissions after the original `ExecuteCommandLists` call using a nonblocking lock, avoiding the Present/fence lock inversion seen during startup.
- Keep the OpenXR projection layer alive until `xrEndFrame` completes.
- Retain shared runtime snapshots across frame callbacks and remove the temporary null runtime during replacement.
- Validate temporal-upscaler setup and output textures. Disable failed or explicitly unsupported backends safely.
- Add Rose's updater, player order and weapon lookup, world-scale/body-IK properties and matching upper-body mesh visibility expected by the DLC scripts.

## Install

1. Install your usual REFramework VR files and configure a working Windows-compatible OpenXR runtime in the game bottle. For Shadows of Rose, install [NDRW0's mod](https://www.nexusmods.com/residentevilvillage/mods/647) and, if used, [oziman's camera fix](https://www.nexusmods.com/residentevilvillage/mods/832) first.
2. Close the game. Back up its existing `dinput8.dll` and `reframework` directory outside the game folder.
3. Copy **only `dinput8.dll`** from this RE8 archive next to `re8.exe`. Keep all your existing Lua mods and their settings.
4. Keep the Windows x64 `openxr_loader.dll` from your VR setup. Move `openvr_api.dll` out of the game folder to select the tested OpenXR path; this build attempts OpenVR first.
5. Configure `dinput8` as **native, then builtin** in the same CrossOver bottle. Start the headset/runtime connection and launch the game normally.

This minimal archive contains REFramework only. It does not replace game settings or install a headset runtime, Nexus mods, video patches or optional upscaler DLLs. Installing another mod afterward may replace `dinput8.dll`; copy this build back last if needed.

## Video playback

The separate video fix used in the tested setup is [l00sed/re-engine-macos](https://github.com/l00sed/re-engine-macos), revision `4606b9efeb8dcf2f8370a0142eaf68178c0f5e9a`. Follow that project's instructions if movies prevent the Windows game from starting.

Its installer may set `D3DM_ENABLE_METALFX=0`. Recheck the effective game-process environment before enabling the optional upscaler below. The confirmed local video launch reused a matching decoded cache; fresh decoding of every cutscene was not tested.

## Optional temporal upscaling

The tested setup uses the menu's **DLSS/NGX API path backed by GPTK MetalFX** on Apple Silicon. It is not NVIDIA DLSS running on an Apple GPU. The compatible backend dependencies must be obtained separately; they are not in this release.

The local configuration used `D3DM_ENABLE_METALFX=1`, `DXMT_ENABLE_NVEXT=0` and REFramework **Quality** mode. Both eyes initialized and processed frames at 1152×1323 → 1728×1984 per eye. This is a recorded configuration, not a required resolution or a performance claim. An explicit `D3DM_ENABLE_METALFX=0` disables this Wine/D3D12 upscaler route.

## Validation and limits

- The released DLL matches the installed, working RE8 DLL byte for byte.
- The publisher confirmed VR operation in Shadows of Rose; logs confirmed both-eye temporal upscaling and a working OpenXR stream.
- Queue-selection regression tests cover auxiliary queues, ambiguous initial selection, swapchain/device changes, invalid cached queues and thread selection.
- Full campaigns, every scripted encounter, Mercenaries and every Wine/headset combination have not been tested. Third-party aiming/HUD behavior is unchanged; no new Lua aiming patch is included.
- Runtime snapshot changes address the observed failure; they are not a claim that every possible reinitialization race is resolved.

SHA-256 (`dinput8.dll`):

```text
032d51281cf9e9497aa908ea801cc07d5edbac746ed81172074e595575389e85
```

## Source and build

Base: upstream `pd-upscaler` commit `f18adbe05ae937bdb01f788e6531fefd012ffed1`, with this fork's RE7 changes at `13da395d693a6cd00d41629ec636da8296305d90`.

Follow [COMPILING.md](COMPILING.md), initialize submodules, configure x64 Release with `DEVELOPER_MODE=OFF`, and build target `RE8`. The published RE8 binary was cross-built on macOS using clang-cl, MSVC SDK/CRT and Microsoft-compiled DXBC shaders; machine-specific cross-build configuration is kept out of the shared build defaults.

The RE7 archive in version 0.2 reuses the verified 0.1 binary. New shared source changes have been validated with RE8, not a newly rebuilt RE7 binary.

Run the focused queue test with a host C++ compiler:

```sh
c++ -std=c++17 -Wall -Wextra -Werror -Isrc tests/submitted_queue_selection.cpp -o /tmp/reframework-queue-test
/tmp/reframework-queue-test
```

To roll back, close the game and restore your previous DLL and runtime-loader arrangement. Your existing scripts, settings and saves remain separate.

Credits: praydog and REFramework contributors; original Shadows of Rose VR work by NDRW0 and camera fix by oziman. Compatibility changes and testing: armax9. REFramework is MIT-licensed; bundled components retain their respective notices.
