#pragma once

#include <vector>

#include <QOpenGLWidget.h>

struct Point {
	int x;
	int y;
	Point() {
		x = 0;
		y = 0;
	}
	Point(int a, int b) {
		x = a;
		y = b;
	}
};

class GLWidget : public QOpenGLWidget
{
	Q_OBJECT
public:
	explicit GLWidget(QWidget *parent = 0);
	~GLWidget();

	void GLWidget::initializeGL();
	void GLWidget::paintGL();
	void GLWidget::resizeGL(int w, int h);

	void GLWidget::mousePressEvent(QMouseEvent* event);

	float dx, dy, dz = 0.0f;

	int mode = 0;

	QString fileName;

	std::vector<Point> pointList;

private slots:
	void resetBtnClicked();
	void modeBtnClicked();
	void loadBtnClicked();
};

