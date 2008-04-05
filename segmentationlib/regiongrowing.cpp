#include "regiongrowing.h" 
 

/*! constructor
 *
 */
RegionGrowing::
RegionGrowing(DataCube* dc, DataCube* sc,DataCube* oc,RegionGrowingTypes v, int T)
{
  Point3D::datcube=dc;
  stateCube=new DataCube(*dc);
  stateCube->SetAll(-2);
  printCube=sc;
  OtsuCube=oc;
  vMode=v;
  stopTime=T;
}

RegionGrowing::
RegionGrowing(DataCube* dc, DataCube* sc,RegionGrowingTypes v, int T)
{
  Point3D::datcube=dc;
   stateCube=new DataCube(*dc);
  stateCube->SetAll(-2);
  printCube=sc;
  vMode=v;
  stopTime=T;
}

/*! Sets the Point (x,y,z), where Regiongrowing should begin
 *
 */ 

void 
RegionGrowing::
SetStartPoint(int x,int y, int z)
{
  StartPoint=Point3D(x,y,z);
  SetState(x,  y,  z , 0); 
  LastPoint=StartPoint;
  (*printCube)(x,y,z)=1;
  cout<<"Startpunkt gesetzt"<<endl;
}

/*! takes the Point, with the smallest time out of the list,writes this time in the StateCube and sets the value of that same point in the PrintCube to 1
 *
 */
 
void 
RegionGrowing::
GetSmallest()
{
cout <<"Get smallest" << endl;
  LastPoint=liste.front();
  SetState(LastPoint[0],LastPoint[1],LastPoint[2],LastPoint.GetTime());
  liste.pop_front();
  (*printCube)(LastPoint[0],LastPoint[1],LastPoint[2])=1;
}

/*! returns the value of the StateCube at (x,y,z)
 *
 */

double 
RegionGrowing::
GetState(int x, int y, int z)
{
  return (*stateCube)(x,y,z);
}

/*! sets the value of the StateCube at (x,y,z) to value w
 *
 */

void 
RegionGrowing::
SetState(int x, int y, int z, double w)
{
  (*stateCube)(x,y,z)=w;
}

/*! returns value of StateCube at (x,y,z), if it is positiv and in the datacube;if not the value of NOTMEMBERTIME(=infinity)
 *
 */
 
double
RegionGrowing::
GetTime(int x, int y, int z)
{
  if (is_in_datacube(x,y,z) && GetState(x,y,z)>=0)
    return GetState(x,y,z);
  else return NOTMEMBERTIME;
}

/*! Calculate velocity at (x,y,z)
 *
 */

double
RegionGrowing::
GetVelo(int x,int y,int z,double grvalue)
{
  switch (vMode)
    {
    case GrayvalueDivOtsuvalue:
      return 1/ pow(grvalue,25);
      break;
    case Grayvalue:
      return 1/pow(grvalue,5);
      break;
    }
 
}

/*! Calculates the time at (x,y,z)
 *
 */
 
double 
RegionGrowing::
Time(int x, int y, int z, double grvalue )
{
  double v=GetVelo(x,y,z,grvalue);
  double t1=GetTime(x+1, y, z);
  double t2=GetTime(x, y-1, z);
  double t3=GetTime(x-1,y,z);
  double t4=GetTime(x,y+1,z);
  double t5=GetTime(x,y,z+1);
  double t6=GetTime(x,y,z-1);
  priority_queue<double, vector<float>, greater<float> > tq;// calculate max  of time
  tq.push(t1+v);
  tq.push(t2+v);
  tq.push(t3+v);
  tq.push(t4+v);
  tq.push(t5+v);
  tq.push(t6+v);
  double tx=tq.top();
  return tx;
}  
   
/*! returns last point, which was taken from the list
 *
 */

Point3D*  
RegionGrowing:: 
GetLastPoint()
{
  return &LastPoint;
}

// Point3D* 
// RegionGrowing:: 
// GetStartPoint()
// {
//   return &StartPoint;
// }

/*! prints the list with the points from the border of the segmentated structure with some values
 *
 */

void
RegionGrowing:: 
PrintListe()
{
 list<Point3D>::const_iterator pos;
  for ( pos = liste.begin(); pos != liste.end(); ++pos){
     cout << *pos << ' '; 
 }
 cout<<endl; 
}

/*! inserts the  Point with calculated time in the list of the borderpoints
 *
 */

void 
RegionGrowing::
Einfuegen(Point3D* Punkt)
{
  double t=Time((*Punkt)[0],(*Punkt)[1],(*Punkt)[2], Punkt->value());
  Punkt->SetTime(t) ;
  list<Point3D>::iterator pos;
  pos=liste.begin();
  while ((*pos).GetTime()<(*Punkt).GetTime() && pos!=liste.end())
	 {++pos;}	 
  liste.insert(pos, *Punkt);   
  SetState((*Punkt)[0],(*Punkt)[1],(*Punkt)[2], t);
}

/*! returns true if the Point with (x,y,z) is not in the list and not in the structure at this time
 *
 */
 
bool 
RegionGrowing::
isnot_in_statecube(int x, int y, int z)
{
  if (GetState(x,y,z)<=0) { return true;}
  else return false;
}

