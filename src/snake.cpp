#include "snake.hpp"
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <deque>
#include <chrono>
#include <ctime>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// comment this out if GUI is wanted
#define CONSOLE

namespace Snakey
{
    bool gameOver;
    int tailSize;
    std::deque<int> xCoords;
    std::deque<int> yCoords;
    int foodX;
    int foodY;
    int headX;
    int headY;
    int prevY;
    int prevX;
    direction dir;
    direction prevDir;
    std::chrono::duration<double> timeElapsed;
    int count;
    float pixelSize = 16;
    std::string tileset = "../../Textures/textures.png";     //relative path to exe in debug dir
    std::string gulpPath = "../../audio/gulp.ogg";
    bool pause;
    sf::SoundBuffer buffer;
    sf::Sound sound;

    sf::Texture m_tileset;

    // checks if the passed position is occupied by tail
    // only checks against tail not head
    bool isTailHere(int x, int y)
    {
        for (auto pos = 1; pos <= tailSize; pos++)
        {
            if ((xCoords[pos] == x) && (yCoords[pos] == y))
            {
                return true;
            }
        }
        return false;
    }

    void foodGen()
    {
        do
        {
            foodX = rand() % (width - 2) + 2;
            foodY = rand() % (height - 2) + 2;
        } while (isTailHere(foodX, foodY) || ((foodX == headX) && (foodY == headY)));
    }

    void hitBorder()
    {
        //hits left border
        if (headX == -1)     //was 1
        {
            headX = width - 1;
        }
        //hits right border
        else if (headX == width)
        {
            headX = 0;
        }
        //hits top border
        if (headY == -1)
        {
            headY = height - 1;
        }
        //hits bottom border
        else if (headY == height)
        {
            headY = 0;
        }
    }

    void setup()
    {
        gameOver = false;
        xCoords.clear();
        yCoords.clear();
        tailSize = 0;
        headX = 10;
        headY = 10;
        dir = stop;
        prevDir = stop;
        xCoords.push_back(headX);
        yCoords.push_back(headY);
        srand((unsigned)time(NULL));
        foodGen();
        count = 0;
    }

