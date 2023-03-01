#pragma once

#include "main.hpp"
#include <SFML/Graphics/CircleShape.hpp>

class Ball : public sf::Drawable {
public:

  Ball();
  ~Ball() = default;

  void setRadius(int r);

  void update();

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
protected:

  sf::CircleShape circle;
  float speed = 1.F;
  int degree = 0;
};
