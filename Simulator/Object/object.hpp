#ifndef _OBJECT_HPP_
#define _OBJECT_HPP_

#include "../Freeglut/include/GL/freeglut.h"

//#include "../Collada/colladainterface.h"

#include "..\Assimp\cimport.h"
#include "..\Assimp/scene.h"
#include "..\Assimp/postprocess.h"

#include <cmath>
#include <string>
#include <vector>
#include <iostream>

struct point3f
{
  float x;
  float y;
  float z;
};

class Vector3f
{
  public:

  Vector3f();
  Vector3f(float x, float y, float z);

  virtual ~Vector3f();

  float x;
  float y;
  float z;

  void Render(point3f start);
};

class Geometry
{
  public:

  Geometry();
  Geometry(std::vector<std::vector<point3f>>& faces);

  virtual ~Geometry();

  std::vector<std::vector<point3f>> faces;
};

struct dimensions
{
  float width;
  float height;
  float length;
};

class Object
{
  public:

  Object();
  //Object(const Object &obj);
  //Object(Object&& obj) noexcept;
  Object(point3f center, dimensions dims, Vector3f normal);

  //Object operator=(const Object& obj);
  //Object operator=(const Object&& obj);

  virtual ~Object();

  void Render();

  void Move (float dx, float dy, float dz);
  void Place(float x,  float y,  float z);

  void Scale(dimensions new_dims);
  void Scale_rel(dimensions d_dims);

  void Rotate(Vector3f new_front);
  void RotateRel(float rot_matrix[9]);

  point3f  Get_center();
  Vector3f Get_front();
  Vector3f Get_velocity();

  void Set_geometry(/*std::vector<geom_face> faces*/);

  private:

  point3f _center;
  dimensions _dims;

  Geometry _collision_geom;

  Vector3f _front;
  Vector3f _velocity;
};

class Axes
{
  public:

  Axes();
  Axes(point3f center, float lenght, float thickness);

  void Draw();

  void Move(float dx, float dy, float dz);
  void Place(float x, float y, float z);
  void Scale(float lenght, float thickness);
  void ScaleRel(float d_lenght, float d_thickness);

  private:

  point3f _center;

  float _lenght;
  float _thickness;
};

class Collada
{
  public:

  Collada(std::string file);

  virtual ~Collada();

  Geometry Get_geometry();

  private:

  std::string _file;
};

#endif // _OBJECT_HPP_
