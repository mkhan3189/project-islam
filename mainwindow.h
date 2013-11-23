#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "core/style_loader.h"
#include "core/settings_loader.h"

#include "core/data/data_holder.h"

namespace Ui {
class MainWindow;
}

class Extension;
class ExtensionBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void initialize();
    void reloadStyles();
    void loadSettings();
    SettingsLoader *settingsLoader();
    StyleLoader *styleLoader();
    
    data::DataHolder* dataHolder();
protected:
    void resizeEvent(QResizeEvent *);
private slots:
    void on_actionAbout_Qt_triggered();
    
    void on_actionExit_triggered();
    void onExtensionChanged(Extension* extension);
    void on_action_Settings_triggered();
    
    void on_actionFull_Screen_triggered(bool checked);
    
private:
    Ui::MainWindow *ui;
    QWidget* m_container;
    ExtensionBar* m_extensionBar;
    StyleLoader m_styleLoader;
    SettingsLoader m_settingsLoader;
    
    Qt::WindowStates m_previousWindowsState;
    
    data::DataHolder m_dataHolder;
};

#endif // MAINWINDOW_H