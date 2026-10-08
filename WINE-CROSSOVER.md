# REFramework: Resident Evil 7 VR on Wine / CrossOver

Experimental compatibility build based on praydog/REFramework `pd-upscaler`, commit `f18adbe05ae937bdb01f788e6531fefd012ffed1`.

## Changes
- Preserve the active D3D12CreateDevice export on Wine instead of restoring on-disk bytes over D3DMetal's implementation.
- Capture submitted Direct3D 12 DIRECT command queues when swapchain queue-offset discovery fails. Device/thread matching is a heuristic; ambiguous queues remain unsupported.
- Restore the missing ImGui callback type alias needed to compile this revision.

## Install
Close the game. Back up its existing dinput8.dll. Copy the included dinput8.dll next to re7.exe. Keep your existing VR runtime setup and configure dinput8 as native then builtin in the same CrossOver bottle. This archive contains REFramework only; install RE7 VR Reloaded separately from its author if desired.

Tested by the publisher with Resident Evil 7 on macOS, CrossOver Preview, GPTK 4 / D3DMetal and a VR setup. This is experimental: it is not a claim of compatibility with all RE Engine games or all Wine versions. The custom RE7 VR Reloaded menu can still appear horizontally mirrored; that separate plugin is not fixed here.

Video playback failures after logos may require the separate CrossOver Preview GStreamer media fix. A working flat launch is required before troubleshooting VR.

## Build
Check out the base commit, initialize submodules, then apply the three included patches in this order: preserve-export, apiproxy-build-fix, dx12-queue-capture. Follow upstream COMPILING.md, targeting RE7, x64, Release. This asset is the Windows-built DLL tested locally; its hash matches the installed game copy. No RE7 VR Reloaded assets or plugins are redistributed.

Credits: praydog and REFramework contributors. RE7 VR Reloaded: Andyalpa. Compatibility changes and testing: armax9.
