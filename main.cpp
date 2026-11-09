#include <iostream>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

FloatRect hitbox(const RectangleShape& s) {
    Vector2f size = s.getSize();
    Vector2f scale = s.getScale();
    Vector2f p = s.getPosition();
    Vector2f o = s.getOrigin();
    return FloatRect({p.x - o.x * scale.x, p.y - o.y * scale.y}, {size.x * scale.x, size.y * scale.y});
}

bool hitsAnyWall(const RectangleShape& s, const vector<RectangleShape>& walls) {
    FloatRect box = hitbox(s);
    for (const auto& w : walls) {
        if (box.findIntersection(hitbox(w))) return true;
    }
    return false;
}

int main () {
    
    RenderWindow window(VideoMode({200,200}), "Boo");
    RectangleShape character({20.f, 20.f});
    character.setFillColor(Color::Green);
    character.setOrigin({10.f, 10.f});
    character.setPosition({10.f, 10.f});

    vector<RectangleShape> walls;

    RectangleShape wall1({30.f, 30.f});
    wall1.setOrigin({15.f, 15.f});
    wall1.setPosition({50.f, 50.f});
    walls.push_back(wall1);

    RectangleShape wall2({30.f, 30.f});
    wall2.setOrigin({15.f, 15.f});
    wall2.setPosition({150.f, 150.f});
    walls.push_back(wall2);

    Clock clock;
    const float speed = 100.f;

    while (window.isOpen()) {
        const float dt = clock.restart().asSeconds();

        while (const optional event = window.pollEvent()) {
            if (event->is<Event::Closed>()) {
                window.close();
            }
                
            else if (const auto* keyPressed = event->getIf<Event::KeyPressed> ()) {
                if (keyPressed->scancode == Keyboard::Scancode::Space) {
                    character.rotate(degrees(45));
                    if (character.getFillColor() == Color::Green) {
                        character.setFillColor(Color::Blue);
                    } else {
                        character.setFillColor(Color::Green);
                    }
                }
            }
        }

        Vector2f direction{0.f, 0.f};

        if (Keyboard::isKeyPressed(Keyboard::Scan::A)) direction.x -= 1.f;
        if (Keyboard::isKeyPressed(Keyboard::Scan::D)) direction.x += 1.f;
        if (Keyboard::isKeyPressed(Keyboard::Scan::W)) direction.y -= 1.f;
        if (Keyboard::isKeyPressed(Keyboard::Scan::S)) direction.y += 1.f;


        if (direction.x != 0.f || direction.y != 0.f) {
            direction = direction.normalized();
        }

        const Vector2f move = direction * speed * dt;

        character.move({move.x, 0.f});
        if (hitsAnyWall(character, walls)) {
            character.move({-move.x, 0.f});
        }

        character.move({0.f, move.y});
        if (hitsAnyWall(character, walls)) {
            character.move({0.f, -move.y});
        }

        window.clear();

        window.draw(character);
        
        for (const auto& w: walls) window.draw(w);

        window.display();

    }

    return 0;
}