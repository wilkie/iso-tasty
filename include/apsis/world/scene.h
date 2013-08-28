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
      Scene(const char* path);

      /*
       *  Loads a Scene from a JSON description.
       */
      static Apsis::World::Scene& load(const char* path);

      /*
       *  Adds a Map to the scene.
       */
      void addMap(const Apsis::World::Map& map);

      /*
       *  Instantiates an Actor to the scene using a Thing id.
       */
      unsigned int addActor(unsigned int id, float x, float y);

      /*
       *  Instantiates an Actor to the scene by cloning the given Thing.
       */
      unsigned int addActor(const Sprite::Thing& thing, float x, float y);

      /*
       *  Returns the Actor given by its index in the scene.
       */
      Apsis::World::Actor& actor(unsigned int index);
      const Apsis::World::Actor& actor(unsigned int index) const;

      /*
       *  Returns the number of actors currently in the scene.
       */
      unsigned int actorCount() const;

      /*
       *  Returns the number of maps currently in the scene.
       */
      unsigned int mapCount() const;

      /*
       *  Returns the map given by its index in the scene.
       */
      const Apsis::World::Map& map(unsigned int index) const;

      /*
       *  Updates objects in the scene.
       */
      void update(float elapsed);

      /*
       *  Reacts to an input action.
       */
      void act(unsigned int action_id, bool held);

      /*
       *  Draws the scene.
       */
      void draw(const glm::mat4& projection,
                Primitives::Camera& camera,
                const glm::mat4& model) const;
    private:
      // Keeps track of Scenes system-wide.
      static std::vector<std::string> _ids;
      static std::vector<Apsis::World::Scene*> _scenes;

      // Collections of objects within the scene
      std::vector<Apsis::World::Actor>      _actors;
      std::vector<const Apsis::World::Map*> _maps;
    };
  }
}

#endif