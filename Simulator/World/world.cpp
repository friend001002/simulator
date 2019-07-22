#include "world.hpp"

using namespace std;

World::World()
{

}

World::World(const vector<Object>& objects)
{
  this->_objects = { objects };
}

World::~World()
{
  this->_objects.clear();
}

void World::Physics_step()
{

}
