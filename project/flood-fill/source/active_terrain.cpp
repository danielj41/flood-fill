#include "active_terrain.hpp"
#include "glm/glm.hpp"
#include "collision_object.hpp"
#include "collision_manager.hpp"
#include "solid_cube.hpp"
#include "time_manager.hpp"
#include "render_engine.hpp"

ActiveTerrain::ActiveTerrain(Switch* _s, glm::vec3 _initialPos, glm::vec3 _finalPos, float _speed)
    : GameObject(), s(_s), direction(_finalPos - _initialPos), 
      position(_initialPos), initialPos(_initialPos), 
      finalPos(_finalPos), speed(_speed), active(true), timer(0.0f) {}

void ActiveTerrain::setup(){
    
                            
    for(int i = 0; i < 16; i++) {
        for(int j = 4; j < 9; j++) {
                 
            
            SolidCube * at1 = new SolidCube(glm::vec3(i*2 + 1, j*2 + 1, -37));
            at1->setup();

            solidCubes.push_back(at1);

            SolidCube * at2 = new SolidCube(glm::vec3(i*2 + 1, j*2 + 1, -21));
            at2->setup();

            solidCubes.push_back(at2);
            
        }
    }    
}

void ActiveTerrain::update(){
    if(s->isOn() && active && timer < 1.0f) {
        timer += TimeManager::getDeltaTime();
        for(std::list<SolidCube *>:: iterator it = solidCubes.begin(); it != solidCubes.end(); it++){
            (*it)->animateFrom(glm::vec3(0.0f, 15.0f, 0.0f), timer);
        }
    }
    if (s->isOn()) {
        if (!active){
            
            active = true;
            RenderElement *re = RenderEngine::getRenderElement("normalmap");

            timer = 0.0f;

            // Add solidCubes to render engine            
            for(std::list<SolidCube *>:: iterator it = solidCubes.begin(); it != solidCubes.end(); it++){
                re->addObject((*it)->getObject());
                CollisionManager::addCollisionObjectToGrid(*it);
                (*it)->animateFrom(glm::vec3(0.0f, 15.0f, 0.0f), timer);
            }

            // Add solidCubes to grid

            
        }
        /*if (position != finalPos) { 
            position += direction * speed * TimeManager::getDeltaTime();
            
            if (glm::length(position - initialPos) >= glm::length(finalPos - initialPos)) {
                position = finalPos;
            } 
            
            // update solidcubes with new pos
            }*/ 
    } else {
        if (active) {
           
            active = false;
            RenderElement *re = RenderEngine::getRenderElement("normalmap");

            for(std::list<SolidCube *>:: iterator it = solidCubes.begin(); it != solidCubes.end(); it++){
                re->removeObject((*it)->getObject());
                CollisionManager::removeCollisionObjectFromGrid(*it);
            }



        }
        /*if (position != initialPos) { 
            position -= direction * speed;
            if (glm::length(position - finalPos) >= glm::length(initialPos - finalPos)) {
                position = initialPos;
            }
         
            // update solidcubes with new pos
            }*/
    }    
}
   

