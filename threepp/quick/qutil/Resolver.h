//
// Created by byter on 5/23/18.
//

#ifndef THREE_PP_RESOLVER_H
#define THREE_PP_RESOLVER_H

namespace three {
namespace quick {

class Material;
class ShaderMaterial;
class MeshPhongMaterial;
class MeshToonMaterial;
class MeshStandardMaterial;
class MeshLambertMaterial;
class MeshBasicMaterial;

namespace material {

using Typer = three::Typer<Material, ShaderMaterial, MeshPhongMaterial, MeshToonMaterial,
   MeshStandardMaterial, MeshLambertMaterial, MeshBasicMaterial>;

}

}
};
#endif //THREE_PP_RESOLVER_H
