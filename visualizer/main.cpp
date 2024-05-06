#include <unistd.h>

#include <QThread>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QLabel>
#include <QtWidgets/QStylePainter>
#include <QTextEdit>
#include <QPushButton>
#include <QFileDialog>

class LabelWithSignal : public QObject {
   public:
    QLabel* label;
    QTextEdit* data_holder;

    LabelWithSignal(QWidget* parent, QTextEdit* holder) : label(new QLabel(parent)), data_holder(holder) {
    }

   public slots:
    void ShowImage() {
        QString fileName = QFileDialog::getOpenFileName(label->parentWidget(), tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));
        label->setPixmap(fileName);
        QString fileName2 = QFileDialog::getOpenFileName(label->parentWidget(), tr("Open Image"), "/home", tr("Image Files (*.png *.jpg *.bmp)"));
        label->setPixmap(fileName2);
        execl("visualizer/script", " ", fileName.toStdString().c_str(), fileName2.toStdString().c_str(), NULL);
    }
};

class Screen : public QWidget
{
public:
    QTextEdit* l = new QTextEdit(this);
    LabelWithSignal* label = new LabelWithSignal(this, l);
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

        l->setText("aaaaa");
        l->move(500, 500);

        SetUpConnections();
    }
private:

};

int main(int argc, char ** argv) {
    QApplication app(argc, argv);

    Screen * scr = new Screen();
    scr->resize(600, 600);

    scr->SetUp();

    scr->show();

    return app.exec();
}
