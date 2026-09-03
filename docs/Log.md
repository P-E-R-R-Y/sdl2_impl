# sdl2_impl — journal

Marqueurs : 🟢 ajout · 🔴 rupture · 🔵 correctif · ⚪ interne ou doc · 🟡 propose
dans le plan, code non ecrit.

## v0.1.0

- 🟢 remplit `graphic2` + `audio`
- 🟢 point d'entree unique `getModules()`
- 🟢 cibles SHARED et STATIC (la STATIC recompile `sources/sdl2.cpp` pour
  un consommateur qui n'en a pas besoin — constate, pas corrige)

### Faille mesuree, pas corrigee

- 🔴 `createTexture`/`createSoundBuffer` etc. rendent un objet mort
  (`isReady()==false`) au lieu de `nullptr` quand le chargement echoue.
  ici, `isReady()==false` est propage sans crash — sdl2 est le vendor le plus tolerant des quatre

## Propose, pas ecrit

- 🟡 `claims()` : rien — sdl2 passe par Metal, aucune ressource a confisquer
- 🟡 cible STATIC → INTERFACE, zero object code
- 🟡 des exemples : n'en a toujours aucun
