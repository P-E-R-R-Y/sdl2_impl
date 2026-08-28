/**
 * @file sdl2.cpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief sdl2 implementation of IGraphic2Module/IAudioModule
 * @date 2026-08-28
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDL2_CPP_
#define SDL2_CPP_

#include "SdlAudioModule.hpp"
#include "SdlGraphicModule.hpp"

//pas de getGraphic3Module() : SDL ne donne qu'un contexte, sans chargeur de
//maillage ni squelette. Get<IGraphic3Module>("sdl2") reste nullptr.

extern "C" IGraphic2Module *getGraphic2Module() {
    static SdlGraphicModule instance;
    return &instance;
}

extern "C" IAudioModule *getAudioModule() {
    static SdlAudioModule instance;
    return &instance;
}

/**
 * @brief Tout ce que cette dll fournit, sans avoir a nommer un contrat.
 *
 * Le point d'entree d'AnonymousModuleManager : un nom unique, le meme pour
 * toutes les dll, donc trouvable sans connaitre les contrats a l'avance.
 * Le chargeur trie ensuite par IModule::type().
 *
 * IModule ** termine par nullptr, et pas un std::vector : un vecteur ne
 * traverse pas un dlopen sans supposer la meme ABI de bibliotheque standard
 * des deux cotes.
 */
extern "C" IModule **getModules() {
    static IModule *list[] = {
        getGraphic2Module(),
        getAudioModule(),
        nullptr
    };

    return list;
}

/** @} */

#endif /* !SDL2_CPP_ */
