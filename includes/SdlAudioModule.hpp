/**
 * @file SdlAudioModule.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @copyright Copyright (c) 2026
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLAUDIO_MODULE_HPP
#define SDLAUDIO_MODULE_HPP

#include "IAudioModule.hpp"

#include "audio/SdlMusic.hpp"
#include "audio/SdlSound.hpp"
#include "audio/SdlSoundBuffer.hpp"

#include <SDL.h>
#include <SDL_mixer.h>

/**
 * @brief SDL_mixer derriere IAudioModule.
 *
 * Le module est independant du module graphique : on peut prendre le son de
 * SDL et l'image de sfml. Il ouvre donc son propre sous-systeme audio, et
 * n'attend rien de la video.
 */
class SdlAudioModule : public IAudioModule {

public:
    SdlAudioModule() = default;
    ~SdlAudioModule() { stop(); }

    const char *type() const override { return IAudioModule::contract; }
    const char *name() const override { return "sdl2"; }

    // music
    audio::IMusic *createMusic(std::string path) override {
        if (!start())
            return nullptr;
        _opened++;
        return new SdlMusic(path);
    }
    void deleteMusic(audio::IMusic *music) override { drop(music); }

    // sound buffer
    audio::ISoundBuffer *createSoundBuffer(std::string path) override {
        if (!start())
            return nullptr;
        _opened++;
        return new SdlSoundBuffer(path);
    }
    void deleteSoundBuffer(audio::ISoundBuffer *buffer) override { drop(buffer); }

    // sound
    audio::ISound *createSound(audio::ISoundBuffer *buffer) override {
        if (!buffer)
            return nullptr;
        _opened++;
        return new SdlSound(*static_cast<SdlSoundBuffer *>(buffer));
    }
    void deleteSound(audio::ISound *sound) override { drop(sound); }

private:
    /** @brief Ouvre le peripherique au premier son demande. */
    bool start() {
        if (_started)
            return true;
        if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0)
            return false;
        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
            SDL_QuitSubSystem(SDL_INIT_AUDIO);
            return false;
        }
        _started = true;
        return true;
    }

    void stop() {
        if (!_started)
            return;
        Mix_CloseAudio();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
        _started = false;
    }

    /** @brief Detruit, et rend le peripherique quand plus rien ne joue. */
    template <typename T>
    void drop(T *object) {
        if (!object)
            return;
        delete object;
        if (_opened > 0)
            _opened--;
        if (_opened == 0)
            stop();
    }

    unsigned _opened = 0;
    bool _started = false;
};

/** @} */

#endif /* !SDLAUDIO_MODULE_HPP */
