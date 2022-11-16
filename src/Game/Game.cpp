#include"Game.h"

#include"../Resources/ResourceManager.h"
#include"../Renderer/ShaderProgram.h"
#include"../Renderer/Texture2D.h"
#include"../Renderer/Sprite.h"
#include"../Renderer/AnimatedSprite.h"

#include"Tank.h"

#include<GLFW/glfw3.h>
#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<iostream>

Game::Game(const glm::vec2& windowSize)
	: m_eCurrentGameState(EGameState::Active)
	, m_windowSize(windowSize)
{
	m_keys.fill(false);

}

Game::~Game()
{

}

void Game::render()
{
	//ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
	if (m_ptrTank)
	{
		m_ptrTank->render();
	}
}

void Game::update(const uint64_t delta)
{
	//ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
	if (m_ptrTank)
	{
		if (m_keys[GLFW_KEY_UP])
		{
			m_ptrTank->setOrientation(Tank::EOrientation::Top);
			m_ptrTank->move(true);
		}
		else if (m_keys[GLFW_KEY_DOWN])
		{
			m_ptrTank->setOrientation(Tank::EOrientation::Bottom);
			m_ptrTank->move(true);
		}
		else if (m_keys[GLFW_KEY_LEFT])
		{
			m_ptrTank->setOrientation(Tank::EOrientation::Left);
			m_ptrTank->move(true);
		}
		else if (m_keys[GLFW_KEY_RIGHT])
		{
			m_ptrTank->setOrientation(Tank::EOrientation::Right);
			m_ptrTank->move(true);
		}
		else
		{
			m_ptrTank->move(false);
		}
		m_ptrTank->update(delta);
	}
}

void Game::setKey(const int key, const int action)
{
	m_keys[key] = action;
}

bool Game::init()
{
	auto ptrDefaultShaderProgram = ResourceManager::loadShaders("DefaultShader", "res/Shaders/vertex.vert", "res/Shaders/fragment.frag");
	if (!ptrDefaultShaderProgram)
	{
		std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
		return false;
	}

	auto ptrSpriteShaderProgram = ResourceManager::loadShaders("SpriteShader", "res/Shaders/vert_Sprite.vert", "res/Shaders/frag_Sprite.frag");
	if (!ptrSpriteShaderProgram)
	{
		std::cerr << "Can't create shader program: " << "SpriteShader" << std::endl;
		return false;
	}

	auto tex = ResourceManager::loadTexture("DefaultTexture", "res/textures/map_16x16.png");

	std::vector<std::string> subTextureNames = {
		"bricksBlock", "topBricks", "bottomBricks", "leftBricks", "rightBricks", "topLeftBrick", "topRightBrick", "bottomLeftBrick",
		"bottomRightBrick", "betonBlock", "topBeton", "bottomBeton", "leftBeton", "rightBeton", "topLeftBeton", "topRightBeton",
		"bottomLeftBeton", "bottomRightBeton", "water1", "water2", "water3", "trees", "ice", "wall",
		"eagle", "deadEagle", "nothing", "respawn1", "respawn2", "respawn3", "respawn4"
	};
	auto ptrTextureAtlas = ResourceManager::loadTextureAtlas(
														"DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(subTextureNames), 16, 16);

	auto ptrAnimatedSprite = ResourceManager::loadAnimatedSprite("NewAnimatedSprite",
																"DefaultTextureAtlas", "SpriteShader", 100, 100, "betonBlock");

	ptrAnimatedSprite->setPosition(glm::vec2(300, 300));
	std::vector<std::pair<std::string, uint64_t>> waterState;
	waterState.emplace_back(std::make_pair<std::string, uint64_t>("water1", 1000000000));
	waterState.emplace_back(std::make_pair<std::string, uint64_t>("water2", 1000000000));
	waterState.emplace_back(std::make_pair<std::string, uint64_t>("water3", 1000000000));

	std::vector<std::pair<std::string, uint64_t>> eagleState;
	eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1000000000));
	eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1000000000));

	ptrAnimatedSprite->insertState("waterState", std::move(waterState));
	ptrAnimatedSprite->insertState("eagleState", std::move(eagleState));

	ptrAnimatedSprite->setState("waterState");

	ptrDefaultShaderProgram->use();
	ptrDefaultShaderProgram->setInt("tex", 0);

	glm::mat4 modelMatrix_1 = glm::mat4(1.f);
	modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0.f));
	glm::mat4 modelMatrix_2 = glm::mat4(1.f);
	modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0.f));

	glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

	ptrDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

	ptrSpriteShaderProgram->use();
	ptrSpriteShaderProgram->setInt("tex", 0);
	ptrSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);



	
	std::vector<std::string> tanksSubTextureNames = {"tankTop1", "tankTop2", "tankLeft1", "tankLeft2",
												"tankBottom1", "tankBottom2", "tankRight1", "tankRight2" };

	auto ptrTanksTextureAtlas = ResourceManager::loadTextureAtlas(
		"TanksTextureAtlas", "res/textures/tanks.png", std::move(tanksSubTextureNames), 16, 16);

	auto ptrTanksAnimatedSprite = ResourceManager::loadAnimatedSprite("TanksAnimatedSprite",
		"TanksTextureAtlas", "SpriteShader", 100, 100, "tankTop1");

	std::vector<std::pair<std::string, uint64_t>> tankTopState;
	tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("tankTop1", 500000000));
	tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("tankTop2", 500000000));

	std::vector<std::pair<std::string, uint64_t>> tankLeftState;
	tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("tankLeft1", 500000000));
	tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("tankLeft2", 500000000));

	std::vector<std::pair<std::string, uint64_t>> tankBottomState;
	tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("tankBottom1", 500000000));
	tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("tankBottom2", 500000000));

	std::vector<std::pair<std::string, uint64_t>> tankRightState;
	tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("tankRight1", 500000000));
	tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("tankRight2", 500000000));

	ptrTanksAnimatedSprite->insertState("tankTopState", std::move(tankTopState));
	ptrTanksAnimatedSprite->insertState("tankLeftState", std::move(tankLeftState));
	ptrTanksAnimatedSprite->insertState("tankBottomState", std::move(tankBottomState));
	ptrTanksAnimatedSprite->insertState("tankRightState", std::move(tankRightState));

	ptrTanksAnimatedSprite->setState("tankTopState");

	m_ptrTank = std::make_unique <Tank> (ptrTanksAnimatedSprite, 0.0000001f, glm::vec2(100.0f, 100.0f));

	return true;
}