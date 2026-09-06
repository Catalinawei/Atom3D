#pragma once
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <typeinfo>
#include <typeindex>

namespace AtomToolKit {

	class Component;

	class Actor
	{
	public:

		Actor();
		virtual ~Actor();

		void Update(float deltaTime);

		template <typename T>
		T* GetComponent() {
			auto it = components[std::type_index(typeid(T))];
			if (!it) std::cout << "WHY IS I TNULLPTR\n";
			return it ? static_cast<T*>(it) : nullptr;
		}

		template <typename T>
		void AddComponent(T* component) {
			components[std::type_index(typeid(T))] = component;
		}

	private:
		std::unordered_map<std::type_index, Component*> components;
	};

}