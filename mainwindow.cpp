#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 设置窗口大小
    setFixedSize(1500, 1000);
    // 创建 QPushButton 控件
    QPushButton *button = new QPushButton("  getHexText", this);
    button->setGeometry(10, 10, 150, 30);
    connect(button, &QPushButton::clicked, this, &MainWindow::getHexText);

    // 设置初始参数
    int blockSize = 3;
    int numBlocks = 200;
    int startX = 750; // 圆心坐标
    int startY = 500;
    double angleIncrement = 1.8 * M_PI / 180; // 角度增量

    // 生成32组色块
    for (int i = 0; i < 32; ++i) {
        // 计算当前半径
        int radius = 20 + i * 10;

        // 创建 QLabel 控件
        for (int j = 0; j < numBlocks; ++j) {
            // 计算当前角度
            double angle = j * angleIncrement;

            // 计算当前块的位置
            int x = startX + radius * cos(angle);
            int y = startY + radius * sin(angle);

            QLabel *label = new QLabel(this);
            label->setFixedSize(blockSize, blockSize);
            label->move(x, y);
            label->setStyleSheet("background-color: white;");
            labels.push_back(label);
        }
    }


}
 int j = 0;
// 点击按钮时改变颜色
void MainWindow::getHexText()
{

    int led[100][8] = {0x0};



    int bit = 0;
    // 左一到中间刷一次
    QString tmp = "";
    for (int j = 0; j < 100; j++) {
        tmp = "";
        bit = 0;
        for (int i = 0; i< 32; i++) {
            // labels[6400-i*200-100 + j]->setStyleSheet("background-color: black;");
            //1-32位
            if (labels[6400 - i * 200 - 100 + j]->palette().background().color() == Qt::black) {
                 //   qDebug()<<"第"<<j<<"行, 的 第 "<<i <<"位为1  "<<endl;
                    tmp += "1";
                    led[j][bit/8] |= 1<< (bit %8);
            } else {
                 tmp += "0";
            }

            bit++;
        }
        //可以与上面的for合并 为了符合人类阅读改为1-32 32-64
        for (int i = 0; i< 32; i++) {
               //32-64位
            if ( labels[i*200+ j]->palette().background().color() == Qt::black) {

                //qDebug()<<"第"<<j<<"行, 的 第 "<< 32 + i <<"位为1  "<<endl;
                tmp += "1";
                led[j][bit/8] |= 1<< (bit %8);
            } else {
                 tmp += "0";
            }

            bit++;
        }


    }

    QString result = "char ledHex[100][8];\n";
      for (int j = 0; j < 100 ; ++j) {
          result += "ledHex["+ QString::number(j) + "][] = {";
            for (int i = 0; i < 8; ++i) {
                result += QString("0x%1, ").arg(led[j][i], 2, 16, QChar('0'));
            }
            // 去掉最后的逗号和空格
            result.chop(2);
            result += "};\n";
      }


    // 创建一个文本编辑框，并将结果写入

    textEdit->setText(result);
    textEdit->setReadOnly(true); // 设置为只读
    textEdit->setFixedSize(800, 400); // 设置大小
    textEdit->show();

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 按下左键，颜色变黑
        QLabel *label = qobject_cast<QLabel*>(childAt(event->pos()));
        if (label) {
            label->setStyleSheet("background-color: black;");
        }
    } else if (event->button() == Qt::RightButton) {
        // 按下右键，颜色变白
        QLabel *label = qobject_cast<QLabel*>(childAt(event->pos()));
        if (label) {
            label->setStyleSheet("background-color: white;");
        }
    }

    // 记录鼠标按下位置
    lastPos = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    // 鼠标按下并拖动时，经过色块，颜色变黑或变白
    if (event->buttons() & Qt::LeftButton || event->buttons() & Qt::RightButton) {
        QLabel *label = qobject_cast<QLabel*>(childAt(event->pos()));
        if (label) {
            if (event->buttons() & Qt::LeftButton) {
                // 按下左键，颜色变黑
                label->setStyleSheet("background-color: black;");
            } else if (event->buttons() & Qt::RightButton) {
                // 按下右键，颜色变白
                label->setStyleSheet("background-color: white;");
            }
        }
    }
}



#include <QPainter>
#include <QFont>

// 确定汉字和位置
QString chinese = "好";
int chineseSize = 350;
int chineseX = 750 - chineseSize / 2; // 汉字的中心位置
int chineseY = 500 + chineseSize / 2;
void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    // 创建 QPainter 对象
    QPainter painter(this);

    // 设置字体
    QFont font;
    font.setPixelSize(chineseSize);
    painter.setFont(font);

    // 获取文本边界框
    QFontMetrics metrics(font);
    QRect chineseRect = metrics.boundingRect(QRect(0, 0, chineseSize, chineseSize), Qt::AlignCenter, chinese);

    // 检测被汉字覆盖到的色块并将其颜色设置为黑色
    for (QLabel *label : labels) {
        QRect blockRect(label->x(), label->y(), 3, 3);
        if (chineseRect.intersects(blockRect)) {
            label->setStyleSheet("background-color: black;");
        }
    }

    // 绘制完成后移除汉字
    chinese.clear();
}

