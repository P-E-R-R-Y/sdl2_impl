/**
 * @file SdlKeyboard.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLKEYBOARD_HPP_
#define SDLKEYBOARD_HPP_

//Sdl
#include <SDL.h>

//Interface
#include "IKeyboard.hpp"

#include <unordered_map>
#include <vector>

class SdlWindow;

/**
 * @brief Le clavier d'UNE fenetre.
 *
 * Rien n'est lu par SDL_GetKeyboardState() : cet etat est global au
 * processus, donc la borne et le jeu liraient la meme chose meme si le
 * focus est sur l'un des deux. Tout vient des evenements que la fenetre a
 * recus, ce qui est aussi ce que fait le vendor sfml.
 */
class SdlKeyboard : public graphic::IKeyboard {

    public:
        SdlKeyboard(const SdlWindow &window) : _window(window) {}
        ~SdlKeyboard() override = default;

        //definis dans SdlWindow.hpp, une fois SdlWindow complete
        std::vector<Keys> whichKeyDown() const override;
        bool isKeyPressed(Keys key) const override;
        bool isKeyReleased(Keys key) const override;
        bool isKeyDown(Keys key) const override;
        bool isKeyUp(Keys key) const override;

    private:
        const SdlWindow &_window;

        const std::unordered_map<graphic::IKeyboard::Keys, SDL_Scancode> _keys = {
            //line 0
            {graphic::IKeyboard::Keys::KEY_ESCAPE, SDL_SCANCODE_ESCAPE},
            {graphic::IKeyboard::Keys::KEY_F1, SDL_SCANCODE_F1},
            {graphic::IKeyboard::Keys::KEY_F2, SDL_SCANCODE_F2},
            {graphic::IKeyboard::Keys::KEY_F3, SDL_SCANCODE_F3},
            {graphic::IKeyboard::Keys::KEY_F4, SDL_SCANCODE_F4},
            {graphic::IKeyboard::Keys::KEY_F5, SDL_SCANCODE_F5},
            {graphic::IKeyboard::Keys::KEY_F6, SDL_SCANCODE_F6},
            {graphic::IKeyboard::Keys::KEY_F7, SDL_SCANCODE_F7},
            {graphic::IKeyboard::Keys::KEY_F8, SDL_SCANCODE_F8},
            {graphic::IKeyboard::Keys::KEY_F9, SDL_SCANCODE_F9},
            {graphic::IKeyboard::Keys::KEY_F10, SDL_SCANCODE_F10},
            {graphic::IKeyboard::Keys::KEY_F11, SDL_SCANCODE_F11},
            {graphic::IKeyboard::Keys::KEY_F12, SDL_SCANCODE_F12},
            //line 1
            {graphic::IKeyboard::Keys::KEY_1, SDL_SCANCODE_1},
            {graphic::IKeyboard::Keys::KEY_2, SDL_SCANCODE_2},
            {graphic::IKeyboard::Keys::KEY_3, SDL_SCANCODE_3},
            {graphic::IKeyboard::Keys::KEY_4, SDL_SCANCODE_4},
            {graphic::IKeyboard::Keys::KEY_5, SDL_SCANCODE_5},
            {graphic::IKeyboard::Keys::KEY_6, SDL_SCANCODE_6},
            {graphic::IKeyboard::Keys::KEY_7, SDL_SCANCODE_7},
            {graphic::IKeyboard::Keys::KEY_8, SDL_SCANCODE_8},
            {graphic::IKeyboard::Keys::KEY_9, SDL_SCANCODE_9},
            {graphic::IKeyboard::Keys::KEY_0, SDL_SCANCODE_0},
            {graphic::IKeyboard::Keys::KEY_MINUS, SDL_SCANCODE_MINUS},
            {graphic::IKeyboard::Keys::KEY_EQUAL, SDL_SCANCODE_EQUALS},
            {graphic::IKeyboard::Keys::KEY_BACKSPACE, SDL_SCANCODE_BACKSPACE},
            //line 2
            {graphic::IKeyboard::Keys::KEY_TAB, SDL_SCANCODE_TAB},
            {graphic::IKeyboard::Keys::KEY_Q, SDL_SCANCODE_Q},
            {graphic::IKeyboard::Keys::KEY_W, SDL_SCANCODE_W},
            {graphic::IKeyboard::Keys::KEY_E, SDL_SCANCODE_E},
            {graphic::IKeyboard::Keys::KEY_R, SDL_SCANCODE_R},
            {graphic::IKeyboard::Keys::KEY_T, SDL_SCANCODE_T},
            {graphic::IKeyboard::Keys::KEY_Y, SDL_SCANCODE_Y},
            {graphic::IKeyboard::Keys::KEY_U, SDL_SCANCODE_U},
            {graphic::IKeyboard::Keys::KEY_I, SDL_SCANCODE_I},
            {graphic::IKeyboard::Keys::KEY_O, SDL_SCANCODE_O},
            {graphic::IKeyboard::Keys::KEY_P, SDL_SCANCODE_P},
            {graphic::IKeyboard::Keys::KEY_LEFT_BRACKET, SDL_SCANCODE_LEFTBRACKET},
            {graphic::IKeyboard::Keys::KEY_RIGHT_BRACKET, SDL_SCANCODE_RIGHTBRACKET},
            //line 3
            {graphic::IKeyboard::Keys::KEY_CAPS_LOCK, SDL_SCANCODE_CAPSLOCK},
            {graphic::IKeyboard::Keys::KEY_LEFT_CONTROL, SDL_SCANCODE_LCTRL},
            {graphic::IKeyboard::Keys::KEY_A, SDL_SCANCODE_A},
            {graphic::IKeyboard::Keys::KEY_S, SDL_SCANCODE_S},
            {graphic::IKeyboard::Keys::KEY_D, SDL_SCANCODE_D},
            {graphic::IKeyboard::Keys::KEY_F, SDL_SCANCODE_F},
            {graphic::IKeyboard::Keys::KEY_G, SDL_SCANCODE_G},
            {graphic::IKeyboard::Keys::KEY_H, SDL_SCANCODE_H},
            {graphic::IKeyboard::Keys::KEY_J, SDL_SCANCODE_J},
            {graphic::IKeyboard::Keys::KEY_K, SDL_SCANCODE_K},
            {graphic::IKeyboard::Keys::KEY_L, SDL_SCANCODE_L},
            {graphic::IKeyboard::Keys::KEY_SEMICOLON, SDL_SCANCODE_SEMICOLON},
            {graphic::IKeyboard::Keys::KEY_APOSTROPHE, SDL_SCANCODE_APOSTROPHE},
            {graphic::IKeyboard::Keys::KEY_BACKTICK, SDL_SCANCODE_GRAVE},
            {graphic::IKeyboard::Keys::KEY_ENTER, SDL_SCANCODE_RETURN},
            //line 4
            {graphic::IKeyboard::Keys::KEY_LEFT_SHIFT, SDL_SCANCODE_LSHIFT},
            {graphic::IKeyboard::Keys::KEY_BACKSLASH, SDL_SCANCODE_BACKSLASH},
            {graphic::IKeyboard::Keys::KEY_Z, SDL_SCANCODE_Z},
            {graphic::IKeyboard::Keys::KEY_X, SDL_SCANCODE_X},
            {graphic::IKeyboard::Keys::KEY_C, SDL_SCANCODE_C},
            {graphic::IKeyboard::Keys::KEY_V, SDL_SCANCODE_V},
            {graphic::IKeyboard::Keys::KEY_B, SDL_SCANCODE_B},
            {graphic::IKeyboard::Keys::KEY_N, SDL_SCANCODE_N},
            {graphic::IKeyboard::Keys::KEY_M, SDL_SCANCODE_M},
            {graphic::IKeyboard::Keys::KEY_COMMA, SDL_SCANCODE_COMMA},
            {graphic::IKeyboard::Keys::KEY_PERIOD, SDL_SCANCODE_PERIOD},
            {graphic::IKeyboard::Keys::KEY_SLASH, SDL_SCANCODE_SLASH},
            {graphic::IKeyboard::Keys::KEY_RIGHT_SHIFT, SDL_SCANCODE_RSHIFT},
            //line 5
            {graphic::IKeyboard::Keys::KEY_LEFT_ALT, SDL_SCANCODE_LALT},
            {graphic::IKeyboard::Keys::KEY_LEFT_SUPER, SDL_SCANCODE_LGUI},
            {graphic::IKeyboard::Keys::KEY_SPACE, SDL_SCANCODE_SPACE},
            {graphic::IKeyboard::Keys::KEY_RIGHT_ALT, SDL_SCANCODE_RALT},
            {graphic::IKeyboard::Keys::KEY_RIGHT_SUPER, SDL_SCANCODE_RGUI},
            {graphic::IKeyboard::Keys::KEY_RIGHT_CONTROL, SDL_SCANCODE_RCTRL},
            //keypad
            {graphic::IKeyboard::Keys::KEY_PAD_0, SDL_SCANCODE_KP_0},
            {graphic::IKeyboard::Keys::KEY_PAD_1, SDL_SCANCODE_KP_1},
            {graphic::IKeyboard::Keys::KEY_PAD_2, SDL_SCANCODE_KP_2},
            {graphic::IKeyboard::Keys::KEY_PAD_3, SDL_SCANCODE_KP_3},
            {graphic::IKeyboard::Keys::KEY_PAD_4, SDL_SCANCODE_KP_4},
            {graphic::IKeyboard::Keys::KEY_PAD_5, SDL_SCANCODE_KP_5},
            {graphic::IKeyboard::Keys::KEY_PAD_6, SDL_SCANCODE_KP_6},
            {graphic::IKeyboard::Keys::KEY_PAD_7, SDL_SCANCODE_KP_7},
            {graphic::IKeyboard::Keys::KEY_PAD_8, SDL_SCANCODE_KP_8},
            {graphic::IKeyboard::Keys::KEY_PAD_9, SDL_SCANCODE_KP_9},
            {graphic::IKeyboard::Keys::KEY_PAD_MINUS, SDL_SCANCODE_KP_MINUS},
            {graphic::IKeyboard::Keys::KEY_PAD_PLUS, SDL_SCANCODE_KP_PLUS},
            {graphic::IKeyboard::Keys::KEY_PAD_DOT, SDL_SCANCODE_KP_PERIOD},
            {graphic::IKeyboard::Keys::KEY_PAD_ENTER, SDL_SCANCODE_KP_ENTER},
            //arrows
            {graphic::IKeyboard::Keys::KEY_UP, SDL_SCANCODE_UP},
            {graphic::IKeyboard::Keys::KEY_DOWN, SDL_SCANCODE_DOWN},
            {graphic::IKeyboard::Keys::KEY_LEFT, SDL_SCANCODE_LEFT},
            {graphic::IKeyboard::Keys::KEY_RIGHT, SDL_SCANCODE_RIGHT},
        };
};

/** @} */

#endif /* !SDLKEYBOARD_HPP_ */
