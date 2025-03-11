#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

bool intersect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::Vector2f &intersection);
int rayNumber = 100;

struct Wall{
    sf::Vertex side[2] = {};
    sf::Vector2f start;
    sf::Vector2f end;

    Wall(sf::Vector2f start, sf::Vector2f end){
        this-> start = start;
        this-> end = end;

        side[0] = start;
        side[1] = end;

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

Character player(100, 100); // Creazione del player

struct Ray{
    sf::Vertex rayLine[2] = {};
    sf::Vector2f start;
    sf::Vector2f end;

    Ray(sf::Vector2f start, vector<Wall> walls, int rayCont){
        this-> start = start;

        float coneAngle = 60;
        float offSet = coneAngle / rayNumber;
        float angle = player.direction - (coneAngle/2) + rayCont * offSet;
        float endX = 1000 * cos(angle * 3.14159f / 180.0f);
        float endY = 1000 * sin(angle * 3.14159f / 180.0f);
        end = sf::Vector2f(endX, endY);

        for (auto wall : walls){
            intersect(start, end, wall.start, wall.end, end);
        }
        rayLine[0] = start;
        rayLine[1] = end;
        

    }
};


void drawPlayer(float x, float y, float rotation);
void playerInput();
void playerLogic();
void drawGrid();
void drawRays();


int windowWidth = 800, windowHeight = 600; // Grandezza finestra
sf::RenderWindow window (sf::VideoMode(windowWidth, windowHeight), "prova raycasting");
sf::Event event;

vector<Wall> walls; // Creazopme del vettore che contiene in muri
vector<Ray> rays;
bool moveUp = false, moveDown = false, moveLeft = false, moveRight = false;


int main(){
    window.setFramerateLimit(60);
    walls.push_back(Wall(sf::Vector2f(100,100), sf::Vector2f(400,100)));
    


    // Loop principale
    while(window.isOpen()){
        // Gestine eventi
        while (window.pollEvent(event)){

            if (event.type == event.Closed)
                window.close();            

            playerInput();
        }

        // Logica nel loop
        playerLogic();

        rays.clear();
        for (int i = 0; i < rayNumber; i++){
            rays.push_back(Ray(sf::Vector2f(player.x,player.y), walls, i));
        }



        // Disegno a schermo
        window.clear(sf::Color::Black);
        drawGrid();

        drawPlayer(player.x, player.y, player.direction);
        drawRays();
        window.draw(walls[0].side, 2, sf::Lines);
        
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


void playerInput(){
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

    if (event.type == sf::Event::KeyPressed){
        // Spara proiettile
        if (event.key.code == sf::Keyboard::Space){
            //spara
        }
    }
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


bool intersect(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3, sf::Vector2f p4, sf::Vector2f &intersection){
    float denom = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
    if (denom == 0)
        return false; // Segmenti paralleli, nessuna intersezione

    float t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / denom;
    float u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / denom;

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        intersection.x = p1.x + t * (p2.x - p1.x);
        intersection.y = p1.y + t * (p2.y - p1.y);
        return true;
    }

    return false;
}


void drawRays()
{
    for (int i = 0; i < rays.size(); i++){
        window.draw(rays[i].rayLine, 2, sf::Lines);
    }
    
}