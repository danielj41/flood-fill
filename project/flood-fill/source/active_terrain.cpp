#include "active_terrain.hpp"

#include <set>
#include <string>

#include <iostream>
#include "debug_macros.h"

#include "glm/glm.hpp"
#include "collision_object.hpp"
#include "collision_manager.hpp"
#include "solid_cube.hpp"
#include "time_manager.hpp"
#include "render_engine.hpp"
#include "level_template.hpp"
#include "director.hpp"

ActiveTerrain::ActiveTerrain(Switch* _s, glm::vec3 _initialPos, glm::vec3 _finalPos, float _speed)
    : GameObject(), s(_s), direction(_finalPos - _initialPos), 
      position(_initialPos), initialPos(_initialPos), 
      finalPos(_finalPos), speed(_speed), active(false), doneAnimating(true), timer(0.0f) {}

void ActiveTerrain::setup(){
    
                            
    fillTypes = ((LevelTemplate *)Director::getScene())->getFillTypes();

    for(int i = 0; i < 16; i++) {
        for(int j = 6; j < 11; j++) {
                 
            
            SolidCube * at1 = new SolidCube(glm::vec3(i*2 + 1, j*2 + 1, -37));
            at1->setup();
            RenderEngine::getRenderGrid()->removeObject(at1->getObject());
            RenderEngine::getRenderElement("shadow")->removeObject(at1->getObject());

            solidCubes.push_back(at1);

            SolidCube * at2 = new SolidCube(glm::vec3(i*2 + 1, j*2 + 1, -21));
            at2->setup();
            RenderEngine::getRenderGrid()->removeObject(at2->getObject());
            RenderEngine::getRenderElement("shadow")->removeObject(at2->getObject());

            solidCubes.push_back(at2);
            
        }
    }    
}

void ActiveTerrain::update(){

   
    for (std::set<int>::iterator i = fillTypes->begin(); i != fillTypes->end(); i++) {
        INFO("In fillTypes Set: " << std::to_string(*i));
    }
    
    if(s->isOn() && active && !doneAnimating) {
        timer += TimeManager::getDeltaTime();
        for(std::list<SolidCube *>:: iterator it = solidCubes.begin(); it != solidCubes.end(); it++){
            (*it)->animateFrom(glm::vec3(0.0f, 15.0f, 0.0f), timer);
        }
        if(timer > 1.0f) {
            doneAnimating = true;
            for(std::list<SolidCube *>:: iterator it = solidCubes.begin(); it != solidCubes.end(); it++) {
                RenderElement *re = RenderEngine::getRenderElement("normalmap");
                re->removeObject((*it)->getObject());
                RenderEngine::getRenderGrid()->addObject((*it)->getObject(), re);
            }
        }
    }
    if (s->isOn()) {
        if (!active){
            
            active = true;
            RenderElement *re = RenderEngine::getRenderElement("normalmap");

            timer = 0.0f;
            doneAnimating = false;

            // Add solidCubes to render engine            
            for(std::list<SolidCube *>:: iterator it = solidCubes.begin(); it != solidCubes.end(); it++){
                re->addObject((*it)->getObject());
                RenderEngine::getRenderElement("shadow")->addObject((*it)->getObject());
                CollisionManager::addCollisionObjectToGrid(*it);
                (*it)->animateFrom(glm::vec3(0.0f, 15.0f, 0.0f), timer);
            }
            
            fillTypes->insert(LevelTemplate::TOGGLE_FILL);
          
           
           

            
        }
        /*if (position != finalPos) { 
            position += direction * speed * TimeManager::getDeltaTime();
            
            if (glm::length(position - initialPos) >= glm::length(finalPos - initialPos)) {
                position = finalPos;
            } 
            
            // update solidcubes with new pos
            }*/ 
    } else {
        if (active && doneAnimating) {
            
            active = false;

            for(std::list<SolidCube *>:: iterator it = solidCubes.begin(); it != solidCubes.end(); it++){
                RenderEngine::getRenderGrid()->removeObject((*it)->getObject());
                RenderEngine::getRenderElement("shadow")->removeObject((*it)->getObject());
                CollisionManager::removeCollisionObjectFromGrid(*it);
            }
            
            fillTypes->erase(LevelTemplate::TOGGLE_FILL);

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
   

