#include "widget.h"
#include "ui_widget.h"

#include <QMessageBox>

Widget* g_inst = NULL;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    m_cam = NULL;
    m_bStart = false;

    g_inst = this;

    ui->cmbTriggerSource->addItem("Line1");
    ui->cmbTriggerSource->addItem("Software");
    on_btnRefresh_clicked();
    UpdateUIState(0);
}

Widget::~Widget()
{
    if(m_cam != NULL)
    {
        on_btnStop_clicked();
        NeptuneCloseCamera(m_cam);
        m_cam = NULL;

        ui->cmbPixelFormat->clear();
        UpdateUIState(0);
    }

    delete ui;
}

void Widget::UpdateUIState(int iType)
{
    ui->cmbCameraList->setEnabled(true);
    ui->cmbPixelFormat->setDisabled(true);
    ui->btnStart->setDisabled(true);
    ui->btnStop->setDisabled(true);

    ui->btnGain->setEnabled(true);
    ui->btnShutter->setEnabled(true);
    ui->chkGainAuto->setEnabled(true);
    ui->chkShutterAuto->setEnabled(true);
    ui->chkTriggerEnable->setEnabled(true);
    ui->cmbTriggerSource->setEnabled(true);
    ui->editTriggerParameter->setEnabled(true);
    ui->btnTriggerPara->setEnabled(true);

    if(iType == 0)
    {
        ui->editGain->setDisabled(true);
        ui->editShutter->setDisabled(true);
        ui->btnGain->setDisabled(true);
        ui->btnShutter->setDisabled(true);
        ui->chkGainAuto->setDisabled(true);
        ui->chkShutterAuto->setDisabled(true);
        ui->chkTriggerEnable->setDisabled(true);
        ui->cmbTriggerSource->setDisabled(true);
        ui->editTriggerParameter->setDisabled(true);
        ui->btnTriggerPara->setDisabled(true);
        ui->btnRunSW->setDisabled(true);
    }
    else if(iType == 1) // open
    {
        ui->cmbCameraList->setDisabled(true);
        ui->cmbPixelFormat->setEnabled(true);
        ui->btnStart->setEnabled(true);
        ui->btnStop->setDisabled(true);
    }
    else if(iType == 2) // start
    {
        ui->cmbCameraList->setDisabled(true);

        ui->cmbPixelFormat->setDisabled(true);

        ui->btnStart->setDisabled(true);
        ui->btnStop->setEnabled(true);
    }
    else if(iType == 3) // stop
    {
        ui->cmbCameraList->setDisabled(true);

        ui->cmbPixelFormat->setEnabled(true);

        ui->btnStart->setEnabled(true);
        ui->btnStop->setDisabled(true);
    }
}

void Widget::on_btnRefresh_clicked()
{
    int nNum = 0;

    if(m_cam != NULL)
    {
        on_btnStop_clicked();
        NeptuneCloseCamera(m_cam);
        m_cam = NULL;

        ui->cmbPixelFormat->clear();
        UpdateUIState(0);
    }

    NeptuneNumberOfCameras(&nNum);
    ui->cmbCameraList->clear();

    for(int i = 0 ; i < nNum ; i++)
    {
        char strID[64] = {0,};

        NeptuneGetDisplayName(i, strID);
        ui->cmbCameraList->addItem(strID);
    }
}

