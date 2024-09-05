#pragma once

#include <QQuickItem>
#include <QQuickView>
#include <QPointer>

#include <set>

class  FramelessQuickWindow : public QQuickView
{
	Q_OBJECT
	Q_PROPERTY(bool activity READ isActive NOTIFY activeChanged)
	Q_PROPERTY(bool needShadow READ needShadow WRITE setNeedShadow NOTIFY needShadowChanged)
	Q_PROPERTY(QQuickItem* titleBar READ titleBar WRITE setTitleBar NOTIFY titleBarChanged)
	Q_PROPERTY(bool resizeable READ resizeable WRITE setResizeable NOTIFY resizeableChanged)
	Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
    Q_PROPERTY(QWindow* parentWin READ transientParent WRITE setTransientParent NOTIFY transientParentChanged)
public:
	FramelessQuickWindow(QWindow *parent = nullptr);

	Q_INVOKABLE void addIgnore(QQuickItem* item);
    Q_INVOKABLE void reqActive();
	
	void setResizeable(bool isResizeable);
	bool resizeable() const noexcept { return m_bResizeable; }

	void setTitleBar(QQuickItem* item);
	QQuickItem* titleBar() const noexcept { return m_pTitleBar; }

	bool needShadow() const noexcept { return m_needShadow; }
	int borderWidth() const noexcept { return m_borderWidth; }
	
	void setBorderWidth(int width);
	void setNeedShadow(bool needShadow);
	
signals:
	void parentWinChanged();
	void titleBarChanged();
	void resizeableChanged();
	void needShadowChanged();
	void borderWidthChanged();
    void transientParentChanged();
    void closeAll();
protected:
	bool nativeEvent(const QByteArray &eventType, void *message, long *result);
private:
	void updateShadow();
	
private:
	int m_borderWidth = 0;
	bool m_needShadow = true;
    bool m_bResizeable = false;
	QQuickItem* m_pTitleBar = nullptr;
	std::set<QQuickItem*> m_ignoreItem;
};
