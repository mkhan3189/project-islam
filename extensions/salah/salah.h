﻿#ifndef SALAH_H
#define SALAH_H

#include "core/extension/extension_base.h"

class SalahTimes;
class SettingsTabWidgetForm;
class QiblaCompass;
class SalahClock;

class Salah : public ExtensionBase
{
    Q_OBJECT
    Q_INTERFACES(ExtensionBase)
    Q_PLUGIN_METADATA(IID "ProjectIslam.Api.ExtensionBase.Salah.v0.1.0")
private:
    static const unsigned int kMajorVersion = 0;
    static const unsigned int kMinorVersion = 1;
    static const unsigned int kPatchVersion = 0;

    static const char* kAuthor;
    static const char* kName;
    static const char* kTitle;
    static const char* kDescription;
public:
    Salah();
    ~Salah();
    
    virtual bool initialize(int argc, const char **argv);
    void initializeSettingsTabDialog();
    void initializeMenu();
    void displayClocks();
    
    virtual inline unsigned int getMajorVersion() { return kMajorVersion; }
    virtual inline unsigned int getMinorVersion() { return kMinorVersion; }
    virtual inline unsigned int getPatchVersion() { return kPatchVersion; }
    virtual inline const char* getAuthor() { return kAuthor; }
    virtual inline const char* getTitle() { return kTitle; }
    virtual inline const char* getName() { return kName; }
    virtual inline const char* getDescription() { return kDescription; }
    
public slots:
    
    virtual void onContainerGeometryChanged(int, int);
    virtual void onActivated();
    virtual void onDeactivated();
    void onPrayerTime(bool);
    void onPrayerTimeAboutToStart(int minutesLeft);
    void onPrayerTimeAboutToOver(int minutesLeft);
    void checkDayChange();
private:
    SalahTimes* m_salahTimes;
    SettingsTabWidgetForm* m_settingsWidgetForm;
    QiblaCompass* m_qiblaCompass;
    
    int m_minutesToPrayerAboutToOver;
    
    int m_minutesToPrayerAboutToStartFajr;
    int m_minutesToPrayerAboutToStartDhuhr;
    int m_minutesToPrayerAboutToStartAsr;
    int m_minutesToPrayerAboutToStartMaghrib;
    int m_minutesToPrayerAboutToStartIsha;
    int m_minutesToPrayerAboutToStartJumuah;
    
    SalahClock* m_fajrClock;
    SalahClock* m_dhuhrClock;
    SalahClock* m_asrClock;
    SalahClock* m_maghribClock;
    SalahClock* m_ishaClock;
};

#endif // SALAH_H
