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

SelectArea::SelectArea(QWidget *parent) :QDialog(parent, Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint),
	m_drawing(false)
{
	setAttribute(Qt::WA_TranslucentBackground);

	m_selectedArea = getWholeScreenArea();
	m_transparentBackground = QBrush(QColor(0, 255, 0, 8));
	m_selectBackground = QBrush(QColor(0, 255, 0, 32));
	m_selectBorder = QPen(QColor(0, 255, 0, 128), 1, Qt::DashLine);
	m_textBoxFont = QFont();
	m_textBoxFont.setPixelSize(24);
	
	resizeToFullScreen();
	
	m_textBoxMessage = "Click and drag ribbon to select screen area\n\nPress Enter to accept selected area\nPress Esc to cancel";
	QFontMetrics fm(m_textBoxFont);
	m_textBoxRect = fm.boundingRect(rect(), Qt::AlignCenter, m_textBoxMessage);
	m_textBoxRect.adjust(-16, -16, 16, 16);
	
}

void SelectArea::paintEvent(QPaintEvent *event)
{
	QPainter p(this);	
	
	p.fillRect(0, 0, width(), height(), m_transparentBackground);

	QRect drawrect = m_selectedArea.translated(-pos());

	p.fillRect(drawrect, m_selectBackground);
	p.setPen(m_selectBorder);
	p.drawRect(drawrect);

	QRect textbox(width() / 2 - (m_textBoxRect.width()/2), height() / 2 - (m_textBoxRect.height() / 2), m_textBoxRect.width(), m_textBoxRect.height());
	p.fillRect(textbox, QColor(0, 0, 0));
	p.setPen(QColor(255, 255, 255));
	p.drawRect(textbox);
	p.setPen(QColor(255, 255, 255));
	p.setFont(m_textBoxFont);
	p.drawText(rect(), Qt::AlignCenter, m_textBoxMessage);
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

QRect SelectArea::selectedArea(void)
{
	return m_selectedArea;
}

void SelectArea::resizeToFullScreen()
{
	QRect screenarea = getWholeScreenArea();
	move(screenarea.topLeft());
	resize(screenarea.size());
}

void SelectArea::mouseMoveEvent(QMouseEvent *evt)
{
	if (m_drawing)
	{
		QPoint p = evt->pos() + pos();

		QPoint topLeft(qMin(m_drawingAnchor.x(), p.x()), qMin(m_drawingAnchor.y(), p.y()));
		QPoint bottomRight(qMax(m_drawingAnchor.x(), p.x()), qMax(m_drawingAnchor.y(), p.y()));
		m_selectedArea = QRect(topLeft, bottomRight);
		
		repaint();
	}
}

void SelectArea::mousePressEvent(QMouseEvent *evt)
{
	if (evt->button() == Qt::LeftButton) 
	{
		if (!m_drawing)
		{
			m_drawingAnchor = evt->pos() + pos();
			m_selectedArea = QRect(m_drawingAnchor, m_drawingAnchor);
			m_drawing = true;
		}
	}
}

void SelectArea::mouseReleaseEvent(QMouseEvent *evt)
{
	if (evt->button() == Qt::LeftButton)
	{
		if (m_drawing)
		{
			m_drawing = false;

		}
	}
}

void SelectArea::keyPressEvent(QKeyEvent *evt)
{
	if (evt->key() == Qt::Key_Enter || evt->key() == Qt::Key_Return) {
		QDialog::accept();
		return;
	}

	QDialog::keyPressEvent(evt);
}