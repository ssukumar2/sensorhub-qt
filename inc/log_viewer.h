#pragma once
#include <QWidget>
#include <QTextEdit>
#include <QTimer>

class LogViewer : public QWidget {
    Q_OBJECT
public:
    explicit LogViewer(QWidget* parent = nullptr);
    void appendLog(const QString& level, const QString& message);
    void clear();

private:
    QTextEdit* textEdit_;
    int maxLines_ = 500;
};
