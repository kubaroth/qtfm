#pragma once

#include <QtGui>
#include <QDockWidget>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QPushButton>
#include <QApplication>
#include <QStatusBar>
#include <QMenu>
#include <QDBusConnection>
#include <QDBusError>
#include <QtConcurrent/QtConcurrent>
#include <QShortcut>
#include <fcntl.h>

#include "tabbar.h"

#include <QShortcut>
#include <QStandardItemModel>
#include <QSet>


#include <QVariant>
#include <QMetaType>
struct ProjectTab{;
    QString name;
    QList<QString> paths;
};
Q_DECLARE_METATYPE(ProjectTab);


class TabSets : public QStandardItemModel
{
  Q_OBJECT

public slots:
  void echo(const QModelIndex &index) {
    qDebug()<<"echo111 "<< index;
  }
  void echo2() { qDebug()<<"echo 2 ";}
  void updateCurrentTabSet(const QModelIndex &index);
  void renameTabSet();
  void addNew();
  void deleteTabSet();

public:
  TabSets(QObject * parent = 0, tabBar * _tabs=NULL, QLineEdit * _lineedit=NULL){
    tabs = _tabs;
    lineedit = _lineedit;
    configPath = QString("%1/.tabsets.json").arg(QDir::homePath());
  }

  void loadTabProjects();
  void saveTabProjects();

  QString currentTabProject = QString("AAA");
  QModelIndex projectIndex;
  QList<QVariant> tabsProjects;
  tabBar * tabs;               // alias
  QLineEdit * lineedit;        // alias
  QSet<QString> tapProjectNamesSet;
  QString configPath;
  int backupVer=0;
};
