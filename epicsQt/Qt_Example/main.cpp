#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QtMath>
#include <QDebug>

class ControlValve : public QWidget {
public:
    ControlValve(QWidget *parent = 0) : QWidget(parent) {
        value = 0;
    }

protected:
    void paintEvent(QPaintEvent *event) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

#if 0
        int width = this->width();
        int height = this->height();

        // Draw the valve body
        painter.setBrush(Qt::gray);
        painter.drawEllipse(QPoint(width / 2, height / 2), width / 2, height / 2);

        // Draw the valve arm
        painter.setBrush(Qt::white);
        QPoint armCenter(width / 2, height / 2);
        int armLength = width / 3;
        int armWidth = width / 15;
        int angle = 90 + (180 * value / 100);
        painter.save();
        painter.translate(armCenter);
        painter.rotate(angle);
        painter.drawRoundedRect(-armWidth / 2, -armLength / 2, armWidth, armLength, armWidth / 2, armWidth / 2);
        painter.restore();
#else
#if 1
		QPen pen;
		QBrush brush;
		QRect rect = this->geometry ();
		QColor colour;
		QColor boarderColour;
		bool washedOut = false;
		QString text;
		int f = 0;             // fraction
		int g = 0;             // co-fraction
		int sum;

		// work variables
		int numPoints, ds;
		int p, q;
		int xc, x0, x1, x2, x3;
		int yc, y0, y1, y2, y3;
		double dx, dy;

		// Get basic colour property.
		int ew = rect.width()*0.01;

		pen.setWidth (ew);
		//pen.setStyle (this->edgeStyle);
		painter.setPen (pen);

		brush.setStyle (Qt::SolidPattern);
		brush.setColor (colour);
		painter.setBrush (brush);
		rect.moveTo (ew / 2, ew / 2);

		//rect.setWidth (rect.width () - ew);
		//rect.setHeight (rect.height () - ew);

		rect.setWidth (rect.width ());
		rect.setHeight (rect.height ());

		int startAngle = 30*16;
		int spanAngle  = 120*16;

		painter.drawChord (rect, startAngle, spanAngle );
#if 1 
		QPointF polygon [128];  // 30 snake points
		polygon[0] = QPointF(rect.width()*0.5, 0);
		polygon[1] = QPointF(rect.width()*0.5, rect.height()*0.75 );
		polygon[2] = QPointF(0,  rect.height()*0.5);
		polygon[3] = QPointF(rect.left(),  rect.bottom());
		polygon[4] = QPointF(rect.width(), rect.height()*0.5);
		polygon[5] = QPointF(rect.right(), rect.bottom());
		polygon[6] = polygon[1];
	
		painter.drawPolygon (polygon, 7);
#endif

#endif

#if 0
    QPointF const A(6.31, 5.773);
    QPointF const B(20.53, 19.836);
    QPointF const C(13.42, 12.805);
    double  const radius(10.0);
 
    QRectF const r(C.x() - radius, C.y() - radius, radius * 2, radius * 2);
    double const startAngle = 16 * atan2(A.y() - C.y(), A.x() - C.x()) * 180.0 / M_PI;
    double const endAngle   = 16 * atan2(B.y() - C.y(), B.x() - C.x()) * 180.0 / M_PI;
    double const spanAngle = endAngle - startAngle;
 
    //QImage img(500, 500, QImage::Format_RGB32);
    //img.fill(Qt::white);
    //QPainter p(&img);
    painter.scale(10.0, 10.0);
    painter.setPen(Qt::red);
    painter.drawRect(r);
    painter.setPen(Qt::black);
    painter.drawArc(r, 90, 360);
    //p.end();
    //img.save("test.png");
#endif
#endif
    }

    void mousePressEvent(QMouseEvent *event) {
        if (event->buttons() & Qt::LeftButton) {
            // Calculate the new valve position based on the mouse click
            QPoint valveCenter(width() / 2, height() / 2);
            QPoint mousePos = event->pos();
            int dx = mousePos.x() - valveCenter.x();
            int dy = mousePos.y() - valveCenter.y();
            double angle = qAtan2(dy, dx) * 180 / M_PI;

            // Ensure the valve doesn't go beyond 0-100 range
            if (angle < 0)
                angle = 0;
            else if (angle > 100)
                angle = 100;

            value = angle;
            update(); // Redraw the valve with the new position
        }
    }

private:
    int value;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ControlValve controlValve;
    controlValve.resize(200, 200);
    controlValve.show();

    return app.exec();
}
