#ifndef OCTREE_H
#define OCTREE_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

///------------- POINT ---------------------------------///
int porcentaje=0.1;

template<typename T>
class Point_3
{
  public:
  T x, y, z;
  Point_3(T a, T b, T c) {
      x=a;
      y=b;
      z=c;
  }
  Point_3() {
      x=-1;
      y=-1;
      z=-1;
  }
  bool operator !=(Point_3<T> &pnt)
  {
    return (x != pnt.x || y != pnt.y || z != pnt.z);
  }
  bool operator ==(Point_3<T> &pnt)
  {
    return (x == pnt.x && y == pnt.y && z == pnt.z);
  }

};

///------------- OCTREE NODO ---------------------------------///

template<typename T>
class ONode
{
 public:
      Point_3<T> lmin, lmax;
      int numPoints;
      vector<Point_3<T>> data;
      ONode<T>* child[8];
      ONode(Point_3<T> p1, Point_3<T> p2)
      {
          lmin=p1;
          lmax=p2;
          numPoints=(lmax.x - lmin.x)*(lmax.y - lmin.y)*(lmax.z - lmin.z)*porcentaje;
        for(int i=0;i<8;i++)
        {
            child[i] = nullptr;
        }
      }
};
///------------- OCTREE ---------------------------------///
template<typename T>
class Octree
{
  public:
  ONode<T>* root;

  Octree(Point_3<T> lower_bottom_left, Point_3<T> upper_front_right)
  {
    root = new ONode<T>(lower_bottom_left, upper_front_right);
  }
  bool find(Point_3<T> val, ONode<T>* &pnt, ONode<T>* &p)
  {
    ONode<T> *sig;
    pnt = root;
    p = nullptr;

    for( ;pnt; p = pnt, pnt = sig){
      sig = nullptr;
      for(int i = 0; i < 8; ++i)
      {
        if(pnt->child[i] && verificar_limite(val, pnt->child[i]))
        {
            sig = pnt->child[i];
        }
      }
      if(!sig)
      {
        for(auto point : pnt->data)
        {
          if(point == val)
          {
              return true;
          }
        }
        return false;
      }
    }
    return false;
  }

  bool find(Point_3<T> point)
  {
    ONode<T>* tmp, *tmp2;
    return (fit(point, root) && find(point, tmp, tmp2));
  }

  void insert(Point_3<T> point, ONode<T> *head = nullptr)
  {
    ONode<T> *pos, *tmp;
    if(verificar_limite(point, root) && !find(point, pos, tmp))
    {
      if(pos->data.size() == pos->numPoints)
      {
        vector<Point_3<T>>tmp = pos->data;
        tmp.push_back(point);
        pos->data.clear();
        T _mx = (pos->lmax.x + pos->lmax.x)/2, _my = (pos->lmax.y+pos->lmax.y)/2, _mz = (pos->lmax.z + pos->lmax.z)/2;
        pos->child[0] = new ONode<T>(Point_3<T>(pos->lmin.x, pos->lmin.y,pos->lmin.z), Point_3<T>(_mx, _my, _mz));
        pos->child[1] = new ONode<T>(Point_3<T>(_mx, pos->lmin.y,pos->lmin.z), Point_3<T>(pos->lmin.x, _my, _mz));
        pos->child[2] = new ONode<T>(Point_3<T>(pos->lmin.x, _my,pos->lmin.z), Point_3<T>(_mx, pos->lmin.y, _mz));
        pos->child[3] = new ONode<T>(Point_3<T>(_mx, _my,pos->lmin.z), Point_3<T>(pos->lmin.x, pos->lmin.y, _mz));
        pos->child[4] = new ONode<T>(Point_3<T>(pos->lmin.x, pos->lmin.y, _mz), Point_3<T>(_mx, _my,pos->lmin.z));
        pos->child[5] = new ONode<T>(Point_3<T>(_mx, pos->lmin.y, _mz), Point_3<T>(pos->lmin.x, _my,pos->lmin.z));
        pos->child[6] = new ONode<T>(Point_3<T>(pos->lmin.x, _my, _mz), Point_3<T>(_mx, pos->lmin.y,pos->lmin.z));
        pos->child[7] = new ONode<T>(Point_3<T>(_mx, _my, _mz), Point_3<T>(pos->lmin.x, pos->lmin.y,pos->lmin.z));
        for(auto p : tmp) insert(p, pos);
      }
      else
      {
        pos->data.push_back(point);
      }
    }
  }

  void borrar(Point_3<T> point)
  {
    ONode<T> *pos, *parent;
    if(fit(point, root) && find(point, pos, parent))
    {
      auto it = pos->data.begin();
      while(*it != point) ++it;
      pos->data.borrar(it);
      if(parent)
      {
        int count = 0;
        for(int i = 0; i < 8; ++i) count += parent->child[i]->data.size();
        if(count == pos->numPoints)
        {
          for(int i = 0; i < 8; ++i)
          {
            for(auto p : parent->child[i]->data)
              parent->data.push_back(p);
            delete parent->child[i];
            parent->child[i] = nullptr;
          }
        }}
    }
  }
  bool verificar_limite(Point_3<T> point, ONode<T> *ONode)
  {
    bool X = point.x < (ONode->lmax).x && point.x > (ONode->lmin).x;
    bool Y = point.y < (ONode->lmax).y && point.y > (ONode->lmin).y;
    bool Z = point.z < (ONode->lmax).z && point.z > (ONode->lmin).z;
    return (X && Y && Z);
  }

};

#endif // OCTREE_H
