#ifndef APSIS_INTERFACE_WINDOW_H
#define APSIS_INTERFACE_WINDOW_H

#include "apsis/engine/graphics.h"

#include "apsis/geometry/rectangle.h"

#include "apsis/world/object.h"

#include "apsis/input/binding.h"

#include "apsis/registry/widget.h"

#include "apsis/interface/event.h"

#include <stddef.h>

//Apsis::Graphics::Context
//position(position) <- push position to stack
//pop <- pop back to parent position
//position() <- get global position

//drawing functions?

//fonts?

//layout?

//caching?

/*

Engine::Interface
  loadWidget(const char* json) -- loads a widget
  load(const char* json) -- loads an interface
  use(const char* name) -- uses the already loaded interface

Engine::Object
  loadWidget(const char* name) -- loads a widget through Interface::Engine
  loadInterface(const char* name) -- loads interface through Interface::Engine
  loadScene(const char* name) -- loads scene through Scene::Engine

Engine::Scene
  load(const char* json) -- loads the scene
  use(const char* name) -- uses the given scene

*/

namespace Apsis {
  namespace Interface {
    class Window {
    public:

      Window(float x,
             float y,
             float width,
             float height,
             Event::Init& init,
             Event::Draw& draw,
             Event::Input& input,
             Event::Motion& motion,
             Event::Update& update,
             Event::Enter& enter,
             Event::Leave& leave);

      Window(const Registry::Widget& widget,
             float x,
             float y,
             float width,
             float height);

      /*
       *  Returns the position of this Window within its parent.
       */
      const Apsis::Geometry::Rectangle& position() const;

      /*
       *  Returns the next sibling Window.
       */
      const Apsis::Interface::Window& next() const;

      /*
       *  Returns the previous sibling Window.
       */
      const Apsis::Interface::Window& prev() const;

      /*
       *  Returns the first child Window.
       */
      const Apsis::Interface::Window& child() const;

      /*
       *  Returns the number of children.
       */
      unsigned int childCount() const;

      /*
       *  Adds the given Window as a child.
       */
      void add(Apsis::Interface::Window& window);

      /*
       *  Detaches this window from its parent.
       */
      void detach();

      /*
       *  Returns true if this Window has a parent.
       */
      bool attached() const;

      /*
       *  Draws this Window and all of its children.
       */
      void draw(Apsis::Engine::Graphics& graphics) const;

      /*
       *  Updates this Window and all of its children.
       */
      void update(float elapsed);

      /*
       *  Tells the window that there is some input interaction.
       */
      void input(bool pressed,
                 const Apsis::Geometry::Point& point,
                 const Apsis::Input::Binding& binding);

      /*
       *  Tells the window that there is pointer movement across this Window.
       */
      void motion(const Apsis::Geometry::Point& point);

      /*
       *  Tells the window that a pointer device has entered its bounds at
       *  the given point.
       */
      void enter(const Apsis::Geometry::Point& point);

      /*
       *  Tells the window that a pointer device has left its bounds at
       *  the given point.
       */
      void leave(const Apsis::Geometry::Point& point);

      /*
       *  Returns true if the given point is within this Window. The
       *  coordinates are local to the Window's parent.
       */
      bool contains(float x, float y) const;

      /*
       *  Returns the descendant Window at the foreground at the given point.
       *  The coordinates are local to the Window's parent. Will return its
       *  self when the point is not over any other Window. Will return self if
       *  the point is outside of this Window.
       */
      Apsis::Interface::Window& at(float x, float y);
      const Apsis::Interface::Window& at(float x, float y) const;

      /* TODO: cleanup this description.
       *  Returns the child Window at the foreground at the given point.
       *  The coordinates are local to the Window's parent. Will return its
       *  self when the point is not over any other Window. Will return self if
       *  the point is outside of this Window.
       */
      Apsis::Interface::Window& childAt(float x, float y);
      const Apsis::Interface::Window& childAt(float x, float y) const;

      /*
       *  Returns whether or not this Window has focus.
       */
      bool focused() const;

      /*
       *  Returns whether or not this Window is hovered over by
       *  a pointing device.
       */
      bool hovered() const;

    private:
      // Dimensions
      Geometry::Rectangle _position;

      // List of other windows
      Interface::Window* _next;
      Interface::Window* _prev;

      // Children
      Interface::Window* _child;
      unsigned int _childCount;

      // Parent
      Interface::Window* _parent;

      // Which child has focus
      Interface::Window* _focus;

      // Which child is hovered
      Interface::Window* _hover;

      // Do we have focus
      bool _focused;

      // Are we hovered
      bool _hovered;

      Apsis::World::Object _object;

      // Events
      Event::Init&   _init;
      Event::Draw&   _draw;
      Event::Input&  _input;
      Event::Motion& _motion;
      Event::Enter&  _enter;
      Event::Leave&  _leave_;
      Event::Update& _update;

      // Update interval (a timer)
      float _updateInterval;
      float _updateElapsed;
    };
  }
}

#endif