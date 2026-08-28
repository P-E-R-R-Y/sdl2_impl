/**
 * @file SdlText.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLTEXT_HPP_
#define SDLTEXT_HPP_

//Sdl
#include <SDL.h>
#include <SDL_ttf.h>

//Interface
#include "IText.hpp"

//encapsulation
#include "SdlFont.hpp"

#include <string>

/**
 * @brief Une chaine rendue en texture, refaite quand elle change.
 *
 * SDL_ttf rend vers une SDL_Surface, qu'il faut televerser en SDL_Texture
 * pour la dessiner - une opration bien trop chere pour chaque frame. On
 * garde donc la texture et un drapeau : tant que ni le texte, ni la police,
 * ni le corps, ni la couleur ne bougent, on redessine la meme.
 *
 * Comme SdlTexture, la construction attend le premier dessin : c'est la
 * seule ou le renderer est connu.
 */
class SdlText : public graphic::IText {

    public:
        SdlText(std::string data, SdlFont &font) : _text(data), _font(&font) {}

        ~SdlText() override { drop(); }

        bool isReady() const override { return _font && _font->isReady(); }

        void setText(const std::string text) override { _text = text; _dirty = true; }
        std::string getText() const override { return _text; }

        void setFont(graphic::IFont *font) override {
            _font = static_cast<SdlFont *>(font);
            _dirty = true;
        }

        void setFontSize(unsigned int size) override { _size = size; _dirty = true; }
        unsigned int getFontSize() const override { return _size; }

        void setTextColor(Color color) override { _color = color; _dirty = true; }
        Color getTextColor() const override { return _color; }

        void setPosition(Vector2f position) override { _position = position; }
        Vector2f getPosition() const override { return _position; }

        //la rotation passe par le dessin : elle ne change pas la texture
        void setRotation(float angle) override { _rotation = angle; }
        float getRotation() const override { return _rotation; }

        friend class SdlWindow;

    private:
        /** @brief La texture a jour pour @p renderer, refaite si besoin. */
        SDL_Texture *handle(SDL_Renderer *renderer) const {
            if (!renderer || !_font || !_font->isReady())
                return nullptr;
            if (_texture && !_dirty && _owner == renderer)
                return _texture;

            drop();

            const SDL_Color color{_color.r, _color.g, _color.b, _color.a};
            /* La variante _Wrapped, et pas la simple : celle-ci rend une seule
             * ligne et avale les '\n', la ou sfml et raylib les respectent.
             * Une longueur de zero ne coupe qu'aux retours a la ligne. */
            SDL_Surface *surface = TTF_RenderUTF8_Blended_Wrapped(_font->handle(_size), _text.c_str(), color, 0);

            if (!surface)
                return nullptr;
            _texture = SDL_CreateTextureFromSurface(renderer, surface);
            _width = surface->w;
            _height = surface->h;
            SDL_FreeSurface(surface);

            _owner = renderer;
            _dirty = false;
            return _texture;
        }

        void drop() const {
            if (_texture)
                SDL_DestroyTexture(_texture);
            _texture = nullptr;
            _owner = nullptr;
        }

        std::string _text;
        SdlFont *_font = nullptr;
        unsigned _size = 24;
        Color _color{255, 255, 255, 255};
        Vector2f _position{0, 0};
        float _rotation = 0.f;

        /* mutable : le cache n'est pas l'etat du texte, seulement sa forme
         * televersee. handle() est const parce que dessiner ne modifie pas
         * ce que le texte DIT. */
        mutable SDL_Texture *_texture = nullptr;
        mutable SDL_Renderer *_owner = nullptr;
        mutable bool _dirty = true;
        mutable int _width = 0;
        mutable int _height = 0;
};

/** @} */

#endif /* !SDLTEXT_HPP_ */
