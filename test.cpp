#include "catch_amalgamated.hpp"
#include "snake.hpp"

TEST_CASE( "Snake tests", "[snake]" ) {
    Snakey::setup();

    SECTION( "Test isTailHere function" ){
        Snakey::xCoords.push_back(9);
        Snakey::xCoords.push_back(8);
        Snakey::xCoords.push_back(7);
        Snakey::xCoords.push_back(6);
        Snakey::yCoords.push_back(10);
        Snakey::yCoords.push_back(10);
        Snakey::yCoords.push_back(10);
        Snakey::yCoords.push_back(10);
        Snakey::tailSize = 4;

        //passed pos is tail end
        REQUIRE( Snakey::isTailHere(6,10) == true );

        //passed pos is head
        REQUIRE( Snakey::isTailHere(Snakey::headX,Snakey::headY) == false );

        //passed pos is tail body
        REQUIRE( Snakey::isTailHere(8,10) == true );

        //passed pos is empty position
        REQUIRE( Snakey::isTailHere(5,10) == false );
    }
    
    SECTION( "Test foodGen function" ){
        srand(1);
        Snakey::foodGen();
        
        REQUIRE( Snakey::foodX != Snakey::headX );
        REQUIRE( Snakey::foodY != Snakey::headY );
    }
    
    SECTION( "Test hitBorder function" ){
        Snakey::headX = Snakey::width;
        //right border is hit
        Snakey::hitBorder();
        REQUIRE( Snakey::headX == 2 );

        Snakey::headX = 1;
        //left border is hit
        Snakey::hitBorder();
        REQUIRE( Snakey::headX == (Snakey::width - 1) );

        Snakey::headY = 1;
        //top border is hit
        Snakey::hitBorder();
        REQUIRE( Snakey::headY == (Snakey::height - 1) );

        Snakey::headY = Snakey::height;
        //bottom border is hit
        Snakey::hitBorder();
        REQUIRE( Snakey::headY == 2 );
    }

    SECTION( "Assert gameover when tail end is hit" ){
        Snakey::xCoords.push_back(9);
        Snakey::xCoords.push_back(9);
        Snakey::xCoords.push_back(10);
        Snakey::xCoords.push_back(10);
        Snakey::yCoords.push_back(10);
        Snakey::yCoords.push_back(11);
        Snakey::yCoords.push_back(11);
        Snakey::yCoords.push_back(10);
        Snakey::tailSize = 4;
        Snakey::dir = Snakey::down;
        Snakey::logic();
        
        REQUIRE( Snakey::isTailHere(Snakey::headX, Snakey::headY) == true );
        REQUIRE( Snakey::gameOver == true );
    }

    SECTION( "Assert gameover when body is hit" ){
        Snakey::xCoords.push_back(9);
        Snakey::xCoords.push_back(9);
        Snakey::xCoords.push_back(10);
        Snakey::xCoords.push_back(10);
        Snakey::xCoords.push_back(10);
        Snakey::yCoords.push_back(10);
        Snakey::yCoords.push_back(11);
        Snakey::yCoords.push_back(11);
        Snakey::yCoords.push_back(10);
        Snakey::yCoords.push_back(9);
        Snakey::tailSize = 5;
        Snakey::dir = Snakey::down;
        Snakey::logic();
        
        REQUIRE( Snakey::isTailHere(Snakey::headX, Snakey::headY) == true );
        REQUIRE( Snakey::gameOver == true );
    }

    SECTION( "Assert tail grows when food is ate" ){
        REQUIRE( Snakey::tailSize == 0 );

        Snakey::foodX = 9;
        Snakey::foodY = 10;
        Snakey::headX = 9;
        Snakey::headY = 10;
        Snakey::dir = Snakey::righty;
        Snakey::logic();

        REQUIRE( Snakey::tailSize == 1 );
        REQUIRE( Snakey::xCoords.size() == 2 );
        REQUIRE( Snakey::yCoords.size() == 2 );
    }

}
