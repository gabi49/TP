#ifndef MAFENETRE_H
#define MAFENETRE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MaFenetre; }
QT_END_NAMESPACE

class MaFenetre : public QWidget
{
    Q_OBJECT

public:
    MaFenetre(QWidget *parent = nullptr);
    ~MaFenetre();

    private slots: void on_Connect_clicked();
    private slots: void on_Disconnect_clicked();
    private slots: void on_selectionCarte_clicked();
    private slots: void on_mise_a_jour_clicked();

    void on_Buy_clicked();

    void on_charger_clicked();

private:
    Ui::MaFenetre *ui;
};
#endif // MAFENETRE_H
