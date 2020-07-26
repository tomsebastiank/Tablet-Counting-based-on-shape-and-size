#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;



int main()
{
 Mat imageRead,imageTemp1,imageTemp2,imageTemp3,imageBrownLabel, imageWhiteLabel,image1,image2; 


image1 = imread( "tablets.png", 3);
image2 = imread( "tablets.png", 3 );

imageRead = imread( "tablets.png", 0 );//reading image as a gray level image


imageRead.copyTo(imageTemp1);
imageRead.copyTo(imageTemp2);
imageRead.copyTo(imageTemp3);






  if( !imageRead.data )
    { return -1; }


  
  
  



imageTemp1 = imageRead>55;
imageTemp2 = imageRead<100;

imageTemp3 =  imageTemp1  & imageTemp2;


//  intensity level slicing between the levels specified. between 55 and 100. This is inorder to select brown tablets




int erosion_size = 3;  //morphological operation to obtain brown tablets only,removing the noise
 Mat element = getStructuringElement(cv::MORPH_RECT,
cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
cv::Point(erosion_size, erosion_size) );
 


erode(imageTemp3,imageTemp3,element);

Mat statsBrown;
Mat centroidsBrown;
  
int n=connectedComponentsWithStats(imageTemp3, imageBrownLabel, statsBrown,centroidsBrown,8,CV_32S);
 
int brownRound = n-1;// n-1 connected components and 1 background component


int rectX=0;
int rectY=0;
int rectWidth=0;
int rectHeight=0;

for(int i = 1; i < n; i++ )
  {
  rectX = statsBrown.ptr<int>(i)[0];
  rectY = statsBrown.ptr<int>(i)[1];
  rectWidth = statsBrown.ptr<int>(i)[2];
  rectHeight = statsBrown.ptr<int>(i)[3];
  
  	Point center( (rectX+(rectWidth)/2), (rectY+(rectHeight)/2)  );
        int radius = (rectWidth)/2;
  


 	 // circle center
 	circle( image1, center, 3, Scalar(0,255,0), -1, 8, 0 );
       // circle outline
       circle( image1, center, radius, Scalar(0,0,255), 3, 8, 0 );
  

      
   }
   
   
  
 
 
 
 
 
 
 //code for differntiating between white tablets
 
 imageTemp1 = imageRead>100;// thresholding the intensity levels above 100 and storing in imageTemp1 
 
erosion_size = 10;  //morphological operation to remove the connections
 element = getStructuringElement(cv::MORPH_RECT,
cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1),
cv::Point(erosion_size, erosion_size) );
 


erode(imageTemp1,imageTemp1,element);

Mat statsWhite;
Mat centroidsWhite;
int m=connectedComponentsWithStats(imageTemp1, imageWhiteLabel, statsWhite,centroidsWhite,8,CV_32S);
 

int whiteRound=0;
int whiteOblong=0;



for(int i = 1; i < m; i++ )
  {
 rectX = statsWhite.ptr<int>(i)[0];
  rectY = statsWhite.ptr<int>(i)[1];
  rectWidth = statsWhite.ptr<int>(i)[2];
  rectHeight = statsWhite.ptr<int>(i)[3];
  
int widthHeightDiff =abs(rectWidth-rectHeight);
  
Point center( (rectX+(rectWidth)/2), (rectY+(rectHeight)/2)  );
int radius = (rectWidth)/2+10;
  
  
  if(widthHeightDiff<5) 
  
  
  {
  
  // logic for differentiating circular and oblong white tablets,for circular bounding rectangle is almost square. 
  	

		whiteRound++;

 	 // circle center
 	circle( image2, center, 3, Scalar(0,255,0), -1, 8, 0 );
       // circle outline
       circle( image2, center, radius, Scalar(0,0,255), 3, 8, 0 );
  }
  
  else
  
  {
  whiteOblong++;
  
  circle( image2, center, 3, Scalar(255,255,0), -1, 8, 0 );
  
  
  }

      
   }
   
cout<<endl<<"In the given image there are  " << brownRound<<"  BROWN CIRCULAR tablets  " << whiteRound<<"  WHITE CIRCULAR tablets  "<< whiteOblong <<"  WHITE OBLONG tablets ";


  namedWindow( "BrownTablets", CV_WINDOW_AUTOSIZE );
  imshow( "BrownTablets", image1);
  
  namedWindow( "WhiteTablets", CV_WINDOW_AUTOSIZE );
  imshow( "WhiteTablets", image2);
  
 
imwrite("brown.png",image1);
imwrite("white.png",image2);

//final count display


  waitKey(0);
  return 0;
}
