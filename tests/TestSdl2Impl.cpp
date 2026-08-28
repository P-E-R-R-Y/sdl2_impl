#include <gtest/gtest.h>

#include "IModuleManager.hpp"
#include "IGraphic2Module.hpp"
#include "IGraphic3Module.hpp"
#include "IAudioModule.hpp"

#include <string>

struct Sdl2ImplTest : ::testing::Test {
    IModuleManager modules;
};

TEST_F(Sdl2ImplTest, LoadsGraphic2AndAudioButNotGraphic3) {
    ASSERT_TRUE(modules.Load(SDL2_IMPL_PATH, "sdl2"));

    IGraphic2Module *graphic2 = modules.Get<IGraphic2Module>("sdl2");
    IGraphic3Module *graphic3 = modules.Get<IGraphic3Module>("sdl2");
    IAudioModule *audioModule = modules.Get<IAudioModule>("sdl2");

    ASSERT_NE(graphic2, nullptr);
    ASSERT_NE(audioModule, nullptr);
    EXPECT_STREQ(graphic2->name(), "sdl2");

    // pas de 3D : une couverture partielle n'est pas une erreur, juste un nullptr
    EXPECT_EQ(graphic3, nullptr);
}

TEST_F(Sdl2ImplTest, FullWalkthrough) {
    ASSERT_TRUE(modules.Load(SDL2_IMPL_PATH, "sdl2"));
    IGraphic2Module *graphic = modules.Get<IGraphic2Module>("sdl2");
    IAudioModule *audio = modules.Get<IAudioModule>("sdl2");
    ASSERT_NE(graphic, nullptr);
    ASSERT_NE(audio, nullptr);

    graphic::IWindow2 *window = graphic->createWindow(200, 200, "sdl2_impl test");
    ASSERT_NE(window, nullptr);
    window->setFrameLimit(60);

    // texture/font sont partagees, independantes des sprites/textes batis dessus
    graphic::ITexture *texture = graphic->createTexture(std::string(ASSETS_DIR) + "/image.png");
    graphic::IFont *font = graphic->createFont(std::string(ASSETS_DIR) + "/font.ttf");
    ASSERT_NE(texture, nullptr);
    ASSERT_NE(font, nullptr);
    EXPECT_TRUE(texture->isReady());
    EXPECT_TRUE(font->isReady());

    graphic::ISprite *sprite = graphic->createSprite(texture);
    graphic::IText *text = graphic->createText("sdl2", font);
    graphic::IPolygon *polygon = graphic->createPolygon({{0, 0}, {60, 0}, {60, 40}, {0, 40}});
    ASSERT_NE(sprite, nullptr);
    ASSERT_NE(text, nullptr);
    ASSERT_NE(polygon, nullptr);

    // le rectangle donne deux triangles : le decoupage en oreilles a tourne
    EXPECT_TRUE(polygon->isReady());
    EXPECT_EQ(polygon->getPoints().size(), 4u);

    graphic::IKeyboard *keyboard = graphic->createKeyboard(window);
    graphic::IMouse *mouse = graphic->createMouse(window);
    ASSERT_NE(keyboard, nullptr);
    ASSERT_NE(mouse, nullptr);

    // une frame complete : rien ne doit exiger d'evenement pour tourner
    window->pollEvent();
    window->beginDraw();
    window->drawPoly(polygon);
    window->drawSprite(sprite);
    window->drawText(text);
    window->endDraw();
    EXPECT_TRUE(window->isOpen());

    // le son passe par le meme vendor, mais par un module independant
    audio::ISoundBuffer *buffer = audio->createSoundBuffer(std::string(ASSETS_DIR) + "/step.wav");
    ASSERT_NE(buffer, nullptr);
    audio::ISound *sound = audio->createSound(buffer);
    ASSERT_NE(sound, nullptr);
    sound->setVolume(0.f);
    sound->play();
    sound->stop();

    audio->deleteSound(sound);
    audio->deleteSoundBuffer(buffer);

    graphic->deletePolygon(polygon);
    graphic->deleteText(text);
    graphic->deleteSprite(sprite);
    graphic->deleteFont(font);
    graphic->deleteTexture(texture);
    graphic->deleteMouse(mouse);
    graphic->deleteKeyboard(keyboard);
    graphic->deleteWindow(window);
}

TEST_F(Sdl2ImplTest, OpensASecondWindow) {
    ASSERT_TRUE(modules.Load(SDL2_IMPL_PATH, "sdl2"));
    IGraphic2Module *graphic = modules.Get<IGraphic2Module>("sdl2");
    ASSERT_NE(graphic, nullptr);

    // la ou raylib rend nullptr : SDL en ouvre autant qu'on veut, et chacune
    // a sa propre file d'evenements grace a l'aiguillage par windowID
    graphic::IWindow2 *first = graphic->createWindow(160, 120, "first");
    graphic::IWindow2 *second = graphic->createWindow(160, 120, "second");

    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_NE(first, second);

    // window() prete la premiere, celle que les invites empruntent
    EXPECT_EQ(graphic->window(), first);

    graphic->deleteWindow(second);
    graphic->deleteWindow(first);
}

/**
 * @brief L'inverse de la chaine accepts : la 2D ne fournit pas la 3D.
 *
 * IGraphic3Module::accepts ne contient que "graphic3", donc un vendor qui
 * declare "graphic2" reste invisible a une demande de 3D. Une couverture
 * partielle est un nullptr, pas une erreur.
 */
TEST_F(Sdl2ImplTest, AGraphic2VendorNeverAnswersAGraphic3Request) {
    ASSERT_TRUE(modules.Load(SDL2_IMPL_PATH, "sdl2"));

    ASSERT_NE(modules.Get<IGraphic2Module>("sdl2"), nullptr);
    EXPECT_EQ(modules.Get<IGraphic3Module>("sdl2"), nullptr);

    EXPECT_EQ(modules.GetAllByType<IGraphic2Module>().size(), 1u);
    EXPECT_TRUE(modules.GetAllByType<IGraphic3Module>().empty());
}
