#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(const QString& currentUrl, int refreshInterval, QWidget* parent = nullptr);
    QString backendUrl() const;
    int refreshInterval() const;
private:
    QLineEdit* urlEdit_;
    QSpinBox* intervalSpin_;
};
