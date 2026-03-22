#include "../ui/plotcanvas.h"
#include <QPainterPath>

PlotCanvas::PlotCanvas(QWidget *parent)
    : QWidget(parent)
    , m_xMin(0.0), m_xMax(30.0)
    , m_yMin(0.0), m_yMax(5.0)
    , m_marginLeft(70)
    , m_marginBottom(50)
    , m_marginRight(30)
    , m_marginTop(30)
    , m_showHint(false)
{
    // 必須啟用滑鼠追蹤，否則 mouseMoveEvent 只有在按住滑鼠時才會觸發
    setMouseTracking(true);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
}

void PlotCanvas::setData(const std::vector<Point>& points, double xMax, double yMax)
{
    m_curvePoints = points;
    m_xMax = xMax;
    m_yMax = yMax;
    update(); // 觸發重繪
}

void PlotCanvas::clear()
{
    m_curvePoints.clear();
    m_showHint = false;
    update();
}

// --- 核心幾何邏輯：數值 -> 像素 ---

int PlotCanvas::valToPxX(double val)
{
    int plotWidth = width() - m_marginLeft - m_marginRight;
    double ratio = (val - m_xMin) / (m_xMax - m_xMin);
    return m_marginLeft + (int)(ratio * plotWidth);
}

int PlotCanvas::valToPxY(double val)
{
    int plotHeight = height() - m_marginTop - m_marginBottom;
    double ratio = (val - m_yMin) / (m_yMax - m_yMin);
    // Y 軸是反向的，所以用底部像素往上減
    int plotBottom = height() - m_marginBottom;
    return plotBottom - (int)(ratio * plotHeight);
}

// --- 核心幾何邏輯：像素 -> 數值 ---

double PlotCanvas::pxToValX(int px)
{
    int plotWidth = width() - m_marginLeft - m_marginRight;
    double ratio = (double)(px - m_marginLeft) / plotWidth;
    return m_xMin + ratio * (m_xMax - m_xMin);
}

double PlotCanvas::pxToValY(int py)
{
    int plotHeight = height() - m_marginTop - m_marginBottom;
    int plotBottom = height() - m_marginBottom;
    double ratio = (double)(plotBottom - py) / plotHeight;
    return m_yMin + ratio * (m_yMax - m_yMin);
}

// --- 事件處理 ---

void PlotCanvas::mouseMoveEvent(QMouseEvent *event)
{
    m_mousePos = event->pos();

    // 檢查滑鼠是否在繪圖區域內
    if (m_mousePos.x() >= m_marginLeft && m_mousePos.x() <= (width() - m_marginRight)) {
        if (m_mousePos.y() >= m_marginTop && m_mousePos.y() <= (height() - m_marginBottom)) {
            m_showHint = true;
        } else {
            m_showHint = false;
        }
    } else {
        m_showHint = false;
    }

    update(); // 重繪以更新提示框
}

void PlotCanvas::leaveEvent(QEvent *event)
{
    m_showHint = false;
    update();
}

void PlotCanvas::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 先畫背景
    painter.fillRect(rect(), Qt::white);

    // 繪製繪圖核心區的邊框
    QRect plotRect(m_marginLeft, m_marginTop,
                   width() - m_marginLeft - m_marginRight,
                   height() - m_marginTop - m_marginBottom);
    painter.setPen(Qt::black);
    painter.drawRect(plotRect);

    // 暫時先不寫畫曲線和提示框的細節，我們先確認架構
}