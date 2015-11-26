//
//  Ball.cpp
//  GAM-1532 OSX Game
//
//  Created by Bradley Flood on 2013-01-09.
//  Copyright (c) 2013 Algonquin College. All rights reserved.
//

#include "Ball.h"
#include "Game.h"
#include "Paddle.h"
#include "Brick.h"
#include "../Constants/Constants.h"
#include "../Screen Manager/ScreenManager.h"
#include "../OpenGL/OpenGL.h"




Ball::Ball() : GameObject()
{
	m_BallTexture = new OpenGLTexture("Ball");
	m_Spin = 0.f;
}

Ball::~Ball()
{
	if(m_BallTexture != NULL)
	{
		delete m_BallTexture;
		m_BallTexture = NULL;
	}
}

void Ball::update(double aDelta)
{
  //Calculate the ball's x and y position
  setX(getX() + (getDirectionX() * getSpeed() * aDelta));
  setY(getY() + (getDirectionY() * getSpeed() * aDelta));
  
    m_Spin += getSpeed() * (aDelta);
    if(m_Spin > 360.0f)
    {
        m_Spin -= 360.0f;
        
    }

  //Vertical bounds check
  if(getY() - getRadius() < 0.0f)
  {
    //There was a collision at the top, reverse the y-direction
	setY(getRadius());
	setDirectionY(getDirectionY() * -1.0f);
  }
  else if(getY() - getRadius() > ScreenManager::getInstance()->getScreenHeight())
  {
    //The ball has gone off the bottom of the screen
	 
    setIsActive(false);
	
  
    //Get the Game object from ScreenManager, and call gameOver. This will reset the game.

    Game* game = (Game*)ScreenManager::getInstance()->getScreenForName(GAME_SCREEN_NAME);
    game->gameOver();
	//reset();
	float screenWidth = ScreenManager::getInstance()->getScreenWidth();
  float screenHeight = ScreenManager::getInstance()->getScreenHeight();

  //Reset the radius
  setRadius(screenWidth * GAME_BALL_RADIUS_PERCENTAGE);
  
  //Reset the x and y position

  setX((screenWidth - getRadius()) / 2.0f);
  setY((screenHeight - getRadius()) / 2.0f);

    //Reset the direction
  setDirectionX(1.0f);
  setDirectionY(-1.0f);
  
  //Set the ball back to being active
  setIsActive(true);
	
	
  }
  
  //Horizontal bounds check
  if(getX() - getRadius() < 0.0f)
  {
    //There was a collision on the left side, reverse the x-direction
	setX(getRadius());
	setDirectionX(getDirectionX() * -1.0f);
  }
  else if(getX() + getRadius() > ScreenManager::getInstance()->getScreenWidth())
  {
    //There was a collision on the right side, reverse the x-direction
	setX(ScreenManager::getInstance()->getScreenWidth() - getRadius());
	setDirectionX(getDirectionX() * -1.0f);
  }
}

void Ball::paint()
{
  //Draw an orange ball with a white outline
  OpenGLRenderer::getInstance()->setLineWidth(2.0f);
  //OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorGreen());
  OpenGLRenderer::getInstance()->drawTexture(m_BallTexture,getX() - 20, getY() - 20,m_Spin);
  OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorRed());
  //OpenGLRenderer::getInstance()->drawCircle(getX(), getY(), getRadius(), false);
  OpenGLRenderer::getInstance()->setLineWidth(1.0f);
}

void Ball::reset()
{
  //Get the screen width and height
  float screenWidth = ScreenManager::getInstance()->getScreenWidth();
  float screenHeight = ScreenManager::getInstance()->getScreenHeight();

  //Reset the radius
  setRadius(screenWidth * GAME_BALL_RADIUS_PERCENTAGE);
  
  //Reset the x and y position
  setX((screenWidth - getRadius()) / 2.0f);
  setY((screenHeight - getRadius()) / 2.0f);
  
  //Reset the speed
  Game* game = (Game*)ScreenManager::getInstance()->getScreenForName(GAME_SCREEN_NAME);
  //if(game->getLives() == 0)
  //{
	setSpeed(GAME_BALL_DEFAULT_SPEED);
  //}
  
  
  //Reset the direction
  setDirectionX(1.0f);
  setDirectionY(-1.0f);
  
  //Set the ball back to being active
  setIsActive(true);
}

const char* Ball::getType()
{
  return GAME_BALL_TYPE;
}

