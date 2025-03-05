#pragma once

#include <QMainWindow>

#include "FrequencySender.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class FrequencyApplication;
}
QT_END_NAMESPACE

class FrequencyApplication : public QMainWindow
{
    Q_OBJECT

public:
    FrequencyApplication(QWidget *parent = nullptr);
    ~FrequencyApplication();

private slots:
    // void on_zeroButton_clicked();
    void on_setFrequencyButton_clicked();
    void on_frequencySlider_valueChanged();
    void on_zeroButton_clicked();

private:
    int maxFreq_, minFreq_;

    Ui::FrequencyApplication *ui;
    std::unique_ptr<FrequencyGenerator> generator;
};
