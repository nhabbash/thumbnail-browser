#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QDirIterator>
#include <QDir>
#include <QDebug>
#include <QSet>
#include <QCheckBox>
#include <QLabel>
#include <QDialog>
#include <QFileDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateUi(QString* path);

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void checkBox_stateChanged();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
