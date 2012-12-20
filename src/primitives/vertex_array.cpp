#include "iso-tasty/primitives/vertex_array.h"

// Include GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#ifndef NO_GL
  #ifdef _WIN32
  #include <windows.h>
  #endif

  #include <GL/gl.h>
  #include <GL/glu.h>
#endif

// glm::value_ptr
#include <glm/gtc/type_ptr.hpp>

IsoTasty::Primitives::VertexArray::VertexArray() {
  glGenVertexArrays(1, &this->_vao);
}

IsoTasty::Primitives::VertexArray::~VertexArray() {
  if (_counter.isAlone()) {
    glDeleteVertexArrays(1, &this->_vao);
  }
}

void IsoTasty::Primitives::VertexArray::use() {
  glBindVertexArray(this->_vao);
}

void IsoTasty::Primitives::VertexArray::bindElements(VertexBuffer& buffer) {
  glBindVertexArray(this->_vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.identifier());
}

void IsoTasty::Primitives::VertexArray::useProgram(Program& program) {
  glBindVertexArray(this->_vao);
  glUseProgram(program.identifier());

  // Only add if not already there
  for(unsigned int i = 0; i < _programs.size(); i++){
    if (_programs[i].identifier() == program.identifier()) {
      return;
    }
  }
  _programs.push_back(program);
}

void IsoTasty::Primitives::VertexArray::draw() {
  glBindVertexArray(this->_vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

int IsoTasty::Primitives::VertexArray::defineUniform(const char* name, Program& program) {
  useProgram(program);
  GLint uniform = glGetUniformLocation(program.identifier(), name);
  std::string key = name;
  _uniforms[name] = uniform;
  return uniform;
}

void IsoTasty::Primitives::VertexArray::uploadUniform(const char* name, glm::mat4& mat) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, mat);
}

void IsoTasty::Primitives::VertexArray::uploadUniform(int identifier, glm::mat4& mat) const {
  glUniformMatrix4fv(identifier, 1, GL_FALSE, glm::value_ptr(mat));
}

void IsoTasty::Primitives::VertexArray::uploadUniform(const char* name,
                                                      int         value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void IsoTasty::Primitives::VertexArray::uploadUniform(int identifier,
                                                      int value) const {
  glUniform1i(identifier, value);
}

void IsoTasty::Primitives::VertexArray::bindTexture(unsigned int slot, Texture& texture) {
  std::map<unsigned int, bool>::iterator it = _textures.find(slot);
  _textures.erase(it);

  _textures[slot] = true;

  if (slot > 31) {
    throw "Cannot bind texture. Slot too high.";
  }

  glActiveTexture(GL_TEXTURE0  + slot);
  glBindTexture(GL_TEXTURE_2D, texture.identifier());
}

unsigned int IsoTasty::Primitives::VertexArray::identifier() const {
  return this->_vao;
}