#include "Ball.hpp"

#include <cmath>
#include <SFML/Graphics/RenderTarget.hpp>

Ball::Ball() :
  degree(rand() % 360),
  speed(static_cast<float>(rand() % 50 + 100) / 100.F) {

  sf::Color color(rand() % 255, rand() % 255, rand() % 255, 144);
  circle.setFillColor(color);
  color.a = 255;
  circle.setOutlineColor(color);
  circle.setPosition(videoMode.width / 2.F, videoMode.height / 2.F);
}

void Ball::setRadius(int r) {
  circle.setRadius(static_cast<float>(r));
  circle.setPointCount(static_cast<uintptr_t>(r) * 2);
  circle.setOutlineThickness(r / 8.F);

  r += r / 10;
  circle.setOrigin(sf::Vector2f(static_cast<float>(r), static_cast<float>(r)));
}

void Ball::update() {
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

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(circle, states);
}
