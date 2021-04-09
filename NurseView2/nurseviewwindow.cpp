#include "nurseviewwindow.h"
#include "ui_nurseviewwindow.h"

NurseViewWindow::NurseViewWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NurseViewWindow)
{
    ui->setupUi(this);

    /************************************************************/

    ui->tableWidget->setWindowTitle("illness list");
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(3);
    QStringList header;
    header<<"����"<<"�Ա�"<<"����"<<"ʱ��"<<"ҽ��";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setAlternatingRowColors(true);
    ui->tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);

    /************************************************************/
    signalMapper = new QSignalMapper(this);
    connect(signalMapper,SIGNAL(mapped(const QString &)),this,SLOT(changed(const QString &)));

    /*for(int i=0;i<30;i++){
        QComboBox *tmp=new QComboBox();
        tmp->addItem("");
        tmp->addItem("doctor1");
        tmp->addItem("doctor2");
        tmp->addItem("doctor3");
        connect(tmp,SIGNAL(currentIndexChanged(int)),signalMapper,SLOT(map()));
        signalMapper->setMapping(tmp,QString("%1-%2").arg(i).arg(5));

        cbDianYa.append(tmp);

    }*/

    lgd = new LoginDialog;
    connect(lgd,SIGNAL(sendnursename(QString)),this,SLOT(setnursename(QString)));
    connect(lgd, SIGNAL(sendJSON(QString)),this,SLOT(setJSON(QString)));
    connect(lgd,SIGNAL(sendip(QString)),this,SLOT(setip(QString)));
    connect(lgd,SIGNAL(sendport(int)),this,SLOT(setport(int)));

    this->hide();
    //this->hostip = "10.25.35.67";
    //this->hostport = 2345;
    this->mytcp = new QTcpSocket(this);
    lgd->exec();
    this->show();

}

NurseViewWindow::~NurseViewWindow()
{
    delete ui;
}

void NurseViewWindow::setJSON(QString JSONArray)
{
    //qDebug() << JSONArray;
    //JSONArray = a;
    //parsejson(JSONArray);


    QScriptEngine engine;
    QScriptValue sc = engine.evaluate("value=" + JSONArray);
    int num = sc.property("num").toInt32();
    ui->tableWidget->setRowCount(num);
    //qDebug() << "num:" << sc.property("num").toString();
    patient = new Patient[num];
    for(int i = 1; i <= num; i++)
    {
        char *citoa = (char*)malloc(sizeof(char)*4);
        itoa(i,citoa,10);

        QString number(citoa);
        QString pid = "id" + number;
        QString name = "name" + number;
        QString gender = "sex" + number;
        QString age = "age" + number;
        QString time = "time" + number;

        patient[i-1].setpid(sc.property(pid).toString());
        patient[i-1].setname(sc.property(name).toString());
        patient[i-1].setage(sc.property(age).toString());
        patient[i-1].setgender(sc.property(gender).toString());
        patient[i-1].settime(sc.property(time).toString());
    }

    //QSignalMapper *signalMapper = new QSignalMapper(this);
    cbDianYa.clear();
    for(int i=0;i<num;i++){
        QComboBox *tmp=new QComboBox();
        tmp->addItem("");
        tmp->addItem("doctor1");
        tmp->addItem("doctor2");
        tmp->addItem("doctor3");
        connect(tmp,SIGNAL(currentIndexChanged(int)),signalMapper,SLOT(map()));
        signalMapper->setMapping(tmp,QString("%1-%2").arg(i).arg(5));
        cbDianYa.append(tmp);

    }
    //connect(signalMapper,SIGNAL(mapped(const QString &)),this,SLOT(changed(const QString &)));


    for(int i = 0;i < num; i++)
    {
        //patient[i].printpatient();
        //qDebug()<<i;
        ui->tableWidget->removeCellWidget(i,4);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(patient[i].getname()));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(patient[i].getgender()));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(patient[i].getage()));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(patient[i].gettime()));
        ui->tableWidget->setCellWidget(i,4,cbDianYa.at(i));
        //ui->tableWidget->removeCellWidget();

    }
    //delete(patient);
}

