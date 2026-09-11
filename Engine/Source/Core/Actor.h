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
			if (!it) std::cout << "the component is nullptr\n";
			return it ? static_cast<T*>(it) : nullptr;
		}

		template <typename T>
		void AddComponent(T* component) {
			components[std::type_index(typeid(T))] = component;
		}

		std::string GetName() const noexcept{
			return m_name;
		}

		void SetName(const char* name) {
			m_name = name;
		}

	private:
		std::unordered_map<std::type_index, Component*> components;

		std::string m_name;
	};

}