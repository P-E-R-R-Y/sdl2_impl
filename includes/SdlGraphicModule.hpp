/**
 * @file SdlGraphicModule.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @copyright Copyright (c) 2026
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLGRAPHIC_MODULE_HPP
#define SDLGRAPHIC_MODULE_HPP

#include "IGraphic2Module.hpp"

#include "window/SdlWindow.hpp"

#include "event/SdlGamepad.hpp"
#include "event/SdlKeyboard.hpp"
#include "event/SdlMouse.hpp"

#include "graphic/SdlFont.hpp"
#include "graphic/SdlPolygon.hpp"
#include "graphic/SdlSprite.hpp"
#include "graphic/SdlText.hpp"
#include "graphic/SdlTexture.hpp"

#include <SDL_image.h>
#include <SDL_ttf.h>

/**
 * @brief SDL2 ne remplit que IGraphic2Module.
 *
 * SDL n'a pas de 3D : il donne un contexte - OpenGL, Metal, Vulkan - et
 * s'arrete la. Il n'y a ni chargeur de glTF, ni squelette, ni camera, donc
 * pas de IGraphic3Module ici, et surtout pas de faux IModel.
 *
 * Il sait en revanche ouvrir PLUSIEURS fenetres, comme sfml : createWindow
 * en rend une nouvelle a chaque appel, et un jeu invite peut donc avoir la
 * sienne.
 */
class SdlGraphicModule : public IGraphic2Module {

public:
    SdlGraphicModule() = default;
    ~SdlGraphicModule() = default;

    const char *type() const override { return IGraphic2Module::contract; }
    const char *name() const override { return "sdl2"; }

    // window
    graphic::IWindow2 *createWindow(int32_t screenWidth, int32_t screenHeight, std::string title) override {
        if (!start())
            return nullptr;

        SdlWindow *created = new SdlWindow(screenWidth, screenHeight, title);

        if (!created->isOpen()) {
            delete created;
            return stop(), nullptr;
        }

        _opened++;
        /* On retient la premiere : c'est celle que window() pretera aux
         * invites qui ne veulent pas de la leur. */
        if (!_window)
            _window = created;
        return created;
    }

    void deleteWindow(graphic::IWindow2 *window) override {
        if (!window)
            return;
        if (window == _window)
            _window = nullptr;
        delete window;

        if (_opened > 0)
            _opened--;
        /* La derniere fermee rend SDL au systeme. La borne relache un
         * vendor avant d'en initialiser un autre : si on gardait le
         * sous-systeme video ouvert, deux vendors se disputeraient le
         * meme peripherique d'affichage. */
        if (_opened == 0)
            stop();
    }

    graphic::IWindow2 *window() override { return _window; }

    // input - la fenetre est donnee a la construction, comme une ITexture
    // l'est a une ISprite : le lien ne peut jamais manquer, et aucune
    // methode n'a a le reconcilier plus tard
    graphic::IKeyboard *createKeyboard(graphic::IWindow *window) override {
        return new SdlKeyboard(*static_cast<SdlWindow *>(window));
    }
    void deleteKeyboard(graphic::IKeyboard *keyboard) override { delete keyboard; }

    graphic::IMouse *createMouse(graphic::IWindow *window) override {
        return new SdlMouse(*static_cast<SdlWindow *>(window));
    }
    void deleteMouse(graphic::IMouse *mouse) override { delete mouse; }

    graphic::IGamepad *createGamepad(graphic::IWindow *window) override {
        return new SdlGamepad(*static_cast<SdlWindow *>(window));
    }
    void deleteGamepad(graphic::IGamepad *gamepad) override { delete gamepad; }

    // texture
    graphic::ITexture *createTexture(std::string path) override { return new SdlTexture(path); }
    void deleteTexture(graphic::ITexture *texture) override { delete texture; }

    // font
    graphic::IFont *createFont(std::string path) override { return new SdlFont(path); }
    void deleteFont(graphic::IFont *font) override { delete font; }

    // polygon
    graphic::IPolygon *createPolygon(std::vector<Vector2f> points) override { return new SdlPolygon(points); }
    void deletePolygon(graphic::IPolygon *polygon) override { delete polygon; }

    // sprite
    graphic::ISprite *createSprite(graphic::ITexture *texture) override {
        return new SdlSprite(*static_cast<SdlTexture *>(texture));
    }
    void deleteSprite(graphic::ISprite *sprite) override { delete sprite; }

    // text
    graphic::IText *createText(std::string text, graphic::IFont *font) override {
        return new SdlText(text, *static_cast<SdlFont *>(font));
    }
    void deleteText(graphic::IText *text) override { delete text; }

private:
    /**
     * @brief Ouvre SDL au premier besoin, et une seule fois.
     *
     * Rien n'est fait dans le constructeur du module : celui-ci s'execute
     * au dlopen, bien avant qu'on sache si ce vendor sera choisi. Ouvrir le
     * peripherique d'affichage a ce moment le confisquerait aux autres.
     */
    bool start() {
        if (_started)
            return true;
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
            return false;
        TTF_Init();
        IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
        _started = true;
        return true;
    }

    void stop() {
        if (!_started)
            return;
        IMG_Quit();
        TTF_Quit();
        /* QuitSubSystem et pas SDL_Quit : on ne rend QUE ce qu'on a pris.
         * SDL_Quit arrete la bibliotheque entiere - y compris le sous-systeme
         * audio ouvert par SdlAudioModule, et, sdl2-compat partageant son
         * libSDL3 avec sdl3_impl, l'etat de l'autre vendor. */
        SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
        _started = false;
    }

    /// La premiere fenetre ouverte : celle que window() prete.
    graphic::IWindow2 *_window = nullptr;

    unsigned _opened = 0;
    bool _started = false;
};

/** @} */

#endif /* !SDLGRAPHIC_MODULE_HPP */
