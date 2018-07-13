#include "GLWidget.h"
#include "Entity.h"
#include <GL/glut.h>
#include "QmouseEvent"
#include "QLabel"
#include "QPainter"
#include "QColor"
#include "QFileDialog"
#include "qopenglfunctions.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	em = EntityManager::getInstance();
	setFocusPolicy(Qt::StrongFocus);
	
	// Load obj file
	std::ifstream fin;
	fin.open("humbird.obj");
	if (fin.is_open()) {
		qDebug("Obj file loaded.");
		std::string line;

		while (!std::getline(fin, line).eof()) {
			std::istringstream lineStream(line);
			std::string header;
			lineStream >> header;
			if (std::strcmp("v", header.c_str()) == 0) {
				float x, y, z;
				std::string str = line.substr(2);
				std::sscanf(str.c_str(), "%f %f %f", &x, &y, &z);
				vertices.push_back(Vector3(x, y, z));
			}
			else if (std::strcmp("vt", header.c_str()) == 0) {
				float x, y, z;
				std::string str = line.substr(3);
				std::sscanf(str.c_str(), "%f %f %f", &x, &y, &z);
			}
			else if (std::strcmp("f", header.c_str()) == 0) {
				std::string str = line.substr(2);
				int vertexIndex[3], uvIndex[3];
				std::sscanf(str.c_str(), "%d/%d %d/%d %d/%d", &vertexIndex[0], &uvIndex[0],
					&vertexIndex[1], &uvIndex[1],
					&vertexIndex[2], &uvIndex[2]
					);
				vertexIndices.push_back(vertexIndex[0]);
				vertexIndices.push_back(vertexIndex[1]);
				vertexIndices.push_back(vertexIndex[2]);
				uvIndices.push_back(uvIndex[0]);
				uvIndices.push_back(uvIndex[1]);
				uvIndices.push_back(uvIndex[2]);
			}
		}
		fin.close();
	}
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
	if (mode == 0) { // Paint mode
		QPainter p(this);
		QImage img(fileName);
		p.begin(this);
		// Draw image
		if (!img.isNull()) {
			p.drawImage(rect(), img);
		}
		p.setPen(Qt::red);
		p.setBrush(Qt::red);
		// Draw points
		if (draw_mode == 0) {
			for (auto it = pointList.begin(); it != pointList.end(); it++) {
				p.drawEllipse(it->x, it->y, 5, 5);
			}
		}

		// Draw rects
		if (draw_mode == 1) {
			p.setBrush(Qt::NoBrush);
			p.setPen(Qt::green);
			if (pressing) {
				p.drawRect(pressingRect.left_top.x, pressingRect.left_top.y, pressingRect.right_bottom.x - pressingRect.left_top.x, pressingRect.right_bottom.y - pressingRect.left_top.y);
			}

			p.setPen(Qt::red);
			for (auto it = rectList.begin(); it != rectList.end(); it++) {
				Rect rect = *it;
				p.drawRect(rect.left_top.x, rect.left_top.y, rect.right_bottom.x - rect.left_top.x, rect.right_bottom.y - rect.left_top.y);
			}
		}

		p.end();
	}
	else if (mode == 1) { // 3D view mode
		QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0 + move_x, 0 + move_y, 0 + move_z, 0, 0, -5, 0, 1, 0);

		// Draw all entities
		std::vector<Entity> entities = em->getAllEntity();
		for (auto it = entities.begin(); it != entities.end(); it++) {
			glLoadIdentity();
			float ratio = 1 / it->size;
			glTranslatef(it->pos.x * ratio, it->pos.y * ratio, it->pos.z * ratio);
			glColor3f(1, 0, 0);
			glutSolidSphere(5, 20, 20);
		}

		glLoadIdentity();
		QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
		glTranslatef(0, 0, -10);
		f->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices.size()), &vertices[0], GL_STATIC_DRAW);
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

	// Pressing event
	if (!pressing) {
		pressingRect.left_top = Point(x, y);
		pressingRect.right_bottom = Point(x, y);
	}
	pressing = true;

	update();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();

	if (pressing) {
		pressingRect.right_bottom = Point(x, y);
		update();
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
	int x = event->x();
	int y = event->y();
	int center_x = x;
	int center_y = y;

	pressing = false;
	// Save draw rect
	pressingRect.right_bottom = Point(x, y);
	rectList.push_back(pressingRect);

	float width = 300;
	float height = 200;

	if (draw_mode == 1) {
		center_x = (pressingRect.right_bottom.x + pressingRect.left_top.x) / 2;
		center_y = (pressingRect.right_bottom.y + pressingRect.left_top.y) / 2;
	}

	// Calculate click position in 3D space
	dx = center_x - width / 2;
	dy = height / 2 - center_y;
	dz = -(height / 2) / tan(3.14 / 4 * 0.5);

	float size = sqrt((pressingRect.right_bottom.x - pressingRect.left_top.x) * (pressingRect.right_bottom.y - pressingRect.left_top.y)) / 16;

	pointList.push_back(Point(x, y));

	// Add entity
	if (draw_mode == 0) {
		em->addEntity(Entity(dx, dy, dz));
	}
	else if (draw_mode == 1) {
		em->addEntity(Entity(dx, dy, dz, size));
	}

	QLabel* label = this->parentWidget()->findChild<QLabel*>("label");
	label->setText(QString("X = %1 Y = %2 Size = %3").arg(x).arg(y).arg(size));

	update();
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{
	switch (event->key()) {
	case Qt::Key_Up:
		move_z += 10;
		break;
	case Qt::Key_Down:
		move_z -= 10;
		break;
	case Qt::Key_Left:
		move_x += 10;
		break;
	case Qt::Key_Right:
		move_x -= 10;
		break;
	}
	update();
}

void GLWidget::resetBtnClicked() {
	pointList.clear();
	rectList.clear();
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
