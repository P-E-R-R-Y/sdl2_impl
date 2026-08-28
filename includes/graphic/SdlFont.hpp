/**
 * @file SdlFont.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLFONT_HPP_
#define SDLFONT_HPP_

//Sdl
#include <SDL.h>
#include <SDL_ttf.h>

//Interface
#include "IFont.hpp"

#include <iostream>
#include <string>

/**
 * @brief Une police ouverte a une taille donnee.
 *
 * SDL_ttf n'a pas la notion de police independante du corps : une TTF_Font
 * EST une taille. handle(size) recale la taille avant de rendre, ce qui
 * laisse deux SdlText de corps differents partager la meme police - chacun
 * la remet a la sienne juste avant de tracer.
 */
class SdlFont : public graphic::IFont {

    public:
        SdlFont(std::string path) : _path(path) {
            _font = TTF_OpenFont(_path.c_str(), static_cast<int>(_size));
            if (!_font)
                std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        }

        ~SdlFont() {
            if (_font)
                TTF_CloseFont(_font);
        }

        bool isReady() const override {
            return _font != nullptr;
        }

        /** @brief La police, recalee sur @p size si besoin. */
        TTF_Font *handle(unsigned size) const {
            if (_font && size != _size) {
                TTF_SetFontSize(_font, static_cast<int>(size));
                _size = size;
            }
            return _font;
        }

    private:
        std::string _path;
        TTF_Font *_font = nullptr;

        /// mutable : recaler la taille ne change rien de ce que la police EST
        mutable unsigned _size = 24;
};

/** @} */

#endif /* !SDLFONT_HPP_ */
