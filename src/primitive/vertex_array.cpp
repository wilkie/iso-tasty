#include "apsis/primitive/vertex_array.h"

#include "apsis/backend/sdl.h"

#include "apsis/engine/log.h"

using namespace Apsis;

unsigned int Primitive::VertexArray::_current_vao = 0xffffffff;

static void _throwError(const char* function, const char* message) {
  Engine::Log::error("Primitive", "VertexArray", function, message);
}

#ifdef DEBUG_THROW_GL_ERRORS
static void _throwGLError(const char* function) {
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    const char* errorString;
    switch ( error ) {
      case GL_INVALID_ENUM: errorString = "invalid enumerant"; break;
      case GL_INVALID_VALUE: errorString = "invalid value"; break;
      case GL_INVALID_OPERATION: errorString = "invalid operation"; break;
      case GL_STACK_OVERFLOW: errorString = "stack overflow"; break;
      case GL_STACK_UNDERFLOW: errorString = "stack underflow"; break;
      case GL_OUT_OF_MEMORY: errorString = "out of memory"; break;
      case GL_TABLE_TOO_LARGE: errorString = "table too large"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: errorString = "invalid framebuffer operation"; break;
//      case GL_TEXTURE_TOO_LARGE: errorString = "texture too large"; break;
      default: errorString = "unknown GL error"; break;
    }
    _throwError(function, errorString);
  }
}
#endif

Primitive::VertexArray::VertexArray() {
#ifdef JS_MODE // Older GL / GLES2
#else
  glGenVertexArrays(1, &this->_vao);
#endif

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("~constructor");
#endif
}

Primitive::VertexArray::~VertexArray() {
  if (_counter.isAlone()) {
#ifdef JS_MODE // Older GL / GLES2
#else
  glDeleteVertexArrays(1, &this->_vao);
#endif

#ifdef DEBUG_THROW_GL_ERRORS
    _throwGLError("~destructor");
#endif
  }
}

void Primitive::VertexArray::_bind() const {
  bindProgram();

#ifdef JS_MODE
#else
  glBindVertexArray(this->_vao);
#endif

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("use");
#endif
}

void Primitive::VertexArray::use() {
  _bind();
}

void Primitive::VertexArray::bind(VertexBuffer& buffer) {
  _bind();

  if (buffer.target() == VertexBuffer::Target::Elements) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer.identifier());
    if (_elementBuffer.size() > 0) {
      _elementBuffer.clear();
    }
    _elementBuffer.push_back(buffer);
  }
  else if (buffer.target() == VertexBuffer::Target::Data) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer.identifier());
    if (_dataBuffer.size() > 0) {
      _dataBuffer.clear();
    }
    _dataBuffer.push_back(buffer);
  }

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("bind");
#endif
}

void Primitive::VertexArray::useProgram(const Registry::Program& program) {
  useProgram(program.program());

  for (unsigned int i = 0; i < program.uniformCount(); i++) {
    defineUniform(program.uniformName(i));
  }
}

void Primitive::VertexArray::useProgram(const Program& program) {
  _bind();
  glUseProgram(program.identifier());

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("useProgram");
#endif

  // Only add if not already there
  for(unsigned int i = 0; i < _programs.size(); i++) {
    if (_programs[i].identifier() == program.identifier()) {
      return;
    }
  }
  _programs.push_back(program);
}

void Primitive::VertexArray::draw() const {
  _bind();

  if (_dataBuffer.size() > 0) {
    _dataBuffer[0].bind();
  }

  unsigned int count = 0;
  if (_elementBuffer.size() > 0) {
    count = _elementBuffer[0].count();
    _elementBuffer[0].bind();
  }

  _bindTextures();

  // TODO: Handle multiple arrays for when count is too big for GL_UNSIGNED_SHORT
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, (void*)0);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("draw(glDrawElements)");
#endif
}

void Primitive::VertexArray::drawRange(unsigned int start, unsigned int count) const {
  _bind();

  if (_elementBuffer.size() > 0) {
    _elementBuffer[0].bind();
  }

  if (_dataBuffer.size() > 0) {
    _dataBuffer[0].bind();
  }

  _bindTextures();

  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, (void*)(start * sizeof(GLushort)));

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("drawRange(glDrawElements)");
#endif
}

