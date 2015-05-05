#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DirectoryTableModel.h"
#include <QMessageBox>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    DirectoryTableModel* model = new DirectoryTableModel(this);
    ui->tableView->setModel(model);

    QLabel* status = new QLabel("", this);
    ui->statusBar->addWidget(status);

    connect(model, SIGNAL(updateInfoStatusBar(QString)), status, SLOT(setText(QString)));
    connect(model, SIGNAL(disableRecordsButton(bool)), ui->loadRecordsButton, SLOT(setDisabled(bool)));
    connect(model, SIGNAL(enableRecordsButton(bool)), ui->loadRecordsButton, SLOT(setEnabled(bool)));
    connect(model, SIGNAL(disableRecordsButton(bool)), ui->saveRecordsButton, SLOT(setDisabled(bool)));
    connect(model, SIGNAL(enableRecordsButton(bool)), ui->saveRecordsButton, SLOT(setEnabled(bool)));
    connect(model, SIGNAL(disableRecordsButton(bool)), ui->addRecordButton, SLOT(setDisabled(bool)));
    connect(model, SIGNAL(enableRecordsButton(bool)), ui->addRecordButton, SLOT(setEnabled(bool)));
    connect(ui->addRecordButton, SIGNAL(clicked()), model, SLOT(onAddRecord()));
    connect(ui->loadRecordsButton, SIGNAL(clicked()), model, SLOT(onLoadRecords()));
    connect(ui->saveRecordsButton, SIGNAL(clicked()), model, SLOT(onSaveRecords()));
}

MainWindow::~MainWindow()
{
    delete ui;
}
