#include"Sprite.h"
#include"ShaderProgram.h"
#include"Texture2D.h"
#include<glm/mat4x4.hpp>
#include<glm/gtc/matrix_transform.hpp>

namespace Renderer {

	Sprite::Sprite(std::shared_ptr<Texture2D> ptrTexture,
		std::string initialSubTexture,
		std::shared_ptr<ShaderProgram> ptrShaderProgram,
		const glm::vec2& position,
		const glm::vec2& size,
		const float rotation)
		: m_ptrTexture(std::move(ptrTexture))
		, m_ptrShaderProgram(std::move(ptrShaderProgram))
		, m_position(position)
		, m_size(size)
		, m_rotation(rotation)
	{
	const GLfloat vertexCoords[]{
		//   X,     Y
			0.0f,  0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,

			1.0f, 1.0f,
			1.0f,  0.0f,
			0.0f,  0.0f
		};

	auto subTexture = m_ptrTexture->getSubTexture(std::move(initialSubTexture));

		const GLfloat textureCoords[]{
		//   U,     V
			subTexture.leftBottomUV.x,  subTexture.leftBottomUV.y,
			subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
			  subTexture.rightTopUV.x, subTexture.rightTopUV.y,

			  subTexture.rightTopUV.x, subTexture.rightTopUV.y,
			  subTexture.rightTopUV.x,  subTexture.leftBottomUV.y,
			subTexture.leftBottomUV.x,  subTexture.leftBottomUV.y
		};

		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glGenBuffers(1, &m_vertexCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), &vertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glGenBuffers(1, &m_textureCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_textureCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), &textureCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	Sprite::~Sprite()
	{
		glDeleteBuffers(1, &m_vertexCoordsVBO);
		glDeleteBuffers(1, &m_textureCoordsVBO);
		glDeleteVertexArrays(1, &m_VAO);
	}

	void Sprite::render() const
	{
		m_ptrShaderProgram->use();
		glm::mat4 model(1.0f);
		/* from the back to the front */
		model = glm::translate(model, glm::vec3(m_position, 0.0f));
		model = glm::translate(model, glm::vec3(0.5f * m_size.x, 0.5f * m_size.y, 0.0f));
		model = glm::rotate(model, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3 (-0.5f * m_size.x, -0.5f * m_size.y, 0.0f));
		model = glm::scale(model, glm::vec3(m_size, 1.0f));

		glBindVertexArray(m_VAO);
		m_ptrShaderProgram->setMatrix4("modelMat", model);
		glActiveTexture(GL_TEXTURE0);
		m_ptrTexture->bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
	}
	void Sprite::setPosition(const glm::vec2& position)
	{
		m_position = position;
	}
	void Sprite::setSize(const glm::vec2& size)
	{
		m_size = size;
	}
	void Sprite::setRotation(const float rotation)
	{
		m_rotation = rotation;
	}
}