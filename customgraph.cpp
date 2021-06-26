#include "customgraph.h"

#include <QPainter>
#include <QPainterPath>
#include <QPainterPathStroker>
#include <QStyleOption>
#include <QtMath>

CustomGraph::CustomGraph(QWidget *parent) : QWidget(parent)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    m_SerieNameFont = font();
    m_ValuesFont = font();
}

void CustomGraph::Clear()
{
    m_MinValue = 0.0;
    m_MaxValue = 0.0;
    m_Serie.clear();
}

void CustomGraph::SetSerie(QVector<double> vSerie)
{
    m_Serie = vSerie;

    if (!m_Serie.isEmpty())
    {
        m_MinValue = 1e5;
        m_MaxValue = -1e5;
        foreach(auto s, m_Serie)
        {
            m_MinValue = qMin(m_MinValue, s);
            m_MaxValue = qMax(m_MaxValue, s);
        }
    }
}

void CustomGraph::SetSerie(
        QVector<double> vSerie,
        const QColor& vSerieColor,
        const double& vLineThickness)
{
    m_Serie = vSerie;

    if (!m_Serie.isEmpty())
    {
        m_MinValue = 1e5;
        m_MaxValue = -1e5;
        foreach(auto s, m_Serie)
        {
            m_MinValue = qMin(m_MinValue, s);
            m_MaxValue = qMax(m_MaxValue, s);
        }
    }

    m_SerieColor = vSerieColor;
    m_LineThickness = vLineThickness;
}

void CustomGraph::SetPadding(
        const double& vPaddingLeft,
        const double& vPaddingBottom,
        const double& vPaddingRight,
        const double& vPaddingTop)
{
    m_PaddingLeft = vPaddingLeft;
    m_PaddingBottom = vPaddingBottom;
    m_PaddingRight = vPaddingRight;
    m_PaddingTop = vPaddingTop;
}

void CustomGraph::SetVerticalGradient(
        const QColor& vStart,
        const QColor& vEnd)
{
    m_GradientStartColor = vStart;
    m_GradientEndColor = vEnd;
}

void CustomGraph::SetLinesColor(const QColor& vLinesColor)
{
    m_LinesColor = vLinesColor;
}

void CustomGraph::SetValuesColor(const QColor& vValuesColor)
{
    m_ValuesColor = vValuesColor;
}

void CustomGraph::SetValuesFont(const QFont& vValuesFont)
{
    m_ValuesFont = vValuesFont;
}

void CustomGraph::SetSerieName(const QString& vSerieName)
{
    m_SerieName = vSerieName;
}

void CustomGraph::SetSerieCurveStyle(
        const QColor& vSerieColor,
        const double& vLineThickness)
{
    m_SerieColor = vSerieColor;
    m_LineThickness = vLineThickness;
}

void CustomGraph::SetSerieNameStyle(
        const QFont& vSerieNameFont,
        const QColor& vSerieNameColor)
{
    m_SerieNameColor = vSerieNameColor;
    m_SerieNameFont = vSerieNameFont;
}

void CustomGraph::SetSerieName(
        const QString& vSerieName,
        const QFont& vSerieNameFont,
        const QColor& vSerieNameColor,
        const Qt::AnchorPoint& vSerieNameAnchorPoint)
{
    m_SerieName = vSerieName;
    m_SerieNameColor = vSerieNameColor;
    m_SerieNameFont = vSerieNameFont;
    m_SerieNameAnchorPoint = vSerieNameAnchorPoint;
}

void CustomGraph::SetDrawSerieName(const bool& vDrawSerieName)
{
    m_DrawSerieName = vDrawSerieName;
}

void CustomGraph::SetDrawRect(const bool& vDrawRect)
{
    m_DrawRect = vDrawRect;
}

void CustomGraph::SetXAxis(
        const int& vCounstStepX,
        const bool &vShowLinesX,
        const bool &vShowValuesX,
        const double &vRangeInfX, const double &vRangeSupX)
{
    m_CountStepsX = vCounstStepX;
    m_ShowLinesX = vShowLinesX;
    m_ShowValuesX = vShowValuesX;
    m_RangeInfX = vRangeInfX;
    m_RangeSupX = vRangeSupX;
}

