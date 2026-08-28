/**
 * @file SdlSprite.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLSPRITE_HPP_
#define SDLSPRITE_HPP_

//Sdl
#include <SDL.h>

//Interface
#include "ISprite.hpp"

//encapsulation
#include "SdlTexture.hpp"

/**
 * @brief Une vue sur une texture : decoupe, position, taille, rotation.
 *
 * Aucune ressource propre - SDL dessine une texture par un simple appel
 * avec deux rectangles. Le sprite n'est donc que ces deux rectangles.
 */
class SdlSprite : public graphic::ISprite {

    public:
        SdlSprite(SdlTexture &texture) : _texture(texture) {
            const Vector2f size = texture.getSize();

            _crop = {0.f, 0.f, static_cast<float>(size.x), static_cast<float>(size.y)};
            _size = size;
        }

        ~SdlSprite() override = default;

        bool isReady() const override {
            return _texture.isReady();
        }

        Rect<float> getBounds() const override {
            return {static_cast<float>(_position.x), static_cast<float>(_position.y),
                    static_cast<float>(_size.x), static_cast<float>(_size.y)};
        }

        void setCrop(Rect<float> rect) override {
            _crop = rect;
        }

        Vector2f getPosition() const override { return _position; }
        void setPosition(Vector2f position) override { _position = position; }

        float getRotation() const override { return _rotation; }
        void setRotation(float angle, bool isRad = false) override {
            //SDL_RenderCopyEx prend des degres : on convertit une seule fois
            _rotation = isRad ? angle * 180.f / 3.14159265f : angle;
        }

        Vector2f getSize() const override { return _size; }
        void setSize(Vector2f size) override { _size = size; }

        friend class SdlWindow;

    private:
        SdlTexture &_texture;

        Rect<float> _crop{};
        Vector2f _position{0, 0};
        Vector2f _size{0, 0};
        float _rotation = 0.f;
};

/** @} */

#endif /* !SDLSPRITE_HPP_ */
