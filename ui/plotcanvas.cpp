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

/*
void PlotCanvas::setData(const std::vector<Point>& points, double xMax, double yMax)
{
    m_curvePoints = points;
    m_xMax = xMax;
    m_yMax = yMax;
    update(); // 觸發重繪
}
*/
// plotcanvas.cpp
void PlotCanvas::setData(const QVector<UICurveData>& curves, double xMax, double yMax)
{
    m_curves = curves; // 直接賦值
    m_xMax = xMax;
    m_yMax = yMax;
    update(); // 觸發畫布重繪
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

    // 1. 填滿背景（純白）
    painter.fillRect(rect(), Qt::white);

    // 2. 計算繪圖區邊界（為了方便後續邏輯使用，我們先計算出四個關鍵像素位置）
    int plotLeft   = m_marginLeft;
    int plotRight  = width() - m_marginRight;
    int plotTop    = m_marginTop;
    int plotBottom = height() - m_marginBottom;

    int plotWidth  = plotRight - plotLeft;
    int plotHeight = plotBottom - plotTop;

    // 3. 繪製座標主軸線
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(plotLeft, plotBottom, plotRight, plotBottom); // X軸
    painter.drawLine(plotLeft, plotBottom, plotLeft, plotTop);    // Y軸


    // 3. 繪製座標主軸線 (L 型)
    QPen axisPen(Qt::black, 2); // 黑色，寬度 2
    painter.setPen(axisPen);



    // 水平 X 軸
    painter.drawLine(plotLeft, plotBottom, plotRight, plotBottom);
    // 垂直 Y 軸
    painter.drawLine(plotLeft, plotBottom, plotLeft, plotTop);

    //------------------------------------------------------
    // 4. 繪製刻度與數字 (X 軸 - 電壓)
    //------------------------------------------------------
    // 我們預設畫 10 個大刻度

    int xTicks = 10;
    painter.setPen(QPen(Qt::black, 1));

    for (int i = 0; i <= xTicks; i++) {
        double val = m_xMin + (i * (m_xMax - m_xMin) / xTicks);
        int px = valToPxX(val);

        // 畫刻度小線段 (往下延伸 5 像素)
        painter.drawLine(px, plotBottom, px, plotBottom + 5);

        // 畫數字標籤
        QString label = QString::number(val, 'f', 1); // 取小數點一位
        int textWidth = painter.fontMetrics().horizontalAdvance(label);
        // 數字畫在刻度下方 20 像素處
        painter.drawText(px - textWidth / 2, plotBottom + 20, label);

        // 選做：畫淡灰色網格線 (垂直線)
        if (i > 0) {
            painter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
            painter.drawLine(px, plotBottom, px, plotTop);
            painter.setPen(QPen(Qt::black, 1)); // 換回黑色畫下一個刻度
        }
    }

    //------------------------------------------------------
    // 5. 繪製刻度與數字 (Y 軸 - 電流)
    //------------------------------------------------------
    // 我們預設畫 5 個大刻度

    int yTicks = 5;
    for (int i = 0; i <= yTicks; i++) {
        double val = m_yMin + (i * (m_yMax - m_yMin) / yTicks);
        int py = valToPxY(val);

        // 畫刻度小線段 (向左延伸 5 像素)
        painter.drawLine(plotLeft, py, plotLeft - 5, py);

        // 畫數字標籤
        QString label = QString::number(val, 'f', 2); // 電流通常較小，取兩位
        int textWidth = painter.fontMetrics().horizontalAdvance(label);
        int textHeight = painter.fontMetrics().height();
        // 數字畫在刻度左側，靠右對齊
        painter.drawText(plotLeft - textWidth - 10, py + textHeight / 4, label);

        // 選做：畫淡灰色網格線 (水平線)
        if (i > 0) {
            painter.setPen(QPen(Qt::lightGray, 1, Qt::DashLine));
            painter.drawLine(plotLeft, py, plotRight, py);
            painter.setPen(QPen(Qt::black, 1));
        }
    }

    // 6. 如果有數據，繪製曲線（我們下一步再做）
    drawCurves(painter);

    // 7. 最後畫左下角提示框（放在最上層）
    //drawHintBox(&painter);

    // 7. 繪製左下角提示框
    if (m_showHint) {
        drawHintBox(&painter);
    }
}

