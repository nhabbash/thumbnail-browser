#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::checkBox_stateChanged()
{
    QList<QCheckBox*> checkBoxes = ui->groupBox->findChildren<QCheckBox*>();
    QSet<QString> checkedCat;
    bool all = false;

    foreach(const QCheckBox *value, checkBoxes)
    {
        if(value->whatsThis() == "all" && value->isChecked()){
            all = true;
            break;
        }

        if(value->isChecked())
            checkedCat.insert(value->whatsThis());
    }

    if(all)
    {
        foreach(QCheckBox *value, checkBoxes)
        {   if(value->whatsThis() != "all"){
                value->setChecked(true);
                value->setDisabled(true);
            }
        }
    }else
    {
        foreach(QCheckBox *value, checkBoxes)
        {   if(value->whatsThis() != "all"){
                value->setDisabled(false);
            }
        }
    }

    for (int i = 0; i < ui->listWidget->count(); ++i)
    {
        QListWidgetItem *item = ui->listWidget->item(i);
        if(!checkedCat.contains(item->text()) && !all)
            item->setHidden(true);
        else
            item->setHidden(false);
    }
}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QDialog* dialog = new QDialog();
    QVBoxLayout *vbox = new QVBoxLayout;
    QLabel* qLabel = new QLabel();
    dialog->setModal(true);
    dialog->setLayout(vbox);

    qLabel->setPixmap(QPixmap(item->whatsThis()));
    qLabel->setScaledContents(true);
    vbox->addWidget(qLabel);
    dialog->setWindowTitle(item->whatsThis());
    dialog->show();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_pushButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);
    updateUi(&path);
}

void MainWindow::updateUi(QString* path) {
    QDirIterator it(*path, QDir::Files | QDir::Dirs | QDir::NoDotDot, QDirIterator::Subdirectories);
    QSet<QString> dirs;
    QSet<QString> pics;
    QSet<QString> acceptedFiles;

    acceptedFiles.insert("jpg");
    acceptedFiles.insert("png");

    while(it.hasNext())
    {
        if(it.fileInfo().isDir() && it.fileInfo().fileName() != ".")
            dirs.insert(it.fileInfo().fileName());
        else if(acceptedFiles.contains(it.fileInfo().completeSuffix()))
            pics.insert(it.fileInfo().canonicalFilePath());
        it.next();
    }

    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setIconSize(QSize(100, 100));
    ui->listWidget->resizeMode();
    ui->listWidget->clear();

    // Fill ListWidget
    foreach (const QString &value, pics){
        QStringList path = value.split("/", QString::SkipEmptyParts);
        QString cat = path.at(path.size() - 2);
        QPixmap pic(value);
        if(pic.isNull())
            break;
        QPixmap scaled = pic.scaled(QSize(120, 120), Qt::IgnoreAspectRatio);
        QIcon ico(scaled);
        QListWidgetItem* item = new QListWidgetItem(ico, cat);
        item->setWhatsThis(value);
        ui->listWidget->addItem(item);
    }

    // Create checkboxes
    QList<QCheckBox*> checkBoxes = ui->groupBox->findChildren<QCheckBox*>();
    if(!checkBoxes.empty()) {
        foreach(QCheckBox *value, checkBoxes)
            value->deleteLater();
    }

    QCheckBox *check = new QCheckBox("all", this);
    check->setWhatsThis("all");
    check->setChecked(true);
    connect(check, SIGNAL(stateChanged(int)), this, SLOT(checkBox_stateChanged()));
    ui->groupBox->layout()->addWidget(check);

    foreach (const QString &value, dirs){
        QCheckBox *check = new QCheckBox(value, this);
        check->setWhatsThis(value);
        check->setChecked(true);
        check->setDisabled(true);
        connect(check, SIGNAL(stateChanged(int)), this, SLOT(checkBox_stateChanged()));
        ui->groupBox->layout()->addWidget(check);
    }
}
