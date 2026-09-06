#pragma once
#include "Component.h"
#include "Actor.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include "Shader.h"

namespace AtomToolKit {
	class Transform
		: public AtomToolKit::Component
	{
	public:

		void SetPosition(glm::vec3 pos) noexcept;
		void Translate(glm::vec3 delta) noexcept;
		constexpr glm::vec3 GetPosition() const noexcept { return m_position; }

		void SetRotation(glm::vec3 rot);
		void SetRotation(glm::quat rot);
		void SetRotation(float pitch, float yaw, float roll) noexcept;
		constexpr glm::quat GetRotation() const noexcept { return m_rotation; }
		void Rotate(glm::vec3 delta) noexcept;
		void Rotate(float pitch, float yaw, float roll) noexcept;

		void SetScale(glm::vec3 scale) noexcept;
		void SetScale(float scale) noexcept;
		constexpr glm::vec3 GetScale() const noexcept { return m_scale; }
		void ScaleBy(glm::vec3 multiplier) noexcept;

		constexpr glm::mat4 GetModel() const noexcept {
			return model;
		}
	public:
		Transform(Shader* shader);
		void Update(float deltaTime) override;

		void UpdateModel(float deltaTime);
	private:
		glm::mat4 model;

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;

		//track if some values are changed
		bool m_isDirty;

		Shader* m_shader;
	};

}