#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class projectile{
    public:
        float width = 14, height = 14;
        float x, y;
        float rotation;
        float movementSpeed = 10;
        bool fired;
        bool collided = false;
        
        sf::RectangleShape shape;
    
        projectile(float x, float y, float rotation){
            this-> x = x;
            this-> y = y;
            this-> rotation = rotation;

            shape.setSize(sf::Vector2f(width, height));
            shape.setFillColor(sf::Color::Blue);
            shape.setOrigin(width/2, height/2);
        }

        void setPos(){
            shape.setPosition(x, y);
        }
};

class character{
    public:
        float x, y;
        float rotation = 0;
        float movementSpeed = 5;
        float rotationSpeed = 5;

        character(float x, float y){
            this-> x = x;
            this-> y = y;
        }
};

class wall{
    public:
        float width, height;
        float x, y;
    
        sf::RectangleShape shape;

        wall(float x, float y, float width, float height){
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

void drawPlayer(sf::RenderWindow& window, float x, float y, float rotation);
void playerLogic();
void projectileLogic();
void drawProjectiles(sf::RenderWindow& window);

int windowWidth = 800, windowHeight = 600; // Grandezza finestra
vector<projectile> projectiles; // Creazione del vettore che contiene i proiettili
vector<wall> walls; // Creazopme del vettore che contiene in muri
character player(100, 100); // Creazione del player
bool moveUp = false, moveDown = false, moveLeft = false, moveRight = false;

int main(){
    sf::RenderWindow window (sf::VideoMode(windowWidth, windowHeight), "prova raycasting");
    window.setFramerateLimit(60);

    walls.push_back(wall(500, 400, 100, 150));


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
                    int x = player.x + 15 * cos(player.rotation * 3.14159f / 180.0f);
                    int y = player.y + 15 * sin(player.rotation * 3.14159f / 180.0f);
                    projectiles.emplace_back(x, y, player.rotation); // Crea un proiettile
                }
            }
        }
        playerLogic();

        //---- LOGICA PROIETTILE ----
        if (projectiles.size() > 0)
            projectileLogic();


        window.clear(sf::Color::Black);

        drawPlayer(window, player.x, player.y, player.rotation);
        
        drawProjectiles(window);
        window.draw(walls[0].shape);

        window.display();
    }


    return 0;
}


void drawPlayer(sf::RenderWindow& window, float x, float y, float rotation){
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
    if (player.rotation > 360){
        player.rotation = 0;
        }
        else if (player.rotation < -360){
            player.rotation = 0;
        }

    // Movimento
    if (moveUp){
        player.x += player.movementSpeed * cos(player.rotation * 3.14159f / 180.0f);
        player.y += player.movementSpeed * sin(player.rotation * 3.14159f / 180.0f);
    }
    else if (moveDown){
        player.x += -player.movementSpeed * cos(player.rotation * 3.14159f / 180.0f);
        player.y += -player.movementSpeed * sin(player.rotation * 3.14159f / 180.0f);
    }
    if (moveLeft){
        player.rotation -= player.rotationSpeed;
    }
    else if (moveRight){
        player.rotation += player.rotationSpeed;
    }
}


void projectileLogic(){
    for (auto& projectile : projectiles) {
        if (!projectile.collided) {
            // Movimento del proiettile
            projectile.x += projectile.movementSpeed * cos(projectile.rotation * 3.14159f / 180.0f);
            projectile.y += projectile.movementSpeed * sin(projectile.rotation * 3.14159f / 180.0f);
            projectile.setPos();
        }
        
        // Collisione con il bordo della finestra
        if (projectile.x > windowWidth || projectile.x < 0 || projectile.y > windowHeight || projectile.y < 0){
            projectile.collided = true;
        }
        
        // Collisione con il muro
        for (auto& wall : walls){
            float wallLeft = wall.x - wall.width / 2 - projectile.width/2;
            float wallRight = wall.x + wall.width / 2 + projectile.width/2;
            float wallTop = wall.y - wall.height / 2 - projectile.height/2;
            float wallBottom = wall.y + wall.height / 2 + projectile.height/2;

            // Verifica se il proiettile si sovrappone al muro
            if (projectile.x > wallLeft && projectile.x < wallRight &&
                projectile.y > wallTop && projectile.y < wallBottom){
                projectile.collided = true;
            }
        }
    }
}


void drawProjectiles(sf::RenderWindow& window){
    for (auto& projectile : projectiles){
            window.draw(projectile.shape);
        }
}