void Widget::on_cmbCameraList_activated(int index)
{
    char strID[64] = {0,};
    char strTmp[128] = {0,};
    NErr status = eNeptuneSuccess;

    NeptuneGetCameraID(index, strID);

    status = NeptuneOpenCamera(strID, &m_cam);
    if(status != eNeptuneSuccess) {
        QMessageBox::critical(this, "Error", QString("OpenCamera Error : %1").arg(strID));
        return;
    }

    // get size info
    NSizeInfo sinfo;
    NeptuneGetSizeInfo(m_cam, &sinfo);
    m_iWidth = sinfo.width;
    m_iHeight = sinfo.height;

    // get pixelformat list
    int npixels = 0;
    QString str;
    NeptuneGetNumAvailablePixelFormats(m_cam, &npixels);
    ui->cmbPixelFormat->clear();
    for(int i = 0 ; i < npixels ; i++)
    {
        NeptuneGetAvailablePixelFormatsIdxToStr(m_cam, i, strTmp);
        ui->cmbPixelFormat->addItem(strTmp);
    }

    NeptuneGetPixelFormatStr(m_cam, strTmp);
    for(int i = 0 ; i < ui->cmbPixelFormat->count() ; i++)
    {
        str = ui->cmbPixelFormat->itemText(i);
        if(strcmp(str.toLatin1().data(), strTmp) == 0) {
            ui->cmbPixelFormat->setCurrentIndex(i);
            break;
        }
    }

    // gain
    int value = 0;
    bool bAuto = false;
    NeptuneGetGain(m_cam, &value, &bAuto);
    ui->editGain->setText(QString("%1").arg(value));
    ui->chkGainAuto->setChecked(bAuto);
    if(bAuto == true)
        ui->editGain->setDisabled(true);
    else
        ui->editGain->setEnabled(true);

    // shutter
    unsigned int uiValue = 0;
    NeptuneGetShutter(m_cam, &uiValue, &bAuto);
    ui->editShutter->setText(QString("%1").arg(uiValue));
    ui->chkShutterAuto->setChecked(bAuto);
    if(bAuto == true)
        ui->editShutter->setDisabled(true);
    else
        ui->editShutter->setEnabled(true);
    // trigger
    NeptuneGetTriggerEnable(m_cam, &bAuto);
    ui->chkTriggerEnable->setChecked(bAuto);

    NeptuneGetTriggerSourceStr(m_cam, strTmp);
    if(strcmp(strTmp, "Line1") == 0)
    {
        ui->cmbTriggerSource->setCurrentIndex(0);
        ui->btnRunSW->setDisabled(true);
    }
    else
    {
        ui->cmbTriggerSource->setCurrentIndex(1);
        ui->btnRunSW->setEnabled(true);
    }

    NeptuneGetTriggerParameter(m_cam, &value);
    ui->editTriggerParameter->setText(QString("%1").arg(value));

    UpdateUIState(1);
}

void Widget::on_cmbPixelFormat_activated(const QString &arg1)
{
    NErr status = eNeptuneSuccess;

    status = NeptuneSetPixelFormatStr(m_cam, arg1.toLatin1().data());
    if(status != eNeptuneSuccess)
    {
        char strTmp[128] = {0,};
        QString str;

        NeptuneGetPixelFormatStr(m_cam, strTmp);
        for(int i = 0 ; i < ui->cmbPixelFormat->count() ; i++)
        {
            str = ui->cmbPixelFormat->itemText(i);
            if(strcmp(str.toLatin1().data(), strTmp) == 0) {
                ui->cmbPixelFormat->setCurrentIndex(i);
                break;
            }
        }

        QMessageBox::critical(this, "Error", QString("SetPixelformat Error : %1").arg(arg1));
    }
}

void ConvertToRGBData(unsigned char *pBuffer, unsigned int uiW, unsigned int uiH, QImage& image)
{
    image = QImage(pBuffer, uiW, uiH, QImage::Format_RGB888);
}

void Widget::slotDisplay(QPixmap pixmap)
{
    pixmap = pixmap.scaled(ui->laPic->geometry().width() - 2, ui->laPic->geometry().height() - 2, Qt::KeepAspectRatio);
    ui->laPic->setPixmap(pixmap);
}

void Widget::RecvFrame(NImageFrame *pData)
{
    QImage qimage;

    memcpy(m_pRGBBuf, pData->pRGBBuf, pData->uiWidth * pData->uiHeight * 3);
    ConvertToRGBData(m_pRGBBuf, pData->uiWidth, pData->uiHeight, qimage);

    ui->laRecvCount->setText(QString("Recieve Frame Count : %1").arg(++m_uiRecvCount));
    emit sigDisplay(QPixmap::fromImage(qimage));
}

