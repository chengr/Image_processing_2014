
#define cvmMul(src1,src2,dst) cvMatMulAdd(src1,src2,0,dst)
#define cvMatMul(src1,src2,dst) cvMatMulAdd((src1),(src2),NULL,(dst))
#define cvMatMulAdd(src1,src2,src3,dst) cvGEMM((src1),(src2),1.,(src3),1.,(dst),0)
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include "roation.h"
using namespace cv;
using namespace std;

void main(){       
	/*
	int in;
	cout<<"Degree:";
	cin>>in;
	roation_deg(in);
	*//*
	int deg=360;
	for(int i=0;i<=24;i++,deg=deg-15){
		roation_deg(deg);
		cout<<"deg:"<<deg<<endl;
		waitKey(0);
	}*/
	merge();
	cout<<"完成後再點及滑鼠:"<<endl;
	waitKey(0);
}
