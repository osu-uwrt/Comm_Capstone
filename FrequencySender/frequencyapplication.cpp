#include "frequencyapplication.h"
#include "./ui_frequencyapplication.h"

#include "FrequencySender.hpp"

FrequencyApplication::FrequencyApplication(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::FrequencyApplication)
{
    ui->setupUi(this);

    ui->currentFrequencyDisplay->display(0);

    minFreq_ = 10000;
    maxFreq_ = 20000;

    generator = std::make_unique<FrequencyGenerator>(minFreq_, maxFreq_);
}

FrequencyApplication::~FrequencyApplication()
{
    delete ui;
}

void FrequencyApplication::on_setFrequencyButton_clicked()
{
    generator->setFrequency((double) ui->frequencySlider->value() / 1000);
    ui->currentFrequencyDisplay->display(ui->setFrequencyDisplay->value());
}

void FrequencyApplication::on_frequencySlider_valueChanged() {

    double frequency = (maxFreq_ - minFreq_) * (double) ui->frequencySlider->value() / 1000 + minFreq_;
    ui->setFrequencyDisplay->display(frequency);
}

void FrequencyApplication::on_zeroButton_clicked() {
    generator->setFrequency(-1);
    ui->currentFrequencyDisplay->display(0);
}

// FrequencyApplication::
