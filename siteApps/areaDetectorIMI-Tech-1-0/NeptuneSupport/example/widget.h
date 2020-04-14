#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QPixmap>

#include "NeptuneAPI.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


public:
    NCamHandle  m_cam;

    int         m_iWidth;
    int         m_iHeight;

    unsigned int m_uiRecvCount;

    bool        m_bStart;
    unsigned char*       m_pRGBBuf;

public:
    void        RecvFrame(NImageFrame *pData);
    static void callback_imgrecv(NImageFrame *pData);

    void    UpdateUIState(int iType); // 1 : Open, 2 : Start, 3 : Stop

public slots:
    void    slotDisplay(QPixmap pixmap);

signals:
    void    sigDisplay(QPixmap pixmap);

private slots:
    void on_btnRefresh_clicked();

    void on_cmbCameraList_activated(int index);

    void on_btnStart_clicked();

    void on_btnStop_clicked();

    void on_cmbPixelFormat_activated(const QString &arg1);

    void on_btnGain_clicked();

    void on_btnShutter_clicked();

    void on_chkGainAuto_clicked(bool checked);

    void on_chkShutterAuto_clicked(bool checked);


    void on_chkTriggerEnable_clicked(bool checked);

    void on_cmbTriggerSource_activated(const QString &arg1);

    void on_btnTriggerPara_clicked();

    void on_btnRunSW_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
