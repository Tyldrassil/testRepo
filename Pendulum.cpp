/**
 *
 *  This program will swing a pendulum on the screen using interpolation and smoothstep functions.
 *
 *  I have made the pendulum change color as it swing back and forth, between purple and red.
 *
 *  @author Thomas Øvsttun Ditman
 *  @file Pendulum.cpp
 *  @date 09.02.2020
 *
 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <functional>
#include <string>
#include <vector>
using namespace std;

///< Declaring constants
const float PI = 3.14159265359f;
const float TWOPI = 2.0f * PI;
const float HALFPI = PI / 2.0f;
const float WINDOW = 1500.f;
const float RADIUS = 175.f;



/**
 *  Main Program
 */
int main()
{
    //Making the window
    sf::RenderWindow window(sf::VideoMode(WINDOW, WINDOW), "Pendulum");
    
    //Starting point of pendulum
    sf::Vector2f origin = sf::Vector2f(200, 1000);
    sf::Vector2f start = origin;
    
    //Marking the middle of the pendulum for lowering the curve
    sf::Vector2f middle = sf::Vector2f(750, 1250);
    
    //End point of pendulum
    sf::Vector2f apex = sf::Vector2f(1300, 1000);
    
    //The "bar" of the pendulum
    sf::VertexArray quad(sf::Quads, 4);
    
    
    //Setting the clock (using variable time instead of t, since t will be used)
    sf::Clock clock;
    clock.restart();
    float time = 0.0f;
    float t = 0.f;
    float sign = 1.f;
    
    //The vector containing the vectors for the triangles for the circle
    vector<sf::Vertex> myTriangles;
    myTriangles.reserve(100);
    
    //Color values for purple and red
    float rs = 255.f, gs = 0.f, bs = 0.f, re = 153.f, ge = 51.f, be = 255.f;
    
    //Setting vectors for interpolating colors on the pendulum
    sf::Vector3f cStart(rs,gs,bs);
    sf::Vector3f cEnd(re,ge,be);
    
    
    /**
     *  Game Loop
     */
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        //
        // Update state
        //
        
        //Deltatime
        float dt = clock.restart().asSeconds();
        time += dt;
        
        //Cycles between 0 and 1, going at half speed.
        t = (1 - fabs(2.f * (time*0.5f - floor(time*0.5f)) - 1.f));
        
        //float t = abs(2.0 * (time - floor(time)) - 1.0f);
        //t = 0.5 * t;
        
        //Smoothstep function
        float s = t * t * (3.f - 2.f * t);
        
        //sf::Vector2f lerp = (start*(1-s))+(apex*s); two points
        
        //Color lerping for the pendulum
        sf::Vector3f clerp = cStart*(1-s)+cEnd*s;
        
        //Quadratic bezier curve
        sf::Vector2f lerp = start*((1-s)*(1-s))+2.f*middle*s*(1-s)+apex*(s*s);
        
        
        
        //float length = (sqrt((lerp.x*lerp.x)+(lerp.y*lerp.y)));
        // Tried doing the pendulum with a normalised lerp,
        //found bezier was easier
        
        //sf::Vector2f nlerp = (lerp/length)*1000.f;
        
        //Variables to help calculate the circle triangles
        float angstep = TWOPI / 20.0f;
        float rad = RADIUS;
        for (float ang = 0.0f; ang < TWOPI; ang += angstep) {
            
            // Draws a circle by drawing one triangle, then rotating cosine sine
            // Then drawing a new triangle and repeating
            myTriangles.push_back(sf::Vertex((lerp),
                                       sf::Color(clerp.x,clerp.y,clerp.z,255)));
            myTriangles.push_back(sf::Vertex(sf::Vector2f(lerp.x + rad*cos(ang),
                                                          lerp.y + rad*sin(ang))
                                      ,sf::Color(clerp.x,clerp.y,clerp.z,255)));
            
            myTriangles.push_back(sf::Vertex(sf::Vector2f
            (lerp.x + rad*cos(ang+angstep), lerp.y + rad*sin(ang+angstep))
                                    ,sf::Color(clerp.x,clerp.y,clerp.z,255)));
        }
        
        //Using quad instead of triangle for variety
        quad[1].position = sf::Vector2f(725, 10);
        quad[2].position = sf::Vector2f(775, 10);
        quad[3].position = sf::Vector2f(lerp.x+25, lerp.y);
        quad[0].position = sf::Vector2f(lerp.x-25, lerp.y);
        
        /**
         *  Rendering
         */
        window.clear();
        
        // draw area
        window.draw(quad);
        window.draw(&myTriangles[0], myTriangles.size(), sf::Triangles);
        
        cout << myTriangles.size() << '\n';
        
        myTriangles.clear();
        
        
        //Various testing outputs
    
        //
        //cout << nlerp.x << " " << nlerp.y << '\n';
        //cout << length << '\n';
        
        
        window.display();
        
    }
    
    return 0;
}
