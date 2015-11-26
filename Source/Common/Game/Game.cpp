/* 
Game Development Level 3
Cavan MacPhail
Last Updated: October 3rd 2013
Section 4

*/



#include "Game.h"
#include "GameObject.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "../OpenGL/OpenGL.h"
#include "../Constants/Constants.h"
#include "..//Utils/Utils.h"
#include "../Screen Manager/ScreenManager.h"


Game::Game()
{

	//Create a new paddle and ball
	//Create all my bricks here!
	// calling screen manager to use percentage constants instead of pixel amounts
	//Initalizing all my meber variables
	float screenWidth = ScreenManager::getInstance()->getScreenWidth();
	float screenHeight = ScreenManager::getInstance()->getScreenHeight();
	addGameObject(new Paddle());
	addGameObject(new Ball());
	float x = screenWidth* GAME_BRICK_X_PERCENTAGE;
	float y = screenHeight* GAME_BRICK_Y_PERCENTAGE;
	m_GameOverTimer = 0.0f;
	m_NumberOfBricks = NUMBER_OF_BRICKS;
	m_NewGame = 0;
	m_SpeedUp = 0;
	m_Win = GAME_DEFAULT_WINS;
	m_Lives = GAME_DEFAULT_LIVES;
	m_CheckGameWin = false;
	m_GameOverCheck = false;
	m_GameTexture = new OpenGLTexture("GameTexture");
	m_GameOver = new OpenGLTexture("gameover");
	m_Heart = new OpenGLTexture("Heart");
	m_GameWin = new OpenGLTexture("GameWin");
	//Creating all my brick in a loop
	//incramenting the x with the width of a brick
	//incramenting the Y when modulus returns no remainder (Meaning I have a full row of 14 bricks)
	for(int i = 1; i <= m_NumberOfBricks ; i++)
	{
		Brick* abrick = new Brick(x,y);
		addGameObject(abrick);
		if(i % 14 > 0)
		{
			x += abrick->getWidth();
		}
		else if( i % 14 == 0)
		{
			x = screenWidth* GAME_BRICK_X_PERCENTAGE;
			y += abrick->getHeight();
		}
	}
	// call reset to draw and set eveything to where it should be when the game starts
	reset();
}

Game::~Game()
{
	// delete all my bricks here
	//Delete all the GameObject's in the vector
	//Deteling textures as well.
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		delete m_GameObjects.at(i);
		m_GameObjects.at(i) = NULL;
	}
	if(m_GameTexture != NULL)
	{
		delete m_GameTexture;
		m_GameTexture = NULL;
	}
	if(m_Heart != NULL)
	{
		delete m_Heart;
		m_Heart = NULL;
	}
	if(m_GameOver != NULL)
	{
		delete m_GameOver;
		m_GameOver = NULL;
	}
	if(m_GameWin != NULL)
	{
		delete m_GameWin;
		m_GameWin = NULL;
	}
	//Clear the pointers from the vector
	m_GameObjects.clear();
}


void Game::update(double aDelta)
{
	//If the GameOver Timer is greater that zero, countdown
	if(m_GameOverTimer > 0.0)
	{
		m_GameOverTimer -= aDelta;
		if(m_GameOverTimer < 0.0)
		{
			m_GameOverTimer = 0.0;
			reset();
		}
		return;
	}

	//Get the ball GameObject, we'll use this for collision detection
	Ball* ball = (Ball*)getGameObjectByType(GAME_BALL_TYPE); //constant!!!
	//Cycle through all the game objects update them and check their collision detection

	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		//Make sure the GameObject is active
		if(m_GameObjects.at(i)->getIsActive() == true)
		{
			//Update the GameObject
			m_GameObjects.at(i)->update(aDelta);


			//Check collision detection against the ball
			if(m_GameObjects.at(i) != ball)
			{
				ball->checkCollision(m_GameObjects.at(i));

			}

		}
	}
	//if my CheckForBricks() function returns true, all my bricks have been cleared
	//this means the end of a level so I need to reset everything
	//I also incrament my wins for the next level, incrament my lives and incrament my speed
	//as long as I haven't finished the fifth game. also reset before the incraments to not set the ball back to default
	// call my gameover if more than 5 wins has occured
	if(checkForBricks() == true)
	{
		m_Win--;
		if(m_Win >= 1)
		{	
			reset();
			
			m_Lives++;
			m_SpeedUp += 200;
			ball->setSpeed(ball->getSpeed() + m_SpeedUp);
		}
		else
		{
			gameOver();
		}

	}

}

