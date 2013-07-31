#include <apsis/hud/filler_bar.h>

#include "apsis/primitives/fragment_shader.h"
#include "apsis/primitives/vertex_shader.h"

#include "apsis/primitives/unlinked_program.h"
#include "apsis/primitives/program.h"

// glm::vec3, glm::vec4, glm::ivec4, glm::mat4
#include <glm/glm.hpp>
// glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_transform.hpp>
// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

Apsis::Hud::FillerBar::FillerBar(Apsis::Primitives::SpriteSheet* spriteSheet,
                             unsigned int startIndex,
                             unsigned int stages,
                             unsigned int count,
                             float x, float y) 
  : _spriteSheet(spriteSheet),
    _stages(stages),
    _count(count),
    _startIndex(startIndex) {
  _position.x = x;
  _position.y = y;

  unsigned int sprite_count = _spriteSheet->count();

  unsigned int vertices_size = 4 * _stages;
  unsigned int elements_size = 6 * _stages;
  _elements = new unsigned int[elements_size];

  // 8 values for each logical vertex: 3 per axis coordinate,
  //                                   2 per texcoord
  this->_vertices = new float[5 * vertices_size];
  
  unsigned int i = 0;
  unsigned int ei = 0;
  unsigned int ti = 0;

  for (unsigned int si = _startIndex; si < (_startIndex + _stages) && si < sprite_count; si++) {
    float coords[4];
    _spriteSheet->textureCoordinates(si, coords);
    Apsis::Primitives::Sprite* sprite = _spriteSheet->sprite(si); 

    _vertices[i * 5 + 0] = -(float)sprite->center_x;
    _vertices[i * 5 + 1] = 0.0f;
    _vertices[i * 5 + 2] = -(float)sprite->center_y;

    _vertices[i * 5 + 3] = coords[0]; //textureCoords[i].x;
    _vertices[i * 5 + 4] = coords[1]; //textureCoords[i].y;

    i++;

    _vertices[i * 5 + 0] = -(float)sprite->center_x + (float)sprite->width;
    _vertices[i * 5 + 1] = 0.0f;
    _vertices[i * 5 + 2] = -(float)sprite->center_y;

    _vertices[i * 5 + 3] = coords[2]; //textureCoords[i].x;
    _vertices[i * 5 + 4] = coords[1]; //textureCoords[i].y;

    i++;
      
    _vertices[i * 5 + 0] = -(float)sprite->center_x + (float)sprite->width;
    _vertices[i * 5 + 1] = 0.0f;
    _vertices[i * 5 + 2] = -(float)sprite->center_y + (float)sprite->height;

    _vertices[i * 5 + 3] = coords[2]; //textureCoords[i].x;
    _vertices[i * 5 + 4] = coords[3]; //textureCoords[i].y;

    i++;
      
    _vertices[i * 5 + 0] = -(float)sprite->center_x;
    _vertices[i * 5 + 1] = 0.0f;
    _vertices[i * 5 + 2] = -(float)sprite->center_y + (float)sprite->height;

    _vertices[i * 5 + 3] = coords[0]; //textureCoords[i].x;
    _vertices[i * 5 + 4] = coords[3]; //textureCoords[i].y;

    i++;

    _elements[ei] = i-4; ei++;
    _elements[ei] = i-3; ei++;
    _elements[ei] = i-1; ei++;

    _elements[ei] = i-3; ei++;
    _elements[ei] = i-2; ei++;
    _elements[ei] = i-1; ei++;
  }

  _vbo.transfer(_vertices, 5 * vertices_size);
  _ebo.transfer(_elements, elements_size);

  _vao.bindElements(_ebo);

  Primitives::VertexShader   vs = Primitives::VertexShader::fromFile("src/shaders/vertex/position.glsl");
  Primitives::FragmentShader fs = Primitives::FragmentShader::fromFile("src/shaders/fragment/flat.glsl");

  Primitives::UnlinkedProgram unlinked;
  unlinked.attach(vs);
  unlinked.attach(fs);
  unlinked.defineFragmentOutput("outColor");
  Primitives::Program program = unlinked.link();

  _vao.useProgram(program);
  program.defineInput("position", _vbo, 3, Primitives::Program::Type::Float, false, 5, 0);
  program.defineInput("texcoord", _vbo, 2, Primitives::Program::Type::Float, false, 5, 3);

  _vao.defineUniform("model", program);
  _vao.defineUniform("view",  program);
  _vao.defineUniform("proj",  program);

  _vao.defineUniform("tex", program);
  _vao.bindTexture(0, *_spriteSheet->texture());
  _vao.uploadUniform("tex", 0);

  value(0);

  Apsis::Primitives::Sprite* sprite = _spriteSheet->sprite(_startIndex);
  _position.width = sprite->width * _count;
  _position.height = sprite->height;
  _itemWidth = sprite->width;
}

Apsis::Primitives::SpriteSheet* Apsis::Hud::FillerBar::spriteSheet() {
  return _spriteSheet;
}

Apsis::Geometry::Rectangle Apsis::Hud::FillerBar::position() {
  return _position;
}

// Draws the actor
void Apsis::Hud::FillerBar::draw(glm::mat4& projection,
                               Primitives::Camera& camera) {
  _vao.uploadUniform("proj", projection);
  _vao.uploadUniform("view", camera.view());
  
  _vao.bindTexture(0, *_spriteSheet->texture());
  _vao.uploadUniform("camera", camera.eye());

  glm::mat4 model = glm::mat4(1.0);

  unsigned int tmp = _value;

  // TODO: don't abuse rectangle coordinates... x should represent the center, not the left
  unsigned int x = _position.x;

  // Draw full
  unsigned int i;

  for (i = (_stages - 1); i <= _value; i += (_stages - 1)) {
    model = glm::translate(glm::mat4(1.0),
                           glm::vec3(x, 0.0, _position.y));
    _vao.uploadUniform("model", model);
    x += _itemWidth;

    _vao.drawRange((_stages - 1) * 6, 6);
  }

  // Draw partial
  if (_value < _count * (_stages-1)) {
    model = glm::translate(glm::mat4(1.0),
                           glm::vec3(x, 0.0, _position.y));
    _vao.uploadUniform("model", model);
    x += _itemWidth;

    _vao.drawRange(((i - _value)) * 6, 6);
  }

  // Draw empty
  for ( ; i < _count * (_stages - 1); i += (_stages - 1)) {
    model = glm::translate(glm::mat4(1.0),
                           glm::vec3(x, 0.0, _position.y));
    _vao.uploadUniform("model", model);
    x += _itemWidth;

    _vao.drawRange(0, 6);
  }
}

unsigned int Apsis::Hud::FillerBar::value() {
  return _value;
}

void Apsis::Hud::FillerBar::value(unsigned int updated) {
  _value = updated;
}