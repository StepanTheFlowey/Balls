#include "main.hpp"

#include <iostream>
#include <Windows.h>
#include <SFML/Graphics.hpp>
#include "Ball.hpp"

#include "resource.h"

sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();

int getInt(std::string str) {
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

  for(auto& i : balls) {
    i.setRadius(radius);
  }

  HRSRC hResource = FindResourceW(NULL, MAKEINTRESOURCEW(ID_IMG1), L"IMG");
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
    for(auto& i : balls) {
      i.update();
    }

    window.clear();
    for(const auto& i : balls) {
      window.draw(i);
    }
    window.display();

    while(window.pollEvent(event)) {
      switch(event.type) {
        case sf::Event::Closed:
          window.close();
          break;
      }
    }
  }

  return EXIT_SUCCESS;
}
