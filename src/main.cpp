// Tabbed pane project template

#include <bb/cascades/Application>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

#include <QLocale>
#include <QTimer>
#include <QTranslator>
#include <Qt/qdeclarativedebug.h>
#include "broadCaster.h"
#include "xbmcQtClient.h"

#include <bb/system/SystemDialog>
#include <bb/system/SystemListDialog>
#include <bb/system/SystemPrompt>
#include <bb/system/SystemCredentialsPrompt>
#include <bb/system/SystemToast>
#include <bb/system/SystemUiButton>
#include <bb/system/SystemUiInputField>
#include <bb/system/SystemUiError>
#include <bb/system/SystemUiInputMode>
#include <bb/system/SystemUiModality>
#include <bb/system/SystemUiPosition>
#include <bb/system/SystemUiResult>

using namespace bb::cascades;
using namespace bb::system;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    // this is where the server is started etc
    Application app(argc, argv);

    // localization support
    QTranslator translator;
    QString locale_string = QLocale().name();
    QString filename = QString( "WOLXBMC_%1" ).arg( locale_string );
    if (translator.load(filename, "app/native/qm")) {
        app.installTranslator( &translator );
    }

	QmlDocument *qml = QmlDocument::create("asset:///main.qml");
    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app.setScene(root);

    // EDITED CODE <
    BroadCaster broadCaster; // udp broadcaster
	XbmcQtClient xbmcClient; // xbmc remote code sender
	qml->setContextProperty("BroadCaster", &broadCaster);
    qml->setContextProperty("XbmcQtClient", &xbmcClient);
    QTimer timer;
    // start storing MACs to sqlite (if any changes have taken effect)
    QObject::connect(&timer, SIGNAL(timeout()), root, SIGNAL(storeMACsToSQL()));
    timer.start(2000);
    // set settings for XBMC remote via main.qml -> XbmcUI.qml
    QTimer::singleShot(200, root, SIGNAL(setXBMCSettings()));

    // EDITED CODE >

    // we complete the transaction started in the app constructor and start the client event loop here
    return Application::exec();
    // when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
