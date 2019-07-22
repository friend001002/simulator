#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include <vector>

#include "../Object/object.hpp"

class World
{
  public:

  World();
  World(const std::vector<Object>& objects);

  virtual ~World();

  bool Add_object();

  void Physics_step();

  private:

  std::vector<Object> _objects;
};

#endif // _WORLD_HPP_
