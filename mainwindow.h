#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>
#include <QTextEdit>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private slots:
    void getHexText();

private:
    Ui::MainWindow *ui;
    QVector<QLabel*> labels;
     QTextEdit *textEdit = new QTextEdit;
      QPoint lastPos;
};

#endif // MAINWINDOW_H

