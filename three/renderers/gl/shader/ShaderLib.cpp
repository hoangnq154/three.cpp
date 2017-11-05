//
// Created by byter on 18.10.17.
//

#include "ShaderLib.h"

static void qInitResource()
{
  static bool done = false;

  if (done) return;
  done = true;

  //must do this outside of namespace - hence this function
  Q_INIT_RESOURCE(ShaderLib);
  Q_INIT_RESOURCE(ShaderChunk);
}

namespace three {
namespace gl {

class Shader;

class LibShader : public three::Shader
{
  friend class three::gl::Shader;

  ShaderID _id;
  QResource _vertex;
  QByteArray _uncompressedVertex;
  const char *_vertexShader = nullptr;

  QResource _fragment;
  QByteArray _uncompressedFragment;
  const char *_fragmentShader = nullptr;

public:
  LibShader(ShaderID id, const UniformValues &uniforms, const char *vertexShader, const char *fragmentShader)
     : three::Shader("", uniforms), _id(id), _vertex(vertexShader), _fragment(fragmentShader)
  {}

  LibShader(const LibShader &shader)
     : three::Shader("", shader._uniforms), _id(shader._id), _vertex(shader._vertexShader), _fragment(shader._fragmentShader)
  {}

  const std::string vertexShader() override
  {
    if (_vertexShader) return _vertexShader;

    if (_vertex.isCompressed()) {
      _uncompressedVertex = qUncompress(_vertex.data(), _vertex.size());
      _vertexShader = _uncompressedVertex.data();
    }
    else
      _vertexShader = (const char *) _vertex.data();

    return _vertexShader;
  }

  const std::string fragmentShader() override
  {
    if (_fragmentShader) return _fragmentShader;

    if (_fragment.isCompressed()) {
      _uncompressedFragment = qUncompress(_fragment.data(), _fragment.size());
      _fragmentShader = _uncompressedFragment.data();
    }
    else
      _fragmentShader = (const char *) _fragment.data();

    return _fragmentShader;
  }
};

class Shader : public three::Shader
{
  LibShader &_libShader;

public:
  Shader(const char *name, LibShader &libShader) : three::Shader(name, _libShader._uniforms), _libShader(libShader)
  {}

  const std::string vertexShader() override
  {
    return _libShader.vertexShader();
  }

