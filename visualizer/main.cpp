#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>

#include <QThread>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QLabel>
#include <QtWidgets/QStylePainter>
#include <QTextEdit>
#include <QPushButton>
#include <QFileDialog>

class WorkerThread : public QThread
{
public:
    QString path1;
    QString path2;

    void SetUp(QString p1, QString p2) {
        path1 = p1;
        path2 = p2;
    }

    void run() override {
        int aaaa;
        int x = fork();
        if (x == 0) {
            // execl("visualizer/script", " ", "/home/kirsiv40/qtbsu/visualizer/pic/basemodel2.png", "/home/kirsiv40/qtbsu/visualizer/pic/top222.JPG", "resultpath");
            execl("visualizer/script", " ", path1.toStdString().c_str(), path2.toStdString().c_str(), "resultpath");
        }
        ::wait(&aaaa);
        std::cout << "neuro ended" << std::endl;
    }
};

class CleverLabel : public QObject {
   public:
    QLabel* label;
    QLabel* label2;
    WorkerThread* thr;
    QPushButton* but;

    CleverLabel(QWidget* parent, QPushButton* other) : label(new QLabel(parent)), label2(new QLabel(parent)), thr(new WorkerThread()), but(other) {
        QObject::connect(thr, &WorkerThread::finished, this, &CleverLabel::NeuroHandler);
    }

   public slots:
    void ShowImage() {
        QString fileName = QFileDialog::getOpenFileName(label->parentWidget(), tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
        if (!fileName.isEmpty()) {
            label->setPixmap(fileName);
        }
        QString fileName2 = QFileDialog::getOpenFileName(label->parentWidget(), tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
        if (!fileName2.isEmpty()) {
            label2->setPixmap(fileName);
        }
        if (!fileName.isEmpty() && !fileName2.isEmpty()) {
            but->setEnabled(false);
            thr->SetUp(fileName, fileName2);
            thr->start();
        }
    }

    void NeuroHandler() {
        std::string path;
        std::ifstream fin("resultpath");
        fin >> path;
        fin.close();
        label->setPixmap(QString(path.data()));
        but->setEnabled(true);
    }
};

class Screen : public QWidget
{
public:
    QPushButton* but = new QPushButton(this);
    CleverLabel* label = new CleverLabel(this, but);

    Screen() {
    }

    void SetUpConnections() {
        QObject::connect(but, &QPushButton::clicked, label, &CleverLabel::ShowImage);
    }

    void SetUp() {
        setWindowTitle("Outfit Manager");

        label->label->setText(QT_VERSION_STR);
        label->label->setScaledContents(true);
        label->label->resize(369, 492);

        label->label2->setScaledContents(true);
        label->label2->resize(200, 200);
        label->label2->move(168, 500);

        but->move(0, 500);
        but->resize(100, 20);

        SetUpConnections();
    }

private:
};

int main(int argc, char ** argv) {
    QApplication app(argc, argv);

    Screen* scr = new Screen;
    scr->resize(369, 800);

    scr->SetUp();

    scr->show();

    return app.exec();
}
