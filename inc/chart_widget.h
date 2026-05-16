#pragma once
#include <QWidget>
#include <QPainter>
#include <vector>

class ChartWidget : public QWidget {
    Q_OBJECT
public:
    explicit ChartWidget(QWidget* parent = nullptr);
    void setData(const std::vector<double>& values);
    void setTitle(const QString& title);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    std::vector<double> values_;
    QString title_ = "Readings";
};
