
#ifndef BRICK_H
#define BRICK_H


#include "GameObject.h"
class OpenGLTexture;

//Paddle class, inherits from GameObject, is controlled by the
//mouse input on the x-axis.
class Brick : public GameObject
{
public:
  Brick(float x, float y);
  ~Brick();
  
  //Implementing the pure virtual lifecycle methods from GameObject
  void update(double delta);
  void paint();
  
  //Overriding the virtual reset method from GameObject
  void reset();
  
  //Implementing the pure virtual type method from GameObject
  const char* getType();
  
  //Setter size methods
  void setSize(float width, float height);
  void setWidth(float width);
  void setHeight(float height);

  //Getter size methods
  void getSize(float &width, float &height);
  float getWidth();
  float getHeight();


  
protected:
  float m_Width;
  float m_Height;
  float m_X;
  float m_Y;
  OpenGLTexture* m_BrickTexture;
};










#endif