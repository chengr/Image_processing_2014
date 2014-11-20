#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <math.h>
#include "roation.h"
using namespace cv;
using namespace std;
////
Mat image;
Mat src,src2;
/// Global variables
//存對映點
float input_p[8][2];
//點及對映點次數
int ads=0;
int ll=0;
//滑鼠事件
void mouse(int event, int x, int y, int flags, void* param);
void merge(){
   char* warp_window;
   char*  warp_window2;
	/// Load the image
	//左圖
   src = imread("l6.jpg", 1 );
   //右圖
   src2 = imread("r6.jpg", 1 );
   warp_window = "左圖";
   warp_window2 = "右圖";
	namedWindow( warp_window, CV_WINDOW_AUTOSIZE );
	namedWindow( warp_window2, CV_WINDOW_AUTOSIZE );
	imshow( warp_window, src );
    imshow( warp_window2, src2 );
	//設定滑鼠
	cvSetMouseCallback(warp_window, mouse);
	cvSetMouseCallback(warp_window2, mouse);
}
//畫出合成圖
void draw(){
	//對映點
	float uv1=input_p[1][0]*input_p[1][1];
	float uv2=input_p[3][0]*input_p[3][1];
	float uv3=input_p[5][0]*input_p[5][1];
	float uv4=input_p[7][0]*input_p[7][1];
	//解A*x=x' need 4 points
	//8*8
	float Arrayx1[]={uv1,input_p[1][0],input_p[1][1],1,
					 uv2,input_p[3][0],input_p[3][1],1,
					 uv3,input_p[5][0],input_p[5][1],1,
					 uv4,input_p[7][0],input_p[7][1],1};
	float Arrayy1[]={uv1,input_p[1][0],input_p[1][1],1,
					 uv2,input_p[3][0],input_p[3][1],1,
					 uv3,input_p[5][0],input_p[5][1],1,
					 uv4,input_p[7][0],input_p[7][1],1};
	float Arrayx2[]={input_p[0][0],input_p[2][0],input_p[4][0],input_p[6][0]};
	float Arrayy2[]={input_p[0][1],input_p[2][1],input_p[4][1],input_p[6][1]};
	//x'
	CvMat *M_x1=cvCreateMat(4,4,CV_32FC1);
    CvMat *M_x2=cvCreateMat(4,1,CV_32FC1);
	//x
    CvMat *SolveSetx=cvCreateMat(4,1,CV_32FC1);
    cvSetData(M_x1,Arrayx1,M_x1->step);
    cvSetData(M_x2,Arrayx2,M_x2->step);
    cvSolve(M_x1,M_x2,SolveSetx,CV_LU);
	///
	CvMat *M_y1=cvCreateMat(4,4,CV_32FC1);
    CvMat *M_y2=cvCreateMat(4,1,CV_32FC1);
	//x
    CvMat *SolveSety=cvCreateMat(4,1,CV_32FC1);
    cvSetData(M_y1,Arrayy1,M_y1->step);
    cvSetData(M_y2,Arrayy2,M_y2->step);
    cvSolve(M_y1,M_y2,SolveSety,CV_SVD);
	//
	float a1=cvGet2D(SolveSetx,0,0).val[0];
	float a2=cvGet2D(SolveSetx,1,0).val[0];
	float a3=cvGet2D(SolveSetx,2,0).val[0];
	float a4=cvGet2D(SolveSetx,3,0).val[0];
	float a5=cvGet2D(SolveSety,0,0).val[0];
	float a6=cvGet2D(SolveSety,1,0).val[0];
	float a7=cvGet2D(SolveSety,2,0).val[0];
	float a8=cvGet2D(SolveSety,3,0).val[0];
	//公式
	int mx=(int)(a1*src.cols*src.rows+a2*src.cols+a3*src.rows+a4);
	int my=(int)(a5*src.cols*src.rows+a6*src.cols+a7*src.rows+a8);
	if(src.rows>my)
		my=src.rows;
	//cout<<"mx:"<<mx<<endl;
	//cout<<"my:"<<my<<endl;
	//設定圖片
	Mat image(my+3,mx+3 , CV_8UC3, Scalar(0,0,0));
	//SECOND INVERSE
	 uv1=input_p[0][0]*input_p[0][1];
	 uv2=input_p[2][0]*input_p[2][1];
	 uv3=input_p[4][0]*input_p[4][1];
	 uv4=input_p[6][0]*input_p[6][1];
	//解A*x=x' need 4 points
	//8*8
	float Arrayx21[]={uv1,input_p[0][0],input_p[0][1],1,
					 uv2,input_p[2][0],input_p[2][1],1,
					 uv3,input_p[4][0],input_p[4][1],1,
					 uv4,input_p[6][0],input_p[6][1],1};
	float Arrayy21[]={uv1,input_p[0][0],input_p[0][1],1,
					 uv2,input_p[2][0],input_p[2][1],1,
					 uv3,input_p[4][0],input_p[4][1],1,
					 uv4,input_p[6][0],input_p[6][1],1};
	float Arrayx22[]={input_p[1][0],input_p[3][0],input_p[5][0],input_p[7][0]};
	float Arrayy22[]={input_p[1][1],input_p[3][1],input_p[5][1],input_p[7][1]};
	//x'
	CvMat *M_x21=cvCreateMat(4,4,CV_32FC1);
    CvMat *M_x22=cvCreateMat(4,1,CV_32FC1);
	//x
    CvMat *SolveSetx2=cvCreateMat(4,1,CV_32FC1);
    cvSetData(M_x21,Arrayx21,M_x21->step);
    cvSetData(M_x22,Arrayx22,M_x22->step);
    cvSolve(M_x21,M_x22,SolveSetx2,CV_LU);
    //PrintMatrix(SolveSetx2,SolveSetx2->rows,SolveSetx2->cols);
	///
	CvMat *M_y21=cvCreateMat(4,4,CV_32FC1);
    CvMat *M_y22=cvCreateMat(4,1,CV_32FC1);
	//x
    CvMat *SolveSety2=cvCreateMat(4,1,CV_32FC1);
    cvSetData(M_y21,Arrayy21,M_y21->step);
    cvSetData(M_y22,Arrayy22,M_y22->step);
    cvSolve(M_y21,M_y22,SolveSety2,CV_SVD);
    //PrintMatrix(SolveSety2,SolveSety2->rows,SolveSety2->cols);
	 a1=cvGet2D(SolveSetx2,0,0).val[0];
	 a2=cvGet2D(SolveSetx2,1,0).val[0];
	 a3=cvGet2D(SolveSetx2,2,0).val[0];
	 a4=cvGet2D(SolveSetx2,3,0).val[0];
	 a5=cvGet2D(SolveSety2,0,0).val[0];
	 a6=cvGet2D(SolveSety2,1,0).val[0];
	 a7=cvGet2D(SolveSety2,2,0).val[0];
	 a8=cvGet2D(SolveSety2,3,0).val[0];
	//inverse mapping
	for(int y=0;y<image.rows;y++)
	{
		for(int x=0;x<image.cols;x++)
		{
			//roation
			Vec3b color;
			double vx= 
					a1*x*y+a2*x+a3*y+a4;
			double vy= 
					a5*x*y+a6*x+a7*y+a8;
			if(vx>=0&&vx<src.cols&&vy>=0&&vy<src.rows){
				//原圖 color(BGR)
				color[0]=bilinear_interpolation(vx,vy,0);
				color[1]=bilinear_interpolation(vx,vy,1);
				color[2]=bilinear_interpolation(vx,vy,2);
				image.at<Vec3b>(Point(x,y)) = color;
			}
			else{       
				color[0]=0;
				color[1]=0;
				color[2]=0;
				image.at<Vec3b>(Point(x,y)) = color;
			}  
		}
	}
	//info. 寫入左圖
	 for(int y=0;y<src.rows;y++)
	{
		for(int x=0;x<src.cols;x++)
		{
			 
			Vec3b color2=image.at<Vec3b>(Point(x,y));
			if(color2[0]==0&&color2[1]==0&&color2[2]==0){
					Vec3b color;
					color = src.at<Vec3b>(Point(x,y));
					image.at<Vec3b>(Point(x,y)) = color;
			}
			else{
				Vec3b color;

				color = src.at<Vec3b>(Point(x,y));
				color[0]=sqrt(color[0]*color2[0]);
				color[1]=sqrt(color[1]*color2[1]);
				color[2]=sqrt(color[2]*color2[2]);
				image.at<Vec3b>(Point(x,y)) = color;
			}
		}
	}
	//對比 亮度 調整
	 /*
	 double alpha=1.3; //< Simple contrast control 
	int beta=5;  //< Simple brightness control
	for( int y = 0; y < image.rows; y++ )
    { 
		for( int x = 0; x < image.cols; x++ )
         { 
			 for( int c = 0; c < 3; c++ )
              {
				  image.at<Vec3b>(y,x)[c] =
					 saturate_cast<uchar>( alpha*( image.at<Vec3b>(y,x)[c] ) + beta );
             }
		 }
    }*/
    namedWindow( "After", CV_WINDOW_AUTOSIZE );
    imshow( "After", image );
}
//bilinear_interpolation
int bilinear_interpolation(double x,double y,int i){
	double xmin = (int)x;
	double xmax =(int)x+1;
	double ymin = (int)y;
	double ymax =(int)y+1;
	//左上(xmin,ymax)----------(xmax,ymax)

	//    (xmin,ymin)----------(xmax,ymin)
	double a=x-xmin;
	double b=ymax-y;
	//src為原圖
	int c0=0,c1=0,c2=0,c3=0;
	if(check(xmin,ymax)){
		Vec3b color0=src2.at<Vec3b>(Point(xmin,ymax));			
		c0=color0[i];          
	}
	if(check(xmax,ymax)){
		Vec3b color1=src2.at<Vec3b>(Point(xmax,ymax));
		c1=color1[i];
	}
	if(check(xmin,ymin)){
		Vec3b color2=src2.at<Vec3b>(Point(xmin,ymin));
		c2=color2[i];
	}
	if(check(xmax,ymin)){
		Vec3b color3=src2.at<Vec3b>(Point(xmax,ymin));
		c3=color3[i];
	}
	int color_v=(1-a)*(1-b)*c0+a*(1-b)*c2+(1-a)*b*c1+a*b*c3;

	return color_v;
}
//檢查是否出界
bool check(double x,double y){
	int xx=x;
	int yy=y;
	if(xx>=0&&xx<src.cols&&yy>=0&&yy<src.rows)
		return true;
	else
		return false;
}

void PrintMatrix(CvMat *Matrix,int Rows,int Cols)
{
	
    for(int i=0;i<Rows;i++)
    {
        for(int j=0;j<Cols;j++)
        {
            printf("%.2f ",cvGet2D(Matrix,i,j).val[0]);
        }
        printf("\n");
    }
	 printf("ddd %d\n",Rows);
	printf("ddd %d\n",Cols);
}
//滑鼠事件 test
void mouse(int event, int x, int y, int flags, void* param)
{
	if(ads<8){
		if(event==CV_EVENT_LBUTTONDOWN||event==CV_EVENT_RBUTTONDOWN){
			printf("now x: %d y: %d\n", x,y);
			input_p[ads][0]=x;
			input_p[ads][1]=y;
			ads++;
		}
	}
	else{
		if(event==CV_EVENT_LBUTTONDOWN||event==CV_EVENT_RBUTTONDOWN){
			printf("DONE....");
			draw();
		}
	}
}