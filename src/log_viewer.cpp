#include "log_viewer.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDateTime>
#include <QScrollBar>

LogViewer::LogViewer(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);
    auto* title = new QLabel("Event Log", this);
    QFont f;
    f.setBold(true);
    title->setFont(f);
    layout->addWidget(title);

    textEdit_ = new QTextEdit(this);
    textEdit_->setReadOnly(true);
    textEdit_->setFont(QFont("Monospace", 9));
    textEdit_->setStyleSheet("background-color: #1e1e1e; color: #d4d4d4;");
    layout->addWidget(textEdit_);
}

void LogViewer::appendLog(const QString& level, const QString& message) {
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString color;
    if (level == "ERROR") color = "#f44747";
    else if (level == "WARN") color = "#cca700";
    else if (level == "INFO") color = "#4ec9b0";
    else color = "#808080";

    QString html = QString("<span style='color:%1'>[%2] %3</span> %4")
        .arg(color, timestamp, level, message.toHtmlEscaped());
    textEdit_->append(html);

    // Auto-scroll to bottom
    auto* bar = textEdit_->verticalScrollBar();
    bar->setValue(bar->maximum());

    // Trim old lines
    if (textEdit_->document()->lineCount() > maxLines_) {
        QTextCursor cursor = textEdit_->textCursor();
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, 50);
        cursor.removeSelectedText();
    }
}

void LogViewer::clear() {
    textEdit_->clear();
}
