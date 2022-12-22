#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <cmath>
#include <iostream>
#include <Windows.h>
#include <SFML\Graphics.hpp>

#include "resource.h"

sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();

class Ball : public sf::Drawable {
  sf::CircleShape circle;
  int degree = 0;
  float speed = 1.F;
public:

  Ball() :
    degree(rand() % 360),
    speed(static_cast<float>(rand() % 50 + 100) / 100) {

    sf::Color color(rand() % 255, rand() % 255, rand() % 255, 144);
    circle.setFillColor(color);
    color.a = 255;
    circle.setOutlineColor(color);
    circle.setPosition(videoMode.width / 2.F, videoMode.height / 2.F);
  }

  void setRadius(const int r) {
    circle.setRadius(r);
    circle.setPointCount(static_cast<size_t>(r) * 2);
    circle.setOutlineThickness(r / 8.F);
    circle.setOrigin(r + r / 10.F, r + r / 10.F);
  }

  void update() {
    if(circle.getPosition().y > videoMode.height) {
      degree = 270;
    }
    if(circle.getPosition().x < 0) {
      degree = 0;
    }
    if(circle.getPosition().y < 0) {
      degree = 90;
    }
    if(circle.getPosition().x > videoMode.width) {
      degree = 180;
    }
    degree += (rand() % 9 - 4);
    if(degree > 359) {
      degree = 0;
    }
    else if(degree < 0) {
      degree = 359;
    }
    const float rad = degree * 0.01745329251994329576923690768489F;
    circle.move(std::cosf(rad) * speed, std::sinf(rad) * speed);
  }

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(circle, states);
  }
};

const int getInt(std::string str) {
  std::cout << str << ": ";
  std::getline(std::cin, str);
  return std::stoi(str);
}

int main() {
  setlocale(LC_ALL, "Russian");
  system("title Настройка");

  const int count = getInt("Кол-во");
  const int radius = getInt("Радиус");

#ifndef DEBUG
  FreeConsole();
#endif // DEBUG

  std::vector<Ball> balls(count);

#pragma omp parallel for
  for(auto& i : balls) {
    i.setRadius(radius);
  }

  HRSRC hResource = FindResourceW(NULL, MAKEINTRESOURCEW(IDB_PNG1), L"PNG");
  if(!hResource) {
    return EXIT_FAILURE;
  }
  HGLOBAL hMemory = LoadResource(NULL, hResource);
  if(!hMemory) {
    return EXIT_FAILURE;
  }
  sf::Image icon;
  icon.loadFromMemory(LockResource(hMemory), SizeofResource(NULL, hResource));

  sf::Event event;

  sf::ContextSettings contextSettings;
  contextSettings.antialiasingLevel = 8;

  sf::RenderWindow window(videoMode, "Balls", sf::Style::Fullscreen, contextSettings);
  window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
  window.setVerticalSyncEnabled(true);
  window.setMouseCursorVisible(false);

  while(window.isOpen()) {
    while(window.pollEvent(event)) {
      switch(event.type) {
        case sf::Event::Closed:
          window.close();
          return EXIT_SUCCESS;
      }
    }

#pragma omp parallel for
    for(auto& i : balls) {
      i.update();
    }

    window.clear();
    for(const auto& i : balls) {
      window.draw(i);
    }
    window.display();
  }
}
