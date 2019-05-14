#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    editor = new CodeEditor(this);

    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(12);

    editor->setFont(font);
    this->setCentralWidget(editor);

    const int tabStop = 2;
    QFontMetrics metrics(font);
    editor->setTabStopWidth(tabStop * metrics.width(' '));

    highlighter = new Highlighter(editor->document());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    currentFile.clear();
//    ui->textEdit->setText(QString());
    this->editor->setPlainText(QString());
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open the file");
    QFile file(filename);
    currentFile = filename;
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }
    setWindowTitle(filename);
    QTextStream in(&file);
    QString text = in.readAll();
//    ui->textEdit->setText(text);
    this->editor->setPlainText(text);
    file.close();
}

void MainWindow::on_actionSave_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        return;
    }
    currentFile = filename;
    setWindowTitle(filename);
    QTextStream out(&file);
//    QString text = ui->textEdit->toPlainText();
    QString text = this->editor->toPlainText();
    out << text;
    file.close();
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Printer Name");
    QPrintDialog pDialog(&printer, this);
    if(pDialog.exec() == QDialog::Rejected){
        QMessageBox::warning(this, "Warning", "Cannot access printer");
        return;
    }
//    ui->textEdit->print(&printer);
    this->editor->print(&printer);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionCopy_triggered()
{
//    ui->textEdit->copy();
    this->editor->copy();
}

void MainWindow::on_actionPaste_triggered()
{
//    ui->textEdit->paste();
    this->editor->paste();
}

void MainWindow::on_actionCut_triggered()
{
//    ui->textEdit->cut();
    this->editor->cut();
}

void MainWindow::on_actionUndo_triggered()
{
//    ui->textEdit->undo();
    this->editor->undo();
}

void MainWindow::on_actionRedo_triggered()
{
//    ui->textEdit->redo();
    this->editor->redo();
}
