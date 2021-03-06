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
	void GLWidget::keyPressEvent(QKeyEvent* event);

	float dx = 0, dy = 0, dz = 0;
	float move_x = 0, move_y = 0, move_z = 0;

	int mode = 0;
	int draw_mode = 1;
	bool pressing = false;
	Rect pressingRect;

	QString fileName;

	std::vector<Point> pointList;
	std::vector<Rect> rectList;

	std::vector<Vector3> vertices;
	std::vector<unsigned int> vertexIndices, uvIndices;

	EntityManager* em;

private slots:
	void resetBtnClicked();
	void modeBtnClicked();
	void loadBtnClicked();
};

