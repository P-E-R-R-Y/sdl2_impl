/**
 * @file SdlMouse.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLMOUSE_HPP_
#define SDLMOUSE_HPP_

//Sdl
#include <SDL.h>

//Interface
#include "IMouse.hpp"

#include <unordered_map>

class SdlWindow;

class SdlMouse : public graphic::IMouse {

    public:
        SdlMouse(SdlWindow &window) : _window(window) {}
        ~SdlMouse() override = default;

        //definis dans SdlWindow.hpp, une fois SdlWindow complete
        bool isButtonPressed(Buttons key) const override;
        bool isButtonReleased(Buttons key) const override;
        bool isButtonDown(Buttons key) const override;
        bool isButtonUp(Buttons key) const override;

        //relative a la fenetre, comme raylib et sfml : le contrat l'impose,
        //sinon aucun test de survol ecrit dessus ne serait portable
        Vector2f getPosition() const override;
        void setPosition(Vector2f position) override;

        float GetMouseWheelMove() const override;

    private:
        SdlWindow &_window;

        const std::unordered_map<Buttons, uint8_t> _buttons = {
            { Buttons::BUTTON_LEFT, SDL_BUTTON_LEFT },
            { Buttons::BUTTON_RIGHT, SDL_BUTTON_RIGHT },
            { Buttons::BUTTON_MIDDLE, SDL_BUTTON_MIDDLE },
            { Buttons::EXTRA_BUTTON_1, SDL_BUTTON_X1 },
            { Buttons::EXTRA_BUTTON_2, SDL_BUTTON_X2 },
        };
};

/** @} */

#endif /* !SDLMOUSE_HPP_ */