void PlotCanvas::drawCurves(QPainter &painter)
{
    if (m_curves.isEmpty()) return;

    QPen pen(Qt::blue, 2);
    painter.setPen(pen);

    // 設定標籤字體大小（稍微小一點，比較精緻）
    QFont font = painter.font();
    font.setPointSize(9);
    painter.setFont(font);

    for (const auto& curve : std::as_const(m_curves)) {

        if (curve.points.isEmpty()) continue;


        QPainterPath path;
        bool first = true;
        QPointF labelPoint; // 紀錄最後一個點的位置


        for (const auto& point : std::as_const(curve.points)) {
            // 使用我們先前寫好的映射函式，這比手算比例更安全
            // 這裡也處理了數據範圍限制 (qBound)
            double xVal = qBound(m_xMin, point.x(), m_xMax);
            double yVal = qBound(m_yMin, point.y(), m_yMax);

            int px = valToPxX(xVal);
            int py = valToPxY(yVal);

            if (first) {
                path.moveTo(px, py);
                first = false;
            } else {
                path.lineTo(px, py);
            }

            // 我們把標籤放在 $V_{ds}$ 大約 85% 的位置，這樣文字就不會被右邊緣切掉
            if (xVal >= m_xMax * 0.85 && labelPoint.isNull()) {
                labelPoint = QPointF(px, py);
            }
        }
        // 1. 畫出曲線
        painter.drawPath(path);

        // 如果沒找到 85% 的點（通常不會），就用最後一點
        if (labelPoint.isNull()) {
            labelPoint = path.currentPosition();
        }

        // 3. 繪製標籤文字 (例如 Vgs=5V)
        QString labelText = curve.label;
        if (!labelText.isEmpty()) {
            painter.save(); // 儲存目前的畫筆狀態

            // 設定標籤字體
            QFont font = painter.font();
            font.setPointSize(8);
            font.setBold(true);
            painter.setFont(font);

            // 計算文字寬度
            int tw = painter.fontMetrics().horizontalAdvance(labelText);
            int th = painter.fontMetrics().height();

            // 在文字下方畫一個微小的白色半透明底色，避免被格線干擾
            QRect textRect(labelPoint.x() - tw/2, labelPoint.y() - th - 5, tw + 4, th);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(255, 255, 255, 200)); // 半透明白
            painter.drawRect(textRect);

            // 畫出黑色文字
            painter.setPen(Qt::black);
            painter.drawText(textRect, Qt::AlignCenter, labelText);

            painter.restore(); // 恢復畫筆狀態，準備畫下一條藍線
        }
    }
}


void PlotCanvas::drawGridAndAxes(QPainter &painter)
{
    int plotLeft = m_marginLeft;
    int plotBottom = height() - m_marginBottom;
    int plotRight = width() - m_marginRight;
    int plotTop = m_marginTop;

    // 繪製座標主線
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(plotLeft, plotBottom, plotRight, plotBottom); // X軸
    painter.drawLine(plotLeft, plotBottom, plotLeft, plotTop);    // Y軸

    // 繪製刻度 (依照你原本的 5 等份邏輯)
    painter.setPen(Qt::gray);
    for (int i = 1; i <= 5; ++i) {
        // X 軸刻度
        int x = plotLeft + i * (plotRight - plotLeft) / 5;
        painter.drawLine(x, plotBottom, x, plotBottom + 5);

        // Y 軸刻度
        int y = plotBottom - i * (plotBottom - plotTop) / 5;
        painter.drawLine(plotLeft, y, plotLeft - 5, y);
    }

    // 繪製 X 軸標籤
    painter.setPen(Qt::black);
    painter.drawText(QRect(plotLeft, plotBottom + 20, plotRight - plotLeft, 20),
                     Qt::AlignCenter, m_xLabel);

    // 繪製 Y 軸標籤 (旋轉 90 度)
    painter.save();
    painter.translate(20, (plotTop + plotBottom) / 2);
    painter.rotate(-90);
    painter.drawText(-50, 0, 100, 20, Qt::AlignCenter, m_yLabel);
    painter.restore();
}

void PlotCanvas::drawHintBox(QPainter *painter)
{
    // 1. 從目前的滑鼠位置反推物理數值
    double vVal = pxToValX(m_mousePos.x());
    double iVal = pxToValY(m_mousePos.y());

    // 2. 準備文字內容
    QString text = QString("V: %1 V\nI: %2 A")
                       .arg(vVal, 0, 'f', 2)
                       .arg(iVal, 0, 'f', 3);

    // 3. 設定框框的位置 (固定在繪圖區左下角)
    // 為了美觀，我們把它畫在 plotBottom 之上 60 像素的位置
    int boxW = 120;
    int boxH = 50;
    int boxX = m_marginLeft + 10;
    int boxY = height() - m_marginBottom - boxH - 10;
    QRect boxRect(boxX, boxY, boxW, boxH);

    // 4. 畫背景 (半透明黑)
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(0, 0, 0, 160)); // 160 是透明度 (0-255)
    painter->drawRoundedRect(boxRect, 5, 5);

    // 5. 畫文字 (白色)
    painter->setPen(Qt::white);
    painter->drawText(boxRect.adjusted(10, 5, -10, -5), Qt::AlignLeft | Qt::AlignVCenter, text);
}