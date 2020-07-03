#include <QtWidgets/QApplication>
#include "MainDlg.h"
#pragma comment(lib,"version.lib")

/************************************************************************/
/* ������:INVO190176                                                    */
/* ��������:C++															*/	
/* �������:QT                                                          */
/* ��ϵ��ʽ:hao.wang@invo.cn											*/
/************************************************************************/

/*��ȡӦ�ó���汾��Ϣ*/
static bool getFileVersion(const char* fullName, QString& version)
{
	bool result = false;
	char* nameBuffer = nullptr;
	do
	{
		
		quint32 nameLen = GetFileVersionInfoSizeA(fullName, 0);
		if (!nameLen)
		{
			break;
		}

		nameBuffer = new(std::nothrow) char[nameLen + 1];
		if (!nameBuffer)
		{
			break;
		}

		bool success = GetFileVersionInfoA(fullName, 0, nameLen, nameBuffer);
		if (!success)
		{
			break;
		}

		struct LanguageCodePage
		{
			WORD language;
			WORD codePage;
		} *translate;

		quint32 queryLen = 0;
		success = VerQueryValue(nameBuffer, (TEXT("\\VarFileInfo\\Translation")), (LPVOID*)&translate, &queryLen);
		if (!success)
		{
			break;
		}
		QString str1, str2;
		str1.setNum(translate->language, 16);
		str2.setNum(translate->codePage, 16);
		str1 = "000" + str1;
		str2 = "000" + str2;
		QString verPath = "\\StringFileInfo\\" + str1.right(4) + str2.right(4) + "\\FileVersion";
		void* queryBuffer = nullptr;
		success = VerQueryValue(nameBuffer, (verPath.toStdWString().c_str()), &queryBuffer, &queryLen);
		if (!success)
		{
			break;
		}
		version = QString::fromUtf16((const unsigned short int*)queryBuffer);
		result = true;
	} while (false);
	SAFE_DELETE_A(nameBuffer);
	return result;
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QString version = "0.0.0";
	if (getFileVersion(argv[0], version))
	{
		QDir dir;
		QString fullName = QString::fromLocal8Bit(argv[0]);
		QString oldName = fullName.mid(fullName.lastIndexOf("\\") + 1);
		QString newName = QString("��¼����[%1].exe").arg(version);
		if (oldName != newName)
		{
			dir.rename(oldName, newName);
		}
	}

	MainDlg* mainDlg = NO_THROW_NEW MainDlg;
	if (!mainDlg)
	{
		return -1;
	}
	mainDlg->setWindowTitle(QString("��¼����[%1]").arg(version));
	mainDlg->show();
	return app.exec();
}
