#include "water_surface_manager.hpp"
#include "time_manager.hpp"

WaterSurfaceManager::WaterSurfaceManager() : current(NULL_PTR), hasNextTimer(0.0f) {}

void WaterSurfaceManager::addWaterSurface(glm::vec3 newPos, int colorMask) {
    if(current == NULL_PTR) {
        current = WaterSurfacePtr(new WaterSurface(newPos, colorMask));
        Director::getScene()->addGameObject(current);
        current->setup();
        hasNextTimer = 0.0f;
    } else {
        nextPos = newPos;
        nextColorMask = colorMask;
        hasNextTimer = 0.3f;
        current->setSpeed(current->getSpeed() * 1.5f);
    }
}

void WaterSurfaceManager::setup() {}

void WaterSurfaceManager::update() {
    if(hasNextTimer > 0.0f) {
        hasNextTimer -= TimeManager::getDeltaTime();
    }
    if(current != NULL_PTR) {
        if(!current->manualUpdate()) {
            float oldSpeed = current->getSpeed();
            current = NULL_PTR;
            if(hasNextTimer > 0.0f) {
                addWaterSurface(nextPos, nextColorMask);
                current->setSpeed(fmin(fmax(oldSpeed, 1.0), 2.5f));
            }
        }
    }
}
