#include <SFML/Graphics.hpp>
#include "SceneManager.h"
#include "LoginScreen.h"
#include<cstdlib>
#include<ctime>

int main()
{
   /*Spritesheet s(5, ".\\blue.png", sf::Vector2i(152, 117));
    s.createAnim(0, 5, sf::Vector2i(142, 0), sf::Vector2i(142, 117), 15);
    s.createAnim(1, 4, sf::Vector2i(426, 117), sf::Vector2i(142, 117), 10);
    CollisionManager m;


    sf::Texture ss;
    ss.loadFromFile(".\\level_7.png");
    sf::Sprite asd(ss);
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Baraf Bros");
    
    sf::Clock clock;


    TextureButton b(".\\play_btn0001.png", ".\\play_btn0002.png");




    // Create a circle
    sf::CircleShape circle(80.f);   // radius = 80
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(360.f, 260.f); // center-ish
    window.setFramerateLimit(30);
    sf::Clock deltaClock;
    float deltaTime = 0.0f;
    while (window.isOpen())
    {
    deltaTime = deltaClock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::W:
                    s.playAnim(0);
                    break;
                case sf::Keyboard::S:
                    s.playAnim(1);
                    break;
                }
            }
            if (event.type == sf::Event::MouseMoved) {

            }
        }

        window.clear(sf::Color::Black);
        window.draw(circle);
        window.draw(asd);
        s.update(deltaTime, window);
        //b.update(window);
        window.display();
    }

    return 0;*/
    /*LoginScreen login;
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Baraf Bros");

    sf::Clock deltaClock;
    float deltaTime = 0.0f;
    while (window.isOpen())
    {
        sf::Event event;
        deltaTime = deltaClock.restart().asSeconds();
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);
        login.update(deltaTime, window, event);
        window.display();
        
    }*/
    // main.cpp

	srand(static_cast<unsigned int>(time(0)));
        sf::RenderWindow window(sf::VideoMode(1280, 720), "Baraf Bros");
        window.setFramerateLimit(60);

        SceneManager sceneManager;
        sceneManager.changeScene(new LoginScreen());
        sceneManager.setWindow(&window);

        sf::Clock clock;

        while (window.isOpen()) {
            float dt = clock.restart().asSeconds();
            if (dt > 0.033f) dt = 0.033f;

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                sceneManager.handleEvent(event, window);
            }
			window.clear(sf::Color(0, 0, 0));
            sceneManager.update(dt);
            sceneManager.draw(window);
            window.display();
        }

        return 0;
    
}
