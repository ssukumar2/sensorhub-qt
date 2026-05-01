#include "chart_widget.h"
#include <algorithm>
#include <cmath>

ChartWidget::ChartWidget(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(200);
    setMinimumWidth(300);
}

void ChartWidget::setData(const std::vector<double>& values) {
    values_ = values;
    update();
}

void ChartWidget::setTitle(const QString& title) {
    title_ = title;
    update();
}

void ChartWidget::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    int w = width();
    int h = height();
    int margin = 40;

    // Background
    p.fillRect(rect(), QColor(30, 30, 30));

    // Title
    p.setPen(QColor(200, 200, 200));
    p.setFont(QFont("Sans", 11, QFont::Bold));
    p.drawText(margin, 20, title_);

    if (values_.empty()) return;

    double minV = *std::min_element(values_.begin(), values_.end());
    double maxV = *std::max_element(values_.begin(), values_.end());
    if (std::abs(maxV - minV) < 0.001) { minV -= 1; maxV += 1; }

    int plotW = w - margin * 2;
    int plotH = h - margin * 2;

    // Grid lines
    p.setPen(QPen(QColor(60, 60, 60), 1));
    for (int i = 0; i <= 4; ++i) {
        int y = margin + (plotH * i / 4);
        p.drawLine(margin, y, w - margin, y);
        double val = maxV - (maxV - minV) * i / 4.0;
        p.setPen(QColor(120, 120, 120));
        p.setFont(QFont("Sans", 8));
        p.drawText(0, y - 6, margin - 4, 12, Qt::AlignRight | Qt::AlignVCenter,
                   QString::number(val, 'f', 1));
        p.setPen(QPen(QColor(60, 60, 60), 1));
    }

    // Line
    p.setPen(QPen(QColor(74, 144, 217), 2));
    int n = static_cast<int>(values_.size());
    for (int i = 1; i < n; ++i) {
        double x0 = margin + plotW * (i - 1.0) / (n - 1);
        double y0 = margin + plotH * (1.0 - (values_[i - 1] - minV) / (maxV - minV));
        double x1 = margin + plotW * (double)i / (n - 1);
        double y1 = margin + plotH * (1.0 - (values_[i] - minV) / (maxV - minV));
        p.drawLine(QPointF(x0, y0), QPointF(x1, y1));
    }

    // Dots
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(74, 144, 217));
    for (int i = 0; i < n; ++i) {
        double x = margin + plotW * (double)i / (n - 1);
        double y = margin + plotH * (1.0 - (values_[i] - minV) / (maxV - minV));
        p.drawEllipse(QPointF(x, y), 3, 3);
    }
}
