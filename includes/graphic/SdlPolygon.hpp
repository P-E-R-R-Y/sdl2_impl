/**
 * @file SdlPolygon.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief
 * @date 2026-08-28
 *
 * @addtogroup sdl2
 * @{
 */

#ifndef SDLPOLYGON_HPP_
#define SDLPOLYGON_HPP_

//Sdl
#include <SDL.h>

//Interface
#include "IPolygon.hpp"

//system : Triangle, Vector2f, epsilond
#include "Shape.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

/**
 * @brief Un contour quelconque, decoupe en triangles une fois pour toutes.
 *
 * SDL_RenderGeometry ne sait tracer que des triangles : le decoupage en
 * oreilles est donc obligatoire ici, exactement comme chez sfml. Il est
 * recopie plutot qu'importe - une trentaine de lignes, et le vendor n'a
 * ainsi aucune dependance de geometrie.
 */
class SdlPolygon : public graphic::IPolygon {

    public:
        SdlPolygon(std::vector<Vector2f> points) : _points(points) {
            triangulate();
            buildVertices();   //une seule fois : le dessin n'a plus qu'a decaler
        }

        ~SdlPolygon() = default;

        bool isReady() const override { return !_vertices.empty(); }

        Vector2f getPosition() const override { return _position; }
        void setPosition(Vector2f position) override { _position = position; }

        Color getColor() const override { return _color; }

        void setColor(Color color) override {
            _color = color;
            for (SDL_Vertex &vertex : _vertices)
                vertex.color = SDL_Color{color.r, color.g, color.b, color.a};
        }

        std::vector<Vector2f> getPoints() const override { return _points; }

        friend class SdlWindow;

    private:
        /**
         * @brief L'angle en B est-il saillant, pour un contour d'aire positive ?
         *
         * Le produit vectoriel de BA par BC est negatif sur un sommet convexe.
         * Nul veut dire trois points alignes : pas d'oreille a couper la.
         */
        static bool is_convex(Vector2f A, Vector2f B, Vector2f C) {
            const Vector2f BA = {A.x - B.x, A.y - B.y};
            const Vector2f BC = {C.x - B.x, C.y - B.y};
            const double cross = BA.cross(BC);

            return (std::abs(cross) < epsilond) ? false : (cross < 0);
        }

        static float polygon_area(const std::vector<Vector2f> &pts) {
            float area = 0;

            for (size_t i = 0; i < pts.size(); ++i) {
                const Vector2f &a = pts[i];
                const Vector2f &b = pts[(i + 1) % pts.size()];

                area += (a.x * b.y - b.x * a.y);
            }
            return area * 0.5f;
        }

        void triangulate() {
            std::vector<Vector2f> tmp = _points;

            if (polygon_area(tmp) < 0)
                std::reverse(tmp.begin(), tmp.end());

            while (tmp.size() >= 3) {
                bool earFound = false;

                for (size_t i = 0; i < tmp.size(); i++) {
                    Vector2f A = tmp[i];
                    Vector2f B = tmp[(i + 1) % tmp.size()];
                    Vector2f C = tmp[(i + 2) % tmp.size()];
                    Triangle<double> t = Triangle<double>{A, B, C};

                    if (is_convex(A, B, C)) {
                        bool isEar = true;

                        for (size_t j = 0; j < tmp.size(); j++) {
                            if (j == i || j == (i + 1) % tmp.size() || j == (i + 2) % tmp.size()) continue;
                            if (t.isInside(tmp[j])) {
                                isEar = false;
                                break;
                            }
                        }

                        if (isEar) {
                            _triangles.push_back(Triangle<double>{A, B, C});
                            tmp.erase(tmp.begin() + (i + 1) % tmp.size());
                            earFound = true;
                            break;
                        }
                    }
                }

                if (!earFound)
                    break;
            }
        }

        /* Sommets en coordonnees LOCALES, une seule fois. Le dessin recopie
         * dans _screen en ajoutant la position : SDL_RenderGeometry n'a pas
         * de transformation, il faut lui donner des coordonnees ecran. */
        void buildVertices() {
            const SDL_Color color{_color.r, _color.g, _color.b, _color.a};

            _vertices.reserve(_triangles.size() * 3);
            for (const Triangle<double> &t : _triangles)
                for (const Vector2f &point : {t.p1, t.p2, t.p3})
                    _vertices.push_back(SDL_Vertex{
                        SDL_FPoint{static_cast<float>(point.x), static_cast<float>(point.y)},
                        color,
                        SDL_FPoint{0.f, 0.f}});
            _screen.resize(_vertices.size());
        }

        Color _color{255, 0, 0, 255};
        Vector2f _position{0, 0};

        std::vector<Vector2f> _points;
        std::vector<Triangle<double>> _triangles;

        std::vector<SDL_Vertex> _vertices;   ///< local, construit une fois
        std::vector<SDL_Vertex> _screen;     ///< local + position, par frame
};

/** @} */

#endif /* !SDLPOLYGON_HPP_ */