  const std::string fragmentShader() override
  {
    return _libShader.fragmentShader();
  }
};

class Shaders
{
  std::unordered_map<ShaderID, three::gl::LibShader> _shaders;

public:
  Shaders()
  {
    qInitResource();

    _shaders.emplace(std::make_pair(ShaderID::basic, LibShader(ShaderID::basic,
                                                               uniformslib::merge(
                                                                  {
                                                                     UniformsID::common,
                                                                     UniformsID::specularmap,
                                                                     UniformsID::envmap,
                                                                     UniformsID::aomap,
                                                                     UniformsID::lightmap,
                                                                     UniformsID::fog
                                                                  }
                                                               ),
                                                               ":chunk/cube_vert.glsl",
                                                               ":chunk/cube_frag.glsl"
    )));
    _shaders.emplace(std::make_pair(ShaderID::lambert, LibShader(ShaderID::lambert,
                                                                 uniformslib::merge(
                                                                    {
                                                                       UniformsID::common,
                                                                       UniformsID::specularmap,
                                                                       UniformsID::envmap,
                                                                       UniformsID::aomap,
                                                                       UniformsID::lightmap,
                                                                       UniformsID::emissivemap,
                                                                       UniformsID::fog,
                                                                       UniformsID::lights
                                                                    }
                                                                 ),
                                                                 ":chunk/meshlambert_vert.glsl",
                                                                 ":chunk/meshlambert_frag.glsl"
    )));
    _shaders.emplace(std::make_pair(ShaderID::phong, LibShader(ShaderID::phong,
                                                               uniformslib::merge(
                                                                  {
                                                                     UniformsID::common,
                                                                     UniformsID::specularmap,
                                                                     UniformsID::envmap,
                                                                     UniformsID::aomap,
                                                                     UniformsID::lightmap,
                                                                     UniformsID::emissivemap,
                                                                     UniformsID::bumpmap,
                                                                     UniformsID::normalmap,
                                                                     UniformsID::displacementmap,
                                                                     UniformsID::gradientmap,
                                                                     UniformsID::fog,
                                                                     UniformsID::lights
                                                                  }
                                                               ),
                                                               ":chunk/meshphong_vert.glsl",
                                                               ":chunk/meshphong_frag.glsl"
    )));
    _shaders.emplace(std::make_pair(ShaderID::standard, LibShader(ShaderID::standard,
                                                                  uniformslib::merge(
                                                                     {
                                                                        UniformsID::common,
                                                                        UniformsID::envmap,
                                                                        UniformsID::aomap,
                                                                        UniformsID::lightmap,
                                                                        UniformsID::emissivemap,
                                                                        UniformsID::bumpmap,
                                                                        UniformsID::normalmap,
                                                                        UniformsID::displacementmap,
                                                                        UniformsID::roughnessmap,
                                                                        UniformsID::metalnessmap,
                                                                        UniformsID::fog,
                                                                        UniformsID::lights
                                                                     }
                                                                  ),
                                                                  ":chunk/meshphysical_vert.glsl",
                                                                  ":chunk/meshphysical_frag.glsl"
    )));
    _shaders.emplace(std::make_pair(ShaderID::points, LibShader(ShaderID::points,
                                                                uniformslib::merge(
                                                                   {
                                                                      UniformsID::points,
                                                                      UniformsID::fog
                                                                   }
                                                                ),
                                                                ":chunk/points_vert.glsl",
                                                                ":chunk/points_frag.glsl"
    )));
    _shaders.emplace(std::make_pair(ShaderID::dashed, LibShader(ShaderID::dashed,
                                                                uniformslib::merge(
                                                                   {
                                                                      UniformsID::common,
                                                                      UniformsID::fog
                                                                   }
                                                                ),
                                                                ":chunk/linedashed_vert.glsl",
                                                                ":chunk/linedashed_frag.glsl"
    )));
    _shaders.emplace(std::make_pair(ShaderID::depth, LibShader(ShaderID::depth,
                                                               uniformslib::merge(
                                                                  {
                                                                     UniformsID::common,
                                                                     UniformsID::displacementmap
                                                                  }
                                                               ),
                                                               ":chunk/depth_vert.glsl",
                                                               ":chunk/depth_frag.glsl"
    )));
    _shaders.emplace(std::make_pair(ShaderID::normal, LibShader(ShaderID::normal,
                                                                uniformslib::merge(
                                                                   {
                                                                      UniformsID::common,
                                                                      UniformsID::bumpmap,
                                                                      UniformsID::normalmap,
                                                                      UniformsID::displacementmap
                                                                   }
                                                                ),
                                                                ":chunk/normal_vert.glsl",
                                                                ":chunk/normal_frag.glsl"
    )));
    _shaders.emplace(std::make_pair(ShaderID::cube, LibShader(ShaderID::cube,
                                                              {
                                                                 uniformslib::Cube(nullptr),
                                                                 uniformslib::Flip(-1),
                                                                 uniformslib::Opacity(1.0f)
                                                              },
                                                              ":chunk/cube_vert.glsl",
                                                              ":chunk/cube_frag.glsl"
    )));
    _shaders.emplace(std::make_pair(ShaderID::equirect, LibShader(ShaderID::equirect,
                                                                  {
                                                                     uniformslib::Equirect(nullptr),
                                                                  },
                                                                  ":chunk/equirect_vert.glsl",
                                                                  ":chunk/equirect_frag.glsl"
    )));
    _shaders.emplace(std::make_pair(ShaderID::distanceRGBA, LibShader(ShaderID::distanceRGBA,
                                                                      uniformslib::merge(
                                                                         {
                                                                            UniformsID::common,
                                                                            UniformsID::displacementmap
                                                                         }
                                                                      ),
                                                                      ":chunk/distanceRGBA_vert.glsl",
                                                                      ":chunk/distanceRGBA_frag.glsl"
    )));
    _shaders.emplace(std::make_pair(ShaderID::shadow, LibShader(ShaderID::shadow,
                                                                uniformslib::merge(
                                                                   {
                                                                      UniformsID::lights,
                                                                      UniformsID::fog
                                                                   }
                                                                ),
                                                                ":chunk/distanceRGBA_vert.glsl",
                                                                ":chunk/distanceRGBA_frag.glsl"
    )));
  }

  LibShader &operator[](ShaderID id)
  {
    return _shaders.at(id);
  }
};

namespace shaderlib {

LibShader &_get(ShaderID id)
{
  static Shaders shaders;

  return shaders[id];
}

three::Shader &get(ShaderID id)
{
  return _get(id);
}

three::Shader::Ptr get(ShaderID id, const char *name)
{
  return three::Shader::Ptr(new three::gl::Shader(name, _get(id)));
}

}

}
}