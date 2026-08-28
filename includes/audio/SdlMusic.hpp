/**
 * @file SdlMusic.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLMUSIC_HPP_
#define SDLMUSIC_HPP_

//Sdl
#include <SDL_mixer.h>

//Interface
#include "IMusic.hpp"

#include <iostream>
#include <string>

/**
 * @brief Un flux long, decode au fil de la lecture.
 *
 * SDL_mixer n'a qu'UN canal de musique pour tout le processus : deux
 * SdlMusic existent tres bien cote a cote, mais la seconde a jouer
 * remplace la premiere. Chaque objet verifie donc que le canal est encore
 * le sien avant d'agir, sinon un stop() sur une musique arretee depuis
 * longtemps couperait celle qui joue.
 */
class SdlMusic : public audio::IMusic {

    public:
        SdlMusic(std::string path) {
            _music = Mix_LoadMUS(path.c_str());
            if (!_music)
                std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        }

        ~SdlMusic() override {
            if (mine())
                Mix_HaltMusic();
            if (_music)
                Mix_FreeMusic(_music);
        }

        bool isReady() const override { return _music != nullptr; }

        void play() override {
            if (!_music)
                return;
            if (mine() && Mix_PausedMusic()) {
                Mix_ResumeMusic();
                return;
            }
            Mix_PlayMusic(_music, _loop ? -1 : 0);
            _current = _music;
            Mix_VolumeMusic(static_cast<int>(_volume * MIX_MAX_VOLUME));
        }

        void pause() override {
            if (mine())
                Mix_PauseMusic();
        }

        void stop() override {
            if (mine()) {
                Mix_HaltMusic();
                _current = nullptr;
            }
            _elapsed = 0.f;
        }

        /// SDL_mixer decode sur son propre fil : rien a pousser par frame
        void update() override {}

        void setVolume(float volume) override {
            _volume = volume;
            if (mine())
                Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
        }

        float getVolume() const override { return _volume; }

        /* La boucle se decide au lancement (Mix_PlayMusic prend le nombre de
         * tours) : la changer en cours de route ne prend effet qu'au
         * prochain play(). */
        void setLoop(bool loop) override { _loop = loop; }
        bool getLoop() const override { return _loop; }

        void setTime(float position) override {
            if (!mine())
                return;
            Mix_RewindMusic();
            if (Mix_SetMusicPosition(position) == 0)
                _elapsed = position;
        }

        /* SDL_mixer ne sait pas dire ou en est la lecture pour tous les
         * formats : on rend la derniere position DEMANDEE, ce qui est
         * exact apres un setTime() et vaut 0 depuis le debut. */
        float getTime() const override { return _elapsed; }

        float getLength() const override {
            if (!_music)
                return 0.f;

            const double length = Mix_MusicDuration(_music);

            return (length < 0) ? 0.f : static_cast<float>(length);
        }

        //aucune spatialisation sur le canal musique : retenu, jamais applique
        void setPosition(Vector3f position) override { _position = position; }
        Vector3f getPosition() const override { return _position; }

        void setVelocity(Vector3f velocity) override { _velocity = velocity; }
        Vector3f getVelocity() const override { return _velocity; }

    private:
        /** @brief Le canal musique joue-t-il encore CETTE musique ? */
        bool mine() const { return _music != nullptr && _current == _music; }

        Mix_Music *_music = nullptr;

        /// le canal est unique : on retient qui l'occupe
        static inline Mix_Music *_current = nullptr;

        float _volume = 1.f;
        bool _loop = false;
        float _elapsed = 0.f;
        Vector3f _position{0, 0, 0};
        Vector3f _velocity{0, 0, 0};
};

/** @} */

#endif /* !SDLMUSIC_HPP_ */
