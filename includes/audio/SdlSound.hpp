/**
 * @file SdlSound.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLSOUND_HPP_
#define SDLSOUND_HPP_

//Sdl
#include <SDL_mixer.h>

//Interface
#include "ISound.hpp"

//encapsulation
#include "SdlSoundBuffer.hpp"

/**
 * @brief Une lecture d'un tampon, sur son propre canal.
 *
 * Le canal est reserve au premier play() et garde jusqu'a la destruction :
 * sans ca, pause() et stop() ne sauraient pas quoi arreter.
 */
class SdlSound : public audio::ISound {

    public:
        SdlSound(SdlSoundBuffer &buffer) : _buffer(buffer) {}

        ~SdlSound() override {
            if (mine())
                Mix_HaltChannel(_channel);
        }

        bool isReady() const override { return _buffer.isReady(); }

        void play() override {
            if (!_buffer.isReady())
                return;
            if (mine() && Mix_Paused(_channel)) {
                Mix_Resume(_channel);
                return;
            }
            _channel = Mix_PlayChannel(-1, _buffer.handle(), 0);
            if (mine())
                Mix_Volume(_channel, static_cast<int>(_volume * MIX_MAX_VOLUME));
        }

        void pause() override {
            if (mine())
                Mix_Pause(_channel);
        }

        void stop() override {
            if (mine())
                Mix_HaltChannel(_channel);
            _channel = -1;
        }

        /// dans [0, 1] comme le contrat, MIX_MAX_VOLUME est un detail de SDL
        void setVolume(float volume) override {
            _volume = volume;
            if (mine())
                Mix_Volume(_channel, static_cast<int>(volume * MIX_MAX_VOLUME));
        }

        float getVolume() const override { return _volume; }

        /* SDL_mixer ne fait pas de spatialisation 3D : il n'a qu'une
         * panoramique gauche/droite. On retient donc la position pour la
         * rendre telle quelle, et on projette x sur la panoramique - ce que
         * le contrat permet, puisqu'il demande de POUVOIR positionner, sans
         * promettre un modele acoustique. */
        void setPosition(Vector3f position) override {
            _position = position;
            if (!mine())
                return;

            const float clamped = (position.x < -1.f) ? -1.f : (position.x > 1.f ? 1.f : position.x);
            const uint8_t right = static_cast<uint8_t>((clamped + 1.f) * 0.5f * 255.f);

            Mix_SetPanning(_channel, static_cast<uint8_t>(255 - right), right);
        }

        Vector3f getPosition() const override { return _position; }

        void setVelocity(Vector3f velocity) override { _velocity = velocity; }
        Vector3f getVelocity() const override { return _velocity; }

    private:
        /** @brief Ce canal joue-t-il encore CE son ? */
        bool mine() const {
            return _channel >= 0 && Mix_GetChunk(_channel) == _buffer.handle();
        }

        SdlSoundBuffer &_buffer;
        int _channel = -1;
        float _volume = 1.f;
        Vector3f _position{0, 0, 0};
        Vector3f _velocity{0, 0, 0};
};

/** @} */

#endif /* !SDLSOUND_HPP_ */
