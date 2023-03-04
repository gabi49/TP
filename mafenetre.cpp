#include "mafenetre.h"
#include "ui_mafenetre.h"

#include "MfErrNo.h"
#include "Core.h"
#include "Sw_Device.h"
#include "Sw_Mf_Classic.h"
#include "Sw_Poll.h"
#include "Sw_ISO14443A-3.h"
#include "TypeDefs.h"
#include "Tools.h"

#include <QtGui>


MaFenetre::MaFenetre(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MaFenetre)
{
    ui->setupUi(this);

}

MaFenetre::~MaFenetre()
{
    delete ui;
}

ReaderName MonLecteur;

void MaFenetre::on_Connect_clicked() {
    int16_t status = MI_OK;
    MonLecteur.Type = ReaderCDC;
    MonLecteur.device = 0;
    status = OpenCOM(&MonLecteur);
    qDebug() << "OpenCOM" << status;

    status = Version(&MonLecteur);
    ui->Affichage->setText(MonLecteur.version);
    ui->Affichage->update();

   // QString Text = ui->fenetre_saisi->toPlainText();
    //qDebug() << "Text : " << Text;

    status = RF_Power_Control(&MonLecteur, TRUE, 0);

}

void MaFenetre::on_Disconnect_clicked() {
    int16_t status = MI_OK;
    RF_Power_Control(&MonLecteur, FALSE, 0);
    status = LEDBuzzer(&MonLecteur, LED_OFF);
    status = CloseCOM(&MonLecteur); qApp->quit();
}


void MaFenetre::on_selectionCarte_clicked()
{
    uint8_t data[240] = {0};
    uint8_t atq[2];
    uint8_t sak[1];
    uint8_t uid[12];
    uint16_t uid_len = 12;

    int16_t status = MI_OK;

    status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
    status = Mf_Classic_Read_Block(&MonLecteur, TRUE, 9, data, AuthKeyA, 2);
    ui->nom->setText((char*)data);
    status = Mf_Classic_Read_Block(&MonLecteur, TRUE, 10, data, AuthKeyA, 2);
    ui->prenom->setText((char*)data);
    //qDebug() << *data;

   /* status = LEDBuzzer(&MonLecteur, LED_GREEN_ON+LED_YELLOW_ON+LED_RED_ON+LED_GREEN_ON);
    DELAYS_MS(1);
    status = LEDBuzzer(&MonLecteur, LED_GREEN_ON);*/

}

void MaFenetre::on_mise_a_jour_clicked()
{
    uint8_t atq[2];
    uint8_t sak[1];
    uint8_t uid[12];
    uint16_t uid_len = 12;
    int16_t status = MI_OK;

    status = LEDBuzzer(&MonLecteur, LED_GREEN_ON+LED_YELLOW_ON+LED_RED_ON+LED_GREEN_ON);
    DELAYS_MS(1);
    status = LEDBuzzer(&MonLecteur, LED_GREEN_ON);

    status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
    char DataIn1[16];
       strncpy(DataIn1, ui->nom->toPlainText().toUtf8().data(), 16);
    status = Mf_Classic_Write_Block(&MonLecteur, TRUE, 	9,(uint8_t*)DataIn1, AuthKeyB, 2 );

    char DataIn[16];
       strncpy(DataIn, ui->prenom->toPlainText().toUtf8().data(), 16);
    status = Mf_Classic_Write_Block(&MonLecteur, TRUE, 	10, (uint8_t*)DataIn, AuthKeyB, 2 );

}


void MaFenetre::on_Buy_clicked()
{
    uint32_t pValue = 0;
    uint8_t atq[2];
    uint8_t sak[1];
    uint8_t uid[12];
    uint16_t uid_len = 12;
    int16_t status = MI_OK;

    uint32_t Valeurdecrementer = 0;
    Valeurdecrementer = ui ->valeurDecrementer->value();
    status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);

    status =  Mf_Classic_Read_Value(&MonLecteur, TRUE, 14, &pValue, AuthKeyA, 3 );


    status =  Mf_Classic_Decrement_Value(&MonLecteur, TRUE, 14, Valeurdecrementer, 13, AuthKeyA, 3 );
    status =  Mf_Classic_Restore_Value(&MonLecteur, TRUE, 13, 14, AuthKeyA, 3);
     ui->nombreUnite->setText(QString::number(pValue));

     status = LEDBuzzer(&MonLecteur, LED_GREEN_ON+LED_YELLOW_ON+LED_RED_ON+LED_GREEN_ON);
     DELAYS_MS(500);
     status = LEDBuzzer(&MonLecteur, LED_RED_ON);

}



void MaFenetre::on_charger_clicked()
{
    uint32_t pValue = 0;
    uint8_t atq[2];
    uint8_t sak[1];
    uint8_t uid[12];
    uint16_t uid_len = 12;
    int16_t status = MI_OK;

     uint32_t ValeurEcrire = 0;
     ValeurEcrire = ui ->valeurIncrementer->value();

     status = ISO14443_3_A_PollCard(&MonLecteur, atq, sak, uid, &uid_len);
     //status = Mf_Classic_Write_Value(&MonLecteur, TRUE, 14, ValeurEcrire,  AuthKeyB, 3 );

     status = Mf_Classic_Increment_Value(&MonLecteur, TRUE, 14, ValeurEcrire, 13, AuthKeyB, 3);
     status =  Mf_Classic_Restore_Value(&MonLecteur, TRUE, 13, 14, AuthKeyB, 3);

     //AFFICAGE DE LA VALEUR FINAL D'UNITE
     status =  Mf_Classic_Read_Value(&MonLecteur, TRUE, 14, &pValue, AuthKeyA, 3 );
     ui->nombreUnite->setText(QString::number(pValue));


     status = LEDBuzzer(&MonLecteur, LED_GREEN_ON+LED_YELLOW_ON+LED_RED_ON+LED_GREEN_ON);
     DELAYS_MS(500);
     status = LEDBuzzer(&MonLecteur, LED_RED_ON);


}
