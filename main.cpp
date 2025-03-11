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
void drawGrid();

int windowWidth = 800, windowHeight = 600; // Grandezza finestra
sf::RenderWindow window (sf::VideoMode(windowWidth, windowHeight), "prova raycasting");
vector<Wall> walls; // Creazopme del vettore che contiene in muri
Character player(100, 100); // Creazione del player
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
                    //spara
                }
            }
        }
        playerLogic();


        window.clear(sf::Color::Black);
        drawGrid();

        drawPlayer(player.x, player.y, player.direction);
        window.draw(walls[0].shape);
        //drawRays();


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


void drawGrid(){
    float sqrSide = 40;

    int widthGridNumber = windowWidth/sqrSide;
    for (int n = 0; n < widthGridNumber; n++){
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(n*sqrSide, 0), sf::Color(128,128,128)),
            sf::Vertex(sf::Vector2f(n*sqrSide,windowHeight), sf::Color(128,128,128))
        };
        window.draw(line, 2, sf::Lines);
    }

    int heightGridNumber = windowHeight/sqrSide;
    for (int n = 0; n < heightGridNumber; n++){
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(0, n*sqrSide), sf::Color(128,128,128)),
            sf::Vertex(sf::Vector2f(windowWidth, n*sqrSide), sf::Color(128,128,128))
        };
        window.draw(line, 2, sf::Lines);
    }
}
