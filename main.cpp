#include <SFML/Graphics.hpp>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <cmath>

static size_t WINDOW_WIDTH = 400;
static size_t WINDOW_HEIGHT = 400;
static const int CHAR_SIZE = 20;
static size_t NUM_CHARS = WINDOW_WIDTH / CHAR_SIZE;

static const std::string CHARMAP = "abcdefghijklmnopqrstuvwxzyABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

struct FlyingChar
{
   sf::Vector2i pos;
   char character;
   sf::Color color;
   float speed;
};

typedef std::vector<std::vector<FlyingChar>> FlyingCols;

float getRandomSpeed()
{
   return 1+std::log(1+(rand()%50)*10);
}

void addLine( FlyingCols& cols )
{
   for( int i = 0; i < static_cast<int>(NUM_CHARS); ++i )
   {
      FlyingChar c;
      c.pos.x = i * CHAR_SIZE;
      c.pos.y = -CHAR_SIZE;
      c.character = CHARMAP[rand()%CHARMAP.size()];
      c.speed = getRandomSpeed();
      std::vector<FlyingChar> vec;
      vec.push_back( c );
      cols.push_back( vec );
   }
}

void addChars( FlyingCols& cols )
{
   for( int i = 0; i < cols.size(); ++i )
   {
      if( cols[i][0].pos.y > 0 )
      {
         FlyingChar c;
         c.pos.x = i * CHAR_SIZE;
         c.pos.y = -CHAR_SIZE;
         c.character = CHARMAP[rand()%CHARMAP.size()];
         c.speed = getRandomSpeed();
         c.color = sf::Color( (rand()%2)*255, (rand()%2)*255, 255, rand()%255 );
         cols[i].insert( cols[i].begin(), c );
      }
   }
}

void removeChars( FlyingCols& cols )
{
   for( int i = 0; i < cols.size(); ++i )
   {
      std::vector<FlyingChar>& chars = cols[i];
      chars.erase(
         std::remove_if( chars.begin(), chars.end(),
                         [](const FlyingChar& c) { return c.pos.y > static_cast<int>(WINDOW_HEIGHT); }
      ), chars.end() );
   }
}

int main()
{
   sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_WIDTH), "Matrix!", sf::Style::Resize);

   sf::Font font;
   if( !font.loadFromFile( "VeraBd.ttf") )
   {
      std::cerr << "Failed to load font";
      return -1;
   }

   FlyingCols cols;
   addLine( cols );

   bool stop = false;
   while (window.isOpen())
   {
      sf::Event event;
      while (window.pollEvent(event))
      {
         if (event.type == sf::Event::Closed)
            window.close();
         else if(event.type == sf::Event::KeyPressed)
             stop = true;
         else if(event.type == sf::Event::KeyReleased)
             stop = false;
         else if(event.type == sf::Event::Resized)
         {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
         }
      }

      if( WINDOW_WIDTH != window.getSize().x )
      {
         WINDOW_WIDTH = window.getSize().x;
         WINDOW_HEIGHT = window.getSize().y;
         NUM_CHARS = WINDOW_WIDTH / CHAR_SIZE;
         std::cerr << "window: " << window.getSize().x << "," << window.getSize().y << "\n";
         cols.clear();
         addLine( cols );
      }

      window.clear();

      addChars( cols );

      for( size_t i = 0; i < cols.size(); ++i )
      {
          for( size_t j = 0; j < cols[i].size(); ++j )
          {
              FlyingChar& c = cols[i][j];
              sf::Text text;
              text.setString( c.character );
              text.setCharacterSize(CHAR_SIZE-5);
              text.setFillColor(c.color);
              text.setFont( font );
              text.setPosition( c.pos.x, c.pos.y );
              window.draw(text);
              if( !stop )
              {
                 c.pos.y += c.speed;
              }
          }
      }

      removeChars( cols );

      window.display();

      usleep(1000 * 10);
   }

   return 0;
}
