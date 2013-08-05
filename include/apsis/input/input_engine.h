#ifndef APSIS_INPUT_ENGINE
#define APSIS_INPUT_ENGINE

#ifndef NO_SDL
#include <SDL/SDL.h>
#endif

#include "apsis/input/event.h"
#include "apsis/input/key.h"
#include "apsis/input/key_binding.h"
#include "apsis/input/key_binding_repository.h"

#include <vector>

namespace Apsis {
  struct Action {
    enum Actions {
      PLAYER_1_LEFT = 0x1000,
      PLAYER_1_RIGHT,
      PLAYER_1_UP,
      PLAYER_1_DOWN,
      PLAYER_1_JUMP
    };
  };

  /*
   *  Handles mapping between keys and events allowing for flexible control
   *    over what keys or buttons are mapped to particular functions.
   */
  class InputEngine {
    public:
      static const int QUIT_EVENT = -1;

      /*
       *  Constructs a bare input engine that can be assigned to an
       *    Apsis::Engine.
       */
      InputEngine();
      ~InputEngine();

      /*
       *  Returns the collection of key bindings. Use this class to manage
       *    the available key bindings.
       */
      KeyBindingRepository* keyBindings();

      /*
       *  Posts the input event and returns the bound event identifier
       *    for any action that has been triggered.
       */
      int post(Apsis::Event& event);

      /*
       *  Returns true when the given event is pressed.
       */
      bool isEventHeld(int event);

    private:
      // Stores the key bindings
      KeyBindingRepository* _bindings;

      // Stores held events
      std::vector<int> _held;
  };

}

#endif
