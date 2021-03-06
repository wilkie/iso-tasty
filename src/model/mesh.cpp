#include "apsis/model/mesh.h"

#include "apsis/primitive/fragment_shader.h"
#include "apsis/primitive/vertex_shader.h"

#include "apsis/primitive/unlinked_program.h"
#include "apsis/primitive/program.h"

Apsis::Model::Mesh::Mesh(std::vector<glm::vec3>& vertices,
                         std::vector<glm::vec3>& normals,
                         std::vector<glm::vec2>& textureCoords,
                         std::vector<unsigned short>& elements)
  : _vbo(Primitive::VertexBuffer::Target::Data),
    _ebo(Primitive::VertexBuffer::Target::Elements) {
  // Create buffer array
  float* vert_array = new float[8 * vertices.size()];

  for (unsigned int i = 0; i < vertices.size(); i++) {
    vert_array[i * 8 + 0] = vertices[i].x;
    vert_array[i * 8 + 1] = vertices[i].y;
    vert_array[i * 8 + 2] = vertices[i].z;

    vert_array[i * 8 + 3] = normals[i].x;
    vert_array[i * 8 + 4] = normals[i].y;
    vert_array[i * 8 + 5] = normals[i].z;

    vert_array[i * 8 + 6] = textureCoords[i].x;
    vert_array[i * 8 + 7] = textureCoords[i].y;
  }
  _vbo.transfer(vert_array, 8 * vertices.size());

  delete [] vert_array;

  unsigned short* element_array = new unsigned short[elements.size()];
  for (unsigned int i = 0; i < elements.size(); i++) {
    element_array[i] = elements[i];
  }
  _ebo.transfer(element_array, elements.size());

  delete [] element_array;

  _vao.bind(_vbo);
  _vao.bind(_ebo);

  Primitive::VertexShader   vs = Primitive::VertexShader::fromFile("assets/shaders/vertex/position.glsl");
  Primitive::FragmentShader fs = Primitive::FragmentShader::fromFile("assets/shaders/fragment/colorize.glsl");
  Primitive::FragmentShader ls = Primitive::FragmentShader::fromFile("assets/shaders/fragment/directional_lighting.glsl");

  const Registry::Program& program = Engine::Object::basic().loadProgram("basic");

  _vao.useProgram(program);
  _vbo.defineInput("position", program.program(), 3, Primitive::VertexBuffer::Type::Float, false, 8, 0);
  _vbo.defineInput("normal",   program.program(), 3, Primitive::VertexBuffer::Type::Float, false, 8, 3);
  _vbo.defineInput("texcoord", program.program(), 2, Primitive::VertexBuffer::Type::Float, false, 8, 6);

  Primitive::Texture t = Primitive::Texture("resources/sample.png");
  _vao.bindTexture(0, t);
  _vao.uploadUniform("tex", 0);

  Primitive::Vector3 emission = {1.0f, 0.0f, 0.0f};
  _materials.push_back(Material(0.2f, 5.0f, 8.0f, emission, 15.0f));
  Primitive::Vector3 position  = {0.0f, 0.0f, 0.0f};
  Primitive::Vector3 direction = {0.0f, 1.0f, 0.0f};
  Primitive::Vector3 color     = {1.0f, 0.0f, 1.0f};
  _lights.push_back(Light(position,
                          direction,
                          color));

  _vao.uploadUniform("material.ambient_intensity", _materials[0].ambientIntensity());
  _vao.uploadUniform("material.diffuse_intensity", _materials[0].diffuseIntensity());
  _vao.uploadUniform("material.specular_intensity",  _materials[0].specularIntensity());
  _vao.uploadUniform("material.shininess",  _materials[0].shininess());
  _vao.uploadUniform("material.emission",  _materials[0].emission());

  _vao.uploadUniform("light.direction", _lights[0].direction());
  _vao.uploadUniform("light.color", _lights[0].color());
}

void Apsis::Model::Mesh::draw(const Primitive::Matrix& projection, const World::Camera& camera, const Primitive::Matrix& model) const {
  _vao.uploadUniform("projection", projection);
  _vao.uploadUniform("view", camera.view());
  _vao.uploadUniform("model", model);

  _vao.uploadUniform("camera", camera.eye());

  _vao.draw();
}
