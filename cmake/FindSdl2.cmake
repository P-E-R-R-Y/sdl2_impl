# SDL2 et ses trois satellites, par pkg-config.
#
# Pas de FetchContent ici, contrairement a sfml : SDL se compile contre les
# frameworks du systeme (Cocoa, CoreAudio, Metal) et l'installation par le
# gestionnaire de paquets est celle que les satellites - ttf, image, mixer -
# ont deja trouvee. Les rebatir separement donnerait deux SDL dans le meme
# processus, chacun avec son etat global.
#
#   brew install sdl2 sdl2_ttf sdl2_image sdl2_mixer
#   apt  install libsdl2-dev libsdl2-{ttf,image,mixer}-dev

set(name sdl2)

if (NOT TARGET Sdl2::All)
  find_package(PkgConfig REQUIRED)

  pkg_check_modules(SDL2       REQUIRED IMPORTED_TARGET sdl2)
  pkg_check_modules(SDL2_TTF   REQUIRED IMPORTED_TARGET SDL2_ttf)
  pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
  pkg_check_modules(SDL2_MIXER REQUIRED IMPORTED_TARGET SDL2_mixer)

  add_library(Sdl2::All INTERFACE IMPORTED)
  set_target_properties(Sdl2::All PROPERTIES
    INTERFACE_LINK_LIBRARIES "PkgConfig::SDL2;PkgConfig::SDL2_TTF;PkgConfig::SDL2_IMAGE;PkgConfig::SDL2_MIXER"
    # sans ca, SDL.h remplace main() par SDL_main() et le point d'entree
    # d'un test ou d'un executable hote ne se lie plus
    INTERFACE_COMPILE_DEFINITIONS "SDL_MAIN_HANDLED"
  )
endif()
