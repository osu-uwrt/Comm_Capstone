#include "frequencyapplication.h"
#include "./ui_frequencyapplication.h"

#include "FrequencySender.hpp"

FrequencyApplication::FrequencyApplication(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FrequencyApplication)
{
    ui->setupUi(this);

    generator = std::make_unique<FrequencyGenerator>(20, 30);
}

FrequencyApplication::~FrequencyApplication()
{
    delete ui;
}
