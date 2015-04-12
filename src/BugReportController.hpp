/*
 * BugReportController.hpp
 *
 *  Created on: 5 avr. 2015
 *      Author: pierre
 */

#ifndef BUGREPORTCONTROLLER_HPP_
#define BUGREPORTCONTROLLER_HPP_

#include <QtCore/QObject>
#include <bb/cascades/ListView>
#include <bb/cascades/WebView>
#include <bb/system/SystemUiResult>

class QNetworkAccessManager;
class IssueListItem;

class BugReportController : public QObject {
    Q_OBJECT;


private:

    static QMap<QString, int>        m_Labels;

    bb::cascades::ListView          *m_ListView;
    bb::cascades::WebView           *m_WebView;
    QNetworkAccessManager           *m_NetworkAccessManager;

    int                              m_TypeIssue;
    QList<IssueListItem*>            m_Issues;
    QString                          m_IssueDescription;


    QString                          m_TmpTitle, m_TmpBody;
    int                              m_TmpLabel;


    void parseIssues                 (const QString& page);
    void parseOneIssue               (const QString& issue);
    void updateView                  ();

    void parseIssuesDescription      (const QString &page);
    void parseOneDescription         (const QString &description);
    void initWebPage                 ();


public:
    BugReportController              (QObject *parent = 0);
    virtual ~BugReportController     ()                               {};



public Q_SLOTS:

    inline void setListView          (QObject *list)                  { m_ListView = dynamic_cast<bb::cascades::ListView*>(list); };
    inline void setWebView           (QObject *web)                   { m_WebView = dynamic_cast<bb::cascades::WebView*>(web); };


    void listIssues                  (int typeIssues);
    void loadIssue                   (int issue);
    void insertIssue                 (const QString &title, const QString &body, int label);
    void insertComment               (const QString &body, int issueNumber);


    void checkReplyIssues            ();
    void checkReplyIssueDescription  ();
    void checkReplyInsertIssue       ();
    void checkReplyInsertComment     ();

    void onPromptFinishedCreateIssue(bb::system::SystemUiResult::Type);
    void onPromptFinishedCreateComment(bb::system::SystemUiResult::Type);


Q_SIGNALS:

    void completed();
    void insertSuccess();
    void insertCommentSuccess();


};




#endif /* BUGREPORTCONTROLLER_HPP_ */
