#include "point3d.h"


DataCube
* Point3D::
datcube; 

 

ostream &operator<<(ostream &ostr, Point3D feld) 
{
  ostr <<"x: "<<feld.data[0]<< " y: "<< feld.data[1]<<" z: "<<feld.data[2]<<" t: "<<feld.time<<" v: "<<feld.value()<< endl;
  return ostr;
};


Point3D::
Point3D()
{
  data[0]=0; 
  data[1]=0;
  data[2]=0;
  time=10;
}

Point3D::
Point3D(int x, int y, int z)
{
  data[0]=x; 
  data[1]=y; 
  data[2]=z;
  time=10;
}

Point3D::
Point3D(const Point3D &help)
{
  data[0]=help.data[0];
  data[1]=help.data[1];
  data[2]=help.data[2];
  time=help.time;
} 

Point3D& 
Point3D::
operator= (const Point3D& p)
{
  data[0]=p.data[0]; 
  data[1]=p.data[1]; 
  data[2]=p.data[2];
  time=p.time;
  return (*this);
}

bool 
Point3D::
operator== ( Point3D r)
{
  if(data[0]==r.data[0] && data[1]==r.data[1] && data[2]==r.data[2]) return true;
  else return false;
}

bool 
Point3D::
operator< (Point3D r)
{
  if(time<r.time) return true;
  else return false;
}



double 
Point3D::
GetTime()
{
  return time;
}

void 
Point3D::
SetTime(double i)
{
  time=i;
}

double 
Point3D::
value()
{
  return (*datcube)(data[0],data[1],data[2]);
}

double
Point3D::
breakvalue()
{


  double w1, w2, w3, w4,w5,w6; 
  if (data[0]<(*datcube).GetSize()[1]-1) w1=(*datcube)(data[0]+1, data[1]);
  else w1=0;
  if (data[1]>0) w2=(*datcube)(data[0], data[1]-1);
  else w2=0;
  if (data[0]>0) w3=(*datcube)(data[0]-1, data[1]);
  else w3=0;
  if (data[1]<(*datcube).GetSize()[2]-1) w4=(*datcube)(data[0], data[1]+1);
  else w4=0;
  if (data[2]<(*datcube).GetSize()[3]-1) w5=(*datcube)(data[0], data[1],data[2]+1);
  else w5=0;
  if (data[2]>0) w6=(*datcube)(data[0], data[1],data[2]-1);
  else w6=0;
  priority_queue<double,vector<float> > tq;
  double v=value();
  double tp1,tp2,tp3,tp4,tp5,tp6;
  if (w1==v) tp1=v;
  else tp1=pow((log(v)-log(w1))/(v-w1),30);
  if (w2==v) tp2=v;
  else tp2=pow((log(v)-log(w2))/(v-w2),30);
  if (w3==v) tp3=v;
  else tp3=pow((log(v)-log(w3))/(v-w3),30);
  if (w4==v) tp4=v;
  else tp4=pow((log(v)-log(w4))/(v-w4),30);
 if (w5==v) tp5=v;
  else tp5=pow((log(v)-log(w5))/(v-w5),30);
 if (w6==v) tp6=v;
  else tp6=pow((log(v)-log(w6))/(v-w6),30);
  tq.push(tp1);
  tq.push(tp2);
  tq.push(tp3);
  tq.push(tp4);
  tq.push(tp5);
  tq.push(tp6);
  v =pow(tq.top(),1);
  return v;
}


// double Point3D::GetVelo(int i) // gibt geschwindigkeit zurueck
// {
//   if (i==1)
//     return pow((*datcube)(data[0],data[1],data[2]),30);
  // if (i==2)
//     {
//       double v1, v2, v3, v4, v5, v6;
//       if (data[0]<(*datcube).GetSize()[1]-1) v1=(*datcube)(data[0]+1, data[1],data[2]);
//       else v1=0;
//       if (data[1]>0) v2=(*datcube)(data[0], data[1]-1,data[2]);
//       else v2=0;
//       if (data[0]>0) v3=(*datcube)(data[0]-1, data[1],data[2]);
//       else v3=0;
//       if (data[1]<(*datcube).GetSize()[2]-1) v4=(*datcube)(data[0], data[1]+1,data[2]);
//       else v4=0;
//       if (data[2]<(*datcube).GetSize()[3]-1) v5=(*datcube)(data[0], data[1],data[2]+1);
//       else v5=0;
//       if (data[0]>0) v6=(*datcube)(data[0], data[1],data[2]-1);
//       else v6=0;
//       priority_queue<double, vector<float>, greater<float> > tq;
//       tq.push(abs(v1-value()));
//       tq.push(abs(v2-value()));
//       tq.push(abs(v3-value()));
//       tq.push(abs(v4-value()));
//       tq.push(abs(v5-value()));
//       tq.push(abs(v6-value()));
//       double v=1/(1+tq.top());
//       return v;
//     }
//   if (i !=1 && i!=2) return 1;
//} 
