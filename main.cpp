#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class Wall{
    public:
        float width, height;
        float x, y;
    
        sf::RectangleShape shape;

        Wall(float x, float y, float width, float height){
            this-> x = x;
            this-> y = y;
            this-> width = width;
            this-> height = height;

            shape.setSize(sf::Vector2f(width, height));
            shape.setPosition(x, y);
            shape.setFillColor(sf::Color::Cyan);
            shape.setOrigin(width/2, height/2);
        }
};


struct Ray{
    sf::Vector2f start;
    sf::Vector2f end;
    bool collided = false;

    Ray (float startX, float startY, float angle, vector<Wall>& walls){
        start = {startX, startY};

        float radAngle = angle * 3.14159f / 180.0f;

        sf::Vector2f direction = {cos(radAngle), sin(radAngle)};
        float maxDistance = 1000.0f;
        end = start + direction * maxDistance;


        for (auto& wall : walls){
            sf::FloatRect wallBounds(wall.x - wall.width / 2, wall.y - wall.height / 2, wall.width, wall.height);
            if (wallBounds.contains(end)){
                collided = true;
                end = {wall.x, wall.y};
                break;
            }
        }
    }
};

class Character{
    public:
        float x, y;
        float direction = 0;
        float movementSpeed = 5;
        float rotationSpeed = 5;

        Character(float x, float y){
            this-> x = x;
            this-> y = y;
        }
};


void drawPlayer(float x, float y, float rotation);
void playerLogic();
void drawRays();
void shootRay(float playerX, float playerY, float rotation, std::vector<Wall>& walls);

int windowWidth = 800, windowHeight = 600; // Grandezza finestra
sf::RenderWindow window (sf::VideoMode(windowWidth, windowHeight), "prova raycasting");
std::vector<Ray> rays; // Lista dei raggi attivi
vector<Wall> walls; // Creazopme del vettore che contiene in muri
Character player(100, 100); // Creazione del player
Ray ray();
bool moveUp = false, moveDown = false, moveLeft = false, moveRight = false;

int main(){
    window.setFramerateLimit(60);

    walls.push_back(Wall(500, 400, 100, 150));


    while(window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){

            if (event.type == event.Closed)
                window.close();

            //---- MOVIMENTO PLAYER ----
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased){
                bool isPressed = (event.type == sf::Event::KeyPressed);
            
                switch (event.key.code){
                        case sf::Keyboard::Up:
                            moveUp = isPressed;
                            break;
                        case sf::Keyboard::Down:
                            moveDown = isPressed;
                            break;
                        case sf::Keyboard::Left:
                            moveLeft = isPressed;
                            break;
                        case sf::Keyboard::Right:
                            moveRight = isPressed;
                            break;
                        default:
                            break;
                    }
                }

            // Input pressione tasti
            if (event.type == sf::Event::KeyPressed){
                // Spara proiettile
                if (event.key.code == sf::Keyboard::Space){
                    shootRay(player.x, player.y, player.direction, walls);
                }
            }
        }
        playerLogic();


        window.clear(sf::Color::Black);

        drawPlayer(player.x, player.y, player.direction);
        
        drawRays();
        window.draw(walls[0].shape);

        window.display();
    }


    return 0;
}


void drawPlayer(float x, float y, float rotation){
    const float sqrSide = 30;
    const float headWidth = sqrSide, headHeight = sqrSide/5; 

    sf::RectangleShape sqr;
    sqr.setSize(sf::Vector2f(sqrSide, sqrSide));
    sqr.setOrigin(sqrSide/2, sqrSide/2);
    sqr.setRotation(rotation);
    sqr.setPosition(x, y);
    sqr.setFillColor(sf::Color::White);

    sf::RectangleShape head;
    head.setSize(sf::Vector2f(headWidth, headHeight));
    head.setOrigin(sqrSide/2, sqrSide/2);
    head.setRotation(rotation + 90);
    head.setPosition(x, y);
    head.setFillColor(sf::Color::Red);

    window.draw(sqr);
    window.draw(head);
}   


void playerLogic(){
    // Controlla che la rotazione stia tra -360 e 360
    if (player.direction > 360){
        player.direction = 0;
        }
        else if (player.direction < -360){
            player.direction = 0;
        }

    // Movimento
    if (moveUp){
        player.x += player.movementSpeed * cos(player.direction * 3.14159f / 180.0f);
        player.y += player.movementSpeed * sin(player.direction * 3.14159f / 180.0f);
    }
    else if (moveDown){
        player.x += -player.movementSpeed * cos(player.direction * 3.14159f / 180.0f);
        player.y += -player.movementSpeed * sin(player.direction * 3.14159f / 180.0f);
    }
    if (moveLeft){
        player.direction -= player.rotationSpeed;
    }
    else if (moveRight){
        player.direction += player.rotationSpeed;
    }
}


void drawRays(){
    for (auto& ray : rays){
        sf::VertexArray line(sf::Lines, 2);
        line[0].position = ray.start;
        line[0].color = sf::Color::Red;  // Colore del raggio
        line[1].position = ray.end;
        line[1].color = sf::Color::Red;

        window.draw(line);
    }
}


void shootRay(float playerX, float playerY, float rotation, std::vector<Wall>& walls) {
    rays.emplace_back(playerX, playerY, rotation, walls);
}