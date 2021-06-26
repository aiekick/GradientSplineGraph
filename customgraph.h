#ifndef CUSTOMGRAPH_H
#define CUSTOMGRAPH_H

#include <QVector>
#include <QWidget>
#include <QSize>
#include <QPen>
#include <QBrush>
#include <QColor>

class CustomGraph : public QWidget
{
    Q_OBJECT
public:
    explicit CustomGraph(QWidget *parent = nullptr);

    void Clear();
    void SetSerie(QVector<double> vSerie);
    void SetSerie(
            QVector<double> vSerie,
            const QColor& vSerieColor,
            const double& vLineThickness);
    void SetPadding(
            const double& vPaddingLeft,
            const double& vPaddingBottom,
            const double& vPaddingRight,
            const double& vPaddingTop);
    void SetVerticalGradient(
            const QColor& vStart,
            const QColor& vEnd);
    void SetLinesColor(const QColor& vLinesColor);
    void SetValuesColor(const QColor& vValuesColor);
    void SetValuesFont(const QFont& vValuesFont);
    void SetSerieName(
            const QString& vSerieName,
            const QFont& vSerieNameFont,
            const QColor& vSerieNameColor,
            const Qt::AnchorPoint& vSerieNameAnchorPoint = Qt::AnchorPoint::AnchorTop);
    void SetSerieName(const QString& vSerieName);
    void SetSerieCurveStyle(
            const QColor& vSerieColor,
            const double& vLineThickness);
    void SetSerieNameStyle(
            const QFont& vSerieNameFont,
            const QColor& vSerieNameColor);
    void SetDrawSerieName(const bool& vDrawSerieName);
    void SetDrawRect(const bool& vDrawRect);
    void SetXAxis(
            const int& vCounstStepX,
            const bool &ShowLinesX,
            const bool &ShowValuesX,
            const double &vRangeInfX, const double &vRangeSupX);
    void SetYAxis(
            const int& vCounstStepY,
            const bool &ShowLinesY,
            const bool &ShowValuesY,
            const int& vPrecisionY);
    void SetBackgroundColor(const QColor& vBackgroundColor);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void setFont(const QFont& vFont);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<double> m_Serie;

    QColor m_SerieColor = QColor(0,0,0,255);
    QColor m_BackgroundColor = QColor(255,255,255,255);
    QColor m_GradientStartColor = QColor(255,255,255,255);
    QColor m_GradientEndColor = QColor(255,255,255,255);
    QColor m_LinesColor = QColor(0,0,0,255);

    QFont m_ValuesFont;
    QColor m_ValuesColor = QColor(0,0,0,255);

    double m_LineThickness = 1.0;

    double m_MinValue = 0.0;
    double m_MaxValue = 0.0;

    double m_PaddingLeft = 10.0;
    double m_PaddingBottom = 10.0;
    double m_PaddingRight = 10.0;
    double m_PaddingTop = 10.0;

    bool m_DrawRect  = true;

    QString m_SerieName = "";
    QFont m_SerieNameFont;
    QColor m_SerieNameColor = QColor(0,0,0,255);
    Qt::AnchorPoint m_SerieNameAnchorPoint = Qt::AnchorPoint::AnchorTop;
    bool m_DrawSerieName = true;

    int m_CountStepsX = -1;
    bool m_ShowLinesX = false;
    bool m_ShowValuesX = false;
    double m_RangeInfX = 0.0;
    double m_RangeSupX = 0.0;

    int m_CountStepsY = -1;
    bool m_ShowLinesY = false;
    bool m_IntersectLinesY = false;
    bool m_ShowValuesY = false;
    int m_PrecisionY = 5;
};

#endif // CUSTOMGRAPH_H
