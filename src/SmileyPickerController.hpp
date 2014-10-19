/*
 * SmileyPickerController.hpp
 *
 *  Created on: 5 avr. 2014
 *      Author: pierre
 */

#ifndef SMILEYPICKERCONTROLLER_HPP_
#define SMILEYPICKERCONTROLLER_HPP_

#include <QtCore/QObject>
#include <QString>
#include <QList>

#include <bb/cascades/WebView>

class SmileyPickerController : public QObject {
	Q_OBJECT;

	private:

	bb::cascades::WebView 				*m_WebView;
	QString 							 m_SmileysPage;
	QString								 m_Page;

	int									 m_lastId;
	QList<int>							 m_IndexSubpagesInFile;

	// ----------------------------------------------------------------------------------------------
	public:
		SmileyPickerController(QObject *parent = 0) : QObject(parent), m_WebView(NULL), m_lastId(0) {};
		virtual ~SmileyPickerController() {};


	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		void loadDefautSmiley();


		inline void setWebView(QObject *webView) 				{m_WebView = dynamic_cast<bb::cascades::WebView*>(webView); }



	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void complete();




	// ----------------------------------------------------------------------------------------------
	private:

		void parse(const QString &page, int startingPosition = 0);
		void updateView(const QString &webpage);

};




#endif /* SMILEYPICKERCONTROLLER_HPP_ */
