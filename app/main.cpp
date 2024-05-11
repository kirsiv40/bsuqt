#include <QThread>
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QLabel>
#include <QtWidgets/QStylePainter>
#include <QTextEdit>
#include <QFileDialog>
#include <QInputDialog>
#include <QMouseEvent>
#include <iostream>
#include <QPropertyAnimation>
#include <QPoint>

const int screen_w = 400;
const int screen_h = 860;
const int anim_duration = 150;
const double anim_scale = 0.95;

class Item : public QLabel
{
   public:
    QPropertyAnimation* move_anim;
    QPropertyAnimation* r_move_anim;
    QPropertyAnimation* size_anim;
    QPropertyAnimation* r_size_anim;

    Item(QWidget* parent = nullptr) : QLabel(parent) {
        move_anim = new QPropertyAnimation(this, "pos", parent);
        r_move_anim = new QPropertyAnimation(this, "pos", parent);
        size_anim = new QPropertyAnimation(this, "size", parent);
        r_size_anim = new QPropertyAnimation(this, "size", parent);
        move_anim->setDuration(anim_duration);
        r_move_anim->setDuration(anim_duration);
        size_anim->setDuration(anim_duration);
        r_size_anim->setDuration(anim_duration);
    }

    void CalculateAnimations() {
        double pos_shift = (1.0F - anim_scale) / 2;
        QPoint point_size = QPoint(size().rwidth(), size().rheight());  // so we can add up point and size

        move_anim->setStartValue(pos());
        move_anim->setEndValue(pos() + pos_shift * point_size);

        r_move_anim->setStartValue(pos() + pos_shift * point_size);
        r_move_anim->setEndValue(pos());

        size_anim->setStartValue(size());
        size_anim->setEndValue(anim_scale * size());

        r_size_anim->setStartValue(anim_scale * size());
        r_size_anim->setEndValue(size());
    }

   protected:
    void mousePressEvent(QMouseEvent* event) override {
        std::cerr << "pressed\n";
        move_anim->start();
        size_anim->start();
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        std::cerr << "released\n";
        r_move_anim->start();
        r_size_anim->start();
    }
};

class CollageScene : public QWidget
{
   public:
    std::vector<Item*> items_;

    CollageScene(std::vector<QString> files) {
        items_.resize(files.size());
        for (size_t i = 0; i < items_.size(); i++) {
            items_[i] = new Item(this);
            items_[i]->setPixmap(files[i]);
        }
    }

    void SetUp() {
        setWindowTitle("Collage");   

        int sum_h = 10 * (items_.size() - 1) + 40;
        int max_w = 0;
        for (auto& item : items_) {
            sum_h += item->pixmap().height();
            max_w = std::max(max_w, item->pixmap().width());
        }
        max_w += 20;

        double scaling_factor = std::min(static_cast<double>(screen_w) / static_cast<double>(max_w),
                                         static_cast<double>(screen_h) / static_cast<double>(sum_h));

        int y_coord = 20;
        for (auto* item : items_) {
            const auto w = scaling_factor * static_cast<double>(item->pixmap().width());
            const auto h = scaling_factor * static_cast<double>(item->pixmap().height());
            item->setScaledContents(true);
            item->resize(w, h);

            const int align_w = (screen_w - w) / 2;

            item->move(align_w, y_coord);
            y_coord += h + 10;
        }

        for (auto* item : items_) {
            item->CalculateAnimations();
        }

    }
};

int main(int argc, char ** argv) {
    QApplication app(argc, argv);

    int items_cnt = QInputDialog::getInt(nullptr, "a", "a");
    std::vector<QString> files(items_cnt);
    for (auto& file : files) {
        file = QFileDialog::getOpenFileName(nullptr, "Open Image", "/home", "Image Files (*.png *.jpg *.bmp *.jpeg)");
    }
    CollageScene scr(files);

    scr.resize(screen_w, screen_h);
    scr.SetUp();
    scr.show();

    return app.exec();
}