void Ball::checkCollision(GameObject* aGameObject)
{
  //Make sure the ball is active, the gameObject isn't NULL and that it is also active
  if(getIsActive() == true && aGameObject != NULL && aGameObject->getIsActive() == true)
  {
    //Determine if the gameObject is a Paddle
    if(strcmp(aGameObject->getType(), GAME_PADDLE_TYPE) == 0)
    {
      handlePaddleCollision((Paddle*)aGameObject);
    }
	 if(strcmp(aGameObject->getType(), GAME_BRICK_TYPE) == 0)
    {
      handleBrickCollision((Brick*)aGameObject);
    }
  }
}

void Ball::handlePaddleCollision(Paddle* aPaddle)
{
  //Calculate the ball's distance from the paddle
  float distanceX = fabsf(getX() - aPaddle->getX() - (aPaddle->getWidth() / 2.0f));
  float distanceY = fabsf(getY() - aPaddle->getY() - (aPaddle->getHeight() / 2.0f));
  
  //If the distance on the x-axis is greater than half-the-width of the paddle + the ball's radius, then 
  //there is no way they can be colliding and return out of this method, no more collision handling is needed.
  if(distanceX > ((aPaddle->getWidth() / 2.0f) + getRadius()))
  {
    return; 
  }
  
  //If the distance on the y-axis is greater than half-the-height of the paddle + the ball's radius, then 
  //there is no way they can be colliding and return out of this method, no more collision handling is needed.
  if(distanceY > ((aPaddle->getHeight() / 2.0f) + getRadius())) 
  {
    return; 
  }
  
  //If we got here (passed the previous 2 if checks), then there is a good chance that a collision has occured.

  //If the distance on the x-axis is less than half-the-width of the paddle, then we have a collision on top of 
  //the paddle, set the ball's Y value and y-direction accordingly.
  if(distanceX <= (aPaddle->getWidth() / 2.0f))
  {
    setY(aPaddle->getY() - getRadius());
    setDirectionY(getDirectionY() * -1.0f);
    return;
  } 
  
  //If the distance on the y-axis is less than half-the-height of the paddle, then we have a collision on either 
  //side of the paddle, set the x-direction accordingly.
  if(distanceY <= (aPaddle->getHeight() / 2.0f))
  {
    setDirectionX(getDirectionX() * -1.0f);
    return;
  }
}
void Ball::handleBrickCollision(Brick* aBrick)
{
  //Calculate the ball's distance from the paddle
  float distanceX = fabsf(getX() - aBrick->getX() - (aBrick->getWidth() / 2.0f));
  float distanceY = fabsf(getY() - aBrick->getY() - (aBrick->getHeight() / 2.0f));
  
  //If the distance on the x-axis is greater than half-the-width of the paddle + the ball's radius, then 
  //there is no way they can be colliding and return out of this method, no more collision handling is needed.
  if(distanceX > ((aBrick->getWidth() / 2.0f) + getRadius()))
  {
    return; 
  }
  
  //If the distance on the y-axis is greater than half-the-height of the paddle + the ball's radius, then 
  //there is no way they can be colliding and return out of this method, no more collision handling is needed.
  if(distanceY > ((aBrick->getHeight() / 2.0f) + getRadius())) 
  {
    return; 
  }
  
  //If we got here (passed the previous 2 if checks), then there is a good chance that a collision has occured.

  //If the distance on the x-axis is less than half-the-width of the paddle, then we have a collision on top of 
  //the paddle, set the ball's Y value and y-direction accordingly.
  if(distanceX <= (aBrick->getWidth() / 2.0f))
  {

    setDirectionY(getDirectionY() * -1.0f);
	aBrick->setIsActive(false);

    return;
  } 
  
  //If the distance on the y-axis is less than half-the-height of the paddle, then we have a collision on either 
  //side of the paddle, set the x-direction accordingly.
  if(distanceY <= (aBrick->getHeight() / 2.0f))
  {

    setDirectionX(getDirectionX() * -1.0f);
	aBrick->setIsActive(false);

    return;
  }
}




void Ball::setRadius(float aRadius)
{
  m_Radius = aRadius;
}

float Ball::getRadius()
{
  return m_Radius;
}

void Ball::setSpeed(float aSpeed)
{
  m_Speed = aSpeed;
}

float Ball::getSpeed()
{
  return m_Speed;
}

void Ball::setDirection(float aDirectionX, float aDirectionY)
{
  setDirectionX(aDirectionX);
  setDirectionY(aDirectionY);
}

void Ball::setDirectionX(float aDirectionX)
{
  m_DirectionX = aDirectionX;
}

void Ball::setDirectionY(float aDirectionY)
{
  m_DirectionY = aDirectionY;
}

void Ball::getDirection(float &aDirectionX, float &aDirectionY)
{
  aDirectionX = getDirectionX();
  aDirectionY = getDirectionY();
}

float Ball::getDirectionX()
{
  return m_DirectionX;
}

float Ball::getDirectionY()
{
  return m_DirectionY;
}

