#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FPtree.h"
#include <QFileDialog>
#include <stdlib.h>
QString fileName;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    setFixedSize(this->width(), this->height());
    ui->mainToolBar->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open File"),".");
    ui->label_3->setText(fileName.split("/")[fileName.split("/").size()-1]);
}

void MainWindow::deleteAllSlot()
{
    printf("list length: %d\n", ui->listWidget->count());
    fflush(stdout);
    ui->listWidget->clear();
    ui->listWidget_2->clear();
}

void MainWindow::on_pushButton_clicked()
{

    string path = fileName.toStdString();
    double sup = atof(ui->lineEdit->text().toLatin1().data());
    double bel = atof(ui->lineEdit_2->text().toLatin1().data());
    FPtree * f = new FPtree(sup, bel);
    f->read(path.c_str());
    f->cal();


    /*
     * output: cout every frequent pattern
     */
    printf("Frequent pattern\n");
    total_cnt_sort.clear();
    total_cnt_sort_vector.clear();
    deleteAllSlot();
    int c = 0;
    now_ft = &f->ft;
    for(map<vector<string>, int>::iterator i = total_cnt.begin(); i != total_cnt.end(); i++){
        vector<string> _vector = i->first;
        int cnt = i->second;
        sort(_vector.begin(), _vector.end(), f->cmp);
        total_cnt_sort[_vector] = cnt;
        total_cnt_sort_vector.push_back(pair<vector<string>, int>(_vector, cnt));
    }
    sort(total_cnt_sort_vector.begin(), total_cnt_sort_vector.end(), cmp_map);
    for(int i = 0; i < total_cnt_sort.size(); i++){
        c++;
        QString text = "";
        for(int j = 0; j < total_cnt_sort_vector[i].first.size(); j++){
            text += (total_cnt_sort_vector[i].first)[j].c_str();
            text += " ";
        }
        text += QString::number(total_cnt_sort_vector[i].second, 10);
        QListWidgetItem* lst = new QListWidgetItem(text, ui->listWidget);
        ui->listWidget->insertItem(c, lst);
    }

    //Association analysis
    printf("Associate analysis\n");
    c = 0;
    for(map<vector<string>, int>::iterator i = total_cnt_sort.begin(); i != total_cnt_sort.end(); i++){
        vector<string> _vector = i->first;
        int cnt = _vector.size();
        for(int l = 1; l < (1 << cnt) - 1; l++){
            vector<string> v1;
            vector<string> v2;
            for(int k = 1; k <= cnt; k++){
                if((l >> (k - 1)) & 1) v1.push_back(_vector[k-1]);
                else v2.push_back(_vector[k-1]);
            }
            if(total_cnt_sort.find(v1) == total_cnt_sort.end()){
                printf("error\n");
                continue;
            }
            double _bel = (i->second) / double(total_cnt_sort[v1]);
            if(_bel < f->lim_b) continue;
            c++;
            c++;
            QString text = "";

            for(int j = 0; j < v1.size(); j++){
                text += QString((v1)[j].c_str()) + " ";
            }
            text += "-> ";
            for(int j = 0; j < v2.size(); j++){
                text += QString((v2)[j].c_str()) + " ";
            }
            text += QString::number(_bel, 'g', 4);
            QListWidgetItem* lst = new QListWidgetItem(text, ui->listWidget_2);
            ui->listWidget_2->insertItem(c, lst);
        }
    }


    delete f;

}
