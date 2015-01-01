#include "journal.h"
#include "ui_journal.h"
#include "QSqlQueryModel"
#include "QSqlQuery"
#include "addjournal.h"
#include "QDebug"
#include "viewjournal.h"
#include "QMessageBox"
#include "addissue.h"
#include "viewjournal.h"
#include "addvolume.h"
/* Current Journal Selected */
QString curJourn;
QString curIssue;
QString curJournName;
QString curVol;
Journal::Journal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Journal)
{
    ui->setupUi(this);
    setTableView();
    setVolumeView("-999");
}

Journal::~Journal()
{
    delete ui;
}

void Journal::on_toolButton_clicked()
{
    AddJournal *aj = new AddJournal;
    aj->exec();
   connect(aj,SIGNAL(destroyed()),aj,SLOT(deleteLater()));
    setTableView();

}

void Journal::on_toolButton_3_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();


    QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "Delete Journal", "Are you sure you want to delete this journal, this will delete associated volumes and issues?",
                                   QMessageBox::Yes|QMessageBox::No);
     if (reply == QMessageBox::Yes) {

         QString data = ui->tableView->model()->data(ui->tableView->model()->
         index(index.row(),0)).toString();
         QSqlQuery delqry(QString("delete from journal where id = %1").arg(data));
         delqry.exec();
         QSqlQuery delqry2(QString("delete from journal_volume where journal_id = %1").arg(data));
         delqry2.exec();
         QSqlQuery delqry3(QString("delete from journal_issue where journal_id = %1").arg(data));
         delqry3.exec();
         setTableView();

     } else {
       qDebug() << "Yes was *not* clicked";
     }



}

void Journal::on_tableView_clicked(const QModelIndex &index)
{
    ui->tableView->selectRow(index.row());
    QString data = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),0)).toString();
    qDebug()<<data.toInt();
    curJourn = data;
    QString data2 = ui->tableView->model()->data(ui->tableView->model()->index(index.row(),1)).toString();
    qDebug()<<data2;
     curJournName = data2;

    setVolumeView(data);
}

void Journal::setTableView(){
    QSqlQueryModel *model = new QSqlQueryModel;

      model->setQuery("SELECT id,name FROM journal");
      model->setHeaderData(0,Qt::Horizontal,tr("ID"));
      model->setHeaderData(1,Qt::Horizontal,tr("Journal Name"));
      ui->tableView->setModel(model);
      ui->tableView->verticalHeader()->setVisible(false);


    ui->tableView->resizeColumnsToContents();
    QHeaderView* header = ui->tableView->horizontalHeader();

    header->setStretchLastSection(true);

    ui->tableView->setHorizontalHeader(header);
}

void Journal::setTableView(QString arg){
    QSqlQueryModel *model = new QSqlQueryModel;
    QString qr = QString("SELECT id,name FROM JOURNAL WHERE NAME LIKE '\%%1\%'").arg(arg);
      model->setQuery(qr);
      model->setHeaderData(0,Qt::Horizontal,tr("ID"));
       model->setHeaderData(1,Qt::Horizontal,tr("Journal Name"));
      ui->tableView->setModel(model);
      ui->tableView->verticalHeader()->setVisible(false);


    ui->tableView->resizeColumnsToContents();
    QHeaderView* header = ui->tableView->horizontalHeader();

    header->setStretchLastSection(true);

    ui->tableView->setHorizontalHeader(header);
    ui->tableView->resizeRowsToContents();
}
void Journal::setVolumeView(QString index){
    QSqlQueryModel *model = new QSqlQueryModel;

      model->setQuery(QString("SELECT volume FROM journal_volume where journal_id = %1 order by volume DESC").arg(index));
       model->setHeaderData(0,Qt::Horizontal,tr("Volume"));
      ui->tableView_2->setModel(model);
      ui->tableView_2->verticalHeader()->setVisible(false);


    ui->tableView_2->resizeColumnsToContents();
    QHeaderView* header = ui->tableView_2->horizontalHeader();

    header->setStretchLastSection(true);

    ui->tableView_2->setHorizontalHeader(header);
    ui->tableView_2->resizeRowsToContents();


}

