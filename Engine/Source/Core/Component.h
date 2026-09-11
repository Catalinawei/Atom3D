#pragma once

namespace AtomToolKit {
	class Actor;

	class Component
	{
	public:
		Component() = default;

		virtual void Update(float deltaTime) {}

		virtual ~Component() {}
	};
}
