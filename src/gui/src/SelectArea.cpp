/*
* barrier -- mouse and keyboard sharing utility
* Copyright (C) 2018 Debauchee Open Source Group
*
* This package is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* found in the file LICENSE that should have accompanied this file.
*
* This package is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "SelectArea.h"

SelectArea::SelectArea(QWidget *parent) :QWidget(parent, Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint)
{
	//setAttribute(Qt::WA_NoSystemBackground);
	//setAttribute(Qt::WA_TranslucentBackground);
	//setAttribute(Qt::WA_PaintOnScreen);
	//setAttribute(Qt::WA_TransparentForMouseEvents);
	m_selectedArea = getWholeScreenArea();
	m_selectBackground = QBrush(QColor(0, 255, 0, 64));
	m_selectBorder = QPen(QColor(0, 255, 0, 128), 8, Qt::DashLine);

	resizeToFullScreen();
}

void SelectArea::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	p.fillRect(0, 0, width(), height(), m_selectBackground);
	p.setPen(m_selectBorder);
	p.drawRect(0, 0, width(), height());
}

QRect SelectArea::getWholeScreenArea(void)
{
	QList<QScreen*> screens = QGuiApplication::screens();
	QRect screenrect;
	for (auto screen : screens)
	{
		QRect vgrect = screen->virtualGeometry();
		screenrect = screenrect.united(vgrect);
	}
	return screenrect;
}

void SelectArea::setSelectedArea(QRect r) 
{
	QRect screenarea = getWholeScreenArea();
	m_selectedArea = screenarea.intersected(r);
}

void SelectArea::resizeToFullScreen()
{
	QRect screenarea = getWholeScreenArea();
	move(screenarea.topLeft());
	resize(screenarea.size());
}

