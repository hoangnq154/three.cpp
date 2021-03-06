//
// Created by byter on 5/6/18.
//

#ifndef THREE_PP_CONVEXHULL_H
#define THREE_PP_CONVEXHULL_H

#include <threepp/quick/objects/ThreeQObject.h>
#include <threepp/quick/scene/Scene.h>
#include <threepp/material/MeshBasicMaterial.h>
#include <threepp/material/MeshLambertMaterial.h>
#include <threepp/objects/Mesh.h>
#include <threepp/extras/QuickHull.h>

namespace three {
namespace quick {

/**
 * creates a convex hull around the child object.
 */
class ConvexHull : public ThreeQObject
{
Q_OBJECT
  Q_PROPERTY(ThreeQObject *hulledObject READ hulledObject WRITE setHulledObject NOTIFY hulledObjectChanged)
  Q_CLASSINFO("DefaultProperty", "hulledObject")

  DynamicMesh::Ptr _mesh;

  ThreeQObject *_hulled = nullptr;
  OnObjectChangedId _hulledConnection = nullptr;

protected:
  three::Object3D::Ptr _create() override
  {
    BufferGeometry::Ptr geometry;

    if(_hulled) {
      _hulled->create(_scene, nullptr);
    }

    return nullptr;
  }

  void updateMaterial() override {
    _mesh->setMaterial(material()->getMaterial());
  }

  void updateHulled(Object3D::Ptr updated, ObjectState state)
  {
    if(state == ObjectState::Removed) {
      if(updated) {
        updated->remove(_object);
        _parentObject->remove(updated);
        _object->dispose();
      }
    }
    else if(updated) {
      _parentObject->add(updated);

      updated->updateMatrix();
      updated->updateMatrixWorld(true);

      extras::QuickHull hull(updated);
      BufferGeometry::Ptr geometry = hull.createGeometry();

      _mesh = DynamicMesh::make(geometry);
      _mesh->setMaterial(material()->getMaterial());

      _object = _mesh;
      _object->visible() = _visible;
      updated->add(_object);
    }
  }

public:
  ConvexHull(QObject *parent = nullptr) : ThreeQObject(parent) {}

  ThreeQObject *hulledObject() const {return _hulled;};

  void setHulledObject(ThreeQObject *hulled)
  {
    if(_hulled != hulled) {
      if(_hulledConnection) {
        _hulled->onObjectChanged.disconnect(_hulledConnection);
        _hulledConnection = nullptr;
      }
      _hulled = hulled;

      if(_parentObject) {
        auto hulledObject = _hulled->create(_scene, nullptr);
        if(hulledObject) {
          updateHulled(hulledObject, ObjectState::Added);
        }
      }
      else
        _hulledConnection = _hulled->onObjectChanged.connect(*this, &ConvexHull::updateHulled);

      emit hulledObjectChanged();
    }
  }

signals:
  void hulledObjectChanged();
};

}
}
#endif //THREE_PP_CONVEXHULL_H
