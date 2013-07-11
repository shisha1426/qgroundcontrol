#include <QDebug>
#include <QDeclarativeContext>
#include <QGraphicsObject>
#include "LinkManager.h"
#include "MainWindow.h"

#include "apmtoolbar.h"

APMToolBar::APMToolBar(QWidget *parent):
    QDeclarativeView(parent)
{
    // Configure our QML object
    this->rootContext()->setContextProperty("globalObj", this);
    setSource(QUrl::fromLocalFile("qml/ApmToolBar.qml"));
    setResizeMode(QDeclarativeView::SizeRootObjectToView);

    QObject *root = rootObject();
    connect(LinkManager::instance(),SIGNAL(newLink(LinkInterface*)),
            this, SLOT(updateLinkDisplay(LinkInterface*)));
}

void APMToolBar::setFlightViewAction(QAction *action)
{
    connect(this, SIGNAL(triggerFlightView()), action, SIGNAL(triggered()));
}

void APMToolBar::setFlightPlanViewAction(QAction *action)
{
    connect(this, SIGNAL(triggerFlightPlanView()), action, SIGNAL(triggered()));
}

void APMToolBar::setHardwareViewAction(QAction *action)
{
    connect(this, SIGNAL(triggerHardwareView()), action, SIGNAL(triggered()));
}

void APMToolBar::setSoftwareViewAction(QAction *action)
{
    connect(this, SIGNAL(triggerSoftwareView()), action, SIGNAL(triggered()));
}

void APMToolBar::setSimulationViewAction(QAction *action)
{
    connect(this, SIGNAL(triggerSimulationView()), action, SIGNAL(triggered()));
}

void APMToolBar::setTerminalViewAction(QAction *action)
{
    connect(this, SIGNAL(triggerTerminalView()), action, SIGNAL(triggered()));
}

void APMToolBar::setConnectMAVAction(QAction *action)
{
    connect(this, SIGNAL(connectMAV()), action, SIGNAL(triggered()));
}

void APMToolBar::selectFlightView()
{
    qDebug() << "APMToolBar: SelectFlightView";
    emit triggerFlightView();
}

void APMToolBar::selectFlightPlanView()
{
    qDebug() << "APMToolBar: SelectFlightPlanView";
    emit triggerFlightPlanView();
}

void APMToolBar::selectHardwareView()
{
    qDebug() << "APMToolBar: selectHardwareView";
    emit triggerHardwareView();
}

void APMToolBar::selectSoftwareView()
{
    qDebug() << "APMToolBar: selectSoftwareView";
    emit triggerSoftwareView();
}

void APMToolBar::selectSimulationView()
{
    qDebug() << "APMToolBar: selectSimulationView";
}

void APMToolBar::selectTerminalView()
{
    qDebug() << "APMToolBar: selectTerminalView";
}

void APMToolBar::connectMAV()
{
    qDebug() << "APMToolBar: connectMAV ";

    bool connected = LinkManager::instance()->getLinks().last()->isConnected();
    bool result;

    if (!connected && LinkManager::instance()->getLinks().count() < 3)
    {
        // No Link so prompt to connect one
        MainWindow::instance()->addLink();
    } else if (!connected) {
        // Need to Connect Link
        result = LinkManager::instance()->getLinks().last()->connect();

    } else if (connected && LinkManager::instance()->getLinks().count() > 2) {
        // result need to be the opposite of success.
        result = !LinkManager::instance()->getLinks().last()->disconnect();
    }
    qDebug() << "result = " << result;
    emit MAVConnected(result);
}

APMToolBar::~APMToolBar()
{
    qDebug() << "Destory APM Toolbar";
}

void APMToolBar::showConnectionDialog()
{
    // Displays a UI where the user can select a MAV Link.
    qDebug() << "APMToolBar: showConnectionDialog link count ="
             << LinkManager::instance()->getLinks().count();

    LinkInterface *link = LinkManager::instance()->getLinks().last();
    bool result;

    if (link && LinkManager::instance()->getLinks().count() >= 3)
    {
        // Serial Link so prompt to config it
        result = MainWindow::instance()->configLink(link);

        if (!result)
            qDebug() << "Link Config Failed!";
    } else {
        // No Link so prompt to create one
        MainWindow::instance()->addLink();
    }

}

void APMToolBar::updateLinkDisplay(LinkInterface* newLink)
{
    qDebug() << "APMToolBar: updateLinkDisplay";
    QObject *object = rootObject();

    if (newLink){
        qint64 baudrate = newLink->getNominalDataRate();
        object->setProperty("baudrateLabel", QString::number(baudrate));

        QString linkName = newLink->getName();
        object->setProperty("linkNameLabel", linkName);
    }
}
