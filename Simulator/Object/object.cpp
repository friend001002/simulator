#include "object.hpp"

using namespace std;

Vector3f::Vector3f()
{
  this->x = this->y = this->z = {};
}

Vector3f::Vector3f(float x, float y, float z)
{
  this->x = { x };
  this->y = { y };
  this->z = { z };
}

Vector3f::~Vector3f()
{
  this->x = this->y = this->z = {};
}

void Vector3f::Render(point3f begin)
{
  glDisable(GL_DEPTH_TEST);

  float prev_color[4];
  glGetFloatv(GL_CURRENT_COLOR, prev_color);

  glColor3f(1.0F, 1.0F, 0.0F);

  glBegin(GL_LINES);
    glVertex3f(begin.x,           begin.y,           begin.z);
    glVertex3f(begin.x + this->x, begin.y + this->y, begin.z + this->z);
  glEnd();

  glColor3f(prev_color[0], prev_color[1], prev_color[2]);

  glEnable(GL_DEPTH_TEST);
}

Geometry::Geometry()
{
}

Geometry::~Geometry()
{
  for (vector<point3f> el : this->faces)
  {
    el.empty();
  }

  this->faces.empty();
}

Geometry::Geometry(std::vector<std::vector<point3f>>& faces)
{
  this->faces = { faces };
}

Object::Object()
{
  this->_center = {};
  this->_dims.width = this->_dims.height = this->_dims.length = { 2.0F };

  this->_front = { 0.0F, 1.0F, 0.0F };

  this->Set_geometry();
}

/*Object::Object(const Object& obj)
{
  this->_height = { obj._height };
  this->_length = { obj._length };
  this->_width  = { obj._width };
  this->_x      = { obj._x };
  this->_y      = { obj._y };
  this->_z      = { obj._z };
}

Object Object::operator=(const Object& obj)
{
  if (&obj != this)
  {
    this->_height = { obj._height };
    this->_length = { obj._length };
    this->_width = { obj._width };
    this->_x = { obj._x };
    this->_y = { obj._y };
    this->_z = { obj._z };
  }

  return *this;
}

Object::Object(Object&& obj) noexcept
{
  if (&obj != this)
  {
    this->_height = { obj._height };
    this->_length = { obj._length };
    this->_width = { obj._width };
    this->_x = { obj._x };
    this->_y = { obj._y };
    this->_z = { obj._z };

    obj.~Object();
  }
}

Object Object::operator=(const Object&& obj)
{
  if (&obj != this)
  {
    this->_height = { obj._height };
    this->_length = { obj._length };
    this->_width = { obj._width };
    this->_x = { obj._x };
    this->_y = { obj._y };
    this->_z = { obj._z };

    obj.~Object();
  }

  return *this;
}*/

Object::Object(point3f center, dimensions dims, Vector3f front)
{
  this->_center = { center };

  this->_dims.width  = { fabs(dims.width)  };
  this->_dims.height = { fabs(dims.height) };
  this->_dims.length = { fabs(dims.length) };

  this->_front = { front };

  //this->Update_normal();
}

Object::~Object()
{
  this->_center = {};
  this->_dims = {};

  this->_front.~Vector3f();
  this->_velocity.~Vector3f();

  this->_collision_geom.~Geometry();
}

void Object::Render()
{
  float prev_color[4];
  glGetFloatv(GL_CURRENT_COLOR, prev_color);

  /*for (size_t i {}; i < this->_collision_faces.size(); ++i)
  {
    glColor3f(0.0F, (float)i / (float)this->_collision_faces.size(), 0.0F);

    glBegin(GL_TRIANGLES);

    for (size_t j {}; j < this->_collision_faces[i].vertices.size(); ++j)
    {
      glVertex3f(  this->_collision_faces[i].vertices[j].x 
                  , this->_collision_faces[i].vertices[j].y 
                  , this->_collision_faces[i].vertices[j].z);
    }

    glEnd();
  }*/

  size_t col {};

  for (vector<point3f> face : this->_collision_geom.faces)
  {
    glColor3f(0.0F, (float)col / (float)this->_collision_geom.faces.size(), 0.0F);

    glBegin(GL_TRIANGLES);

    for (point3f vertex : face)
    {
      glVertex3f(vertex.x, vertex.y, vertex.z);
    }

    glEnd();

    ++col;
  }

  glColor3f(prev_color[0], prev_color[1], prev_color[2]);
}

