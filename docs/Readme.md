# sdl2_impl

version: 0.1.0

[![Docs](https://img.shields.io/badge/docs-doxygen-blue.svg)](https://p-e-r-r-y.github.io/sdl2_impl)

> SDL2 (API 2.32, via sdl2-compat sur cette machine) behind [igraphic](https://github.com/P-E-R-R-Y/igraphic) and
> [iaudio](https://github.com/P-E-R-R-Y/iaudio).

Exports `getGraphic2Module()` and `getAudioModule()`. **No
`getGraphic3Module`** — SDL renders no 3D scene graph, so the symbol is
absent rather than returning something broken.
`Get<IGraphic3Module>("sdl2")` is `nullptr`.

## Two ways to take it

| target | artefact | use |
|---|---|---|
| `sdl2_impl` | `sdl2_impl.dylib` | the plugin, what `ModuleManager::Load()` opens |
| `sdl2_impl_static` | `libsdl2_impl.a` | linked in, brings its include dirs |

## Windows are real, and there can be several

Unlike raylib, `createWindow()` opens a **new** window every call, so a
hosted game gets its own. SDL's event queue is process-wide, not per window,
so `SdlWindow` keeps a registry of live windows and spreads each event to the
one it names — events without a window (`SDL_QUIT`, device hotplug) reach
everyone.

## Resources outlive the renderer

`createTexture(path)` and `createText(text, font)` receive no window, but SDL
needs a `SDL_Renderer` to build a texture. Both keep the renderer-independent
data and upload lazily at draw time, so an object survives a window change.