//void Journal::setIssueView(QString index){
//    QSqlQueryModel *model = new QSqlQueryModel;

//      model->setQuery(QString("SELECT issue,month,year FROM journal_issue where journal_id = %1 and volume = %2").arg(curJourn).arg(index));
//       //model->setHeaderData(0,Qt::Horizontal,tr("Volume"));
//      ui->tableView_3->setModel(model);
//      ui->tableView_3->verticalHeader()->setVisible(false);


//    ui->tableView_3->resizeColumnsToContents();
//    QHeaderView* header = ui->tableView_3->horizontalHeader();

//    header->setStretchLastSection(true);

//    ui->tableView_3->setHorizontalHeader(header);
//    ui->tableView_3->resizeRowsToContents();


//}

void Journal::on_tableView_2_activated(const QModelIndex &index)
{
    QString data = ui->tableView_2->model()->data(ui->tableView_2->model()->index(index.row(),0)).toString();
    curVol=data;
    ViewJournal *vj = new ViewJournal(curJourn,curJournName,curVol);

    vj->exec();
}

//void Journal::on_tableView_3_clicked(const QModelIndex &index)
//{
//    ui->tableView_3->selectRow(index.row());
//}

//void Journal::on_tableView_3_activated(const QModelIndex &index)
//{
//    QString data = ui->tableView_3->model()->data(ui->tableView_3->model()->index(index.row(),0)).toString();
//    curIssue = data;



//}

void Journal::on_toolButton_5_clicked()
{
    addVolume *av = new addVolume;
    av->setJournalDets(curJourn,curJournName);
   int retCode= av->exec();
    //if accepted then reset view for volume of current journal
    if(retCode==1) setVolumeView(curJourn);
    connect(av,SIGNAL(destroyed()),av,SLOT(deleteLater()));
}

//void Journal::on_toolButton_6_clicked()
//{
//    addIssue *ai = new addIssue;
//    ai->setJournalDets(curJourn,curJournName,curVol);
//    int retCode=ai->exec();
//    if(retCode==1) setIssueView(curVol);
//    connect(ai,SIGNAL(destroyed()),ai,SLOT(deleteLater()));
//}

//void Journal::on_toolButton_7_clicked()
//{
//    QModelIndex index = ui->tableView_3->currentIndex();


//    QMessageBox::StandardButton reply;
//     reply = QMessageBox::question(this, "Delete Issue", "Are you sure you want to delete this issue?",
//                                   QMessageBox::Yes|QMessageBox::No);
//     if (reply == QMessageBox::Yes) {

//         QString data = ui->tableView_3->model()->data(ui->tableView_3->model()->
//         index(index.row(),0)).toString();
//         QSqlQuery delqry(QString("delete from journal_issue where issue = %1").arg(data));
//         delqry.exec();
//         setIssueView(curVol);

//     } else {
//       qDebug() << "Yes was *not* clicked";
//     }
//}

void Journal::on_toolButton_4_clicked()
{
    QModelIndex index = ui->tableView_2->currentIndex();


    QMessageBox::StandardButton reply;
     reply = QMessageBox::question(this, "Delete Volume", "Are you sure you want to delete this volume, this will also delete issues associated with it?",
                                   QMessageBox::Yes|QMessageBox::No);
     if (reply == QMessageBox::Yes) {

         QString data = ui->tableView_2->model()->data(ui->tableView_2->
                        model()->index(index.row(),0)).toString();

         QSqlQuery delqry(QString("delete from journal_volume where volume = %1").arg(data));
         delqry.exec();
         QSqlQuery delqry2(QString("delete from journal_issue where volume = %1").arg(data));
         delqry2.exec();
         setVolumeView(curJourn);
         //setIssueView("0");

     } else {
       qDebug() << "Yes was *not* clicked";
     }
}

void Journal::on_lineEdit_textChanged(const QString &arg1)
{
    setTableView(ui->lineEdit->text());
}
