# Wine / CrossOver releases

## 0.2 — RE7 and RE8

### RE8 / Shadows of Rose — added

- Preserve D3DMetal exports and capture usable DIRECT command queues.
- Pin validated swapchain queues across auxiliary VR queue submissions.
- Avoid blocking submission capture behind the Present/fence monitor.
- Fix the lifetime of the projection layer passed to `xrEndFrame`.
- Retain runtime ownership across frame callbacks and replacement.
- Guard failed temporal-upscaler initialization and missing output textures.
- Add native Rose integration required by the original DLC scripts.
- Include queue-selection regression coverage and an RE8 installation guide.

### RE7

- Keep the verified 0.1 DLL unchanged, with its own archive and guide.

## 0.1 — RE7

- Preserve active D3D12 exports on Wine/D3DMetal.
- Capture submitted DIRECT queues when swapchain-offset discovery fails.
- Restore the ImGui callback type alias required by the pinned source revision.
