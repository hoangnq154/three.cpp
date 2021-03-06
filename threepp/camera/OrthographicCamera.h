//
// Created by byter on 09.09.17.
//

#ifndef THREEPP_ORTOGRAPHICCAMERA_H
#define THREEPP_ORTOGRAPHICCAMERA_H

#include "Camera.h"

namespace three {

class DLX OrthographicCamera : public Camera
{
  float _left;
  float _right;
  float _top;
  float _bottom;

protected:
  OrthographicCamera(float left, float right, float top, float bottom, float near, float far)
     : Camera(object::Typer(this), near, far), _left(left), _right(right), _top(top), _bottom(bottom)
  {
       updateProjectionMatrix();
  }

  OrthographicCamera(const OrthographicCamera &camera)
     : Camera(camera, object::Typer(this)), _left(camera._left), _right(camera._right), _top(camera._top), _bottom(camera._bottom)
  {
    updateProjectionMatrix();
  }

public:
  using Ptr = std::shared_ptr<OrthographicCamera>;
  static Ptr make(float left, float right, float top, float bottom, float near=0.1f, float far=2000.0f) {
    return Ptr(new OrthographicCamera(left, right, top, bottom, near, far));
  }

  OrthographicCamera *cloned() const override {
    return new OrthographicCamera(*this);
  }

  math::Ray ray(float x, float y) const override;

  void updateProjectionMatrix() override;

  void set(float left, float right, float top, float bottom) {
    _left = left; _right = right; _top = top; _bottom = bottom;
    updateProjectionMatrix();
  }

  void set(float left, float right, float top, float bottom, float near, float far) {
    _left = left; _right = right; _top = top; _bottom = bottom; _near = near; _far = far;
    updateProjectionMatrix();
  }

  float left() const {return _left;}
  float right() const {return _right;}
  float top() const {return _top;}
  float bottom() const {return _bottom;}

  void setLeft(float left) {_left = left;}
  void setRight(float right) {_right = right;}
  void setTop(float top) {_top = top;}
  void setBottom(float bottom) {_bottom = bottom;}
};

}
#endif //THREEPP_ORTOGRAPHICCAMERA_H
