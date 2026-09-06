#include "Actor.h"
#include "Component.h"
#include <iostream>

AtomToolKit::Actor::Actor() {}


void AtomToolKit::Actor::Update(float deltaTime) {
	for (auto& i : components) {
		i.second->Update(deltaTime);
	}
}

AtomToolKit::Actor::~Actor() {}