#include <QCoreApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <tesseract/baseapi.h>
//#include <tesseract/tesseract.h>
#include <leptonica/allheaders.h>
#include <iostream>

using namespace tesseract;
using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    char *outText = nullptr;

    TessBaseAPI *api = new TessBaseAPI();

    //if(api->Init(NULL, "eng"))
    if(api->Init(nullptr, "eng")) {
        cerr << "Could Not Initialize tesseract" << endl;
        exit(-1);
    };

	FILE *fpng = fopen("./test.png", "rb");
    Pix *image = pixReadStreamPng(fpng);
	
	api->SetVariable("user_defined_dpi", "300");
	//api->SetVariable("tessedit_char_blacklist", "_");
	//api->SetVariable("classify_bln_numeric_mode", "1");
    //Pix *image = pixRead("./test.png");
    api->SetImage(image);

    outText = api->GetUTF8Text();

    //cout <<"OCR Out Text: \n" << outText << endl;
    cout <<"OCR Out Text: \n" << "\""<<outText << "\"" << endl;


    FILE *fp = fopen("./test.txt", "wb");
    fputs(outText, fp);
    fclose(fp);
	fclose(fpng);

    api->End();
    delete [] outText;
    pixDestroy(&image);

#if 0
	//opencv2 test
    Mat img_color;

    VideoCapture cap(0);

    if(!cap.isOpened())
    {
       cerr << "Camera Open Error!!" << endl;
       return(-1);
    };

    Size size = Size(static_cast<int>(cap.get(CAP_PROP_FRAME_WIDTH)), static_cast<int>(cap.get(CAP_PROP_FRAME_HEIGHT)));

    VideoWriter writer;
    double fps = 30.0;

    writer.open("output.avi", VideoWriter::fourcc('M','J', 'P','G'), fps, size,true);

    if(!writer.isOpened())
    {
        cerr << "Save Error!!" << endl;
        return(-1);
    }

    while(true)
    {
        cap.read(img_color);

        if(img_color.empty())
        {
            cerr << "Empty Frame Captured!!" << endl;
            break;
        };

        writer.write(img_color);
        imshow("Color", img_color);

        if(waitKey(25) >= 0) break;
    };
#endif

    return 0;
    //return a.exec();
}

