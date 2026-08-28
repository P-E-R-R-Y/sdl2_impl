/**
 * @file SdlSoundBuffer.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLSOUNDBUFFER_HPP_
#define SDLSOUNDBUFFER_HPP_

//Sdl
#include <SDL_mixer.h>

//Interface
#include "ISoundBuffer.hpp"

#include <iostream>
#include <string>

/**
 * @brief Un echantillon decode en memoire, partageable par plusieurs sons.
 *
 * Mix_Chunk EST le tampon : SDL_mixer le decode entierement au chargement,
 * puis le joue sur le canal qu'on veut. C'est exactement le partage que le
 * contrat decrit - un buffer, plusieurs ISound.
 */
class SdlSoundBuffer : public audio::ISoundBuffer {

    public:
        SdlSoundBuffer(std::string path) {
            _chunk = Mix_LoadWAV(path.c_str());
            if (!_chunk)
                std::cerr << "Failed to load sound: " << Mix_GetError() << std::endl;
        }

        ~SdlSoundBuffer() {
            if (_chunk)
                Mix_FreeChunk(_chunk);
        }

        bool isReady() const override { return _chunk != nullptr; }

        /**
         * @brief La duree, en secondes.
         *
         * SDL_mixer ne la donne pas : on la deduit de la taille brute et du
         * format ouvert par le module. C'est exact tant que le tampon est
         * au format du peripherique, ce que Mix_LoadWAV garantit puisqu'il
         * reechantillonne au chargement.
         */
        float getLength() const override {
            int frequency = 0;
            uint16_t format = 0;
            int channels = 0;

            if (!_chunk || !Mix_QuerySpec(&frequency, &format, &channels) || frequency == 0)
                return 0.f;

            const int bytes = (SDL_AUDIO_BITSIZE(format) / 8) * channels;

            if (bytes == 0)
                return 0.f;
            return static_cast<float>(_chunk->alen) / static_cast<float>(bytes * frequency);
        }

        Mix_Chunk *handle() const { return _chunk; }

    private:
        Mix_Chunk *_chunk = nullptr;
};

/** @} */

#endif /* !SDLSOUNDBUFFER_HPP_ */