void NurseViewWindow::parsejson(QString a)
{

}


void NurseViewWindow::changed(QString position)
{
    char* CJSONArray = (char*)malloc(sizeof(char)*4096);
    memset(CJSONArray, 0, sizeof(char)*4096);
    QStringList coordinates = position.split("-");
    int row=coordinates[0].toInt();
    //char* chid=patient[row].id;
    //qDebug()<< patient[row].getpid() << " chosed " <<cbDianYa.at(row)->currentText();
    //patient[row].getpid();
    this->mytcp->abort();
    this->mytcp->connectToHost(hostip,hostport);
    //this->mytcp->connectToHost(hostip,hostport);
    if(this->mytcp->waitForConnected())
    {
        char *choosedoctor = (char*)malloc(sizeof(char)*256);
        memset(choosedoctor,0,sizeof(char)*256);

        char cDoctor[10] = {0};
        char cPid[10] = {0};
        char cNursename[10] = {0};
        strcat(cDoctor, cbDianYa.at(row)->currentText().toStdString().c_str());
        strcat(cPid, patient[row].getpid().toStdString().c_str());
        strcat(cNursename, nursename.toStdString().c_str());

        sprintf(choosedoctor,"{\"action\":\"3\",\"username\":\"%s\",\"id\":\"%s\",\"doctorname\":\"%s\"}"
                , cNursename, cPid, cDoctor);
        this->mytcp->write(choosedoctor,strlen(choosedoctor));

        if(this->mytcp->waitForReadyRead())
        {
            ui->tableWidget->setRowCount(0);
            this->mytcp->read(CJSONArray, 4096);
            QString QSJSONArray = CJSONArray;
            this->setJSON(QSJSONArray);
        }

        free(choosedoctor);
        free(CJSONArray);
    }

}


void NurseViewWindow::setip(QString newip)
{
    this->hostip = newip;
    //qDebug() << "in nurse window " <<newip;
}

void NurseViewWindow::setport(int newport)
{
    this->hostport = newport;
    //qDebug() << "in nurse window " <<newport;
}

void NurseViewWindow::on_pushbutton_clicked()
{
    char* CJSONArray = (char*)malloc(sizeof(char)*4096);
    memset(CJSONArray, 0, sizeof(char)*4096);
    //char* chid=patient[row].id;
    //qDebug()<< patient[row].getpid() << " chosed " <<cbDianYa.at(row)->currentText();
    //patient[row].getpid();
    this->mytcp->abort();
    this->mytcp->connectToHost(hostip,hostport);
    //this->mytcp->connectToHost(hostip,hostport);
    if(this->mytcp->waitForConnected())
    {
        char *choosedoctor = (char*)malloc(sizeof(char)*256);
        char cNursename[10] = {0};
        memset(choosedoctor,0,sizeof(char)*256);
        strcat(cNursename, nursename.toStdString().c_str());

        //char cDoctor[10] = {0};
        //char cPid[10] = {0};
        //strcat(cDoctor, cbDianYa.at(row)->currentText().toStdString().c_str());
        //strcat(cPid, patient[row].getpid().toStdString().c_str());
        sprintf(choosedoctor,"{\"action\":\"2\",\"username\":\"%s\"}",cNursename);

        this->mytcp->write(choosedoctor,strlen(choosedoctor));

        if(this->mytcp->waitForReadyRead())
        {
            ui->tableWidget->setRowCount(0);
            this->mytcp->read(CJSONArray, 4096);
            QString QSJSONArray = CJSONArray;
            this->setJSON(QSJSONArray);
        }

        free(choosedoctor);
        free(CJSONArray);
    }

}

void NurseViewWindow::setnursename(QString newname)
{
    this->nursename = newname;
    //qDebug() << newname;
}
