#ifndef APSIS_AGENT_MOVER_H
#define APSIS_AGENT_MOVER_H

#include "apsis/world/map.h"

#include "apsis/geometry/rectangle.h"
#include "apsis/geometry/point.h"

#include <set>
#include <string>
#include <vector>

namespace Apsis {
  namespace Agent {
    class Mover {
    public:
      Mover(char* rule);

      /*
       *  Update intended point and return reason for change.
       */
      virtual bool update(float elapsed,
                          std::set<unsigned int>& states,
                          const Apsis::Geometry::Rectangle& original,
                          Apsis::Geometry::Point& updated);

      bool supercedes(const char* rule);

      void supercede(const char* rule);

      char* rule();
    private:
      std::set<unsigned int> _supercedes;
      char*                  _rule;
      unsigned int           _id;

      static unsigned int registerRule(const char* rule);
      static std::vector<std::string> _ids;
    };
  }
}

#endif