// bool 
// RegionGrowing::
// is_in_list(int x, int y, int z)
// {
//   if (GetState(x,y,z)==-1) {return true;}
//       else return false;
// }

/*! returns true if the Point with (x,y,z) is in the datacube
 *
 */
bool 
RegionGrowing::
is_in_datacube(int x, int y, int z)
{
  if (x>=0 && y>=0 && z>=0 && x<((*stateCube).GetSize()[1]) && y<((*stateCube).GetSize()[2]) && z<((*stateCube).GetSize()[3])){ return true;}
  else return false;
}

/*! inserts the 6 Neighbourpoints of point, if they exist in the list of the borderpoints
 *
 */

void 
RegionGrowing::
SetNeighbourPoints(Point3D* Punkt)
{ 
cout <<"set neighbourpoints" << endl;
  Point3D* NP1=new Point3D((Punkt->data[0])+1,Punkt->data[1],Punkt->data[2]);
  if (is_in_datacube((*NP1)[0],(*NP1)[1],(*NP1)[2]))
    {
      //if (is_in_list((*NP1)[0],(*NP1)[1],(*NP1)[2]))
      //{liste.remove(*NP1);
      //Einfuegen(NP1);}
      if (isnot_in_statecube((*NP1)[0],(*NP1)[1],(*NP1)[2]))
	{Einfuegen(NP1);}
      else delete NP1;
     
    }
  Point3D* NP2=new Point3D(Punkt->data[0],(Punkt->data[1])-1, Punkt->data[2]);
  if (is_in_datacube((*NP2)[0],(*NP2)[1],(*NP2)[2]))
    {
      //if (is_in_list((*NP2)[0],(*NP2)[1],(*NP2)[2]))
      //{liste.remove(*NP2);
      //Einfuegen(NP2);}
      if (isnot_in_statecube((*NP2)[0],(*NP2)[1],(*NP2)[2]))
	{Einfuegen(NP2);}
      else delete NP2;
    }
  Point3D* NP3=new Point3D(Punkt->data[0]-1,Punkt->data[1], Punkt->data[2]);
  if (is_in_datacube((*NP3)[0],(*NP3)[1],(*NP3)[2]))
    { 
      //if (is_in_list((*NP3)[0],(*NP3)[1],(*NP3)[2]))
      //{liste.remove(*NP3);
      //Einfuegen(NP3);}
      if (isnot_in_statecube((*NP3)[0],(*NP3)[1],(*NP3)[2]))
	{Einfuegen(NP3);}
      else delete NP3;
      }
  Point3D* NP4=new Point3D(Punkt->data[0],Punkt->data[1]+1, Punkt->data[2]);
  if (is_in_datacube((*NP4)[0],(*NP4)[1],(*NP4)[2]))
    {
      // if (is_in_list((*NP4)[0],(*NP4)[1],(*NP4)[2]))
// 	{liste.remove(*NP4);
// 	Einfuegen(NP4);}
      if (isnot_in_statecube((*NP4)[0],(*NP4)[1],(*NP4)[2]))
	{Einfuegen(NP4);}
      else delete NP4;
    } 
  Point3D* NP5=new Point3D(Punkt->data[0],Punkt->data[1], Punkt->data[2]+1);
  if (is_in_datacube((*NP5)[0],(*NP5)[1],(*NP5)[2]))
    {
      //if (is_in_list((*NP5)[0],(*NP5)[1],(*NP5)[2]))
	//{liste.remove(*NP5);
	//Einfuegen(NP5);}
      if (isnot_in_statecube((*NP5)[0],(*NP5)[1],(*NP5)[2]))
	{Einfuegen(NP5);}
      else delete NP5;
    }
 Point3D* NP6=new Point3D(Punkt->data[0],Punkt->data[1], Punkt->data[2]-1);
  if (is_in_datacube((*NP6)[0],(*NP6)[1],(*NP6)[2]))
    {
      //if (is_in_list((*NP6)[0],(*NP6)[1],(*NP6)[2]))
      //{liste.remove(*NP6);
      //Einfuegen(NP6);}
      if (isnot_in_statecube((*NP6)[0],(*NP6)[1],(*NP6)[2]))
	{Einfuegen(NP6);}
      else delete NP6;
      }
}
/*! calculate until the time of all points in the list of borderpoints  >= StopTime 
 *
 */

void 
RegionGrowing::calculateRegionGrowing()
{
//  char statecube[500];
//   char info[500];
//   int i=0;
  
 
  while (GetState(LastPoint[0],LastPoint[1], LastPoint[2])<stopTime)
    { 
//       sprintf(statecube,".../examples/tmp/pc%d_%05d", vMode, i);
//       sprintf(info,".../examples/tmp/info%d_%05d",vMode,i);
//       printCube->writeRAW(statecube,info );
      // for (int j=0; j<200; j++)
	{
	  // PrintListe();
	  
	  SetNeighbourPoints(GetLastPoint());
	  GetSmallest();
	  // PrintListe();
	}
      //   i++;
    }
  //   printCube->writeSlicesPPM("tmp/sc3%5d.ppm",3);
  //   printCube->writeSlicesPPM("tmp/sc2%5d.ppm",2);
  //   printCube->writeSlicesPPM("tmp/sc1%5d.ppm",1);
  cout <<"regiongrowing end" << endl;
}
  
		      

