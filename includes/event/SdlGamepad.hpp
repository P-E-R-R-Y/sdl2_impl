/**
 * @file SdlGamepad.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLGAMEPAD_HPP_
#define SDLGAMEPAD_HPP_

//Sdl
#include <SDL.h>

//Interface
#include "IGamepad.hpp"

#include <unordered_map>

class SdlWindow;

/**
 * @brief La manette numero _index, ouverte a la construction.
 *
 * SDL_GameController normalise deja les boutons : pas de table d'indices
 * bruts a deviner comme chez sfml, l'enum du contrat se pose dessus
 * presque un pour un.
 */
class SdlGamepad : public graphic::IGamepad {

    public:
        SdlGamepad(const SdlWindow &window, int index = 0) : _window(window), _index(index) {
            if (SDL_IsGameController(_index))
                _pad = SDL_GameControllerOpen(_index);
        }

        ~SdlGamepad() override {
            if (_pad)
                SDL_GameControllerClose(_pad);
        }

        bool isAvailable() const override {
            return _pad != nullptr && SDL_GameControllerGetAttached(_pad);
        }

        //definis dans SdlWindow.hpp, une fois SdlWindow complete
        bool isButtonPressed(Button button) const override;
        bool isButtonReleased(Button button) const override;

        bool isButtonDown(Button button) const override {
            return isAvailable() && SDL_GameControllerGetButton(_pad, _buttons.at(button)) != 0;
        }
        bool isButtonUp(Button button) const override { return !isButtonDown(button); }

        float getAxisMovement(Axis axis) const override {
            if (!isAvailable())
                return 0.f;
            //SDL rend un Sint16 : on ramene dans [-1, 1] comme le contrat
            return SDL_GameControllerGetAxis(_pad, _axes.at(axis)) / 32767.f;
        }

        friend class SdlWindow;

    private:
        const SdlWindow &_window;
        int _index;
        SDL_GameController *_pad = nullptr;

        const std::unordered_map<IGamepad::Button, SDL_GameControllerButton> _buttons = {
            {IGamepad::BUTTON_A, SDL_CONTROLLER_BUTTON_A},
            {IGamepad::BUTTON_B, SDL_CONTROLLER_BUTTON_B},
            {IGamepad::BUTTON_X, SDL_CONTROLLER_BUTTON_X},
            {IGamepad::BUTTON_Y, SDL_CONTROLLER_BUTTON_Y},
            {IGamepad::BUTTON_LEFT_BUMPER, SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
            {IGamepad::BUTTON_RIGHT_BUMPER, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
            {IGamepad::BUTTON_BACK, SDL_CONTROLLER_BUTTON_BACK},
            {IGamepad::BUTTON_START, SDL_CONTROLLER_BUTTON_START},
            {IGamepad::BUTTON_LEFT_THUMB, SDL_CONTROLLER_BUTTON_LEFTSTICK},
            {IGamepad::BUTTON_RIGHT_THUMB, SDL_CONTROLLER_BUTTON_RIGHTSTICK},
            {IGamepad::BUTTON_DPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_UP},
            {IGamepad::BUTTON_DPAD_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
            {IGamepad::BUTTON_DPAD_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN},
            {IGamepad::BUTTON_DPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT},
        };

        const std::unordered_map<IGamepad::Axis, SDL_GameControllerAxis> _axes = {
            {IGamepad::AXIS_LEFT_X, SDL_CONTROLLER_AXIS_LEFTX},
            {IGamepad::AXIS_LEFT_Y, SDL_CONTROLLER_AXIS_LEFTY},
            {IGamepad::AXIS_RIGHT_X, SDL_CONTROLLER_AXIS_RIGHTX},
            {IGamepad::AXIS_RIGHT_Y, SDL_CONTROLLER_AXIS_RIGHTY},
            {IGamepad::AXIS_LEFT_TRIGGER, SDL_CONTROLLER_AXIS_TRIGGERLEFT},
            {IGamepad::AXIS_RIGHT_TRIGGER, SDL_CONTROLLER_AXIS_TRIGGERRIGHT},
        };
};

/** @} */

#endif /* !SDLGAMEPAD_HPP_ */
