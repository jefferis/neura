#include <list>
#include <queue>
#include "point3d.h"
#define NOTMEMBERTIME 100000                                            

enum RegionGrowingTypes {GrayvalueDivOtsuvalue, Grayvalue};
class RegionGrowing
{
public:
  RegionGrowing(DataCube* dc, DataCube* sc,RegionGrowingTypes vMode, int T ); 
  RegionGrowing(DataCube* dc, DataCube* sc, DataCube* oc,RegionGrowingTypes vMode, int T ); 
  void SetStartPoint(int x, int y , int z);
  void calculateRegionGrowing();// vMode: Velocitymodus(1: (Grayvalue)^30; 2:(Grayvalue/otsuValue)^10) StopTime: Sets the 
  DataCube* printCube;

private:
  RegionGrowingTypes vMode;
  int stopTime;
  Point3D StartPoint;
  DataCube* OtsuCube;
  DataCube* stateCube;
  list<Point3D> liste;
  Point3D LastPoint;
  Point3D* GetStartPoint();
  Point3D* GetLastPoint();
  void SetNeighbourPoints(Point3D* LastPoint); //3-dim
  void PrintListe();
  void SetState(int x, int y, int z ,double w);
  double GetState(int x, int y, int z);
  double GetVelo(int x, int y, int z, double grvalue);
  double Time(int x, int y, int z, double grvalue );
  double GetTime(int x, int y, int z);
  void Einfuegen(Point3D* Punkt);
  void GetSmallest();
  /* bool is_in_list(int x, int y, int z); */
  bool isnot_in_statecube(int x, int y, int z);
  bool is_in_datacube(int x, int y, int z);

};
  
