#pragma once

#include <vector>
#include "Entity.h"

#include <QOpenGLWidget.h>

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
	void GLWidget::mouseMoveEvent(QMouseEvent* event);
	void GLWidget::mouseReleaseEvent(QMouseEvent* event);

	float dx, dy, dz = 0.0f;

	int mode = 0;
	int draw_mode = 1;
	bool pressing = false;
	Rect pressingRect;

	QString fileName;

	std::vector<Point> pointList;
	std::vector<Rect> rectList;

	EntityManager* em;

private slots:
	void resetBtnClicked();
	void modeBtnClicked();
	void loadBtnClicked();
};

