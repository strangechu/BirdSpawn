#include "GLWidget.h"
#include "Entity.h"
#include <GL/glut.h>
#include "QmouseEvent"
#include "QLabel"
#include "QPainter"
#include "QColor"
#include "QFileDialog"
#include <cmath>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	em = EntityManager::getInstance();
}


GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
	glClearColor(0.2, 0.2, 0.2, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
}

void GLWidget::paintGL()
{

	QPainter p(this);

	if (mode == 0) { // Paint mode
		QImage img(fileName);
		p.begin(this);
		// Draw image
		if (!img.isNull()) {
			p.drawImage(rect(), img);
		}
		p.setPen(Qt::red);
		p.setBrush(Qt::red);
		// draw click point
		for (auto it = pointList.begin(); it != pointList.end(); it++) {
			p.drawEllipse(it->x, it->y, 5, 5);
		}
		p.end();
	}
	else if (mode == 1) { // 3D view mode
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0, 0, 0, 0, 0, -5, 0, 1, 0);

		// Draw all entities
		std::vector<Entity> entities = em->getAllEntity();
		for (auto it = entities.begin(); it != entities.end(); it++) {
			glLoadIdentity();
			glTranslatef(it->pos.x, it->pos.y, it->pos.z);
			glColor3f(1, 0, 0);
			glutSolidSphere(5, 20, 20);
		}
	}
}

void GLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / h, 0.01, 1000.0);
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();
	float width = 300;
	float height = 200;

	// Calculate click position in 3D space
	dx = x - width / 2;
	dy = height / 2 - y;
	dz = -(height / 2) / tan(3.14 / 4 * 0.5);

	QLabel* label = this->parentWidget()->findChild<QLabel*>("label");
	label->setText(QString("X = %1 Y = %2").arg(x).arg(y));

	pointList.push_back(Point(x, y));

	em->addEntity(Entity(dx, dy, dz));

	update();
}

void GLWidget::resetBtnClicked() {
	pointList.clear();
	em->clear();
	update();
}

void GLWidget::modeBtnClicked() {
	switch (mode) {
	case 0:
		mode = 1;
		break;
	case 1:
		mode = 0;
		break;
	}
	update();
}

void GLWidget::loadBtnClicked() {
	fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), fileName, tr("Image Files (*.png *.jpg *.gif)"));
	update();
}