/*void Object::Update_normal()
{
  this->_normal.x = this->_x + 1.0F;
  this->_normal.y = this->_y;
  this->_normal.z = this->_z;*/

  /*vector3f va {  this->_collision_vertices[1].x - this->_collision_vertices[0].x
               , this->_collision_vertices[1].y - this->_collision_vertices[0].y
               , this->_collision_vertices[1].z - this->_collision_vertices[0].z };

  vector3f vb {  this->_collision_vertices[2].x - this->_collision_vertices[1].x
               , this->_collision_vertices[2].y - this->_collision_vertices[1].y
               , this->_collision_vertices[2].z - this->_collision_vertices[1].z };

  this->_normal = {  va.y * vb.z - va.z * vb.y
                   , va.z * vb.x - va.x * vb.z
                   , va.x * vb.y - va.y * vb.x };
}*/

void Object::Set_geometry(/*vector<geom_face> vertices*/)
{
  /*for (geom_face face : vertices)
  {
    this->_collision_faces.push_back(face);
  }*/

  this->_collision_geom.faces.push_back( {  { -1.0F, -1.0F,  1.0F } // 1
                                          , { -1.0F,  1.0F, -1.0F }
                                          , { -1.0F, -1.0F, -1.0F }
                                         }
                                       );

  this->_collision_geom.faces.push_back( {  { -1.0F, -1.0F,  1.0F }
                                          , { -1.0F,  1.0F,  1.0F }
                                          , { -1.0F,  1.0F, -1.0F }
                                         }
                                       );

  this->_collision_geom.faces.push_back( {  { -1.0F, -1.0F,  1.0F } // 2
                                          , { -1.0F, -1.0F, -1.0F }
                                          , {  1.0F, -1.0F, -1.0F }
                                         }
                                       );

  this->_collision_geom.faces.push_back( {  { -1.0F, -1.0F,  1.0F }
                                          , {  1.0F, -1.0F, -1.0F }
                                          , {  1.0F, -1.0F,  1.0F }
                                         }
                                       );

  this->_collision_geom.faces.push_back( {  {  1.0F,  1.0F,  1.0F } // 3
                                          , {  1.0F, -1.0F,  1.0F }
                                          , {  1.0F, -1.0F, -1.0F }
                                         }
                                       );

  this->_collision_geom.faces.push_back( {  {  1.0F, -1.0F, -1.0F }
                                          , {  1.0F,  1.0F, -1.0F }
                                          , {  1.0F,  1.0F,  1.0F }
                                         }
                                       );

  this->_collision_geom.faces.push_back( {  {  1.0F, 1.0F,  1.0F } // 4
                                          , {  1.0F, 1.0F, -1.0F }
                                          , { -1.0F, 1.0F,  1.0F }
                                         }
                                       );

  this->_collision_geom.faces.push_back( {  {  1.0F, 1.0F, -1.0F }
                                          , { -1.0F, 1.0F, -1.0F }
                                          , { -1.0F, 1.0F,  1.0F }
                                         }
                                       );

  this->_collision_geom.faces.push_back( {  { -1.0F, -1.0F, 1.0F } // 5
                                          , {  1.0F, -1.0F, 1.0F }
                                          , {  1.0F,  1.0F, 1.0F }
                                         }
                                       );

  this->_collision_geom.faces.push_back( {  {  1.0F,  1.0F, 1.0F }
                                          , { -1.0F,  1.0F, 1.0F }
                                          , { -1.0F, -1.0F, 1.0F }
                                         }
                                       );

  this->_collision_geom.faces.push_back( {  {  1.0F,  1.0F, -1.0F } // 6
                                          , {  1.0F, -1.0F, -1.0F }
                                          , { -1.0F, -1.0F, -1.0F }
                                         }
                                       );

  this->_collision_geom.faces.push_back( {  { -1.0F, -1.0F, -1.0F }
                                          , { -1.0F,  1.0F, -1.0F }
                                          , {  1.0F,  1.0F, -1.0F }
                                         }
                                       );
}

void Object::Move(float dx, float dy, float dz)
{
  this->_center.x += dx;
  this->_center.y += dy;
  this->_center.z += dz;

  /*for (size_t i{}; i < this->_collision_geom.size(); ++i)
  {
    for (size_t j{}; j < this->_collision_geom[i].vertices.size(); ++j)
    {
      this->_collision_geom[i].vertices[j].x += dx;
      this->_collision_geom[i].vertices[j].y += dy;
      this->_collision_geom[i].vertices[j].z += dz;
    }
  }*/

  for (vector<point3f>& face : this->_collision_geom.faces)
  {
    for (point3f& vertex : face)
    {
      vertex.x += dx;
      vertex.y += dy;
      vertex.z += dz;
    }
  }
}

