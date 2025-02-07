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

private:
    Ui::FrequencyApplication *ui;

    std::unique_ptr<FrequencyGenerator> generator;
};
