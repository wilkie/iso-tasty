#include "apsis/interface/panel.h"

#include "apsis/sprite/sheet.h"

void Apsis::Interface::Panel::init(Apsis::World::Object& object) {
  static unsigned int font_id = Apsis::Registry::Property::id("font");
  static unsigned int text_id    = Apsis::Registry::Property::id("text");
  static unsigned int font_id_id = Apsis::Registry::Property::id("__fontid");

  if (!object.has(font_id)) {
    object.set(font_id, "assets/fonts/Cinzel/Cinzel-Bold.ttf");
  }

  if (!object.has(text_id)) {
    object.set(text_id, "Start");
  }

  const char* fontname = object.get(font_id).asCString();

  const Apsis::Sprite::Font& font = Apsis::Sprite::Font::load(fontname);

  object.set(font_id_id, (long)font.id());
}

void Apsis::Interface::Panel::draw(Apsis::Engine::Graphics& graphics,
                                   const Apsis::Geometry::Rectangle& position,
                                   const Apsis::World::Object& object) {
  static unsigned int text_id = Apsis::Registry::Property::id("text");
  static unsigned int font_id = Apsis::Registry::Property::id("__fontid");

  const char* text        = object.get(text_id).asCString();
  unsigned int font_index = object.get(font_id).asInteger();

  // Draw text
  graphics.font(font_index);
  graphics.draw(position.left(), position.top(), text);
}