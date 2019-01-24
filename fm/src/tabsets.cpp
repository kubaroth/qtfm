#include "tabsets.h"


void TabSets::loadTabProjects(){
  /*Load json and build menu
  */
  // FIXME: check if file exists
  QFile loadFile(configPath);
  if (!loadFile.exists()) {
    QFile saveFile(configPath);
    saveFile.open(QIODevice::WriteOnly);

    QJsonObject settings;
    settings["recent"] = QString();
    QJsonArray projects;  // empty anyway
    settings["projects"] = projects;
    QJsonDocument saveDoc(settings);
    saveFile.write(saveDoc.toJson());
    return;
  }
  loadFile.open(QIODevice::ReadOnly);

  QJsonParseError jsonError;
  QJsonDocument jsondata = QJsonDocument::fromJson(loadFile.readAll(),&jsonError);
  if (jsonError.error != QJsonParseError::NoError){
      qDebug() << jsonError.errorString();
  }

  QMap<QString, QVariant> map = jsondata.toVariant().toMap();

  // currentTabProject = map["recent"].toString();
  /// Generate list of teb-sets
  // qDebug() << currentTabProject;


  tabsProjects = map["projects"].toList();

  for(auto it=tabsProjects.begin(); it!=tabsProjects.end(); ++it){
      // qDebug() << it->toMap();
      QString name = it->toMap()["name"].toString();

      if (!tapProjectNamesSet.contains(name)){
        tapProjectNamesSet.insert(name);
        auto item = new QStandardItem(name);
        // FIXME:do we need to store this?
        // item->setData(it->toMap()["tabs"].toList());
        this->appendRow(item);
      }
  }
}

void TabSets::saveTabProjects(){

    QJsonArray tabsJsonArray;

    // //FIXME: check if the currentTabProject is in the tabsProjects (New/Rename)
    // for(auto it=tabsProjects.begin(); it!=tabsProjects.end(); ++it){
    // }


    auto bakpath = QString("%1/tabsetsBAK_%2.json").arg(QDir::tempPath(), QString::number(backupVer % 10));
    QFile::remove(bakpath);
    QFile::copy(configPath, bakpath);
    backupVer += 1;

    for(auto it=tabsProjects.begin(); it!=tabsProjects.end(); ++it){
        // qDebug() << it->toMap();
        QJsonObject tabProject;
        if (it->toMap()["name"].toString() == currentTabProject){
              QJsonArray tabpaths;
              // QList<QString> tabpathsQvariant;
              for (auto i=0; i<tabs->count(); ++i){
                QString path = tabs->tabData(i).toString();
                tabpaths.append(path);
                // tabpathsQvariant.append(path);
              }

              tabProject["name"] = currentTabProject;
              tabProject["tabs"] = tabpaths;

              // tabsProjects["tabs"] = tabpathsQvariant;
        }
        else{
            tabProject["name"] = it->toMap()["name"].toString();
            QList<QVariant> ll = it->toMap()["tabs"].toList();
            QJsonArray tabpaths;
            for (auto path : ll){
               tabpaths.append(path.toString());
            }
            tabProject["tabs"] =  tabpaths;
        }
      tabsJsonArray.append(tabProject);
    }

    QFile saveFile(configPath);
    saveFile.open(QIODevice::WriteOnly);

    QJsonObject settings;
    settings["recent"] = currentTabProject;
    settings["projects"] = tabsJsonArray;
    QJsonDocument saveDoc(settings);
    saveFile.write(saveDoc.toJson());
}

void TabSets::updateCurrentTabSet(const QModelIndex &index){
    /*
    /// save the current selection in the json file
    get the neame of the selection from the view and update currentTabProject
    //lookup in the map
    //create tabs
    */

    projectIndex = index;

    if (currentTabProject==""){
        qDebug() << "currentTabProject is Empty";
        return;
    }
    // SAVE state of the project we are moving out from
    saveTabProjects();
    // LOAD:
    loadTabProjects();

    /// Load tabs
    auto item = this->itemFromIndex(index);
    // qDebug() << item->text();
    QString name = item->text();

    QList<QString> this_tabs;
    for (auto x : tabsProjects) {
       if (x.toMap()["name"] == name){
            for (auto path : x.toMap()["tabs"].toList()){
              this_tabs.append(path.toString());
            }
         break;
       }
    }

    // remove existing tabs
    while (tabs->count() != 0)
        tabs->closeTab();

    // qDebug() << tabs->count();
    for (auto path : this_tabs){
        tabs->addNewTab(path,2);
    }

    currentTabProject = name;
}

void TabSets::renameTabSet(){

  auto item = this->itemFromIndex(projectIndex);
  // qDebug() << item->text() << " " << projectIndex.row();
  auto entry = tabsProjects[projectIndex.row()].toMap();
  // qDebug() << tabsProjects[projectIndex.row()];
  QString oldName = entry["name"].toString();
  QString newName = item->text();
  // qDebug()<< entry["name"]<< "----" << item->text();
  entry["name"] = item->text();
  QVariant v(entry);
  tabsProjects[projectIndex.row()] = v;

  /// Reset the set of names to update
  tapProjectNamesSet.remove(oldName);
  tapProjectNamesSet.insert(newName);

  saveTabProjects();

}

void TabSets::addNew(){
    QString newName = lineedit->text();
    if ((newName=="") || tapProjectNamesSet.contains(newName))
      return;
    ProjectTab s;
    QList<QString> paths;
    QVariant v;
    v.setValue(s);
      QJsonObject jo = v.toJsonObject();
      jo["name"] = newName;

  tabsProjects.append(jo);
  tapProjectNamesSet.insert(newName);
   auto item = new QStandardItem(newName);
   this->appendRow(item);

   // Now we need to signal view to
    emit itemChanged(item);
}

void TabSets::deleteTabSet(){
  auto item = this->itemFromIndex(projectIndex);
  // qDebug() << item->text() << " " << projectIndex.row();
  auto entry = tabsProjects[projectIndex.row()].toMap();
  // qDebug() << tabsProjects[projectIndex.row()];
  QString oldName = entry["name"].toString();

  /// Reset the set of names to update
  tapProjectNamesSet.remove(oldName);

  /// Now update tabsProjects
  tabsProjects.removeAt(projectIndex.row());
  // remove the row
  removeRow(projectIndex.row());

  saveTabProjects();

}
