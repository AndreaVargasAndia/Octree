#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Octree.h"
using namespace std;


///------------- MAIN ---------------------------------///


int main()
{
    ifstream points("../files/eyeball.obj");
    string s;
    float x, y, z;
    Point_3 <float > p;

    vector < Point_3 <float> > limite;
    limite[0]=Point_3<float>(-400,-400,-400);
    limite[1]=Point_3<float>(400, 400, 400);

    Octree<float> tree(limite[0],limite[1]);

    //----Insertar todos los puntos en el octree ----//
    while (!points.eof()){
        points>> s;
        if(s != "v") break;
        points >> x >> y >> z;
        tree.insert(Point_3<float>(x, y, z));

    }
    tree.insert(Point_3<float>(6,7,9));
    tree.insert(Point_3<float>(3,5,4));
    tree.insert(Point_3<float>(3,5,4));


    return 0;


}