void Object::Place(float x, float y, float z)
{
  /*for (size_t i{}; i < this->_collision_faces.size(); ++i)
  {
    for (size_t j{}; j < this->_collision_faces[i].vertices.size(); ++j)
    {
      if (this->_x != x)
      {
        this->_collision_faces[i].vertices[j].x += x;
      }

      if (this->_y != y)
      {
        this->_collision_faces[i].vertices[j].y += y;
      }

      if (this->_z != z)
      {
        this->_collision_faces[i].vertices[j].z += z;
      }
    }
  }

  this->_x = { x };
  this->_y = { y };
  this->_z = { z };

  this->Update_vertices();
  this->Update_normal();*/

  float dx { x - this->_center.x };
  float dy { y - this->_center.y };
  float dz { z - this->_center.z };

  this->Move(dx, dy, dz);
}

void Object::Scale(dimensions new_dims)
{
  //this->_dims  = { new_dims };


}

void Object::Scale_rel(dimensions d_dims)
{
  //this->_dims.width  += d_dims.width;
  //this->_dims.height += d_dims.height;
  //this->_dims.length += d_dims.length;
}

void Object::Rotate(Vector3f new_front)
{
  
}

void Object::RotateRel(float rot_matrix[9])
{
  this->_front.x = { rot_matrix[0] * this->_front.x + rot_matrix[1] * this->_front.y + rot_matrix[2] * this->_front.z };
  this->_front.y = { rot_matrix[3] * this->_front.x + rot_matrix[4] * this->_front.y + rot_matrix[5] * this->_front.z };
  this->_front.z = { rot_matrix[6] * this->_front.x + rot_matrix[7] * this->_front.y + rot_matrix[8] * this->_front.z };

  for (vector<point3f>& face : this->_collision_geom.faces)
  {
    for (point3f& vertex : face)
    {
      vertex.x = { rot_matrix[0] * vertex.x + rot_matrix[1] * vertex.y + rot_matrix[2] * vertex.z };
      vertex.y = { rot_matrix[3] * vertex.x + rot_matrix[4] * vertex.y + rot_matrix[5] * vertex.z };
      vertex.z = { rot_matrix[6] * vertex.x + rot_matrix[7] * vertex.y + rot_matrix[8] * vertex.z };
    }
  }
}

point3f Object::Get_center()
{
  return this->_center;
}

Vector3f Object::Get_front()
{
  return this->_front;
}

Vector3f Object::Get_velocity()
{
  return this->_velocity;
}

Axes::Axes()
{
  this->_center.x = this->_center.y = this->_center.z = {};

  this->_lenght = this->_thickness = { 1.0F };
}

Axes::Axes(point3f center, float lenght, float thickness)
{
  this->_center = { center };

  this->_lenght    = { fabs(lenght) };
  this->_thickness = { fabs(thickness) };
}