void Widget::callback_imgrecv(NImageFrame *pData)
{
    Widget* wid = (Widget*)g_inst;
    wid->RecvFrame(pData);
}

void Widget::on_btnStart_clicked()
{
    QString str;

    connect(this, SIGNAL(sigDisplay(QPixmap)), this, SLOT(slotDisplay(QPixmap)));

    m_pRGBBuf = new unsigned char[m_iWidth * m_iHeight * 3];

    str = ui->cmbPixelFormat->currentText();
    if(str.contains("Bayer") == true)
        NeptuneSetBayerConversion(m_cam, eNeptuneBayerNearest);
    else
        NeptuneSetBayerConversion(m_cam, eNeptuneBayerNone);

    m_uiRecvCount = 0;
    NeptuneStartAcquisition(m_cam, callback_imgrecv);

    m_bStart = true;

    UpdateUIState(2);
}

void Widget::on_btnStop_clicked()
{
    m_bStart = false;

    disconnect(this, SIGNAL(sigDisplay(QPixmap)), this, SLOT(slotDisplay(QPixmap)));

    NeptuneStopAcquisition(m_cam);


    if(m_pRGBBuf != NULL)
    {
        delete [] m_pRGBBuf;
        m_pRGBBuf = NULL;
    }

    UpdateUIState(3);
}


void Widget::on_btnGain_clicked()
{
    int value = 0;
    bool bAuto = false;

    value = ui->editGain->text().toInt();
    bAuto = ui->chkGainAuto->isChecked();

    if(value < 0) value = 0;
    if(value > 511) value = 511;

    NeptuneSetGain(m_cam, value, bAuto);
    ui->editGain->setText(QString("%1").arg(value));

}

void Widget::on_btnShutter_clicked()
{
    unsigned int value = 0;
    bool bAuto = false;

    value = ui->editShutter->text().toInt();
    bAuto = ui->chkShutterAuto->isChecked();

    if(value < 1) value = 1;
    if(value > 3600000000) value = 3600000000;

    NeptuneSetShutter(m_cam, value, bAuto);
    ui->editShutter->setText(QString("%1").arg(value));

}

void Widget::on_chkGainAuto_clicked(bool checked)
{
    int value = 0;
    bool bAuto = false;
    if(checked == false)
    {
        NeptuneGetGain(m_cam, &value, &bAuto);
        ui->editGain->setText(QString("%1").arg(value));
        ui->editGain->setEnabled(true);
    }
    else
        ui->editGain->setDisabled(true);
}

void Widget::on_chkShutterAuto_clicked(bool checked)
{
    unsigned int value = 0;
    bool bAuto = false;
    if(checked == false)
    {
        NeptuneGetShutter(m_cam, &value, &bAuto);
        ui->editShutter->setText(QString("%1").arg(value));
        ui->editShutter->setEnabled(true);
    }
    else
        ui->editShutter->setDisabled(true);
}


void Widget::on_chkTriggerEnable_clicked(bool checked)
{
    NeptuneSetTriggerEnable(m_cam, checked);
    m_uiRecvCount = 0;
}

void Widget::on_cmbTriggerSource_activated(const QString &arg1)
{
    NeptuneSetTriggerSourceStr(m_cam, arg1.toLatin1().data());
    if(arg1.compare("Line1") == 0)
        ui->btnRunSW->setDisabled(true);
    else
        ui->btnRunSW->setEnabled(true);
}

void Widget::on_btnTriggerPara_clicked()
{
    int value = ui->editTriggerParameter->text().toInt();

    NeptuneSetTriggerParameter(m_cam, value);
}

void Widget::on_btnRunSW_clicked()
{
    m_uiRecvCount = 0;
    NeptuneRunSoftwareTrigger(m_cam);
}
