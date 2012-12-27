#ifndef ISOTASTY_MODEL_LIGHT_H
#define ISOTASTY_MODEL_LIGHT_H

#include "glm/glm.hpp"

namespace IsoTasty {
  namespace Model {
    class Light {
    public:
      /*
       *  Constructs a new representation of a light.
       */
      Light(glm::vec3& position,
            glm::vec3& direction);

      /*
       *  Retrieve the position of the light.
       */
      glm::vec3& position();

      /*
       *  Retrieve the direction of the light.
       */
      glm::vec3& direction();

    private:
      glm::vec3 _position;
      glm::vec3 _direction;
    };
  }
}

#endif