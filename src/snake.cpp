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
    std::string tileset = "H:\\Documents\\Projects\\Git Repos\\Snake\\Textures\\texture.png";
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
        if (headX == 1)
        {
            headX = width - 1;
        }
        else if (headX == width)
        {
            headX = 2;
        }
        if (headY == 1)
        {
            headY = height - 1;
        }
        else if (headY == height)
        {
            headY = 2;
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
        bool load(const std::string &tileset, sf::Vector2f tileSize)
        {
            int tv;

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

            return true;
        }

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
            switch (key)
            {
            case 'w':
                dir = up;
                break;
            case 'a':
                dir = lefty;
                break;
            case 's':
                dir = down;
                break;
            case 'd':
                dir = righty;
                break;
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

    while (true)
    {
        sf::RenderWindow window(sf::VideoMode(800, 800), "Snake Game");
        window.setVerticalSyncEnabled(true);

        TileMap map;
        map.load(tileset, sf::Vector2f(32, 32));

        setup();
        while (window.isOpen())
        {
            while (!gameOver)
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        window.close();
                    }
                }

                window.clear();
                map.load(tileset, sf::Vector2f(32, 32));
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
    }
    return 0;
}
