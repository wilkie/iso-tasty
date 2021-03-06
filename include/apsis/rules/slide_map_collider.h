#ifndef APSIS_RULES_SLIDE_MAP_COLLIDER_H
#define APSIS_RULES_SLIDE_MAP_COLLIDER_H

#include "apsis/world/map.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include "apsis/world/scene.h"
#include "apsis/rules/collide_function.h"

#include "apsis/world/collision_object.h"

#include <set>

namespace Apsis {
  namespace World {
    class Actor;
  }

  namespace Rules {
    class SlideMapCollider {
    public:
      static bool collide(const Apsis::World::Scene& scene,
                          Apsis::World::Object& object,
                          const Apsis::Geometry::Rectangle& original,
                          const Apsis::Geometry::Point& intended);
    };
  }
}

#endif
