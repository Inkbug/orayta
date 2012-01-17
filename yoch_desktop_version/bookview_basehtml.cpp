#include "bookview_basehtml.h"
#include "bookdisplayer.h"
#include "functions.h"

#include <QWebPage>
#include <QWebFrame>
#include <QApplication>
#include <QDebug>

BaseHtmlView::BaseHtmlView( BookDisplayer * parent ) :
    QWebView(parent),
    BookViewInterface(parent),
    loading(false)
{
    //Prevent context menus. If needed, they will be created manually
    setContextMenuPolicy(Qt::PreventContextMenu);

    //The myWebView class will handle all link, but will emit "LinkMenu" when one should be shown
    //page()->setLinkDelegationPolicy(QWebPage::DontDelegateLinks);

    // add Shorcut for copyAction
    copyAction = pageAction(QWebPage::Copy);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setText(tr("Copy text"));
    copyAction->setIcon(QIcon(":/Icons/edit-copy.png"));
    addAction(copyAction);

    QAction* reloadAction = pageAction(QWebPage::Reload);
    reloadAction->setShortcut(QKeySequence::Refresh);
    addAction(reloadAction);

    QObject::connect(this, SIGNAL(titleChanged(QString)), parent, SLOT(setTitle(QString)));
    QObject::connect(this, SIGNAL(loadFinished(bool)), this, SLOT(on_loadFinished(bool)));

    /* pourquoi cette maudite progress bar n'apparait qu'aprés le chargement ??? */
//    QObject::connect(this, SIGNAL(loadProgress(int)), mBookDisp->progressBar(), SLOT(setValue(int)));
}

BaseHtmlView::~BaseHtmlView()
{
//    mBookDisp->progressBar()->disconnect(this);
}

void BaseHtmlView::loadUrl(const QUrl& url)
{
/*
    QFileInfo f("./Images/Wait.gif");
    QString html = simpleHtmlPage("", "<br><br><br><br><img src=\"" + f.absoluteFilePath() + "\" ></img>");
    writetofile(USERPATH + "wait.html", html, "UTF-8", true);

    //load(QUrl::fromLocalFile(USERPATH + "wait.html"));
*/
    //qDebug() << "load started";

    loading = true;
    //  // marche pas, automatiquement remplacé par "", probablement du à load()

    load(url);
    setFocus();
}

void BaseHtmlView::ZoomIn()
{
    // ####### ce hack est là pour ne pas perdre le texte lors du zoom
    //Extremely ugly hack. Why can't webkit zoom like it should?

    //Find how far the scroll bar is in the existing range
    int scrollMax = page()->mainFrame()->scrollBarMaximum(Qt::Vertical);
    int cur = page()->mainFrame()->scrollBarValue(Qt::Vertical);
    float ratio = (float) cur / scrollMax;

    //Zoomin
    setTextSizeMultiplier(textSizeMultiplier() + 0.1);

    //Update range and go back to the same position relative to it
    scrollMax = page()->mainFrame()->scrollBarMaximum(Qt::Vertical);
    page()->mainFrame()->setScrollBarValue(Qt::Vertical, ratio * scrollMax);
}

void BaseHtmlView::ZoomOut()
{
    if (textSizeMultiplier() > 0.1)
    {
        // ####### idem
        //Extremely ugly hack. Why can't webkit zoom like it should?

        //Find how far the scroll bar is in the existing range
        int scrollMax = page()->mainFrame()->scrollBarMaximum(Qt::Vertical);
        int cur = page()->mainFrame()->scrollBarValue(Qt::Vertical);
        float ratio = (float) cur / scrollMax;

        //Zoomout
        setTextSizeMultiplier(textSizeMultiplier() - 0.1);

        //Update range and go back to the same position relative to it
        scrollMax = page()->mainFrame()->scrollBarMaximum(Qt::Vertical);
        page()->mainFrame()->setScrollBarValue(Qt::Vertical, ratio * scrollMax);
    }
}

void BaseHtmlView::normalZoom()
{
    setTextSizeMultiplier(1);
}

void BaseHtmlView::searchText(const QString& text, bool backwards)
{
    setFocus();

    if (!backwards) findText(text);
    else findText(text, QWebPage::FindBackward);
}

void BaseHtmlView::setTextSizeMultiplier(qreal factor)
{
    //Set a busy cursor before actually resizing the text, and restores it by the end
    QApplication::setOverrideCursor(Qt::WaitCursor);

    //Resize the text
    QWebView::setTextSizeMultiplier(factor);

    QApplication::restoreOverrideCursor();
}

void BaseHtmlView::setInternalLocation(const QString& location)
{
    if (loading)
    {
        addDelayedAction(
            std::tr1::bind(&BaseHtmlView::setInternalLocation, this, location)  );
    }
    else
    {
        qDebug() << "set internal location " << location;
        page()->currentFrame()->scrollToAnchor(location);
    }
}

void BaseHtmlView::execScript(const QString& script)
{
    if (loading)
    {
        addDelayedAction(
            std::tr1::bind(&BaseHtmlView::execScript, this, script)  );
    }
    else
    {
        page()->mainFrame()->evaluateJavaScript(script);
    }
}

//Returns the value of given JS variable
QString BaseHtmlView::getJSVar(const QString& var)
{
    QVariant val = page()->mainFrame()->evaluateJavaScript("ReturnValue(" + var + ");");

    //Return the requested var
    return val.toString();
}

// cette fonction sert a effectuer certaines actions en différé, car
// la page se charge de maniere asynchrone, et c'est le seul moyen de s'assurer
// que ces traitements seront bien effectués APRES le chargement.
void BaseHtmlView::on_loadFinished(bool ok)
{
    loading = false;

    if (ok)
    {
        //qDebug() << "load finished !";

        while (!delayedActions.empty())
        {
            delayedActions.takeFirst()();
        }
    }
    else
    {
        // ######## les informations contenues dans l'url sont fausse...
        qDebug() << "load failed !" << url();
        setHtml( simpleHtmlPage( tr("404 - Error"), tr("file not found") ) );
    }
}

void BaseHtmlView::addDelayedAction(fn_obj func)
{
    delayedActions.push_back(func);
}