void Game::paint()
{
	//a local variable to incrament my lives from the previous lives
	float space = 0.02f;
	//if my GameOverCheck returns true and my Wins are at max, I have won and the winning screen is displayed.
	if( m_CheckGameWin == true)
	{
		OpenGLRenderer::getInstance()->drawTexture(m_GameWin,0.0f,0.0f);
		return;
	}
	//If it's only gameovercheck true then I have lost all my lives and need to call the regular game over screen
	if ( m_GameOverCheck == true)
	{
		OpenGLRenderer::getInstance()->drawTexture(m_GameOver,0.0f,0.0f);
		return;
	}
	// regular background texture drawn
	OpenGLRenderer::getInstance()->drawTexture(m_GameTexture,0.0f,0.0f);
	//My life texture, Loops through in accordance to how many lives I currently have.
	//this makes it so it will draw however many lives I currently have
	for(int i = 0; i < m_Lives; i++)
	{

		OpenGLRenderer::getInstance()->drawTexture(m_Heart,getWidth() * 0.01 + space,getHeight() * 0.94);
		space += 30.0f;
	}



	//Cycle through and draw all the game objects
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		if(m_GameObjects.at(i)->getIsActive() == true)
		{
			// ****if the brick has collided maybe I want to set the activeobject in the game object base class to false so it wont paint****
			m_GameObjects.at(i)->paint();
		}
	}

	//Draw the outer white walls
	OpenGLRenderer::getInstance()->setForegroundColor(OpenGLColorBlack());
	OpenGLRenderer::getInstance()->setLineWidth(4.0f);
	OpenGLRenderer::getInstance()->drawLine(1.0f, 0.0f, 1.0f, getHeight());
	OpenGLRenderer::getInstance()->drawLine(0.0f, 1.0f, getWidth(), 1.0f);
	OpenGLRenderer::getInstance()->drawLine(getWidth() - 1, 0.0f, getWidth() - 1, getHeight());
	OpenGLRenderer::getInstance()->setLineWidth(1.0f);
}

void Game::reset()
{
	//Cycle through and reset all the game objects
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		m_GameObjects.at(i)->reset();
	}
	//Reset the game over timer to zero
	m_GameOverTimer = 0.0;
}

void Game::gameOver()
{
	//gameOver is called in ball whenever the ball goes off screen so this handles my lives
	//so long as I have more than 0 lives it will only decrament them.
	//If i am at 0 my member variables get reset for a new game, and my GameOverCheck gets set to true
	
	if(m_Lives <= 1)
	{
		m_Win = GAME_DEFAULT_WINS;
		m_Lives = GAME_DEFAULT_LIVES;
		m_GameOverCheck = true;
		m_GameOverTimer = GAME_OVER_TIMER;

	}
	m_Lives -= 1;
	//if I end up winning set my gameovercheck to true to load my winning gameover screen
	if(m_Win <= 0)
	{
		m_CheckGameWin = true;
		m_GameOverTimer = GAME_OVER_TIMER;
	}
}

const char* Game::getName()
{
	return GAME_SCREEN_NAME;
}

void Game::screenWillAppear()
{
	OpenGLRenderer::getInstance()->setBackgroundColor(OpenGLColorCornflowerBlue());
}

void Game::addGameObject(GameObject* aGameObject)
{
	if(aGameObject != NULL)
	{
		m_GameObjects.push_back(aGameObject);
	}
}

GameObject* Game::getGameObjectByType(const char* aType)
{
	//Cycle through a find the game object (if it exists)
	for(unsigned int i = 0; i < m_GameObjects.size(); i++)
	{
		if(strcmp(m_GameObjects.at(i)->getType(), aType) == 0)
		{
			return m_GameObjects.at(i);
		}
	}
	return NULL;
}

void Game::mouseMovementEvent(float aDeltaX, float aDeltaY, float aPositionX, float aPositionY)
{
	//Set the paddle to the x position of the mouse
	Paddle* paddle = (Paddle*)getGameObjectByType(GAME_PADDLE_TYPE);

	//Safety check, paddle could be NULL
	if(paddle != NULL)
	{
		paddle->setX(aPositionX - (paddle->getWidth() / 2.0f));
	}
}

void Game::keyUpEvent(int aKeyCode)
{
	//The if makes my R key only work at the game over screen so people can't just rage quit and start of cause they suck.
	//So the gameovercheck forces the keycode to only work when in a gameover screen
	//all my member variables get set for a new game as well
	// resets everything to be drawn as well
	if(aKeyCode == KEYCODE_R && m_GameOverCheck == true || m_CheckGameWin == true)
	{
		m_CheckGameWin = false;
		m_GameOverCheck = false;
		m_Lives = GAME_DEFAULT_LIVES;
		m_SpeedUp = 0;
		m_Win = GAME_DEFAULT_WINS;
		reset();
	}
}
//***Brad is a boss and gave me the idea on this function which makes everything work nice***
//it just checks for the gameObject vector for the Brick type
// Once they all are set to false which equals my number of bricks
//they are all inactive and need to be redrawn
//So i return true for some logic that takes place earlier
bool Game::checkForBricks()
{
	m_NewGame = 0;
	for(int i = 0; i < m_GameObjects.size(); i++)
	{
		if(getGameObjectByType(GAME_BRICK_TYPE))
		{
			if(m_GameObjects.at(i)->getIsActive() == false)
			{
				m_NewGame++;
				if(m_NewGame >= m_NumberOfBricks)
				{
					return true;
				}
			}
		}
	}
}
