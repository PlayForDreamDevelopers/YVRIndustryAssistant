#include "FramelessQuickWindow.h"

#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>

#include <QTimer>
#include <QtWin>
#pragma comment (lib,"Dwmapi.lib") // Adds missing library, fixes error LNK2019: unresolved external symbol __imp__DwmExtendFrameIntoClientArea
#pragma comment (lib,"user32.lib")

FramelessQuickWindow::FramelessQuickWindow(QWindow *parent)
	: QQuickView(parent)
{
    setFlags(flags() | Qt::FramelessWindowHint);
	
	connect(this, &FramelessQuickWindow::needShadowChanged, this, &FramelessQuickWindow::updateShadow);
	connect(this, &FramelessQuickWindow::transientParentChanged, this, &FramelessQuickWindow::parentWinChanged);
}

Q_INVOKABLE void FramelessQuickWindow::addIgnore(QQuickItem * item)
{
    m_ignoreItem.insert(item);
}

void FramelessQuickWindow::reqActive()
{
    requestActivate();
}

void FramelessQuickWindow::setTitleBar(QQuickItem* item)
{
	if (item != m_pTitleBar)
	{
		m_pTitleBar = item;
		emit titleBarChanged();
		
		connect(item, &QQuickItem::destroyed, this, [=] {
			if (item == m_pTitleBar)
			{
				m_pTitleBar = nullptr;
				emit titleBarChanged();
			}
		});
	}
}

void FramelessQuickWindow::setBorderWidth(int width)
{
	if (m_borderWidth != width)
	{
		m_borderWidth = width;
		emit borderWidthChanged();
	}
}

void FramelessQuickWindow::setNeedShadow(bool needShadow)
{
	if (m_needShadow != needShadow)
	{
		m_needShadow = needShadow;
		emit needShadowChanged();
	}
}

void FramelessQuickWindow::setResizeable(bool isResizeable)
{
	if (m_bResizeable != isResizeable)
	{
		m_bResizeable = isResizeable;
		emit resizeableChanged();
	}
}

bool FramelessQuickWindow::nativeEvent(const QByteArray& eventType, void* message, long* result)
{
	MSG* msg = reinterpret_cast<MSG*>(message);
	
    switch (msg->message)
    {
    case WM_NCHITTEST:
    {
        *result = 0;

        const LONG border_width = m_borderWidth;
        RECT winrect;
        GetWindowRect(HWND(winId()), &winrect);

        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        if(m_bResizeable)
        {
            bool resizeWidth = minimumWidth() != maximumWidth();
            bool resizeHeight = minimumHeight() != maximumHeight();

            if(resizeWidth)
            {
                //left border
                if (x >= winrect.left && x < winrect.left + border_width)
                {
                    *result = HTLEFT;
                }
                //right border
                if (x < winrect.right && x >= winrect.right - border_width)
                {
                    *result = HTRIGHT;
                }
            }
            if(resizeHeight)
            {
                //bottom border
                if (y < winrect.bottom && y >= winrect.bottom - border_width)
                {
                    *result = HTBOTTOM;
                }
                //top border
                if (y >= winrect.top && y < winrect.top + border_width)
                {
                    *result = HTTOP;
                }
            }
            if(resizeWidth && resizeHeight)
            {
                //bottom left corner
                if (x >= winrect.left && x < winrect.left + border_width &&
                        y < winrect.bottom && y >= winrect.bottom - border_width)
                {
                    *result = HTBOTTOMLEFT;
                }
                //bottom right corner
                if (x < winrect.right && x >= winrect.right - border_width &&
                        y < winrect.bottom && y >= winrect.bottom - border_width)
                {
                    *result = HTBOTTOMRIGHT;
                }
                //top left corner
                if (x >= winrect.left && x < winrect.left + border_width &&
                        y >= winrect.top && y < winrect.top + border_width)
                {
                    *result = HTTOPLEFT;
                }
                //top right corner
                if (x < winrect.right && x >= winrect.right - border_width &&
                        y >= winrect.top && y < winrect.top + border_width)
                {
                    *result = HTTOPRIGHT;
                }
            }
        }

        if (0 != *result)
            return true;

        if (!m_pTitleBar)
            return false;


        // support highdpi

        //      auto dpr = this->effectiveDevicePixelRatio();
        //		const POINT globalPos{x, y};
        //		POINT winLocalPos = globalPos;
        //		ScreenToClient(msg->hwnd, &winLocalPos);
        //		auto qtGlobalPos = mapToGlobal(QPoint(winLocalPos.x, winLocalPos.y) / dpr);

        // 这两种方法都可以解决，qt全局坐标与windows全局坐标不一致的问题。
        auto qtGlobalPos = QCursor::pos();

        auto pos = m_pTitleBar->mapFromScene(mapFromGlobal(qtGlobalPos));
        if (!m_pTitleBar->contains(pos))
        {
            return false;
        }

        auto child = m_pTitleBar->childAt(pos.x(), pos.y());
        if (!child)
        {
            *result = HTCAPTION;
            return true;
        }
        else
        {
            if (m_ignoreItem.find(child) != m_ignoreItem.cend())
            {
                *result = HTCAPTION;
                return true;
            }
        }
        return false;
    }
    case WM_SYSCOMMAND:{
        if (61536 == msg->wParam)//禁止任务栏"关闭窗口"
        {
            emit closeAll();
            break;
        }
    }
    case WM_SHOWWINDOW:{
        QTimer::singleShot(100, this, &FramelessQuickWindow::updateShadow);
    }

    default:
        return QQuickView::nativeEvent(eventType, message, result);
    }
}

void FramelessQuickWindow::updateShadow()
{
	HWND hWnd = reinterpret_cast<HWND>(winId());
	if (NULL == hWnd)
		return;

	LONG currentStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (needShadow())
		currentStyle |= WS_MINIMIZEBOX;
	else
		currentStyle &= ~WS_MINIMIZEBOX;
	
	SetWindowLong(hWnd, GWL_STYLE, currentStyle);

	if (QtWin::isCompositionEnabled()) {
		DWMNCRENDERINGPOLICY ncrp = needShadow() ? DWMNCRP_ENABLED : DWMNCRP_DISABLED;
		::DwmSetWindowAttribute((HWND)winId(), DWMWA_NCRENDERING_POLICY, &ncrp, sizeof(ncrp));
		QtWin::extendFrameIntoClientArea(this, 1, 1, 1, 1);
	}
}
