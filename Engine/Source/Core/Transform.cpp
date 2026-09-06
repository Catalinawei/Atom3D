#include "Transform.h"

using namespace AtomToolKit;

Transform::Transform(Shader* shader)
	: Component(), model(glm::mat4(1.0)), 
	m_position(glm::vec3(0.0f)), m_rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
	m_scale(glm::vec3(1.0f)), m_isDirty(true), m_shader(shader){

}

void Transform::Update(float deltaTime) {
	if (m_isDirty) {
		UpdateModel(deltaTime);
		m_isDirty = false;
		//std::cout << "Transform updated! Position: "
			//<< m_position.x << ", " << m_position.y << ", " << m_position.z << "\n";
	}
}

void Transform::SetPosition(glm::vec3 pos) noexcept {
	m_position = pos;
	m_isDirty = true;
}

void Transform::Translate(glm::vec3 delta) noexcept {
	m_position += delta;
	m_isDirty = true;
}

void Transform::SetRotation(glm::vec3 rot) {
	m_rotation = glm::quat(glm::radians(rot));
	m_isDirty = true;
}

void Transform::SetRotation(glm::quat rot) {
	m_rotation = rot;
	m_isDirty = true;
}

void Transform::SetRotation(float pitch, float yaw, float roll) noexcept {
	m_rotation = glm::quat(glm::vec3(pitch, yaw, roll));
	m_isDirty = true;
}

void Transform::Rotate(glm::vec3 delta) noexcept {
	m_rotation = glm::radians(delta) * m_rotation;
	m_isDirty = true;
}

void Transform::Rotate(float pitch, float yaw, float roll) noexcept {
	m_rotation.x += pitch;
	m_rotation.y += yaw;
	m_rotation.z += roll;
	m_isDirty = true;
}

void Transform::SetScale(glm::vec3 scale) noexcept{
	m_scale = scale;
	m_isDirty = true;
}

void Transform::SetScale(float uniformScale) noexcept{
	m_scale = glm::vec3(uniformScale);
	m_isDirty = true;
}

void Transform::ScaleBy(glm::vec3 multiplier) noexcept{
	m_scale *= multiplier;
	m_isDirty = true;
}

void Transform::UpdateModel(float deltaTime) {
	glm::mat4 transformMatrix = glm::mat4(1.0F);

	transformMatrix = glm::translate(transformMatrix, m_position);

	transformMatrix = transformMatrix * glm::mat4_cast(m_rotation);

	transformMatrix = glm::scale(transformMatrix, m_scale);

	model = transformMatrix;
}