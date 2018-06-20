#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

double llenar(vector<double>&vec,int n_pesos)
{
    int i=0;
    while(i<n_pesos){
        double r=1+rand()%(100001-1);

        vec.push_back(r/100000.0);
        i++;
    }
}

vector <vector<double> > create_lawyer(int n_neu,int n_pesos)
{
    vector <vector<double> > capa;
    for (int i=0;i<n_neu;i++)
    {
        vector<double> neuro;
        llenar(neuro,n_pesos);
        capa.push_back(neuro);
    }
    return capa;
}

//#############################################
double sigmoide( double s ){
  return (1.0/(1.0+exp(-1.0*s)));
}
vector<double> h_sigmoidea(vector<double> x)
{
    vector<double>rpta;
    for(int i=0;i<x.size();i++)
    {
        double eps=(1.0/(1.0+exp(-1.0*x[i])));
        rpta.push_back(eps);
    }

    return rpta;
}
long double error(vector<double> sd,vector<double> so)
{
    long double temp=0.0;
    for(int i=0;i<sd.size();i++)
    {
        temp=temp+pow((sd[i]-so[i]),2);
    }
    return temp/2.0;
}

double produc_p(vector<double>v1, vector<double> v2)//producto punto
{
    double resul=0.0;
    for(int i=0;i<v1.size();i++)
    {
        resul=resul+v1[i]*v2[i];
    }
    return resul;
}
vector<double> produc_final(vector< vector<double> >v1, vector<double> v2)// producto de un vector con una matriz
{
    vector<double> resul;
    for(int i=0;i<v1.size();i++)
    {
        resul.push_back(produc_p( v1[i],v2));
    }
    return resul;

}

void imprimir(vector <vector<double> > vec)
{
    for (int i=0;i<vec.size();i++)
    {
        for (int j=0;j<vec[i].size();j++)
    {
        cout<<vec[i][j]<<"//";

    }
    cout<<'\n';
    }
}
void imprimir_vec(vector<double>v)
{
    for (int i=0;i<v.size();i++)
    {
        cout<<v[i]<<"//";
    }
    cout<<endl;
}

void mostrar_dot(vector<double>sh,vector<double>so, vector <vector<double> > w1, vector <vector<double> > w2)//genera el dot
{

    string entrada1 ="digraph G {\n rankdir=LR \n splines=line \n nodesep=.05;\n subgraph cluster_0 { \n color=white;\n node [style=solid,color=blue4, shape=circle,label=\"\"];\n 1 2 3 4; \n label = \"SInput\";} \n subgraph cluster_1 { \n color=white; \n node [style=solid,color=red2, shape=circle,label=\"\"]; \n 12 22 32 42 52 62 72 82;\n label = \"SHidden\";}\n subgraph cluster_2 { \n color=white;\n node [style=solid,color=seagreen2, shape=circle,label=\"\"]; \n 13 23 33; \n label = \"Soutput\";} ";



    //aqui
    ofstream fi("file.txt",ios::trunc);
    //primer for entrada
    fi<<entrada1;
    int num1=1;
    int des1=12;
    int num=12;
    int des=13;

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<sh.size();j++)
        {
            //fi<<num1<<"->"<<des1<<"[label="<<w1[j][i] <<"]"<<endl;
            fi<<num1<<"->"<<des1<<endl;
            des1=des1+10;
        }
        des1=12;
        num1=num1+1;
    }


    for(int i=0;i<so.size();i++)
    {

        for(int j=0;j<sh.size();j++)
        {
            fi<<num<<"->"<<des<<endl;
            num=num+10;

        }
        num=12;
        des=des+10;
    }
    int num2=13;
    //fi<<"node [shape=box, fontsize=16];"<<endl;
    for(int i=0;i<3;i++)
    {
        fi<<so[i]<<"[shape=\"box\"];"<<endl;
        fi<<num2<<"->"<<so[i]<<endl;
        num2=num2+10;
    }
    fi<<"}"<<endl;

    system("dot -Tpng -O file.txt");


}

