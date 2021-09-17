#pragma once
#pragma execution_character_set("utf-8")

#include <QApplication>
#include <QDesktopWidget>
#include <QWidget>
#include <QPalette>
#include <QStyleFactory>
#include <QBitmap>
#include <QPainter>
#include <QMouseEvent>

#include "QMessageBoxEx.hpp"

/*
* @RUN_MAIN_WINDOW,����������
* @param1,������
* @return,int
*/
#define RUN_MAIN_WINDOW(MAIN_WIN)\
int main(int argc,char* argv[])\
{\
QApplication a(argc,argv);\
MAIN_WIN* WIN = NO_THROW_NEW MAIN_WIN;\
if (!WIN)\
	return -1;\
WIN->show();\
return a.exec();\
}

/*
* @OVERRIDE_MOUSE_EVENT,��д����¼�
*/
#define OVERRIDE_MOUSE_EVENT \
\
bool m_mousePress = false;\
\
QPoint m_mousePoint;\
\
virtual void mousePressEvent(QMouseEvent * event)\
{\
	if (event->button() == Qt::LeftButton)\
	{\
		m_mousePress = true;\
		m_mousePoint = event->globalPos();\
	}\
}\
\
virtual void mouseReleaseEvent(QMouseEvent * event)\
{\
	if (m_mousePress)\
		m_mousePress = false;\
}\
\
virtual void mouseMoveEvent(QMouseEvent * event)\
{\
	if (m_mousePress)\
	{\
		int x = event->globalX() - m_mousePoint.x();\
		int y = event->globalY() - m_mousePoint.y();\
		m_mousePoint = event->globalPos();\
		move(this->x() + x, this->y() + y);\
	}\
}

namespace Utility {
	/*
	* @WindowFactory,���ڹ���
	*/
	class Window {
	public:
		/*
		* @getThemeList,��ȡ�����б�
		* @return,const QStringList
		*/
		static const QStringList getThemeList();

		/*
		* @setTheme,��������
		* @param1,��������
		* @return,void
		*/
		static void setTheme(const QString& theme = QString("Fusion"));

		/*
		* @setBorderRadius,���ñ߿�ΪԲ��
		* @param1,��Ҫ���õĲ���
		* @param2,�뾶��С
		* @return,void
		*/
		static void setBorderRadius(QWidget* widget,const QSize& add = {}, qreal radius = 20);

		/*
		* @resizeWindow,�������ڴ�С
		* @param1,������
		* @param2,���ű���
		* @param3,�ӻ�����ڴ�С,true add,false sub
		* @param4,�ӻ�����ٴ�С
		* @return,void
		*/
		static void resize(QWidget* widget, float scale, bool addOrSub = false, float size = 0.0f);

		/*
		* @getScreenSize,��ȡ��Ļ��С
		* @param1,�ڼ�����Ļ
		* @return,QRect
		*/
		static QRect getScreenSize(int screent = -1);
	};
}
