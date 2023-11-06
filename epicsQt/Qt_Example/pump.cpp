#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QPolygon>
#include <QColor>

class PumpGraphicWidget : public QWidget {
public:
    PumpGraphicWidget(QWidget *parent = nullptr) : QWidget(parent) {}

protected:
    void paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        int width = this->width();
        int height = this->height();

        // Draw the pump base
        painter.setBrush(QColor(100, 100, 100));
        painter.drawRect(20, height - 40, width - 40, 20);

        // Draw the pump body
        QRect pumpBodyRect(20, height - 60, width - 40, 40);
        painter.setBrush(QColor(50, 50, 50));
        painter.drawEllipse(pumpBodyRect);

        // Draw the pump top
        QRect pumpTopRect(width / 3, height - 110, width / 3, 50);
        painter.setBrush(QColor(50, 50, 50));
        painter.drawEllipse(pumpTopRect);

        // Draw the pump nozzle
        int nozzleWidth = width / 6;
        int nozzleHeight = 20;
        QRect nozzleRect((width - nozzleWidth) / 2, height - nozzleHeight, nozzleWidth, nozzleHeight);
        painter.setBrush(QColor(100, 100, 100));
        painter.drawRect(nozzleRect);

        // Draw pump impeller blades
        painter.setBrush(Qt::black);
        int bladeCount = 8;
        int bladeWidth = 10;
        int bladeHeight = 20;

        for (int i = 0; i < bladeCount; ++i) {
            int bladeAngle = i * 360 / bladeCount;
            QPolygon blade;
            blade << QPoint(width / 2 - bladeWidth / 2, height - 45);
            blade << QPoint(width / 2 + bladeWidth / 2, height - 45);
            blade << QPoint(width / 2, height - 45 - bladeHeight);
            painter.save();
            painter.translate(width / 2, height - 45);
            painter.rotate(bladeAngle);
            painter.drawPolygon(blade);
            painter.restore();
        }
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    PumpGraphicWidget pumpWidget;
    pumpWidget.resize(300, 300);
    pumpWidget.setWindowTitle("Pump Graphic Example");
    pumpWidget.show();

    return app.exec();
}