void CustomGraph::SetYAxis(
        const int& vCounstStepY,
        const bool &vShowLinesY,
        const bool &vShowValuesY,
        const int& vPrecisionY)
{
    m_CountStepsY = vCounstStepY;
    m_ShowLinesY = vShowLinesY;
    m_ShowValuesY = vShowValuesY;
    m_PrecisionY = vPrecisionY;
}

void CustomGraph::SetBackgroundColor(const QColor& vBackgroundColor)
{
    m_BackgroundColor = vBackgroundColor;
}

////////////////////////////////////////////
///// OVERRIDE /////////////////////////////
////////////////////////////////////////////

QSize CustomGraph::minimumSizeHint() const
{
    return QSize(0, 0);
}

QSize CustomGraph::sizeHint() const
{
    return QSize(100, 100);
}

void CustomGraph::setFont(const QFont& vFont)
{
    QWidget::setFont(vFont);

    m_SerieNameFont = vFont;
    m_ValuesFont = vFont;
}

////////////////////////////////////////////
////////////////////////////////////////////
////////////////////////////////////////////

static inline double siLerp(const double& vInf, const double& vSup, const double& vRatio)
{
    return vInf * (1.0f - vRatio) + vSup * vRatio;
}

void CustomGraph::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing, true);

    painter.save();
    painter.fillRect(QRect(0, 0, width(), height()), m_BackgroundColor);
    painter.restore();

    const double w = (double)width() - m_PaddingLeft - m_PaddingRight;
    const double h = (double)height() - m_PaddingBottom - m_PaddingTop;

    // the left bottom point is the top left
    if (m_DrawRect)
    {
        painter.save();
        painter.setPen(QPen(QBrush(m_LinesColor), 1.0));
        painter.drawRect(QRect(m_PaddingLeft, m_PaddingTop, w, h));
        painter.restore();
    }

    if (!m_Serie.isEmpty())
    {

        const double y_top = m_PaddingTop;
        const double y_bottom = m_PaddingTop + h;
        const double x_left = m_PaddingLeft;
        const double x_right = m_PaddingLeft + w;
        const double rangeY = m_MaxValue - m_MinValue;
        const double maxY = m_MaxValue + rangeY * 0.1;
        const double minY = m_MinValue - rangeY * 0.1;

        painter.save();
        painter.setPen(QPen(QBrush(m_LinesColor), 1.0, Qt::DashLine));

        if (m_CountStepsX != -1)
        {
            QFontMetrics fmvx(m_ValuesFont);
            double x = x_left;
            const double x_step = w / (double)(m_CountStepsX + 1);
            double vx = m_RangeInfX;
            const double trueRangeX = m_RangeSupX - m_RangeInfX;
            const double vx_step = trueRangeX / (double)(m_CountStepsX + 1);
            if (!m_DrawRect)
            {
                painter.drawLine(QPointF(x, y_bottom), QPointF(x, y_top));
            }
            for (int i = 0; i <= m_CountStepsX + 1; i++)
            {
                if (m_ShowValuesX)
                {
                    const QString vs = QString::number(vx);
                    const QRect bounds = fmvx.boundingRect(vs);
                    painter.save();
                    painter.setFont(m_ValuesFont);
                    painter.setPen(m_ValuesColor);
                    painter.drawText(QPointF(x - bounds.width() * 0.5, y_bottom + bounds.height()), vs);
                    painter.restore();
                    vx += vx_step;
                }
                x += x_step;
                if (m_ShowLinesX)
                {
                    if (i < m_CountStepsX || (!m_DrawRect && i <= m_CountStepsX))
                    {
                        painter.drawLine(QPointF(x, y_bottom), QPointF(x, y_top));
                    }
                }
            }
        }

        if (m_CountStepsY != -1)
        {
            QFontMetrics fmvy(m_ValuesFont);
            double y = y_top;
            const double y_step = h / (double)(m_CountStepsY + 1);
            double vy = m_MaxValue;
            const double trueRangeY = maxY - minY;
            const double vy_step = trueRangeY / (double)(m_CountStepsY + 1);
            if (!m_DrawRect)
            {
                painter.drawLine(QPointF(x_left, y), QPointF(x_right, y));
            }
            for (int i = 0; i <= m_CountStepsY + 1; i++)
            {
                if (m_ShowValuesY)
                {
                    const QString vs = QString::number(vy, 'f', 1);
                    const QRect bounds = fmvy.tightBoundingRect(vs);
                    painter.save();
                    painter.setFont(m_ValuesFont);
                    painter.setPen(m_ValuesColor);
                    painter.drawText(QPointF(x_left - bounds.width() - fmvy.horizontalAdvance("1"), y + bounds.height() * 0.5), vs);
                    painter.restore();
                    vy -= vy_step;
                }
                y += y_step;
                if (m_ShowLinesY)
                {
                    if (i < m_CountStepsY || (!m_DrawRect && i <= m_CountStepsY))
                    {
                        painter.drawLine(QPointF(x_left, y), QPointF(x_right, y));
                    }
                }
            }
        }

        painter.restore();

        if (m_DrawSerieName)
        {
            QFontMetrics fmsn(m_SerieNameFont);
            painter.save();
            painter.setFont(m_SerieNameFont);
            painter.setPen(m_SerieNameColor);

            const QRect bounds = fmsn.tightBoundingRect(m_SerieName);

            if (m_SerieNameAnchorPoint == Qt::AnchorPoint::AnchorTop)
            {
                painter.drawText(QPointF((x_left + x_right - bounds.width()) * 0.5, y_top - fmsn.descent()), m_SerieName);
            }
            else if (m_SerieNameAnchorPoint == Qt::AnchorPoint::AnchorBottom)
            {
                painter.drawText(QPointF((x_left + x_right - bounds.width()) * 0.5, y_bottom + fmsn.ascent()), m_SerieName);
            }
            /*else if (m_SerieNameAnchorPoint == Qt::AnchorPoint::AnchorLeft)
            {
                painter.rotate(90);
                painter.drawText(QPointF(x_left - bounds.width() - fmsn.horizontalAdvance("Q"), (y_top + y_bottom) * 0.5), m_SerieName);
            }
            else if (m_SerieNameAnchorPoint == Qt::AnchorPoint::AnchorRight)
            {
                painter.rotate(90);
                painter.drawText(QPointF(x_right + bounds.width() + fmsn.horizontalAdvance("Q"), (y_top + y_bottom) * 0.5), m_SerieName);
            }*/

            painter.restore();
        }

        painter.save();

        QPainterPath path;
        QPainterPathStroker path_stroker;

        QLinearGradient vertical_gradient(m_PaddingLeft, m_PaddingBottom, m_PaddingLeft, m_PaddingBottom + h);
        vertical_gradient.setColorAt(0, m_GradientStartColor);
        vertical_gradient.setColorAt(1, m_GradientEndColor);

        const double x_step = w / (double)(m_Serie.size() - 1);

        const auto& v0 = m_Serie.at((int)0);
        const double r0 = (v0 - minY) / (maxY - minY);
        const double y0 = siLerp(y_top, y_bottom, 1.0 - r0);

        double x = m_PaddingLeft;

        QPointF lp(x, y0), cp = lp;

        path.moveTo(lp);

        for (int i = 1; i < m_Serie.size(); i++)
        {
            const auto& v = m_Serie.at((int)i);
            const double r = (v - minY) / (maxY - minY);
            const double y = siLerp(y_top, y_bottom, 1.0 - r);
            const double x_mid = x + x_step * 0.5;

            x += x_step;
            cp = QPointF(x, y);

            path.cubicTo(QPointF(x_mid, lp.y()), QPointF(x_mid, y), cp);

            lp = cp;
        }

        painter.save();
        painter.setPen(QPen(QBrush(m_SerieColor), m_LineThickness));
        painter.drawPath(path);
        painter.restore();

        path.lineTo(QPointF(cp.x(), y_bottom));
        path.lineTo(QPointF(m_PaddingLeft, y_bottom));

        painter.fillPath(path, vertical_gradient);

        painter.restore();
    }

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(palette().dark().color());
    painter.setBrush(Qt::NoBrush);
}