double backpropagation(vector <vector<double> > &w1,vector <vector<double> >&w2,vector<double>&sh,vector<double>&so,vector<double>&sd,vector<double>&x)
{

    double err=error(sd,so);
    int f_w1=w1.size();
    int c_w1=w1[0].size();
    int f_w2=w2.size();
    int c_w2=w2[0].size();
    cout<<f_w1<<"-"<<c_w1<<endl;
    cout<<f_w2<<"-"<<c_w2<<endl;
    cout<<"entro bp"<<endl;
    cout<<err<<endl;
    while (err>0.001) {
        vector<double>sum_delta;
        for(int i=0;i<c_w2;i++)
        {
            vector<double>deltas;
            double sumatoria_del=0;//anadido
            for(int j=0;j<f_w2;j++)
            {
                double delta = (((-1)*(sd[j]-so[j]))*((1-so[j])*so[j]));
                deltas.push_back(w2[j][i]*delta);
                w2[j][i] = w2[j][i] - (0.01 * delta *sh[i]);
                sumatoria_del=sumatoria_del+(w2[j][i]*delta);


            }
            sum_delta.push_back(sumatoria_del);
        }
        for(int m=0;m<c_w1;m++)
        {
            for(int n=0;n<f_w1;n++)
            {
                w1[n][m] = w1[n][m] - (0.01 * ((sum_delta[n+1]) * ((1-sh[n])*sh[n]) )*x[m]);
            }
        }
        vector<double>sh1;
        sh1=h_sigmoidea(produc_final(w1,x));
        sh.clear();
        sh.push_back(1.0);
        for(int i=0;i<sh1.size();i++)
        {
            sh.push_back(sh1[i]);
        }
        so=h_sigmoidea(produc_final(w2,sh));
        err=error(sd,so);


    }
    cout<<"termino BP"<<endl;
    cout<<err<<endl;
    return err;

}

vector<double> normalizar_vec(vector<double >vec)
{
    vector<double>respuesta;
    double min=*std::min_element(vec.begin(),vec.end());
    double max=*std::max_element(vec.begin(),vec.end());
    for(int i=0;i<vec.size();i++)
    {
        respuesta.push_back((vec[i]-min)/(max-min));
    }
    return respuesta;
}
void MainWindow::on_pushButton_clicked()
{
    vector <vector<double> > w1;
    vector <vector<double> > w2;
    vector <double> sh;
    vector <double> so;
    //double mysd[]={0.9 ,0.2, 0.3};#asi era
    double mysd[3];
    mysd[0]=ui->sd1->text().toDouble();
    mysd[1]=ui->sd2->text().toDouble();
    mysd[2]=ui->sd3->text().toDouble();



    double myints[] = {1.0, 1.0, 0.6734693877551021, 0.2448979591836735, 0.0};
    vector<double> entrada (myints, myints + sizeof(myints) / sizeof(int) );
    vector<double> sd (mysd, mysd + sizeof(mysd) / sizeof(int) );


    w1=create_lawyer(8,5);
    sh=h_sigmoidea(produc_final(w1,entrada));
    vector<double>sh1;
    sh1.push_back(1.0);
    for(int i=0;i<sh.size();i++)
    {
        sh1.push_back(sh[i]);
    }
    imprimir_vec(sh1);
    w2=create_lawyer(3,sh1.size());
    so=h_sigmoidea(produc_final(w2,sh1));
    imprimir_vec(so);

    double err1=error(sd,so);
    //cout<<"entro"<<endl;
    double err2= backpropagation(w1,w2,sh1,so,sd,entrada);
    imprimir_vec(so);
    ui->so1->setText(QString::number(so[0]));
    ui->so2->setText(QString::number(so[1]));
    ui->so3->setText(QString::number(so[2]));


    //imprimir_vec(so);

    //imprimir(w2);
    //system("dot -Tpng -O file2.txt");
    mostrar_dot(sh,so,w1,w2);
    QImage F;
    F.load("file.txt.png");
    ui->img_1->setPixmap(QPixmap::fromImage(F));
    //system("dot -Tpng -O file.txt");

    ui->error_ini->setText(QString::number(err1));
    ui->error_final->setText(QString::number(err2));
}
//------------------------------------ FUNCIONES DEL SEGUNDO TAB

