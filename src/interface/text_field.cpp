#include "apsis/interface/text_field.h"

#include "apsis/sprite/sheet.h"

struct TextFieldData {
  const Apsis::Sprite::Font* font;

  float text_width;
  float text_height;

  float text_x;
  float text_y;
};

void Apsis::Interface::TextField::init(const Apsis::Interface::Window& window,
                                       Apsis::World::Object& object,
                                       const Engine::Object& loader) {
  static unsigned int font_id = Apsis::Registry::Property::id("font");
  static unsigned int text_id = Apsis::Registry::Property::id("text");

  TextFieldData* data = new TextFieldData;

  if (!object.has(font_id)) {
    object.set(font_id, "assets/fonts/Cinzel/Cinzel-Bold.ttf");
  }

  if (!object.has(text_id)) {
    object.set(text_id, "TextField Text");
  }

  const char* fontname = object.get(font_id).asCString();

  const Apsis::Sprite::Font& font = Apsis::Sprite::Font::load(fontname, loader);
  data->font = &font;

  const Apsis::Geometry::Rectangle& position = window.position();

  const char* text = object.get(text_id).asCString();
  data->text_width = font.width(text);
  data->text_height = font.height(text);

  data->text_x = (position.width  - data->text_width)  / 2.0f;
  data->text_y = (position.height - data->text_height - font.descent()) / 2.0f;
  data->text_y += font.ascent();

  object.userData(data);
}

void Apsis::Interface::TextField::draw(Apsis::Engine::Graphics& graphics,
                                       const Apsis::Interface::Window& window,
                                       const Apsis::World::Object& object) {
  static unsigned int text_id = Apsis::Registry::Property::id("text");

  const TextFieldData& data = *(TextFieldData*)object.userData();

  const Apsis::Geometry::Rectangle& position = window.position();

  const char* text = object.get(text_id).asCString();

  // Draw text
  graphics.font(data.font->id());
  graphics.draw(position.left() + data.text_x,
                position.top() + data.text_y,
                text);
}

void Apsis::Interface::TextField::input(bool pressed,
                                        const Apsis::Input::Binding&    binding,
                                        const Apsis::Geometry::Point&   point,
                                        const Apsis::Interface::Window& window,
                                        Apsis::World::Object&           object) {
}

void Apsis::Interface::TextField::motion(const Apsis::Geometry::Point& point,
                                         const Apsis::Interface::Window& window,
                                         Apsis::World::Object& object) {
}

void Apsis::Interface::TextField::enter(const Apsis::Geometry::Point& point,
                                        const Apsis::Interface::Window& window,
                                        Apsis::World::Object& object) {
}

void Apsis::Interface::TextField::update(float elapsed,
                                         Apsis::Geometry::Rectangle& position,
                                         Apsis::World::Object& object) {
}

void Apsis::Interface::TextField::leave(const Apsis::Geometry::Point& point,
                                        const Apsis::Interface::Window& window,
                                        Apsis::World::Object& object) {
}