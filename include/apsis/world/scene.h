#ifndef APSIS_WORLD_SCENE_H
#define APSIS_WORLD_SCENE_H

#include "apsis/world/map.h"
#include "apsis/world/actor.h"

#include "apsis/primitives/camera.h"

#include <vector>

namespace Apsis {
  namespace World {
    class Scene {
    public:
      /*
       *  Creates a new Scene.
       */
      Scene();

      /*
       *  Adds a Map to the scene.
       */
      void addMap(Apsis::World::Map& map);

      /*
       *  Instantiates an Actor to the scene using a Thing id.
       */
      unsigned int addActor(unsigned int id, float x, float y);

      /*
       *  Instantiates an Actor to the scene by cloning the given Thing.
       */
      unsigned int addActor(const Sprite::Thing& thing, float x, float y);

      /*
       *  Returns the Actor given by the id.
       */
      Apsis::World::Actor& actor(unsigned int id);

      /*
       *  Updates objects in the scene.
       */
      void update(float elapsed);

      /*
       *  Draws the scene.
       */
      void draw(const glm::mat4& projection,
                Primitives::Camera& camera,
                const glm::mat4& model) const;
    private:
      // Collections of objects within the scene
      std::vector<Apsis::World::Actor> _actors;
      std::vector<Apsis::World::Map>   _maps;
    };
  }
}

#endif