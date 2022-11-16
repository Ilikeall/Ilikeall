#include"Tank.h"
#include"../Renderer/AnimatedSprite.h"

Tank::Tank(std::shared_ptr<Renderer::AnimatedSprite> ptrSprite, const float velocity, glm::vec2& position)
	: m_eOrientation(EOrientation::Top)
	, m_ptrSprite(std::move(ptrSprite))
	, m_move(false)
	, m_velocity(velocity)
	, m_position(position)
	, m_moveOffset(glm::vec2(0.0f, 1.0f))
{
	m_ptrSprite->setPosition(m_position);
}

void Tank::render()
{
	m_ptrSprite->render();
}

void Tank::setOrientation(const EOrientation eOrientation)
{
	if (m_eOrientation == eOrientation)
	{
		return;
	}
	else
	{
		m_eOrientation = eOrientation;
		switch (m_eOrientation)
		{
		case Tank::EOrientation::Top:
			m_ptrSprite->setState("tankTopState");
			m_moveOffset.x = 0.0f;
			m_moveOffset.y = 1.0f;
			break;
		case Tank::EOrientation::Bottom:
			m_ptrSprite->setState("tankBottomState");
			m_moveOffset.x = 0.0f;
			m_moveOffset.y = -1.0f;
			break;
		case Tank::EOrientation::Left:
			m_ptrSprite->setState("tankLeftState");
			m_moveOffset.x = -1.0f;
			m_moveOffset.y = 0.0f;
			break;
		case Tank::EOrientation::Right:
			m_ptrSprite->setState("tankRightState");
			m_moveOffset.x = 1.0f;
			m_moveOffset.y = 0.0f;
			break;
		default:
			break;
		}
	}
	
}

void Tank::move(const bool move)
{
	m_move = move;
}

void Tank::update(const uint64_t delta)
{
	if (m_move)
	{
		m_position += delta * m_velocity * m_moveOffset;
		m_ptrSprite->setPosition(m_position);
		m_ptrSprite->update(delta);
	}
}