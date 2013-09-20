#ifndef APSIS_INTERFACE_WINDOW_H
#define APSIS_INTERFACE_WINDOW_H

#include "apsis/engine/graphics.h"

#include "apsis/geometry/rectangle.h"

#include "apsis/world/object.h"

#include "apsis/input/binding.h"

#include <stddef.h>

//Apsis::Graphics::Context
//position(position) <- push position to stack
//pop <- pop back to parent position
//position() <- get global position

//drawing functions?

//fonts?

//layout?

//caching?

namespace Apsis {
  namespace Interface {
    class Window {
    public:

      // Init function
      typedef void(&InitEvent)(const Apsis::Interface::Window& window,
                               Apsis::World::Object& object);

      // Draw function
      typedef void(&DrawEvent)(Apsis::Engine::Graphics&        graphics,
                               const Apsis::Interface::Window& window,
                               const Apsis::World::Object&     object);

      // Update function (for animations)
      typedef void(&UpdateEvent)(float elapsed,
                                 Apsis::Geometry::Rectangle& position,
                                 Apsis::World::Object&       object);

      // Input function
      typedef void(&InputEvent)(bool pressed,
                                const Apsis::Input::Binding&    binding,
                                const Apsis::Geometry::Point&   point,
                                const Apsis::Interface::Window& window,
                                Apsis::World::Object&           object);

      // Hover function
      typedef void(&MotionEvent)(const Apsis::Geometry::Point&   point,
                                 const Apsis::Interface::Window& window,
                                 Apsis::World::Object&           object);

      // When input pointer enters bounds
      typedef void(&EnterEvent)(const Apsis::Geometry::Point&   point,
                                const Apsis::Interface::Window& window,
                                Apsis::World::Object&           object);

      // When input pointer leaves bounds
      typedef void(&LeaveEvent)(const Apsis::Geometry::Point&   point,
                                const Apsis::Interface::Window& window,
                                Apsis::World::Object&           object);

      Window(float x,
             float y,
             float width,
             float height,
             InitEvent& init,
             DrawEvent& draw,
             InputEvent& input,
             MotionEvent& motion,
             UpdateEvent& update,
             EnterEvent& enter,
             LeaveEvent& leave);

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
      InitEvent   _init;
      DrawEvent   _draw;
      InputEvent  _input;
      MotionEvent _motion;
      EnterEvent  _enter;
      LeaveEvent  _leave_;
      UpdateEvent _update;

      // Update interval (a timer)
      float _updateInterval;
      float _updateElapsed;
    };
  }
}

#endif