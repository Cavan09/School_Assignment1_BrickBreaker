//
//  Paddle.cpp
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-01-09.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Paddle.h"
#include "../Constants/Constants.h"
#include "../Screen Manager/ScreenManager.h"
#include "../OpenGL/OpenGL.h"


Paddle::Paddle() : GameObject()
{
	m_PaddleTexture = new OpenGLTexture("Paddle");
	reset();
}

Paddle::~Paddle()
{
	if(m_PaddleTexture != NULL)
	{
		delete m_PaddleTexture;
		m_PaddleTexture = NULL;
	}
}

void Paddle::update(double aDelta)
{

}

void Paddle::paint()
{
  //Draw the Red paddle with a white outline
  OpenGLRenderer::getInstance()->setLineWidth(2.0f);
 // OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRed());
  OpenGLRenderer::getInstance()->drawTexture(m_PaddleTexture,getX(), getY());
  OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorWhite());
  //OpenGLRenderer::getInstance()->drawRectangle(getX(), getY(), getWidth(), getHeight(), false);
  OpenGLRenderer::getInstance()->setLineWidth(1.0f);
}

void Paddle::reset()
{
  //Get the screen width and height
  float screenWidth = ScreenManager::getInstance()->getScreenWidth();
  float screenHeight = ScreenManager::getInstance()->getScreenHeight();

  //Reset the paddle's width and height
  setWidth(screenWidth * GAME_PADDLE_WIDTH_PERCENTAGE);
  setHeight(screenHeight * GAME_PADDLE_HEIGHT_PERCENTAGE);
  
  //Reset the x and y position
  setX((screenWidth - getWidth()) / 2.0f);
  setY(screenHeight * GAME_PADDLE_Y_PERCENTAGE);

  //Reset the paddle to active
  setIsActive(true);
}

const char* Paddle::getType()
{
  return GAME_PADDLE_TYPE;
}

void Paddle::setSize(float aWidth, float aHeight)
{
  setWidth(aWidth);
  setHeight(aHeight);
}

void Paddle::setWidth(float aWidth)
{
  m_Width = aWidth;
}

void Paddle::setHeight(float aHeight)
{
  m_Height = aHeight;
}

void Paddle::getSize(float &aWidth, float &aHeight)
{
  aWidth = getWidth();
  aHeight = getHeight();
}

float Paddle::getWidth()
{
  return m_Width;
}

float Paddle::getHeight()
{
  return m_Height;
}
  