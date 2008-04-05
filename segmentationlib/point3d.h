#include "../datalib/datacube.h"
#include "../datalib/testcubes.h"
#include <iostream>
#include <ostream>
#include <algorithm>
#include <queue>

using namespace std;

#ifndef POINT3D

#define POINT3D


class Point3D
{
friend class RegionGrowing;
public:
  Point3D();
  Point3D(int x,int  y, int z);
  Point3D(const Point3D &help) ;
  Point3D& operator= (const Point3D& p);
  int& operator[](int a) {return data[a];};
  bool operator< ( Point3D r);
  bool operator== ( Point3D r);  
  friend ostream& operator<<(ostream &ostr,Point3D feld);
  double value();
  double breakvalue();
  double GetTime();
  void SetTime(double i);
  

private:
  int data[3];
  double time;
  static DataCube* datcube;
};



#endif


