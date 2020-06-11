#include <QCoreApplication>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <tesseract/baseapi.h>
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

    if(api->Init(NULL, "eng"))
    {
        cerr << "Could Not Initialize tesseract" << endl;
        exit(-1);
    };

    Pix *image = pixRead("/home/ctrluser/image/test.png");
    api->SetImage(image);

    outText = api->GetUTF8Text();

    cout <<"OCR Out Text: " << outText << endl;
    FILE *fp = fopen("/home/ctrluser/image/test.txt", "wb");
    fputs(outText, fp);
    fclose(fp);

    api->End();
    delete [] outText;
    pixDestroy(&image);

    return 0;
    //return a.exec();
}

