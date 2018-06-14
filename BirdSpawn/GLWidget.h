#pragma once

#include <vector>

#include <QOpenGLWidget.h>

class Point;
class EntityManager;

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

	EntityManager* em;

private slots:
	void resetBtnClicked();
	void modeBtnClicked();
	void loadBtnClicked();
};

