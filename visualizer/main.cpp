#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <QThread>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QLabel>
#include <QtWidgets/QStylePainter>
#include <QTextEdit>
#include <QPushButton>
#include <QFileDialog>

void clean_child(int sig)
{
    int status;
    waitpid(-1, &status, WNOHANG);
}

class LabelWithSignal : public QObject {
   public:
    QLabel* label;

    LabelWithSignal(QWidget* parent) : label(new QLabel(parent)) {
    }

   public slots:
    void ShowImage() {
        QString fileName = QFileDialog::getOpenFileName(label->parentWidget(), tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));
        label->setPixmap(fileName);
        QString fileName2 = QFileDialog::getOpenFileName(label->parentWidget(), tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));
        label->setPixmap(fileName2);
        if (!fileName.isEmpty() && !fileName2.isEmpty()) {
            pid_t x = fork();
            if (x == 0) {
                execl("visualizer/script", " ", fileName.toStdString().c_str(), fileName2.toStdString().c_str(), NULL);
            }
        }
    }
};

class Screen : public QWidget
{
public:
    LabelWithSignal* label = new LabelWithSignal(this);
    QPushButton* but = new QPushButton(this);

    Screen() {
    }

    void SetUpConnections() {
        QObject::connect(but, &QPushButton::clicked, label, &LabelWithSignal::ShowImage);
    }

    void SetUp() {
        setWindowTitle("Outfit Manager");

        label->label->setText(QT_VERSION_STR);
        label->label->setScaledContents(true);
        label->label->resize(500, 500);

        but->move(0, 500);
        but->resize(100, 20);

        SetUpConnections();
    }
private:

};

int main(int argc, char ** argv) {
    signal(SIGCHLD, clean_child);

    QApplication app(argc, argv);

    Screen * scr = new Screen();
    scr->resize(369, 800);

    scr->SetUp();

    scr->show();

    return app.exec();
}