//60% entrenar ->90 datos
//40% testing ->60
void entrenar(vector< vector<double> >&entrenar,vector< vector<double> >&testing)
{
    ifstream documento ("iris.txt");
    char num[100];
    vector< vector<double> >result;

    vector<double> vec;
    int i=1;
    while(!documento.eof())
    {
        documento.getline(num,100,',');
        //cout<<num<<endl;
        if(i==4)
        {
            double numero;
            stringstream(num)>>numero;
            //cout<<numero<<endl;
            vec.push_back(numero);


            documento.getline(num,100);
            string temp=num;
          //  cout<<num<<endl;
            i=0;
            if(temp=="Iris-setosa")
            {
                vec.push_back(0);
                //imprimir_vec(vec);
                result.push_back(vec);
                vec.clear();
            }
            if(temp=="Iris-versicolor")
            {
                vec.push_back(1);
                //imprimir_vec(vec);
                result.push_back(vec);
                vec.clear();

            }
            if(temp=="Iris-virginica")
            {
                vec.push_back(2);
                //imprimir_vec(vec);
                result.push_back(vec);
                vec.clear();

            }
        }
        else{
            double numero;
            stringstream(num)>>numero;
            //cout<<numero<<endl;
            vec.push_back(numero);
        }
        i++;


    }
    //cout<<num<<endl;
    int j=0;
    //imprimir(result);
    while(j<30)
    {
        entrenar.push_back(result[j]);
        entrenar.push_back(result[j+50]);
        entrenar.push_back(result[j+100]);
        j++;
    }

    //------------------------lenamos el test
    j=0;
    while(j<20)
    {
        testing.push_back(result[j+30]);
        //entrenar.push_back(result[j+50]);
        //entrenar.push_back(result[j+100]);
        j++;
    }
    j=0;
    while(j<20)
    {
        //entrenar.push_back(result[j]);
        testing.push_back(result[j+80]);
        //entrenar.push_back(result[j+100]);
        j++;
    }
    j=0;
    while(j<20)
    {
        //entrenar.push_back(result[j]);
        //entrenar.push_back(result[j+50]);
        testing.push_back(result[j+130]);
        j++;
    }
    //imprimir(testing);
    //imprimir(entrenar);
}


double backpropagation_solo(vector <vector<double> > &w1,vector <vector<double> >&w2,vector<double>sh,vector<double>so,vector<double>sd,vector<double>x)
{

    long double err=error(sd,so);
    int f_w1=w1.size();
    int c_w1=w1[0].size();
    int f_w2=w2.size();
    int c_w2=w2[0].size();
    vector<double>sum_delta;
    for(int i=0;i<c_w2;i++)
    {
        vector<double>deltas;
        double sumatoria_del=0;//anadido
        for(int j=0;j<f_w2;j++)
        {
            double delta = (((-1)*(sd[j]-so[j]))*((1-so[j])*so[j]));
            deltas.push_back(w2[j][i]*delta);
            w2[j][i] = w2[j][i] - (0.5 * delta *sh[i]);
            sumatoria_del=sumatoria_del+(w2[j][i]*delta);

        }
        sum_delta.push_back(sumatoria_del);
    }
    for(int m=0;m<c_w1;m++)
    {
        for(int n=0;n<f_w1;n++)
        {
            w1[n][m] = w1[n][m] - (0.5 * ((sum_delta[n+1]) * ((1-sh[n])*sh[n]) )*x[m]);
        }
    }
    vector<double>sh1;
    sh1=h_sigmoidea(produc_final(w1,x));
    sh.clear();
    sh.push_back(1.0);
    for(int i=0;i<sh1.size();i++)
    {
        sh.push_back(sh1[i]);
    }
    so=h_sigmoidea(produc_final(w2,sh));
    //imprimir_vec(sd);
    err=error(sd,so);
    return err;

}