    class TileMap : public sf::Drawable, public sf::Transformable
    {
    public:
        sf::Vector2f tileSize;
        int tv;
        bool load(const std::string &tileset, sf::Vector2f tileSize)
        {
            // load the tileset texture
            if (!m_tileset.loadFromFile(tileset))
            {
                return false;
            }

            // resize the vertex array to fit the level size
            m_vertices.setPrimitiveType(sf::Quads);
            m_vertices.resize(width * height * 4);

            // populate the vertex array, with one quad per tile
            for (unsigned int x = 0; x < width; ++x)
            {
                for (unsigned int y = 0; y < height; ++y)
                {
                    if ((x == headX) && (y == headY))
                    {
                        //set quad to head texture
                        tv = 2;
                    }
                    else if ((x == foodX) && (y == foodY))
                    {
                        //set quad to food texture
                        tv = 1;
                    }
                    else if (isTailHere(x, y))
                    {
                        //set quad to tail texture
                        tv = 0;
                    }
                    else
                    {
                        //set quad to grid texture
                        tv = 3;
                    }

                    // get a pointer to the current tile's quad
                    sf::Vertex *quad = &m_vertices[(x + y * width) * 4];

                    // define its 4 corners
                    quad[0].position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
                    quad[1].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
                    quad[2].position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);
                    quad[3].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);

                    // define its 4 texture coordinates
                    quad[0].texCoords = sf::Vector2f(0, tv * tileSize.y);
                    quad[1].texCoords = sf::Vector2f(tileSize.x, tv * tileSize.y);
                    quad[2].texCoords = sf::Vector2f(tileSize.x, (tv + 1) * tileSize.y);
                    quad[3].texCoords = sf::Vector2f(0, (tv + 1) * tileSize.y);
                }
            }
            return true;
        }
        /*
        void update()
        {
            // populate the vertex array, with one quad per tile
            for (unsigned int x = 0; x < width; ++x)
                for (unsigned int y = 0; y < height; ++y)
                {
                    if ((x == headX) && (y == headY))
                    {
                        tv = 2;
                    }
                    else if ((x == foodX) && (y == foodY))
                    {
                        tv = 0;
                    }
                    else if (isTailHere(x, y))
                    {
                        tv = 3;
                    }
                    else
                    {
                        tv = 1;
                    }

                    // get a pointer to the current tile's quad
                    sf::Vertex *quad = &m_vertices[(x + y * width) * 4];

                    // define its 4 corners
                    quad[0].position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
                    quad[1].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
                    quad[2].position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);
                    quad[3].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);

                    // define its 4 texture coordinates
                    quad[0].texCoords = sf::Vector2f(0, tv * tileSize.y);
                    quad[1].texCoords = sf::Vector2f(tileSize.x, tv * tileSize.y);
                    quad[2].texCoords = sf::Vector2f(tileSize.x, (tv + 1) * tileSize.y);
                    quad[3].texCoords = sf::Vector2f(0, (tv + 1) * tileSize.y);
                }
        }
*/

    private:
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const
        {
            // apply the transform
            states.transform *= getTransform();

            // apply the tileset texture
            states.texture = &m_tileset;

            // draw the vertex array
            target.draw(m_vertices, states);
        }

        sf::VertexArray m_vertices;
        sf::Texture m_tileset;
    };

    void input()
    {
        if (_kbhit())
        {
            char key = _getch();
            //need to call _getch() again for key code if first call is 0 or 0xE0, key is a function or arrow key
            if((key == 0) || (key == 0xE0))
            {
                key = _getch();
            }
            if(pause)
            {
                if(key == 27)
                {
                    dir = prevDir;
                    pause = false;
                }
            }
            else
            {
                switch (key)
                {
                case 'w':
                case 72:
                //w or up arrow
                    dir = up;
                    break;
                case 'a':
                case 75:
                //a or left arrow
                    dir = lefty;
                    break;
                case 's':
                case 80:
                //s or down arrow
                    dir = down;
                    break;
                case 'd':
                case 77:
                //d key or right arrow
                    dir = righty;
                    break; 
                case 27:
                //esc key was pressed to pause game
                    dir = stop;
                    pause = true;
                }
            }
            if (dir != stop)
            {
                count += 1;
                if (count == 1)
                {
                    prevDir = dir;
                }
            }
        }
    }

    void logic()
    {
        // is move backwards and tail greater than 1? then move in prevdir
        if ((dir == (-1 * prevDir)) && (tailSize > 0))
        {
            dir = prevDir;
        }

        switch (dir)
        {
        case up:
            headY -= 1;
            break;
        case down:
            headY += 1;
            break;
        case lefty:
            headX -= 1;
            break;
        case righty:
            headX += 1;
            break;
        }

        if (dir != stop)
        {
            // check if you hit the border
            hitBorder();
            xCoords.push_front(headX);
            yCoords.push_front(headY);

            // check if you ate food
            if ((headX == foodX) && (headY == foodY))
            {
                sound.play();
                tailSize += 1;
                foodGen();
            }

            // you just moved...thats it
            else
            {
                prevX = xCoords.back();
                prevY = yCoords.back();
                xCoords.pop_back();
                yCoords.pop_back();
            }

            if (isTailHere(headX, headY))
            {
                gameOver = true;
            }

            prevDir = dir;
        }
    }
}

int main()
{
    using namespace Snakey;

    sf::RenderWindow window(sf::VideoMode((width * pixelSize), (height * pixelSize)), "Snake Game");
    window.setVerticalSyncEnabled(true);

    TileMap map;
    if (!map.load(tileset, sf::Vector2f(pixelSize, pixelSize)))
    {
        gameOver = true;
    }

    if(!buffer.loadFromFile(gulpPath))
    {
        //return -1;
        std::cout << "Audio didn't load" << std::endl;
    }
    sound.setBuffer(buffer);

    while (true)
    {
        setup();
        while (window.isOpen() && !gameOver)
        {
            sf::Event event;
            while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        //close window and terminate program
                        window.close();
                        return 0;
                    }
                }

            window.clear();
            map.load(tileset, sf::Vector2f(pixelSize, pixelSize));
            window.draw(map);
            window.display();

            auto start = std::chrono::system_clock::now();

            do
                {
                    input();
                    auto end = std::chrono::system_clock::now();
                    timeElapsed = end - start;
            } while (timeElapsed.count() < timeDelay);

            logic();
        }
    }
    return 0;
}