void Primitive::VertexArray::drawQuads() const {
  _bind();

  unsigned int count = 0;
  if (_elementBuffer.size() > 0) {
    count = _elementBuffer[0].count();
    _elementBuffer[0].bind();
  }

  if (_dataBuffer.size() > 0) {
    _dataBuffer[0].bind();
  }

  _bindTextures();

  glDrawElements(GL_QUADS, count, GL_UNSIGNED_SHORT, (void*)0);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("drawQuads(glDrawElements)");
#endif
}

void Primitive::VertexArray::drawQuadsRange(unsigned int start, unsigned int count) const {
  _bind();

  if (_elementBuffer.size() > 0) {
    _elementBuffer[0].bind();
  }

  if (_dataBuffer.size() > 0) {
    _dataBuffer[0].bind();
  }

  _bindTextures();

  glDrawElements(GL_QUADS, count, GL_UNSIGNED_SHORT, (void*)(start * sizeof(GLushort)));

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("drawQuadsRange(glDrawElements)");
#endif
}

int Primitive::VertexArray::defineUniform(const char* name) {
  _bind();
  const Program& program = _programs[0];
  GLint uniform = glGetUniformLocation(program.identifier(), name);
  if (uniform < 0) {
    char error[1024];
    sprintf(error, "Cannot upload uniform %.250s. Uniform not found.", name);
    _throwError("defineUniform", error);
  }
  std::string key = name;
  _uniforms[name] = uniform;
  return uniform;
}

void Primitive::VertexArray::uploadUniform(const char* name,
                                            const Matrix& mat) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, mat);
}

void Primitive::VertexArray::uploadUniform(int identifier,
                                            const Matrix& mat) const {
  _bind();
  glUniformMatrix4fv(identifier, 1, GL_FALSE, (float*)&mat.value);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("uploadUniform (Matrix)");
#endif
}

void Primitive::VertexArray::uploadUniform(const char* name,
                                            int         value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void Primitive::VertexArray::uploadUniform(int identifier,
                                            int value) const {
  _bind();
  glUniform1i(identifier, value);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("uploadUniform (int)");
#endif
}

void Primitive::VertexArray::uploadUniform(const char* name,
                                            float       value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void Primitive::VertexArray::uploadUniform(int   identifier,
                                            float value) const {
  _bind();
  glUniform1f(identifier, value);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("uploadUniform (float)");
#endif
}

void Primitive::VertexArray::uploadUniform(const char* name,
                                            const Vector3& value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void Primitive::VertexArray::uploadUniform(int         identifier,
                                            const Vector3& value) const {
  _bind();
  glUniform3fv(identifier, 1, (float*)&value);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("uploadUniform (Vector3)");
#endif
}

void Primitive::VertexArray::uploadUniform(const char* name,
                                            const Vector4& value) const {
  std::string key = name;
  GLint uniform = _uniforms.find(key)->second;
  uploadUniform(uniform, value);
}

void Primitive::VertexArray::uploadUniform(int         identifier,
                                            const Vector4& value) const {
  _bind();
  glUniform4fv(identifier, 1, (float*)&value);

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("uploadUniform (Vector4)");
#endif
}

void Primitive::VertexArray::bindTexture(unsigned int slot,
                                          const Texture& texture) {
  std::map<unsigned int, Texture>::iterator it = _textures.find(slot);
  if (it != _textures.end()) {
    _textures.erase(it);
  }

  _textures.insert(_textures.begin(),
                   std::pair<unsigned int, Texture>(slot, texture));

  if (slot > 31) {
    _throwError("bindTexture", "Cannot bind texture. Slot too high.");
  }

  glActiveTexture(GL_TEXTURE0  + slot);
  glBindTexture(GL_TEXTURE_2D, texture.identifier());

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("bindTexture");
#endif
}

unsigned int Primitive::VertexArray::identifier() const {
  return this->_vao;
}

void Primitive::VertexArray::_bindTextures() const {
  std::map<unsigned int, Texture>::const_iterator it;

  for (it = _textures.begin(); it != _textures.end(); ++it) {
    unsigned int slot = it->first;
    const Texture& texture = it->second;

    glActiveTexture(GL_TEXTURE0  + slot);
    glBindTexture(GL_TEXTURE_2D, texture.identifier());

#ifdef DEBUG_THROW_GL_ERRORS
    _throwGLError("_bindTextures");
#endif
  }
}

void Primitive::VertexArray::bindProgram() const {
#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("bindProgram (error on stack)");
#endif

  if (_programs.size() > 0) {
    glUseProgram(_programs[0].identifier());
  }
  else {
    glUseProgram(0);
  }

#ifdef DEBUG_THROW_GL_ERRORS
  _throwGLError("bindProgram");
#endif
}
