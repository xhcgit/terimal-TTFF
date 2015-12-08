#include "FrmConsole.h"
#include "mytool.h"

#include <QDateTime>
#include <QTextStream>
#include <QDir>
#include <QProcess>

/**
 * @brief BinaryConsole::BinaryConsole 自定义binary console显示控件
 * @param parent
 */
FrmConsole::FrmConsole(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrmConsole)
{
    ui->setupUi(this);
    isLocked = false;
}

FrmConsole::~FrmConsole()
{
    delete ui;
}

/**
 * @brief BinaryConsole::clearMsg 清空显示区域
 */
void FrmConsole::clearMsg()
{
    ui->pTextMsg->clear();
}

/**
 * @brief BinaryConsole::lock 锁定显示区域，停止接收
 */
void FrmConsole::setLock()
{
    isLocked = true;
}

void FrmConsole::switchLock()
{
    isLocked = !isLocked;
    if(isLocked)
    {
        ui->btnLock->setText("解锁");
    }else
    {
        ui->btnLock->setText("锁定");
    }

}

/**
 * @brief BinaryConsole::lock 解锁显示区域，继续接收
 */
void FrmConsole::setUnlock()
{
    isLocked = false;
}

/**
 * @brief BinaryConsole::save 打开保存对话框，保存接收到的信息
 */
void FrmConsole::save()
{
    QString tempData=ui->pTextMsg->toPlainText();

    if (tempData.isEmpty())
    {
        return; //如果没有内容则不保存
    }
    //获取程序路径
    QString path = QDir::currentPath();   //获取程序当前目录
    path.replace("/","\\"); //将地址中的"/"替换为"\"，因为在Windows下使用的是"\"
    //获取当前时间作为文件名
    QDateTime now=QDateTime::currentDateTime();
    QString name=now.toString("yyyyMMddHHmmss");
    //拼接文件全路径，存放在log目录中
    static QString fileName=path +"\\" + LOG_FILE_SAVE_DIR + "\\" + name + ".txt";

    QFile file(fileName);
    file.open(QFile::Append | QIODevice::Text);
    QTextStream out(&file);
    out<<tempData;
    file.close();
}

/**
 * @brief BinaryConsole::browserSavedFile 浏览保存文件所在文件夹
 */
void FrmConsole::browserSavedFile()
{
    QString path = QDir::currentPath();   //获取程序当前目录
    path.replace("/","\\"); //将地址中的"/"替换为"\"，因为在Windows下使用的是"\"。
    QProcess::startDetached("explorer "+path+"\\" + LOG_FILE_SAVE_DIR);  //打开上面获取的目录
}

/**
 * @brief BinaryConsole::append 向当前显示区内追加显示内容
 * @param text 待追加显示文本
 */
void FrmConsole::append(const QString &text)
{
    static int count = 0;
    if(!isLocked)
    {
        ui->pTextMsg->insertPlainText(text);
        ui->pTextMsg->insertPlainText(" ");
        ui->pTextMsg->moveCursor(QTextCursor::End);

        count ++;
    }

    if(count == 300)
    {
        clearMsg();
        count = 0;
    }
}

void FrmConsole::append(const QByteArray &ba)
{
    append(MyTool::ByteArrayToHexStr(ba));
}


void FrmConsole::setTitle(const QString &title)
{
    ui->frame->setWindowTitle(title);
}


/**
 * @brief BinaryConsole::on_btnLock_clicked 锁定窗口
 */
void FrmConsole::on_btnLock_clicked()
{
    switchLock();
}
/**
 * @brief BinaryConsole::on_btnClear_clicked 清除窗口
 */
void FrmConsole::on_btnClear_clicked()
{
    clearMsg();
}
/**
 * @brief BinaryConsole::on_btnSave_clicked 保存窗口
 */
void FrmConsole::on_btnSave_clicked()
{
    save();
}
/**
 * @brief BinaryConsole::on_btnBrowser_clicked 浏览文件所在文件夹
 */
void FrmConsole::on_btnBrowser_clicked()
{
    browserSavedFile();
}

/**
 * @brief FrmConsole::appendAsString 以ASCII码追加显示
 * @param ba Hex数组
 * @param code 校验结果
 */
void FrmConsole::appendAsString(QByteArray ba, bool success)
{
    if(!isLocked)
    {
        ui->pTextMsg->appendPlainText(QString(ba));

        if(!success)
        {
            ui->pTextMsg->appendPlainText("校验和错误");
        }
    }
}

/**
 * @brief FrmConsole::appendAsHex 以Hex格式追加显示到文本框中
 * @param ba Hex数组
 * @param code 校验结果
 */
void FrmConsole::appendAsHex(QByteArray ba, bool success)
{
    if(!isLocked)
    {
        QString str = MyTool::ByteArrayToHexStr(ba);
        ui->pTextMsg->appendPlainText(str);

        if(!success)
        {
            ui->pTextMsg->appendPlainText("校验和错误");
        }
    }
}
