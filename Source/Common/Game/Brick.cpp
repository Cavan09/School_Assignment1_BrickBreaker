
#include "Game.h"
#include "Brick.h"
#include "../Constants/Constants.h"
#include "../Screen Manager/ScreenManager.h"
#include "../OpenGL/OpenGL.h"

Brick::Brick(float x, float y) : GameObject()
{
	setPosition(x,y);
	m_BrickTexture = new OpenGLTexture("Brick");
	reset();

}
Brick::~Brick()
{
	if(m_BrickTexture != NULL)
	{
		delete m_BrickTexture;
		m_BrickTexture = NULL;
	}
}
void Brick::update(double aDelta)
{

}

void Brick::paint()
{
  //Draw the Red paddle with a white outline
  OpenGLRenderer::getInstance()->setLineWidth(2.0f);
  //OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorBlack());
  OpenGLRenderer::getInstance()->drawTexture(m_BrickTexture,getX(),getY());
  //OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorBlack());
  //OpenGLRenderer::getInstance()->drawRectangle(getX(), getY(), getWidth(), getHeight(), false);
  OpenGLRenderer::getInstance()->setLineWidth(1.0f);
}

void Brick::reset()
{
  //Get the screen width and height
  float screenWidth = ScreenManager::getInstance()->getScreenWidth();
  float screenHeight = ScreenManager::getInstance()->getScreenHeight();
  m_X = 0.02f;
  m_Y = 0.02f;
  //Reset the brick's width and height
  setWidth(screenWidth * GAME_BRICK_WIDTH_PERCENTAGE);
  setHeight(screenHeight * GAME_BRICK_HEIGHT_PERCENTAGE);



  //Reset the brick to active
  setIsActive(true);
}

const char* Brick::getType()
{
  return GAME_BRICK_TYPE;
}

void Brick::setSize(float aWidth, float aHeight)
{
  setWidth(aWidth);
  setHeight(aHeight);
}

void Brick::setWidth(float aWidth)
{
  m_Width = aWidth;
}

void Brick::setHeight(float aHeight)
{
  m_Height = aHeight;
}


void Brick::getSize(float &aWidth, float &aHeight)
{
  aWidth = getWidth();
  aHeight = getHeight();
}

float Brick::getWidth()
{
  return m_Width;
}

float Brick::getHeight()
{
  return m_Height;
}
