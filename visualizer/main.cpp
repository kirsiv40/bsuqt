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
#include <QCheckBox>
#include <QRadioButton>

constexpr int w = 369;
constexpr int h = 800;

class WorkerThread : public QThread
{
public:
    QString path1;
    QString path2;
    std::string cpos;

    void SetUp(QString p1, QString p2, std::string clpos) {
        path1 = p1;
        path2 = p2;
        cpos = clpos;
    }

    void run() override {
        int aaaa;
        int x = fork();
        if (x == 0) {
            // execl("visualizer/script", " ", "/home/kirsiv40/qtbsu/visualizer/pic/basemodel2.png", "/home/kirsiv40/qtbsu/visualizer/pic/top222.JPG", "resultpath");
            execl("visualizer/script", " ", path1.toStdString().c_str(), path2.toStdString().c_str(), "resultpath", cpos.c_str());
        }
        ::wait(&aaaa);
        std::cout << "neuro ended" << std::endl;
    }
};

class PositionChooseButtons : public QWidget {
   public:
    QRadioButton* top;
    QRadioButton* bot;
    QRadioButton* dress;

    PositionChooseButtons(QWidget* parent) : QWidget(parent) {
    }

    void SetUp() {
        top = new QRadioButton(this);
        bot = new QRadioButton(this);
        dress = new QRadioButton(this);

        top->setGeometry(0, 0, 100, 20);
        bot->setGeometry(125, 0, 100, 20);
        dress->setGeometry(250, 0, 100, 20);

        top->setText("Top");
        bot->setText("Bot");
        dress->setText("Dress");

        top->setChecked(true);
    }
};

class CleverLabel : public QObject {
   public:
    QLabel* label;
    QLabel* label2;
    WorkerThread* thr;
    QPushButton* but;
    QString fileName;
    QString fileName2;
    QLabel* neuroresultimage;
    PositionChooseButtons* cb;

    CleverLabel(QWidget* parent, QPushButton* other, PositionChooseButtons* chk) : 
                label(new QLabel(parent)), label2(new QLabel(parent)), thr(new WorkerThread()),
                but(other), neuroresultimage(new QLabel(parent)), cb(chk) {
        QObject::connect(thr, &WorkerThread::finished, this, &CleverLabel::NeuroHandler);
    }

    void SetUp() {
        label->setText(QT_VERSION_STR);
        label->setScaledContents(true);
        label->resize(w / 2, w / 2);

        label2->setScaledContents(true);
        label2->resize(w / 2, w / 2);
        label2->move(w / 2, 0);

        neuroresultimage->setGeometry(35, 225, 300, 400);
        neuroresultimage->setScaledContents(true);

        neuroresultimage->setPixmap(QPixmap("/home/kirsiv40/qtbsu/visualizer/pic/top222.JPG"));
    }

   public slots:
    void ShowImage() {
        // QString fileName = QFileDialog::getOpenFileName(label->parentWidget(), tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
        // if (!fileName.isEmpty()) {
        //     label->setPixmap(fileName);
        // }
        // QString fileName2 = QFileDialog::getOpenFileName(label->parentWidget(), tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
        // if (!fileName2.isEmpty()) {
        //     label2->setPixmap(fileName);
        // }
        if (!fileName.isEmpty() && !fileName2.isEmpty()) {
            but->setEnabled(false);
            if (cb->top->isChecked()) {
                thr->SetUp(fileName, fileName2, "Upper-body");
            } else if (cb->bot->isChecked()) {
                thr->SetUp(fileName, fileName2, "Lower-body");
            } else {
                thr->SetUp(fileName, fileName2, "Dress");
            }
            thr->start();
        } else {
            //================================================================================================
        }
    }

    void NeuroHandler() {
        std::string path;
        std::ifstream fin("resultpath");
        fin >> path;
        fin.close();
        neuroresultimage->setPixmap(QString(path.data()));
        but->setEnabled(true);
    }
};

class ImageOpenerButton : public QPushButton {
   public:
    ImageOpenerButton(QWidget* parent) : QPushButton(parent) {
    }

    void SetUpConnections() {
        QObject::connect(this, &QPushButton::clicked, this, &ImageOpenerButton::HandlePress);
    }

    void SetUp(QLabel* target, QString* target2) {
        _targetimage = target;
        _targetpath = target2;

        SetUpConnections();
    }

   public slots:
    void HandlePress() {
        QString fileName = QFileDialog::getOpenFileName(this->parentWidget(), tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));
        if (!fileName.isEmpty()) {
            _targetimage->setPixmap(fileName);
            *_targetpath = fileName;
        }
    }

   private:
    QLabel* _targetimage;
    QString* _targetpath;
};

class Screen : public QWidget
{
public:
    QPushButton* but = new QPushButton(this);
    PositionChooseButtons* ps = new PositionChooseButtons(this);

    CleverLabel* cl = new CleverLabel(this, but, ps);

    QPushButton* ok = new QPushButton(this);
    QPushButton* notok = new QPushButton(this);

    ImageOpenerButton* firstimagebut = new ImageOpenerButton(this);
    ImageOpenerButton* secondimagebut = new ImageOpenerButton(this);

    Screen() {
    }

    void SetUpConnections() {
        QObject::connect(but, &QPushButton::clicked, cl, &CleverLabel::ShowImage);
    }

    void SetUp() {
        setWindowTitle("Outfit Manager");

        // cl->label->setText(QT_VERSION_STR);
        // cl->label->setScaledContents(true);
        // cl->label->resize(w / 2, w / 2);

        // cl->label2->setScaledContents(true);
        // cl->label2->resize(w / 2, w / 2);
        // cl->label2->move(w / 2, 0);

        // cl->neuroresultimage->setGeometry(35, 225, 300, 400);
        // cl->neuroresultimage->setScaledContents(true);

        // cl->neuroresultimage->setPixmap(QPixmap("/home/kirsiv40/qtbsu/visualizer/pic/top222.JPG"));

        cl->SetUp();

        but->move(10, 650);
        but->resize(349, 40);
        but->setEnabled(true);
        but->setText("Generate");

        ok->setGeometry(20, h - 50, w / 2 - 30, h / 40);
        notok->setGeometry(w / 2 + 10, h - 50, w / 2 - 30, h / 40);

        firstimagebut->SetUp(cl->label, &cl->fileName);
        secondimagebut->SetUp(cl->label2, &cl->fileName2);

        firstimagebut->setGeometry(10, w / 2 + 10, w / 2 - 30, h / 40);
        secondimagebut->setGeometry(w / 2 + 10, w / 2 + 10, w / 2 - 20, h / 40);

        firstimagebut->setText("Person");
        secondimagebut->setText("Cloth");

        ps->setGeometry(10, 700, 349, 20);
        ps->SetUp();

        SetUpConnections();
    }

private:
};

int main(int argc, char ** argv) {
    QApplication app(argc, argv);

    Screen* scr = new Screen;
    scr->resize(w, h);

    scr->SetUp();

    scr->show();

    return app.exec();
}
