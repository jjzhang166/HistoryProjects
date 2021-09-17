#pragma once
#pragma execution_character_set("utf-8")

#include <QObject>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

namespace Utility {

	extern void setLastError(const QString& error);

	extern const QString& getLastError();

	extern bool compareList(const QStringList& cmp1, const QStringList& cmp2);

	extern bool removeList(const QStringList& list1, QStringList& list2);

	extern QStringList getFileListBySuffixName(const QString& path, const QStringList& suffix);

	namespace File
	{
		/*
		* @setEncoding,���ñ���
		* @param1,[gb2312,utf-8...]
		* @return,void
		*/
		void setEncoding(const char* encoding);

		/*
		* @readFile,��ȡ�ļ�[����1]
		* @param1,�ļ���
		* @param2,���ݻ���
		* @param2,���ݴ�С
		* @return,bool
		*/
		bool read(const QString& name, char*& data, int& size);

		/*
		* @readFile,��ȡ�ļ�[����2]
		* @param1,�ļ���
		* @param2,�ֽ�����
		* @return,bool
		*/
		bool read(const QString& name, QByteArray& bytes);

		/*
		* @readFile,��ȡ�ļ�[����3]
		* @param1,�ļ���
		* @param2,�ı��б�
		* @return,bool
		*/
		bool read(const QString& name, QStringList& textList);

		/*
		* @readFile,��ȡ�ļ�[����4]
		* @param1,�ļ���
		* @param2,����
		* @return,bool
		*/
		bool read(const QString& name, QString& data);

		/*
		* @writeFile,д���ļ�[����1]
		* @param1,�ļ���
		* @param2,���ݻ���
		* @param3,���ݳ���
		* @return,bool
		*/
		bool write(const QString& name, const char* data, const int& size);

		/*
		* @writeFile,д���ļ�[����2]
		* @param1,�ļ���
		* @param2,�ֽ�����
		* @return,bool
		*/
		bool write(const QString& name, const QByteArray& bytes);

		/*
		* @writeFile,д���ļ�[����3]
		* @param1,�ļ���
		* @param2,�ı��б�
		* @return,bool
		*/
		bool write(const QString& name, const QStringList& textList);

		/*
		* @writeFile,д���ļ�[����3]
		* @param1,�ļ���
		* @param2,����
		* @return,bool
		*/
		bool write(const QString& name, const QString& data);

		/*
		* @appendFile,׷���ļ�[����1]
		* @param1,�ļ���
		* @param2,��������
		* @param3,���ݴ�С
		* @return,bool
		*/
		bool append(const QString& name, const char* data, const int& size);

		/*
		* @appendFile,׷���ļ�[����2]
		* @param1,�ļ���
		* @param2,�ֽ�����
		* @return,bool
		*/
		bool append(const QString& name, const QByteArray& bytes);

		/*
		* @appendFile,׷���ļ�[����3]
		* @param1,�ļ���
		* @param2,�ļ��б�
		* @return,bool
		*/
		bool append(const QString& name, const QStringList& textList);

		/*
		* @appendFile,׷���ļ�[����3]
		* @param1,�ļ���
		* @param2,����
		* @return,bool
		*/
		bool append(const QString& name, const QString& data);

		/*
		* @compareFile,�Ա��ļ�
		* @param1,�ļ�1
		* @param2,�ļ�2
		* @return,int;���ļ�ʧ��-1,��ͬ0,��ͬ1
		*/
		int compare(const QString& f1, const QString& f2);

		/*
		* @exist,����
		* @param1,�ļ���
		* @return,bool
		*/
		bool exist(const QString& name);

		/*
		* @getSize,��ȡ��С
		* @param1,�ļ���
		* @return,long long
		*/
		qint64 getSize(const QString& name);

		/*
		* @getCreateTime,��ȡ����ʱ��
		* @param1,�ļ���
		* @return,QString
		*/
		QString getCreateTime(const QString& name);

		/*
		* @getModifyTime,��ȡ�޸�ʱ��
		* @param1,�ļ���
		* @return,const QString
		*/
		QString getModifyTime(const QString& name);

		/*
		* @getAccessTime,��ȡ�޸�ʱ��
		* @param1,�ļ���
		* @return,const QString
		*/
		QString getAccessTime(const QString& name);

		/*
		* @getName,��ȡ����
		* @param1,·����
		* @return,QString
		*/
		QString getName(const QString& path);

