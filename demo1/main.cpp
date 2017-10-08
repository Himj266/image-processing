#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<bits/stdc++.h>
using namespace std;
using namespace cv;
long long sav=0;
Mat image=imread("lena.jpg",1);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EditImage
{
public:
void Zoom();
void help();
void show();
void saven(Mat img);
void save(Mat img);
void Crop();

void Brightness_Contrast();

void Rotate();

void Greyscale();
void blur();

void Menu();

};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EditImage::save(Mat img)
{
    imwrite("lena.jpg",img);
    image=imread("lena.jpg",1);
    cout<<"File Saved...\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EditImage::saven(Mat img)
{
    char s[500];
    sprintf(s,"%lld.jpg",sav++);
    imwrite(s,img);
    cout<<"Image Saved As:- "<<s<<"\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EditImage::Menu()
{
    char e='y';
	while(e!='n'&&e!='N'){
      system("clear");
	cout<<"1.Show Image\n2.Crop\n3.Brightness And Contrast\n4.Rotate\n5.Greyscale\n6.Blur\n7.Zoom\n8.Help\n9.Exit\nENTER YOUR CHOICE :-\n";
	int ch;
    cin>>ch;
    switch(ch)
    {
    case 1:show();
    break;
	case 2: Crop();
	break;
	case 3: Brightness_Contrast();
	break;
	case 4:Rotate();
	break;
	case 5:Greyscale();
	break;
	case 6:blur();
	break;
	case 7:Zoom();
	break;
	case 8: help();
	break;
	case 9:{
	    return;
	}
	default : cout<<"SORRY WRONG CHOICE\n";
    }
//    if(ch==1)
//    show();
//	else if(ch==2) Crop();
//	else if(ch==3) Brightness_Contrast();
//	else if(ch==4) Rotate();
//	else if(ch==5) Greyscale();
//	else if(ch==6) return;
//	else cout<<"SORRY WRONG CHOICE\n";
	cout<<"Do You Want to Continue? [y/n]...";
	cin>>e;
  }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int blurAmount = 15;   // starting value of tracker
Mat output;
void onChange(int sliderValue, void* userData){  //Trackbar call back function
	if (sliderValue <= 0)
		return;
//1st param =input image ;2nd param =output image ,3rd param =blur size
    blur(image, output, Size(sliderValue,sliderValue));
    imshow("BlurWindow", output);
}
void EditImage::blur(){
    system("clear");
    Mat img=image;
    namedWindow("BlurWindow");  //gui window
// 1st param= Name of Slider
// 2nd param=attached to the window ;
// 3rd param= Pass starting point of slider by refrence
// 4rd param=Maximum Value of Slider
// 5th param=Call back function name
	createTrackbar("Blur Value", "BlurWindow", &blurAmount, 50, onChange);
	imshow("BlurWindow", img); // Show Image
	 int key = waitKey(0);
            if(key=='n'||key=='N')
            saven(output);
            else if(key=='s'||key=='S')
            save(output);
	destroyAllWindows(); // exit on any key pressed
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EditImage::show(){
    system("clear");
    cout<<"1.Show Default Image\n2.Another Image\nEnter Your Choice:- ";
    int ch;
    cin>>ch;
    if(ch==1)
    {
        namedWindow("Image");
    imshow("Image",image);
    waitKey(-1);
    destroyWindow("Image");
    }
    else if(ch==2)
    {
        string s;
        cout<<"Enter Name Of The Image:- ";
        cin>>s;
        Mat image=imread(s);
        if(!image.data)
        {
            cout<<"File Not Found...\n";
            return;
        }
    namedWindow("Image");
    imshow("Image",image);
    waitKey(-1);
    destroyWindow("Image");
    }
    else
        cout<<"Wrong Choice\n";

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EditImage::Brightness_Contrast(){
      // Create a window
      system("clear");
     namedWindow("My Window", 1);

     //Create trackbar to change brightness
     int iSliderValue1 = 50;
     createTrackbar("Brightness", "My Window", &iSliderValue1, 100);

      //Create trackbar to change contrast
     int iSliderValue2 = 50;
     createTrackbar("Contrast", "My Window", &iSliderValue2, 100);

      while (true)
     {
          //Change the brightness and contrast of the image
          Mat dst;
          int iBrightness  = iSliderValue1 - 50;
          double dContrast = iSliderValue2 / 50.0;
          image.convertTo(dst, -1, dContrast, iBrightness);

           //show the brightness and contrast adjusted image
          imshow("My Window", dst);

          // Wait until user press some key for 50ms
          int key = waitKey(30);
            if(key=='n'||key=='N')
            saven(dst);
            else if(key=='s'||key=='S')
            save(dst);
            else if( key == 27 )
            {
                destroyAllWindows();
               break;
            }
     }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EditImage::Rotate(){
        system("clear");
        const char* pzRotatedImage = "Rotated Image";
        namedWindow( pzRotatedImage, CV_WINDOW_AUTOSIZE );

        int iAngle = 180;
        createTrackbar("Angle", pzRotatedImage, &iAngle, 360);

        int iImageHieght = image.rows / 2;
        int iImageWidth = image.cols / 2;

        while (true)
        {
            Mat matRotation = getRotationMatrix2D( Point(iImageWidth, iImageHieght), (-iAngle+180), 1 );

            // Rotate the image
            Mat imgRotated;
            warpAffine( image, imgRotated, matRotation, image.size() );
            imshow( pzRotatedImage, imgRotated );

            int key = waitKey(30);
            if(key=='n'||key=='N')
            saven(imgRotated);
            else if(key=='s'||key=='S')
            save(imgRotated);
            else if( key == 27 )
            {
                destroyAllWindows();
               break;
            }
        }

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mat img,ROI;
Rect cropRect(0,0,0,0);
 Point P1(0,0);
 Point P2(0,0);

const char* winName="Crop Image";
bool clicked=false;
void checkBoundary(){
       //check croping rectangle exceed image boundary
       if(cropRect.width>img.cols-cropRect.x)
         cropRect.width=img.cols-cropRect.x;

       if(cropRect.height>img.rows-cropRect.y)
         cropRect.height=img.rows-cropRect.y;

        if(cropRect.x<0)
         cropRect.x=0;

       if(cropRect.y<0)
         cropRect.height=0;
}
void showImage(){
    img=image.clone();
    checkBoundary();
    if(cropRect.width>0&&cropRect.height>0){
        ROI=image(cropRect);
        imshow("cropped",ROI);
    }

    rectangle(img, cropRect, Scalar(0,255,0), 4, 8, 0 );
    imshow(winName,img);
}
void onMouse( int event, int x, int y, int f, void* ){


    switch(event){

        case  CV_EVENT_LBUTTONDOWN  :
                                        clicked=true;

                                        P1.x=x;
                                        P1.y=y;
                                        P2.x=x;
                                        P2.y=y;
                                        break;

        case  CV_EVENT_LBUTTONUP    :
                                        P2.x=x;
                                        P2.y=y;
                                        clicked=false;
                                        break;

        case  CV_EVENT_MOUSEMOVE    :
                                        if(clicked){
                                        P2.x=x;
                                        P2.y=y;
                                        }
                                        break;

        default                     :   break;


    }


    if(clicked){
     if(P1.x>P2.x){ cropRect.x=P2.x;
                       cropRect.width=P1.x-P2.x; }
        else {         cropRect.x=P1.x;
                       cropRect.width=P2.x-P1.x; }

        if(P1.y>P2.y){ cropRect.y=P2.y;
                       cropRect.height=P1.y-P2.y; }
        else {         cropRect.y=P1.y;
                       cropRect.height=P2.y-P1.y; }

    }


showImage();
}
void EditImage::Crop(){
    system("clear");
 cout<<"Click and drag for Selection"<<endl<<endl;

    cout<<"------> Press '8' to move up"<<endl;
    cout<<"------> Press '2' to move down"<<endl;
    cout<<"------> Press '6' to move right"<<endl;
    cout<<"------> Press '4' to move left"<<endl<<endl;

    cout<<"------> Press 'w' increas top"<<endl;
    cout<<"------> Press 'x' increas bottom"<<endl;
    cout<<"------> Press 'd' increas right"<<endl;
    cout<<"------> Press 'a' increas left"<<endl<<endl;

    cout<<"------> Press 't' decrease top"<<endl;
    cout<<"------> Press 'b' decrease bottom"<<endl;
    cout<<"------> Press 'h' decrease right"<<endl;
    cout<<"------> Press 'f' decrease left"<<endl<<endl;

    cout<<"------> Press 'r' to reset"<<endl;
    cout<<"------> Press 'Esc' to quit"<<endl<<endl;

    namedWindow(winName,WINDOW_NORMAL);
    setMouseCallback(winName,onMouse,NULL );
    imshow(winName,image);

    while(1){
    char c=waitKey();
    if(tolower(c)=='s'&&ROI.data){
    save(ROI);
    }
    else if(tolower(c)=='n'&&ROI.data)
        saven(ROI);
    else if(c=='6') cropRect.x++;
    else if(c=='4') cropRect.x--;
    else if(c=='8') cropRect.y--;
    else if(c=='2') cropRect.y++;

    else if(c=='w') { cropRect.y--; cropRect.height++;}
    else if(c=='d') cropRect.width++;
    else if(c=='x') cropRect.height++;
    else if(c=='a') { cropRect.x--; cropRect.width++;}

    else if(c=='t') { cropRect.y++; cropRect.height--;}
    else if(c=='h') cropRect.width--;
    else if(c=='b') cropRect.height--;
    else if(c=='f') { cropRect.x++; cropRect.width--;}

    else if(c==27) {
            destroyAllWindows();
            break;
    }
    else if(c=='r') {cropRect.x=0;cropRect.y=0;cropRect.width=0;cropRect.height=0;}
    showImage();

    }

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EditImage::Greyscale(){
       // Create a new matrix to hold the gray image
       system("clear");
       Mat gray;

       // convert RGB image to gray
       cvtColor(image, gray, CV_BGR2GRAY);
       namedWindow( "GreyScale", CV_WINDOW_AUTOSIZE );
       imshow( "GreyScale", gray );

       int key=waitKey(-1);
       if(key=='n'||key=='N')
        saven(gray);
       else if(key=='s'||key=='S')
        save(gray);
       destroyAllWindows();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Mat dst, tmp;
char* window_name = "Zoom";
void EditImage::Zoom()
{
   /// General instructions
   system("clear");
  printf( "\n Zoom In-Out demo  \n " );
  printf( "------------------ \n" );
  printf( " * [u] -> Zoom in  \n" );
  printf( " * [d] -> Zoom out \n" );
  printf( " * [ESC] -> Close program \n \n" );
  tmp = image;
  dst = tmp;

  /// Create window
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );
  imshow( window_name, dst );

  /// Loop
  while( true )
  {
    int c;
    c = waitKey(10);
    if(c=='s'||c=='S')
        save(dst);
    if(c=='n'||c=='N')
        saven(dst);

    if( (char)c == 27 )
      { destroyAllWindows();
          break; }
    if( (char)c == 'u' )
      { pyrUp( tmp, dst, Size( tmp.cols*2, tmp.rows*2 ) );
        printf( "** Zoom In: Image x 2 \n" );
      }
    else if( (char)c == 'd' )
     { pyrDown( tmp, dst, Size( tmp.cols/2, tmp.rows/2 ) );
       printf( "** Zoom Out: Image / 2 \n" );
     }

    imshow( window_name, dst );
    tmp = dst;
  }
}
void EditImage::help()
{
    system("clear");
    cout<<"------>Press 's' To Save Changes\n------>Press 'n' To Save As New Image\n";
    cout<<"------>Press 'u' For Zoom In & 'd' For Zoom Out In Case Of Zoom\n------>Press 'Esc' To Exit The Image Window\n";

}
int main()
{
//    cout<<"Press 's' To Save Changes\nPress 'n' to save as new image\nPress 'Esc' To Exit The Image Window";
//    delay(4000);
//    cout<<"Hello";
//    system("clear");
    EditImage E;
	E.Menu();
	return 0;
}