void Axes::Draw()
{
  glDisable(GL_DEPTH_TEST);

  float prev_color[4];
  glGetFloatv(GL_CURRENT_COLOR, prev_color);

  static GLuint axes_list { glGenLists(1) };
  glNewList(axes_list, GL_COMPILE);

  glColor3f(1.0F, 0.0F, 0.0F);
  glLineWidth(this->_thickness);

  glBegin(GL_LINE_STRIP);
    glVertex3f(this->_center.x,                         this->_center.y,         this->_center.z);
    glVertex3f(this->_center.x + this->_lenght + 1.0F,  this->_center.y,         this->_center.z);
    glVertex3f(this->_center.x + this->_lenght + 0.75f, this->_center.y + 0.25f, this->_center.z);
    glVertex3f(this->_center.x + this->_lenght + 0.75f, this->_center.y - 0.25f, this->_center.z);
    glVertex3f(this->_center.x + this->_lenght + 1.0f,  this->_center.y,         this->_center.z);
    glVertex3f(this->_center.x + this->_lenght + 0.75f, this->_center.y,         this->_center.z + 0.25f);
    glVertex3f(this->_center.x + this->_lenght + 0.75f, this->_center.y,         this->_center.z - 0.25f);
    glVertex3f(this->_center.x + this->_lenght + 1.0f,  this->_center.y,         this->_center.z);
  glEnd();

  glColor3f(0.0F, 1.0F, 0.0F);

  glBegin(GL_LINE_STRIP);
    glVertex3f(this->_center.x,         this->_center.y,                         this->_center.z);
    glVertex3f(this->_center.x,         this->_center.y + this->_lenght + 1.0f,  this->_center.z);
    glVertex3f(this->_center.x,         this->_center.y + this->_lenght + 0.75f, this->_center.z + 0.25f);
    glVertex3f(this->_center.x,         this->_center.y + this->_lenght + 0.75f, this->_center.z - 0.25f);
    glVertex3f(this->_center.x,         this->_center.y + this->_lenght + 1.0f,  this->_center.z);
    glVertex3f(this->_center.x + 0.25f, this->_center.y + this->_lenght + 0.75f, this->_center.z);
    glVertex3f(this->_center.x - 0.25f, this->_center.y + this->_lenght + 0.75f, this->_center.z);
    glVertex3f(this->_center.x,         this->_center.y + this->_lenght + 1.0f,  this->_center.z);
  glEnd();

  glColor3f(0.0F, 0.0F, 1.0F);

  glBegin(GL_LINE_STRIP);
    glVertex3f(this->_center.x,         this->_center.y,         this->_center.z);
    glVertex3f(this->_center.x,         this->_center.y,         this->_center.z + this->_lenght + 1.0f);
    glVertex3f(this->_center.x + 0.25f, this->_center.y,         this->_center.z + this->_lenght + 0.75f);
    glVertex3f(this->_center.x - 0.25f, this->_center.y,         this->_center.z + this->_lenght + 0.75f);
    glVertex3f(this->_center.x,         this->_center.y,         this->_center.z + this->_lenght + 1.0f);
    glVertex3f(this->_center.x,         this->_center.y + 0.25f, this->_center.z + this->_lenght + 0.75f);
    glVertex3f(this->_center.x,         this->_center.y - 0.25f, this->_center.z + this->_lenght + 0.75f);
    glVertex3f(this->_center.x,         this->_center.y,         this->_center.z + this->_lenght + 1.0f);
  glEnd();

  glEndList();

  glCallList(axes_list);

  glColor3f(prev_color[0], prev_color[1], prev_color[2]);

  glEnable(GL_DEPTH_TEST);
}

void Axes::Move(float dx, float dy, float dz)
{
  this->_center.x += dx;
  this->_center.y += dy;
  this->_center.z += dz;
}

void Axes::Place(float x, float y, float z)
{
  this->_center.x = { x };
  this->_center.y = { y };
  this->_center.z = { z };
}

void Axes::Scale(float lenght, float thickness)
{
  this->_lenght    = { lenght };
  this->_thickness = { thickness };
}

void Axes::ScaleRel(float d_lenght, float d_thickness)
{
  this->_lenght    += d_lenght;
  this->_thickness += d_thickness;
}

Collada::Collada(string file)
{
  this->_file = { file };
}

Collada::~Collada()
{

}

/*Geometry Collada::Get_geometry()
{
  Geometry ret;

  const struct aiScene* scene { aiImportFile(this->_file.c_str(), aiProcessPreset_TargetRealtime_MaxQuality) };

  if (scene)
  {
    vector<point3f> tmp_face;

    for (int curr_mesh {}; curr_mesh < scene->mNumMeshes; ++curr_mesh)
    {
      const struct aiMesh* mesh = scene->mMeshes[curr_mesh];
      size_t counter {};

      for (int curr_face {}; curr_face < mesh->mNumFaces; ++curr_face)
      {
        const struct aiFace* face = &mesh->mFaces[curr_face];
        GLenum face_mode {};

        switch (face->mNumIndices)
        {
          case 1:
            face_mode = GL_POINTS;
            break;

          case 2:
            face_mode = GL_LINES;
            break;

          case 3:
            face_mode = GL_TRIANGLES;
            break;

          default:
            face_mode = GL_POLYGON;
            break;
        }

        tmp_face.clear();

        for (int curr_index {}; curr_index < face->mNumIndices; ++curr_index)
        {
          tmp_face.push_back( {  mesh->mVertices[curr_index + counter].x
                               , mesh->mVertices[curr_index + counter].y
                               , mesh->mVertices[curr_index + counter].z } );

          cout << mesh->mVertices[curr_index + counter].x << ' ' 
               << mesh->mVertices[curr_index + counter].y << ' ' 
               << mesh->mVertices[curr_index + counter].z << '\n';
        }

        ++counter;

        ret.faces.push_back(tmp_face);
      }
    }
  }



  return ret;
}*/