		/*
		* @getBaseName,��ȡ��������
		* @param1,·����
		* @return,QString
		*/
		QString getBaseName(const QString& path);

		/*
		* @getSuffix,��ȡ��׺��
		* @param1,�ļ���
		* @return,QString
		*/
		QString getSuffix(const QString& file);

		/*
		* @getPath,��ȡ·��
		* @param1,�ļ���
		* @return,QString
		*/
		QString getPath(const QString& file);
		
		/*
		* @isFile,�ж��Ƿ�Ϊ�ļ�
		* @param1,�ļ���
		* @return,bool
		*/
		bool isFile(const QString& name);

		/*
		* @isDir,�ж��Ƿ�Ϊ�ļ���
		* @param1,�ļ�����
		* @return,bool
		*/
		bool isDir(const QString& name);

		/*
		* @remove,�Ƴ�
		* @param1,�ļ���
		* @return,bool
		*/
		bool remove(const QString& name);

		/*
		* @rename,������
		* @param1,���ļ�
		* @param2,���ļ�
		* @return,bool
		*/
		bool rename(const QString& oldName, const QString& newName);

		/*
		* @copy,����
		* @param1,Ҫ�������ļ�
		* @param2,���������ļ�
		* @param3,�ļ������Ƿ񿽱�
		* @return,bool
		*/
		bool copy(const QString& file1, const QString& file2, bool existCopy = true);

		/*
		* @move,�ƶ�
		* @param1,Դ�ļ�
		* @param2,Ŀ���ļ�
		* @return,bool
		*/
		bool move(const QString& file1, const QString& file2);

		/*
		* @setHidden,��������
		* @param1,�ļ���
		* @param2,�Ƿ�����
		* @return,bool
		*/
		bool setHidden(const QString& name, bool hidden = true);

		/*
		* @readJson,��json�ļ�
		* @param1,�ļ���
		* @param2,������
		* @return,bool
		*/
		bool readJson(const QString& name, QJsonObject& rootObj);

		/*
		* @writeJson,дjson�ļ�
		* @param1,�ļ���
		* @param2,������
		* @return,bool
		*/
		bool writeJson(const QString& name, const QJsonObject& rootObj);

		/*
		* @repairJson1LevelNode,�޸�1���ڵ�Json�ļ�
		* @param1,�ļ���
		* @param2,�ڵ��б�
		* @param3,���б�
		* @param4,ֵ�б�
		* @return,bool
		*/
		bool repairJson1LevelNode(
			const QString& fileName,
			const QStringList& nodeList,
			const QList<QStringList>& keyList,
			const QList<QStringList>& valueList
		);

		/*
		* @repairJson2LevelNode,�޸�2���ڵ�Json�ļ�
		* @notice,���ֻ���������޸���̬��2���ڵ�
		* @param1,�ļ���
		* @param2,�ڵ��б�
		* @param3,���б�
		* @param4,ֵ�б�
		* @return,bool
		*/
		bool repairJson2LevelNode
		(
			const QString& fileName,
			const QStringList& nodeList,
			const QList<QStringList>& keyList,
			const QList<QStringList>& valueList
		);

		/*
		* @repairJson2LevelNode,�޸�2���ڵ�Json�ļ�
		* @notice,���ֻ���������޸���̬��2���ڵ�
		* @param1,�ļ���
		* @param2,�ڵ��б�
		* @param3,�����б�
		* @param4,�Ӽ��б�
		* @param5,��ֵ�б�
		* @return,bool
		*/
		bool repairJson2LevelNode
		(
			const QString& fileName,
			const QStringList& nodeList,
			const QList<QStringList>& parentKeyList,
			const QList<QStringList>& childKeyList,
			const QList<QStringList>& childValueList
		);

		/*
		* @repairJson2LevelNode,�޸�2���ڵ�Json�ļ�
		* @notice,���ֻ���������޸���̬���Ӽ�����ֵ��һ�µ�2���ڵ�
		* @param1,�ļ���
		* @param2,�ڵ��б�
		* @param3,�����б�
		* @param4,�Ӽ��б�(����)
		* @param5,��ֵ�б�(����)
		* @return,bool
		*/
		bool repairJson2LevelNode
		(
			const QString& fileName,
			const QStringList& nodeList,
			const QList<QStringList>& parentKeyList,
			const QList<const QStringList*>& childKeyList,
			const QList<const QStringList*>& childValueList
		);
	};

}
