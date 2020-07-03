#include "OperationOverloader.h"
#include <cmath>
using namespace std;

Vector operator*(const Matrix& C,const Vector& V)
{
   int d = C.size();
   Vector W(d);
   for (int j=0; j<d; j++)
   {
      W[j]=0.0;
      for (int l=0; l<d; l++) W[j]=W[j]+C[j][l]*V[l];
   }
   return W;
}

Vector operator+(const Vector& V,const Vector& W)
{
   int d = V.size();
   Vector U(d);
    for (int j=0; j<d; j++) U[j] = V[j] + W[j];
   return U;
}

Vector operator+(const double& a,const Vector& V)
{
   int d = V.size();
   Vector U(d);
   for (int j=0; j<d; j++) U[j] = a + V[j];
   return U;
}

Vector operator-(const Vector& V, const Vector& W)
{
    int d = V.size();
    Vector U(d);
    for (int j = 0; j < d; j++) U[j] = V[j] - W[j];
    return U;
}

Vector operator-(const Vector& V, const double& a)
{
    int d = V.size();
    Vector U(d);
    for (int j = 0; j < d; j++) U[j] = V[j] - a;
    return U;
}

Vector operator*(const double& a,const Vector& V)
{
   int d = V.size();
   Vector U(d);
   for (int j=0; j<d; j++) U[j] = a*V[j];
   return U;
}

Vector operator*(const Vector& V,const Vector& W)
{
   int d = V.size();
   Vector U(d);
   for (int j=0; j<d; j++) U[j] = V[j] * W[j];
   return U;
}

Vector exp(const Vector& V)
{
   int d = V.size();
   Vector U(d);
   for (int j=0; j<d; j++) U[j] = exp(V[j]);
   return U;
}

double operator^(const Vector& V,const Vector& W)
{
   double sum = 0.0;
   int d = V.size();
   for (int j=0; j<d; j++) sum = sum + V[j]*W[j];
   return sum;
}

Vector operator+=(Vector& V, const Vector& W)
{
    for (int j = 0; j < V.size(); j++)
        V[j]+=W[j];
    return V;
}

Vector operator-=(Vector& V, const Vector& W)
{
    for (int j = 0; j < V.size(); j++) V[j] -= W[j];
    return V;
}

Vector operator/(const Vector &V, const double& a)
{
    int d = V.size();
    Vector U(d);
    for (int j = 0; j < d; j++) U[j] = V[j] / a;
    return U;
}

ostream & operator<<(ostream & out, Vector & V)
{
    for (Vector::iterator itr= V.begin(); itr != V.end(); itr++)
        out << *itr << "   ";
    out << endl;
    return out;
}

ostream & operator<<(ostream & out, Matrix & W)
{
    for (Matrix::iterator itr= W.begin(); itr != W.end(); itr++)
        out << *itr;
    out << endl;
    return out;
}

double average(const Vector& V)
{
    double average = 0.0;
    int d = V.size();
    for (int j = 0; j < d; j++) 
        average += V[j];
    return average/d;
}

