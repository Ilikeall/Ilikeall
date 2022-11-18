#pragma once
#include<glad/glad.h>
#include<glm/vec2.hpp>
#include"VertexBuffer.h"
#include"IndexBuffer.h"

#include<memory>
#include<string>

namespace Renderer {
	class Texture2D;
	class ShaderProgram;

	class Sprite
	{public:

		Sprite(std::shared_ptr<Texture2D> ptrTexture,
			std::string initialSubTexture,
			std::shared_ptr<ShaderProgram> ptrShaderProgram,
			const glm::vec2& position = glm::vec2 (0.0f, 0.0f),
			const glm::vec2& size = glm::vec2 (1.0f, 1.0f),
			const float rotation = 0.0f);
		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		virtual void render() const;
		void setPosition(const glm::vec2& position);
		void setSize(const glm::vec2& size);
		void setRotation(const float rotation);

	protected:
		std::shared_ptr<Texture2D> m_ptrTexture;
		std::shared_ptr<ShaderProgram> m_ptrShaderProgram;
		glm::vec2 m_position;
		glm::vec2 m_size;
		float m_rotation;
		GLuint m_VAO;
		VertexBuffer m_vertexCoordsBuffer;
		VertexBuffer m_textureCoordsBuffer;
		IndexBuffer m_indexBuffer;
	};
}