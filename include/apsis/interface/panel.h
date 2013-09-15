#ifndef APSIS_INTERFACE_PANEL_H
#define APSIS_INTERFACE_PANEL_H

#include "apsis/interface/window.h"

#include "apsis/world/object.h"

namespace Apsis {
  namespace Interface {
    class Panel {
    public:
      static void init(Apsis::World::Object& object);

      static void draw(Apsis::Engine::Graphics& graphics,
                       const Apsis::Geometry::Rectangle& position,
                       const Apsis::World::Object& object);
    };
  }
}

#endif