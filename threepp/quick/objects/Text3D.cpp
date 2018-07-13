//
// Created by byter on 6/18/18.
//

#include "Text3D.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

#include <threepp/extras/core/Font.h>

namespace three {
namespace quick {

void loadFont(extras::FontData &fontData, const QJsonObject &json)
{
  fontData.resolution = (float)json["resolution"].toDouble();
  fontData.underlineThickness = (float)json["underlineThickness"].toDouble();

  const auto &bbox = json["boundingBox"].toObject();
  float xMin = (float)bbox["xMin"].toDouble();
  float yMin = (float)bbox["xMin"].toDouble();
  float xMax = (float)bbox["xMax"].toDouble();
  float yMax = (float)bbox["yMax"].toDouble();
  fontData.boundingBox = math::Box2({xMin, yMin}, {xMax, yMax});

  const auto &glyphs = json["glyphs"].toObject();
  for(const auto &key : glyphs.keys())
  {
    const auto &gl = glyphs[key].toObject();
    extras::Glyph &glyph = fontData.put(key.toStdWString().at(0));

    glyph.ha = gl["ha"].toInt();

    QString ol = gl["o"].toString();
    const auto &outline = ol.split(' ', QString::SkipEmptyParts);

    for(auto iter = outline.begin(); iter != outline.end(); ) {

      const auto &word = *iter;

      if(word == "m") { // moveTo

        int x = (*++iter).toInt();
        int y = (*++iter).toInt();

        glyph.moveTo(x, y);
      }
      else if(word == "l") { // lineTo

        int x = (*++iter).toInt();
        int y = (*++iter).toInt();

        glyph.lineTo(x, y);
      }
      else if(word == "q") { // quadraticCurveTo

        int x1 = (*++iter).toInt();
        int y1 = (*++iter).toInt();
        int x2 = (*++iter).toInt();
        int y2 = (*++iter).toInt();

        glyph.quadraticCurveTo(x1, y1, x2, y2 );
      }
      else if(word == "b") { // bezierCurveTo

        int x1 = (*++iter).toInt();
        int y1 = (*++iter).toInt();
        int x2 = (*++iter).toInt();
        int y2 = (*++iter).toInt();
        int x3 = (*++iter).toInt();
        int y3 = (*++iter).toInt();

        glyph.bezierCurveTo(x1, y1, x2, y2, x3, y3);
      }

      iter++;
    }
  }
}

void Text3D::append_material(QQmlListProperty<Material> *list, Material *obj)
{
  Text3D *item = qobject_cast<Text3D *>(list->object);
  if (item) item->_materials.append(obj);
}
int Text3D::count_materials(QQmlListProperty<Material> *list)
{
  Text3D *item = qobject_cast<Text3D *>(list->object);
  return item ? item->_materials.size() : 0;
}
Material *Text3D::material_at(QQmlListProperty<Material> *list, int index)
{
  Text3D *item = qobject_cast<Text3D *>(list->object);
  return item ? item->_materials.at(index) : nullptr;
}
void Text3D::clear_materials(QQmlListProperty<Material> *list)
{
  Text3D *item = qobject_cast<Text3D *>(list->object);
  if(item) item->_materials.clear();
}

QQmlListProperty<Material> Text3D::materials()
{
  return QQmlListProperty<Material>(this, nullptr,
                                    &Text3D::append_material,
                                    &Text3D::count_materials,
                                    &Text3D::material_at,
                                    &Text3D::clear_materials);
}

three::Object3D::Ptr Text3D::_create()
{
  QFile file(_font);
  file.open(QIODevice::Text | QIODevice::ReadOnly);
  QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

  extras::FontData fontData;
  loadFont(fontData, doc.object());

  geometry::TextOptions options {extras::Font(fontData)};
  options.height = _height;
  options.size = _size;
  options.curveSegments = _curveSegments;
  options.bevelThickness = _bevelThickness;
  options.bevelSize = _bevelSize;
  options.bevelEnabled = _bevelEnabled;

  switch(_geometryType) {
    case Three::BufferGeometry: {
      _mesh = DynamicMesh::make(geometry::buffer::Text::make(_text.toStdWString(), options));
      break;
    }
    case Three::LinearGeometry: {
      _mesh = DynamicMesh::make(geometry::Text::make(_text.toStdWString(), options));
      break;
    }
  }
  for(const auto &mat : _materials) {
    _mesh->addMaterial(mat->getMaterial());
  }
  return _mesh;
}

}
}