void train_iris_solo(vector<vector<double> > entradas,vector <vector<double> > &w1,vector <vector<double> >&w2,vector<double>&errores)// ENTRENA
{
    vector <double> sh;
    vector <double> so;
    w1=create_lawyer(8,5);
    w2=create_lawyer(3,9);
    double umbral = 0.01;//0.0067
    double sum_error = 10;
    int y=0;
    while(sum_error>umbral)
    {
        vector<long double>vec_error;
        long double sum=0;
        for(int i=0;i<entradas.size();i++)
        {
            vector <double> sd;
            int len=entradas[i].size();
            int clase=entradas[i][len-1];
            entradas[i].pop_back();
            if(clase==0)
            {
                sd.push_back(1);
                sd.push_back(1);
                sd.push_back(1);
            }
            if(clase==1)
            {
                sd.push_back(0.500);
                sd.push_back(0.500);
                sd.push_back(0.500);
            }
            if(clase==2){
                sd.push_back(0);
                sd.push_back(0);
                sd.push_back(0);
            }

            vector<double>vec1=normalizar_vec(entradas[i]);
            vector<double> vacio;
            entradas[i]=vacio;
            if (y==0)
                    entradas[i].push_back(1.0);
            for(int j=0;j<vec1.size();j++)
            {
                entradas[i].push_back(vec1[j]);
            }
            //imprimir_vec(entradas[i]);
            vector<double>sh1;
            sh1=h_sigmoidea(produc_final(w1,entradas[i]));
            sh.clear();
            sh.push_back(1.0);
            for(int i=0;i<sh1.size();i++)
            {
                sh.push_back(sh1[i]);
            }

            so=h_sigmoidea(produc_final(w2,sh));
            long double error;

            error=backpropagation_solo(w1,w2,sh,so,sd,entradas[i]);
            vec_error.push_back(error);
            //cout<<error<<endl;

            sum=sum+error;
            entradas[i].push_back(clase);
            //imprimir_vec(entradas[i]);

            //cout<<i<<endl;

        }
        y++;
        //cout<<sum<<endl;
        sum_error=sum/vec_error.size();
        cout<<sum_error<<endl;
        errores.push_back(sum_error);
        cout<<y<<endl;
     }

}
vector<double> test(vector<double> entrada,vector <vector<double> > w1,vector <vector<double> >w2)
{
    vector<double>vec1=normalizar_vec(entrada);
    entrada.clear();
    entrada.push_back(1.0);
    for(int j=0;j<vec1.size();j++)
    {
        entrada.push_back(vec1[j]);
    }
    vector<double> sh1;
    vector<double> so1;
    sh1=h_sigmoidea(produc_final(w1,entrada));
    vector<double>sh2;
    sh2.push_back(1.0);
    for(int i=0;i<sh1.size();i++)
    {
        sh2.push_back(sh1[i]);
    }
    so1=h_sigmoidea(produc_final(w2,sh2));
    //imprimir_vec(so1);
    return so1;
}
void llenar_archivo(vector <vector<double> > w1,vector <vector<double> >w2,vector<double>errores)
{
    ofstream fs("w1.txt");
    for(int i=0;i<w1.size();i++)
    {
        for(int j=0;j<w1[0].size();j++)
        {
            ostringstream ss;
            ss << w1[i][j];
            string out = ss.str();
            fs<<out<<";";
        }
        fs<<"\n";
    }
    fs.close();
    ofstream fs1("w2.txt");
    for(int i=0;i<w2.size();i++)
    {
        for(int j=0;j<w2[0].size();j++)
        {
            ostringstream ss;
            ss << w2[i][j];
            string out = ss.str();
            fs1<<out<<";";
        }
        fs1<<"\n";
    }
    fs1.close();

    ofstream fs2("errores.txt");
    for(int i=0;i<errores.size();i++)
    {

            ostringstream ss;
            ss << errores[i];
            string out = ss.str();
            fs2<<out;
            fs2<<"\n";
    }
    fs2.close();



}
void cargar(vector <vector<double> >&w1,vector <vector<double> >&w2)
{

    ifstream documento ("w1.txt");
    char num[100];
    int i=1;
    vector<double>vec;
    vector<double>vec1;

    while(!documento.eof())
    {
        documento.getline(num,100,';');
        double numero;
        stringstream(num)>>numero;
        //cout<<numero<<endl;
        vec.push_back(numero);
        if(i==5)
        {
            w1.push_back(vec);
            vec.clear();
            i=0;
        }
        i++;
    }
    //imprimir(w1);
    cout<<"segundo peso"<<endl;
    documento.close();
    //otro vector
    ifstream documento1 ("w2.txt");
    char num1[100];
    int j=1;
    while(!documento1.eof())
    {
        documento1.getline(num1,100,';');
        double numero;
        stringstream(num1)>>numero;
        vec1.push_back(numero);
        if(j==9)
        {
            w2.push_back(vec1);
            vec1.clear();
            j=0;
        }
        j++;
    }
    documento1.close();
    //imprimir(w2);



}
int return_clase(vector<double>v)
{
    double sum=0;
    for(int i=0;i<v.size();i++)
    {
        sum=sum+v[i];
    }
    double  prom=sum/(v.size());
    cout<<prom<<endl;
    if(prom>=0 && prom <=0.33333)
    {
        return 2;
    }
    if(prom>0.33333 && prom <=0.66666)
    {
        return 1;
    }
    if(prom>0.66666 && prom <=1)
    {
        return 0;
    }

}
void MainWindow::on_cargar_pesos_clicked()
{
    vector< vector<double> >d_entradas;
    vector< vector<double> >d_pruebas;
    vector< vector<double> > w2;
    vector< vector<double> > w1;
    vector<double>errores;
    entrenar(d_entradas,d_pruebas);

    //train_iris_solo(d_entradas,w1,w2,errores);//
    //llenar_archivo(w1,w2,errores);// estas funciones train iris_solo llena la matriz de pesos y luego llenamos los archivos


    //vector<double> v({ 5.7,3.0,4.2,1.2});
    //test(v,w1,w2);
    cargar(w1,w2);
    int clas0=0;
    int clas1=0;
    int clas2=0;
    vector< vector<double> > M_confusion;
    imprimir(d_pruebas);
    for (int i=0;i<d_pruebas.size();i++)
    {
        vector<double> v;
        v=test(d_pruebas[i],w1,w2);
        int clase=return_clase(v);
        //cout<<clase<<endl;
        if(clase==0){
            clas0++;
        }
        if(clase==1){
            clas1++;
        }
        if(clase==2)
        {
            clas2++;
        }
        if(i==19)
        {
            vector<double> v1 ({clas0,clas1,clas2});
            M_confusion.push_back(v1);
            clas0=0;
            clas1=0;
            clas2=0;
        }
        if(i==39)
        {
            vector<double> v1 ({clas0,clas1,clas2});
            M_confusion.push_back(v1);
            clas0=0;
            clas1=0;
            clas2=0;
        }



    }
    vector<double> v1 ({clas0,clas1,clas2});
    M_confusion.push_back(v1);
    imprimir(M_confusion);

    //Llenamos la matriz de confusion
    ui->matriz_confusion->setColumnCount(3);
    ui->matriz_confusion->setRowCount(3);

    QStringList cabecera;
    QStringList Ladera;
    QString x1="Setosa";
    QString x2="Versicolor";
    QString x3="Virginica";
    QString s1="Setosa";
    QString s2="Versicolor";
    QString s3="Virginica";

    Ladera<<s1<<s2<<s3;
    cabecera<<x1<<x2<<x3;
    ui->matriz_confusion->setHorizontalHeaderLabels(cabecera);
    ui->matriz_confusion->setVerticalHeaderLabels(Ladera);
    ui->matriz_confusion->resizeColumnsToContents();
    ui->matriz_confusion->resizeRowsToContents();
    double efectividad;
    for(int i=0;i<M_confusion.size();i++)
    {
        for(int j=0;j<M_confusion.size();j++)
        {
            QString texto = QString::number(M_confusion[i][j]);
            ui->matriz_confusion->setItem(i, j, new QTableWidgetItem(texto));
            if(i==j)
            {
                efectividad=efectividad+M_confusion[i][j];
            }
        }
    }
    efectividad=(efectividad/60)*100;
    QString efec=QString::number(efectividad)+'%';
    ui->efectivdad->setText(efec);

}
vector<double> cargar_errores()
{
    ifstream documento ("errores.txt");
    char num[100];
    vector<double>vec;
    while(!documento.eof())
    {
        documento.getline(num,100);
        double numero;
        stringstream(num)>>numero;
        //cout<<numero<<endl;
        vec.push_back(numero);

    }
    //imprimir;
    return vec;
}
void MainWindow::on_graficar_error_clicked()
{
    vector<double>errores=cargar_errores();
    ui->customPlot->legend->setVisible(true);
    QVector<double> x(1000), y(1000);
    for (int i=0; i<1000; ++i)
    {
      x[i] = i/100;
      y[i] = errores[i]*10;
    }

    ui->customPlot->addGraph();
    ui->customPlot->graph()->setData(x, y);
    ui->customPlot->graph()->setName("Error");

    ui->customPlot->graph()->setPen(QPen(Qt::red));
    ui->customPlot->xAxis->setLabel("Tiempo");
    ui->customPlot->yAxis->setLabel("Error");
    ui->customPlot->xAxis->setRange(0,10);
    ui->customPlot->yAxis->setRange(0,6 );
    ui->customPlot->replot();
}
