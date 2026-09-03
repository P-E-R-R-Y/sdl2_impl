# sdl2_impl — changelog

Markers: 🟢 added · 🔴 breaking · 🔵 fix · ⚪ internal or docs · 🟡 proposed
in the plan, no code written yet.

## v0.1.0

- 🟢 fills `graphic2` + `audio`
- 🟢 single entry point `getModules()`
- 🟢 SHARED and STATIC targets (STATIC recompiles `sources/sdl2.cpp` for
  a consumer that doesn't need it — known, not fixed)

### Measured failure, not fixed

- 🔴 `createTexture`/`createSoundBuffer` etc. return a dead object
  (`isReady()==false`) instead of `nullptr` when loading fails.
  here, `isReady()==false` just propagates without a crash — sdl2 is the most forgiving of the four

## Proposed, not written

- 🟡 `claims()`: nothing — sdl2 goes through Metal, nothing to claim
- 🟡 STATIC target → INTERFACE, zero object code
- 🟡 examples: still has none
