/**
 * @file SdlTexture.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLTEXTURE_HPP_
#define SDLTEXTURE_HPP_

//Sdl
#include <SDL.h>
#include <SDL_image.h>

//Interface
#include "ITexture.hpp"

#include <iostream>
#include <string>

/**
 * @brief Une image en memoire vive, televersee a la demande.
 *
 * C'est LA difference avec sfml et raylib, et elle vient du contrat :
 * createTexture(path) ne recoit pas de fenetre, alors qu'une SDL_Texture
 * appartient a un SDL_Renderer, qui appartient a une fenetre.
 *
 * On garde donc la SDL_Surface - de la memoire vive, qui ne depend de rien -
 * et on ne fabrique la texture qu'au premier dessin, pour le renderer qui la
 * demande. Changer de fenetre refabrique, et l'ancienne est liberee.
 *
 * Le meme objet traverse donc un changement de fenetre sans que l'appelant
 * ait a le recharger.
 */
class SdlTexture : public graphic::ITexture {

    public:
        SdlTexture(std::string path) {
            _surface = IMG_Load(path.c_str());
            if (!_surface)
                std::cerr << "Failed to load texture: " << IMG_GetError() << std::endl;
        }

        ~SdlTexture() {
            drop();
            if (_surface)
                SDL_FreeSurface(_surface);
        }

        bool isReady() const override {
            return _surface != nullptr;
        }

        Vector2f getSize() const override {
            if (!_surface)
                return {};
            return {static_cast<double>(_surface->w), static_cast<double>(_surface->h)};
        }

        /** @brief La texture pour CE renderer, fabriquee au besoin. */
        SDL_Texture *handle(SDL_Renderer *renderer) const {
            if (!_surface || !renderer)
                return nullptr;
            if (_texture && _owner == renderer)
                return _texture;

            drop();
            _texture = SDL_CreateTextureFromSurface(renderer, _surface);
            _owner = renderer;
            return _texture;
        }

    private:
        void drop() const {
            if (_texture)
                SDL_DestroyTexture(_texture);
            _texture = nullptr;
            _owner = nullptr;
        }

        SDL_Surface *_surface = nullptr;

        /* mutable : le cache n'est pas l'etat de l'image, juste sa forme
         * televersee pour un renderer donne. */
        mutable SDL_Texture *_texture = nullptr;
        mutable SDL_Renderer *_owner = nullptr;
};

/** @} */

#endif /* !SDLTEXTURE_HPP_ */
