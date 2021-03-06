#ifndef APSIS_AGENT_MOVERS_GRIDLOCK_RIGHT_H
#define APSIS_AGENT_MOVERS_GRIDLOCK_RIGHT_H

#include "apsis/agent/mover.h"
#include "apsis/input/input_engine.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include <set>

namespace Apsis {
  namespace Agent {
    namespace Movers {
      class GridlockRight : public Apsis::Agent::Mover {
      public:
        GridlockRight(Apsis::InputEngine& inputEngine);
        virtual bool update(float elapsed,
                            Apsis::World::Object& object,
                            const Apsis::Geometry::Rectangle& original,
                            Apsis::Geometry::Point& updated);
      private:
        InputEngine* _inputEngine;

        unsigned int _rightAction;

        unsigned int _movingUpState;
        unsigned int _movingLeftState;
        unsigned int _movingRightState;
        unsigned int _movingDownState;
      };
    }
  }
}

#endif
